/*---------------------------------------------------------------------------*
  Project:  Horizon

  Copyright (C)Nintendo Co., Ltd.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Rev: 58466 $
 *---------------------------------------------------------------------------*/

#include <stdlib.h>
#include <string.h>
#include <wchar.h>
#include "Util.h"

//////////////////////////////////////////////////////////////////////////////////////////
// Variable Definitions
//////////////////////////////////////////////////////////////////////////////////////////

namespace
{
    // Heap
    nn::fnd::ExpHeap* s_pExpHeap = NULL;
    // Heap object buffer
    bit8 s_expHeapObjectBuffer[sizeof (nn::fnd::ExpHeap)] NN_ATTRIBUTE_ALIGN(8);
    // Title
    const size_t TITLE_LENGTH = 255;
    wchar_t s_Title[TITLE_LENGTH + 1];
}

//////////////////////////////////////////////////////////////////////////////////////////

namespace scene
{

// Get the length of a wide string.
u32 wstrlen(const wchar_t* pText)
{
    u32 length = 0;

    while (*pText++ != '\0')
    {
        length++;
    }

    return length;
}

// Convert a wide string to a numerical value.
u32 wtoi(const wchar_t* pText)
{
    // Numeric value
    u32 value = 0;

    while (*pText != '\0' && *pText >= '0' && *pText <= '9')
    {
        value = value * 10 + (*pText - '0');
        pText++;
    }

    return value;
}

// Initialize memory.
void SetupMemory(u32 deviceMemorySize)
{
    // Create a heap object
    s_pExpHeap = new(s_expHeapObjectBuffer) nn::fnd::ExpHeap();
    // Assign device memory to a heap
    s_pExpHeap->Initialize(nn::os::GetDeviceMemoryAddress(), deviceMemorySize );
}

// Deallocate memory.
void ReleaseMemory()
{
    s_pExpHeap->Invalidate();
    s_pExpHeap->Finalize();
}

// Get the heap.
nn::fnd::ExpHeap* GetHeap()
{
    return s_pExpHeap;
}

// Allocates memory.
void* Allocate(size_t size, s32 alignment)
{
    return s_pExpHeap->Allocate(size, alignment);
}

// Deallocate memory.
void Free(void* pBuffer)
{
    if (pBuffer)
    {
        s_pExpHeap->Free(pBuffer);
    }
}

// Render the title.
void SetTitle(const wchar_t* pTitle, size_t length)
{
    NN_ASSERT(length < TITLE_LENGTH);

    wcsncpy(s_Title, pTitle, length);
    s_Title[length] = L'\0';
}
void DrawTitle(TextWriter* pTextWriter)
{
    f32 scale = pTextWriter->GetFontScale();
    pTextWriter->SetFontScale(DEFAULT_FONT_SCALE);
    pTextWriter->SetTextColor(102, 153, 255);
    pTextWriter->Printf(2, 2, s_Title);
    pTextWriter->SetFontScale(scale);
}

void DrawTitleForApplication(TextWriter* pTextWriter)
{
    pTextWriter->SetTextColor(102, 153, 255);
    pTextWriter->Printf(2, 2, L"Add-On Content Purchase Application");
}

void DrawTitleForApplet(TextWriter* pTextWriter)
{
    pTextWriter->SetTextColor(0, 255, 0);
    pTextWriter->Printf(2, 2, L"Add-On Content Purchase Applet");
}

// Render the scene ID.
void DrawSceneId(TextWriter* pTextWriter, const wchar_t* pSceneId)
{
#ifdef DEBUG_SHOW_SCENE_ID
    pTextWriter->SetTextColor(255, 255, 0);
    pTextWriter->PutString(346, 2, pSceneId);
#else
    NN_UNUSED_VAR(pTextWriter);
    NN_UNUSED_VAR(pSceneId);
#endif
}

// Render Result.
void DrawResult(TextWriter* pTextWriter, const nn::Result& result)
{
#ifdef DEBUG_SHOW_RESULT
    if (result.IsFailure())
    {
        pTextWriter->SetTextColor(255, 0, 0);
        pTextWriter->Printf(2, 214, L"RESULT: %08X", result.GetPrintableBits());
    }
#else
    NN_UNUSED_VAR(pTextWriter);
    NN_UNUSED_VAR(result);
#endif
}

// Render the busy icon.
void DrawBusyIcon(TextWriter* pTextWriter, u16 x, u16 y, u32 counter)
{
    // Index
    u32 idnex = (counter / 10) % 8;
    // Busy icon
    wchar_t busyIcon[] = {0xE020 + idnex, '\0'};
    // Offset (adjust with offset since only the 4th character is shifted)
    f32 offset = (idnex == 3) ? 0.6f : 0.0f;

    // Current font scale
    f32 fontScale = pTextWriter->GetFontScale();

    pTextWriter->SetFontScale(1.0f);
    pTextWriter->SetTextColor(0, 162, 232);
    pTextWriter->Printf(x + offset, y, busyIcon);

    // Restore font scale to original value
    pTextWriter->SetFontScale(fontScale);
}

// Render the confirmation message.
void DrawConfirmMessage(TextWriter* pTextWriter, const wchar_t* pMessage1, const wchar_t* pMessage2)
{
    pTextWriter->SetTextColor(0, 255, 0);

    // Up
    {
        // Text size
        f32 width  = pTextWriter->CalculateStringWidth(pMessage1);
        f32 height = pTextWriter->CalculateStringHeight(pMessage1);
        // Display position
        f32 x = ((f32)NN_GX_DISPLAY1_HEIGHT - width) / 2.0f;
        f32 y = ((f32)NN_GX_DISPLAY1_WIDTH - 45.0f) / 2.0f - (height + 1.0f);

        pTextWriter->Printf(x, y, pMessage1);
    }
    // Down
    {
        // Text size
        f32 width  = pTextWriter->CalculateStringWidth(pMessage2);
        // Display position
        f32 x = ((f32)NN_GX_DISPLAY1_HEIGHT - width) / 2.0f;
        f32 y = ((f32)NN_GX_DISPLAY1_WIDTH - 45.0f) / 2.0f + 1.0f;

        pTextWriter->Printf(x, y, pMessage2);
    }
}

// Get the DateTime from the UNIX time.
nn::fnd::DateTime GetDateTimeFromUnixTime(s64 unixTime)
{
    // Time
    nn::fnd::DateTime dateTime = nn::fnd::DateTime(1970, 1, 1);
    // Add the UNIX time
    dateTime += nn::fnd::TimeSpan::FromMilliSeconds(unixTime);

    return dateTime;
}

// Add the left button.
void AddLButton(ControlManager* pManager, u32 id, const wchar_t* pCaption)
{
    pManager->Add(new Button(id, 2, 208, 212, 30, pCaption));
}

// Add the right button.
void AddRButton(ControlManager* pManager, u32 id, const wchar_t* pCaption)
{
    pManager->Add(new Button(id, 218, 208, 100, 30, pCaption));
}

// Add the YES/NO button.
void AddYesNoButtons(ControlManager* pManager, u32 idYes, u32 idNo, const wchar_t* pCaptionYes, const wchar_t* pCaptionNo)
{
    pManager->Add(new Button(idYes,   2, 208, 156, 30, pCaptionYes));
    pManager->Add(new Button(idNo,  162, 208, 156, 30, pCaptionNo));
}

} // namespace scene
