/*---------------------------------------------------------------------------*
  Project:  Horizon

  Copyright (C) Nintendo Co., Ltd. All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Rev: 58466 $
 *---------------------------------------------------------------------------*/

#include <stdarg.h>
#include "GuiControlBase.h"

namespace
{
    s32 wstrlen(const wchar_t* pText)
    {
        s32 length = 0;

        while (pText[length] != '\0')
        {
            length++;
        }

        return length;
    }
}

namespace scene
{

// Constructor.
ControlBase::ControlBase(ControlType type, u32 id, s32 x, s32 y, u32 width, u32 height, const wchar_t* pText, void* pExtraData, f32 fontScale)
{
    m_type             = type;
    m_id               = id;
    m_x                = x;
    m_y                = y;
    m_width            = width;
    m_height           = height;
    m_pText            = NULL;
    m_textLength       = 0;
    m_textBufferLength = 0;
    m_TextScale        = fontScale;
    m_textWidth        = 0;
    m_textHeight       = 0;
    m_textAlign        = TEXT_ALIGN_LEFT;
    m_borderWidth      = 1.0f;
    m_state            = CONTROL_STATE_DEFAULT;
    m_pExtraData       = pExtraData;
    m_pManager         = NULL;

    SetText(pText);
}

// Destructor.
ControlBase::~ControlBase()
{
    SetText(NULL);
}

// Sets text.
void ControlBase::SetText(const wchar_t* pText)
{
    if (pText)
    {
        s32 length = wstrlen(pText);

        if (length >= m_textBufferLength)
        {
            delete[] m_pText;

            m_pText            = new wchar_t[length + 1];
            m_textBufferLength = length + 1;
        }

        m_pText[length] = '\0';
        memcpy(m_pText, pText, length * sizeof (m_pText[0]));

        m_textLength = length;

        // Calculate the size
        {
            // Text writer
            TextWriter* pTextWriter = scene::GetTextWriter();

            pTextWriter->SetFontScale(m_TextScale);
            m_textWidth  = pTextWriter->CalculateStringWidth(m_pText);
            m_textHeight = pTextWriter->CalculateStringHeight(m_pText);
        }
    }
    else
    {
        delete[] m_pText;

        m_pText            = NULL;
        m_textLength       = 0;
        m_textBufferLength = 0;
        m_textWidth        = 0;
        m_textHeight       = 0;
    }
}

// Sets text.
void ControlBase::SetFormattedText(const wchar_t* pFormat, ...)
{
    // Buffer.
    wchar_t buffer[NN_DBG_PRINTF_BUFFER_LENGTH + 1] = {0};
    // Argument list
    va_list pArgs;

    va_start(pArgs, pFormat);
    {
        vswprintf(buffer, NN_DBG_PRINTF_BUFFER_LENGTH, pFormat, pArgs);
    }
    va_end(pArgs);

    SetText(buffer);
}

// Calculate the start position for text rendering.
s32 ControlBase::CalculateTextX(s32 offsetL, s32 offsetR)
{
    s32 x = 0;

    switch (m_textAlign)
    {
    case TEXT_ALIGN_LEFT:
        {
            x = m_x + offsetL;
        }
        break;
    case TEXT_ALIGN_RIGHT:
        {
            x = m_x - offsetR + m_width - m_textWidth;
        }
        break;
    case TEXT_ALIGN_CENTER:
        {
            s32 offsetC = offsetL + offsetR;

            x = m_x + offsetL + ((m_width - offsetC) - m_textWidth) / 2;
        }
        break;
    }

    return x;
}

// Calculate the start position for text rendering.
s32 ControlBase::CalculateTextY(s32 offset)
{
    return (m_y + offset + ((m_height - offset) - m_textHeight) / 2);
}

}
