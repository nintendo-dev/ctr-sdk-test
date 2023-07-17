/*---------------------------------------------------------------------------*
  Project:  Horizon

  Copyright (C) Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Rev: 61320 $
 *---------------------------------------------------------------------------*/

#include "TextWriter.h"

//////////////////////////////////////////////////////////////////////////////////////////
// Constant Definitions
//////////////////////////////////////////////////////////////////////////////////////////

// Number of displays
#define DISPLAY_NUM 2

// Display number
#define DISPLAY_0 0
#define DISPLAY_1 1

extern bit8 NNFONT_RECTDRAWERSHADER_BEGIN[];
extern bit8 NNFONT_RECTDRAWERSHADER_END[];

namespace
{
    // Initialization flag
    bool s_isInitialized = false;
    // unit matrix
    nn::math::MTX34 s_mtxIdentify;
    // Projection matrix.
    nn::math::MTX44 s_mtxProjection[DISPLAY_NUM];

    // Color buffer information
    const nn::font::ColorBufferInfo s_colBufferInfos[DISPLAY_NUM] =
    {
        {NN_GX_DISPLAY0_WIDTH, NN_GX_DISPLAY0_HEIGHT, PICA_DATA_DEPTH24_STENCIL8_EXT},
        {NN_GX_DISPLAY1_WIDTH, NN_GX_DISPLAY1_HEIGHT, PICA_DATA_DEPTH24_STENCIL8_EXT}
    };

    // Render setting command
    const u32 s_screenSettingCommands0[] =
    {
        // Viewport settings
        NN_FONT_CMD_SET_VIEWPORT(0, 0, s_colBufferInfos[DISPLAY_0].width, s_colBufferInfos[DISPLAY_0].height),
        // Disable scissoring
        NN_FONT_CMD_SET_DISABLE_SCISSOR(s_colBufferInfos[DISPLAY_0]),
        // Disable w Buffer / Set depth range / Disable polygon offset
        NN_FONT_CMD_SET_WBUFFER_DEPTHRANGE_POLYGONOFFSET(0, 0, 1, 0, s_colBufferInfos[DISPLAY_0])
    };

    // Render setting command
    const u32 s_screenSettingCommands1[] =
    {
        // Viewport settings
        NN_FONT_CMD_SET_VIEWPORT(0, 0, s_colBufferInfos[DISPLAY_1].width, s_colBufferInfos[DISPLAY_1].height),
        // Disable scissoring
        NN_FONT_CMD_SET_DISABLE_SCISSOR(s_colBufferInfos[DISPLAY_1]),
        // Disable w Buffer / Set depth range / Disable polygon offset
        NN_FONT_CMD_SET_WBUFFER_DEPTHRANGE_POLYGONOFFSET(0, 0, 1, 0, s_colBufferInfos[DISPLAY_1])
    };

    // Initialization command
    const u32 s_initCommands[] =
    {
        // Disable culling
        NN_FONT_CMD_SET_CULL_FACE(NN_FONT_CMD_CULL_FACE_DISABLE),
        // Disable stencil test
        NN_FONT_CMD_SET_DISABLE_STENCIL_TEST(),
        // Disable depth test / Make all elements of the color buffer writable
        NN_FONT_CMD_SET_DEPTH_FUNC_COLOR_MASK(false, 0, true, true, true, true, true),
        // Disable early depth test
        NN_FONT_CMD_SET_ENABLE_EARLY_DEPTH_TEST(false),
        // Framebuffer access control.
        NN_FONT_CMD_SET_FBACCESS(true, true, false, false, false, false)
    };
}

//////////////////////////////////////////////////////////////////////////////////////////
namespace scene
{

// Instantiates the object.
TextWriter::TextWriter()
{
    m_pExpHeap      = NULL;
    m_pStringBuffer = NULL;
    m_textColor     = nn::util::Color8(255, 255, 255, 255);
    m_pShaderBuffer = NULL;
    m_targetDisplay = DISPLAY_0;
}

// Destructor.
TextWriter::~TextWriter()
{
    Finalize();
}

// Performs initialization.
void TextWriter::Initialize(nn::fnd::ExpHeap* pExpHeap)
{
    // Initialize shared font
    if (nn::pl::InitializeSharedFont().IsSuccess())
    {
        // Standby until shared font has completed loading
        while (nn::pl::GetSharedFontLoadState() != nn::pl::SHARED_FONT_LOAD_STATE_LOADED)
        {
            nn::os::Thread::Sleep(nn::fnd::TimeSpan::FromMilliSeconds(1));
        }

        // Confirm that shared font loading has not failed
        if (nn::pl::GetSharedFontLoadState() == nn::pl::SHARED_FONT_LOAD_STATE_FAILED)
        {
            NN_PANIC("nn::pl::InitializeSharedFont() failed!\n");
        }
    }

    m_pExpHeap = pExpHeap;

    // Initialize font
    if (!InitializeFont())
    {
        NN_PANIC("InitializeFont() failed!\n");
    }

    // Initialize shader
    if (!InitializeShader())
    {
        NN_PANIC("InitializeShader() failed!\n");
    }

    // Allocate the string buffer
    m_pStringBuffer = AllocateStringBuffer(512);

    if (!m_pStringBuffer)
    {
        NN_PANIC("Allocate string buffer failed!\n");
    }

    // Initialization that must be run only once while running
    if (!s_isInitialized)
    {
        // Create projection matrix
        nn::math::MTX44OrthoPivot(&s_mtxProjection[DISPLAY_0], 0, NN_GX_DISPLAY0_HEIGHT, NN_GX_DISPLAY0_WIDTH, 0, 0, -1, nn::math::PIVOT_UPSIDE_TO_TOP);
        nn::math::MTX44OrthoPivot(&s_mtxProjection[DISPLAY_1], 0, NN_GX_DISPLAY1_HEIGHT, NN_GX_DISPLAY1_WIDTH, 0, 0, -1, nn::math::PIVOT_UPSIDE_TO_TOP);

        // Create the unit matrix
        nn::math::MTX34Identity(&s_mtxIdentify);

        s_isInitialized = true;
    }

    // Set font object
    m_writer.SetFont(&m_font);
    // Since the initial font size is too big, make it smaller
    m_writer.SetScale(0.6f);
}

// Performs shutdown.
void TextWriter::Finalize()
{
    if (m_pExpHeap)
    {
        // Disable buffer for rendering
        void* pDrawBuffer = m_font.SetDrawBuffer(NULL);

        if (pDrawBuffer)
        {
            m_pExpHeap->Free(pDrawBuffer);
        }

        if (m_pShaderBuffer)
        {
            m_pExpHeap->Free(m_pShaderBuffer);
        }
        if (m_pStringBuffer)
        {
            m_pExpHeap->Free(m_pStringBuffer);
        }

        // Initialize variables
        m_pExpHeap      = NULL;
        m_pStringBuffer = NULL;
        m_pShaderBuffer = NULL;
    }
}

// Start rendering.
void TextWriter::BeginRender(GLenum targetDisplay)
{
    if (targetDisplay == NN_GX_DISPLAY0)
    {
        nngxAdd3DCommand(s_screenSettingCommands0, sizeof (s_screenSettingCommands0), true);
        m_targetDisplay = DISPLAY_0;
    }
    else
    {
        nngxAdd3DCommand(s_screenSettingCommands1, sizeof (s_screenSettingCommands1), true);
        m_targetDisplay = DISPLAY_1;
    }

    nngxAdd3DCommand(s_initCommands, sizeof (s_initCommands), true);

    m_writer.SetDispStringBuffer(m_pStringBuffer);
    m_writer.StartPrint();
}

// End rendering.
void TextWriter::EndRender()
{
    m_writer.EndPrint();

    m_drawer.BuildTextCommand(&m_writer);
    m_drawer.DrawBegin();
    {
        // Set projection matrix
        m_drawer.SetProjectionMtx(s_mtxProjection[m_targetDisplay]);
        // Initialize view matrix
        m_drawer.SetViewMtxForText(s_mtxIdentify);

        m_writer.UseCommandBuffer();
    }
    m_drawer.DrawEnd();
    nngxUpdateState(NN_GX_STATE_ALL);
}

// Renders a character string.
void TextWriter::Printf(f32 x, f32 y, const wchar_t* pFormat, ...)
{
    va_list pArgs;

    va_start(pArgs, pFormat);
    {
        m_writer.SetCursor(x, y);
        m_writer.SetTextColor(m_textColor);
        m_writer.VPrintf(pFormat, pArgs);
    }
    va_end(pArgs);
}

// Renders a character string.
void TextWriter::PutString(f32 x, f32 y, const wchar_t* pString)
{
    m_writer.SetCursor(x, y);
    m_writer.SetTextColor(m_textColor);
    m_writer.Print(pString);
}

// Renders a character string. You can specify the maximum width to render.
void TextWriter::PutString(f32 x, f32 y, const wchar_t* pString, f32 limitWidth)
{
    const size_t MAX_BUF = 128;
    const size_t strCount = std::wcslen(pString);

    if(limitWidth <= 0.0f || strCount > (MAX_BUF - 1))
    {
        // Invalid argument. Does nothing.
        return;
    }

    char16 str[ MAX_BUF ] = { 0 };
    std::wcsncpy(str, pString, (MAX_BUF - 1));

    for(s32 i = (strCount - 1); i >= 0; --i)
    {
        const f32 width = this->CalculateStringWidth(str);
        if(limitWidth >= width)
        {
            // Exit loop if less than width argument.
            break;
        }

        // Remove one character from end and recalculate.
        str[i] = L'\0';
    }

    this->PutString(x, y, str);
}

// Set the font color.
void TextWriter::SetTextColor(u8 red, u8 green, u8 blue, u8 alpha)
{
    // Font color
    m_textColor = nn::util::Color8(red, green, blue, alpha);
}

// Get the font size.
f32 TextWriter::GetFontSize()
{
    return m_writer.GetFontHeight();
}

// Sets the font size.
void TextWriter::SetFontSize(f32 size)
{
    m_writer.SetFontSize(size);
}

// Get the font scale.

f32 TextWriter::GetFontScale()
{
    return m_writer.GetScaleH();
}

// Set the font scale.
void TextWriter::SetFontScale(f32 scale)
{
    m_writer.SetScale(scale);
}

// Get the character spacing.
f32 TextWriter::GetCharSpace()
{
    return m_writer.GetCharSpace();
}

// Set the character spacing.
void TextWriter::SetCharSpace(f32 charSpace)
{
    m_writer.SetCharSpace(charSpace);
}

// Calculates the width of a character string.
f32 TextWriter::CalculateStringWidth(const wchar_t* pString)
{
    return m_writer.CalcStringWidth(pString);
}

// Calculates the height of a character string.
f32 TextWriter::CalculateStringHeight(const wchar_t* pString)
{
    return m_writer.CalcStringHeight(pString);
}

// Initialize font.
bool TextWriter::InitializeFont()
{
    // Font buffer
    void* pBuffer = nn::pl::GetSharedFontAddress();
    // Set font resource
    bool isSuccess = m_font.SetResource(pBuffer);

    // Size of the buffer for rendering
    u32 drawBufferSize = nn::font::ResFont::GetDrawBufferSize(pBuffer);
    // Buffer for rendering
    void* pDrawBuffer = m_pExpHeap->Allocate(drawBufferSize, nn::font::ResFont::BUFFER_ALIGNMENT);

    NN_NULL_ASSERT(pDrawBuffer);

    // Set the render buffer
    m_font.SetDrawBuffer(pDrawBuffer);

    return isSuccess;
}

// Perform shader initialization.
bool TextWriter::InitializeShader()
{
    const size_t SHADER_SIZE = NNFONT_RECTDRAWERSHADER_END - NNFONT_RECTDRAWERSHADER_BEGIN;
    void* pShaderBinary = m_pExpHeap->Allocate(SHADER_SIZE);

    NN_NULL_ASSERT(pShaderBinary);

    // Size of the buffer for vertices and command buffer
    u32 vtxBufferCmdBufferSize = nn::font::RectDrawer::GetVertexBufferCommandBufferSize(NNFONT_RECTDRAWERSHADER_BEGIN, SHADER_SIZE);
    // Buffer for vertices and command buffer
    m_pShaderBuffer = m_pExpHeap->Allocate(vtxBufferCmdBufferSize);

    NN_NULL_ASSERT(m_pShaderBuffer);

    // Initialize render object
    m_drawer.Initialize(m_pShaderBuffer, NNFONT_RECTDRAWERSHADER_BEGIN, SHADER_SIZE);

    m_pExpHeap->Free(pShaderBinary);

    return true;
}

// Allocate the string buffer.
nn::font::DispStringBuffer* TextWriter::AllocateStringBuffer(s32 charMax)
{
    // Buffer size.
    const u32 bufferSize = nn::font::CharWriter::GetDispStringBufferSize(charMax);
    // Buffer.
    void* pBuffer = m_pExpHeap->Allocate(bufferSize);

    NN_NULL_ASSERT(pBuffer);

    return nn::font::CharWriter::InitDispStringBuffer(pBuffer, charMax);
}

} // namespace scene
