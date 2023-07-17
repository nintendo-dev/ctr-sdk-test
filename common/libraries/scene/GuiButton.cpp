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

#include "GuiButton.h"

namespace scene
{

// Constructor.
Button::Button(u32 id, s32 x, s32 y, u32 width, u32 height, const wchar_t* pText, void* pExtraData, f32 fontScale)
    : ControlBase(CONTROL_TYPE_BUTTON, id, x, y, width, height, pText, pExtraData, fontScale)
{
    m_textAlign   = TEXT_ALIGN_CENTER;
    m_borderWidth = 2.0f;
    m_state       = CONTROL_STATE_DEFAULT;

    // Border color
    m_borderColors[CONTROL_STATE_DEFAULT]   = nn::util::Color8(192, 192, 192, 255);
    m_borderColors[CONTROL_STATE_DISABLED]  = nn::util::Color8( 32,  32,  32, 255);
    m_borderColors[CONTROL_STATE_SELECTING] = nn::util::Color8( 45, 116, 174, 255);

    // Font color
    m_textColors[CONTROL_STATE_DEFAULT]   = nn::util::Color8(192, 192, 192, 255);
    m_textColors[CONTROL_STATE_DISABLED]  = nn::util::Color8( 32,  32,  32, 255);
    m_textColors[CONTROL_STATE_SELECTING] = nn::util::Color8( 45, 116, 174, 255);
}

// Destructor.
Button::~Button()
{
}

// Processing when pressed with the stylus
void Button::OnPenDown()
{
    m_state = CONTROL_STATE_SELECTING;
}

// Processing when the stylus is lifted
void Button::OnPenUp(bool isIn)
{
    (void)isIn;

    m_state = CONTROL_STATE_DEFAULT;
}

// Processing when sliding with the stylus
void Button::OnPenSlide(bool isIn)
{
    m_state = (isIn ? CONTROL_STATE_SELECTING : CONTROL_STATE_DEFAULT);
}

// Processing when tapped with the stylus
void Button::OnPenTouch()
{
}

// Processing during rendering
void Button::OnDraw()
{
    // Renderer
    demo::RenderSystemDrawing* pRenderSystem = GetRenderSystem();
    // Text writer
    TextWriter* pTextWriter = GetTextWriter();

    // Border
    if (m_borderWidth >= 0.0f)
    {
        nn::util::FloatColor color = m_borderColors[m_state];

        // Render the box
        pRenderSystem->SetColor(color.r, color.g, color.b, color.a);
        scene::util::DrawBox(m_x, m_y, m_width, m_height, m_borderWidth);
    }

    // Text
    if (m_textLength > 0)
    {
        nn::util::Color8 color = m_textColors[m_state];
        // Scale settings
        pTextWriter->SetFontScale(m_TextScale);

        // Text render position
        s32 tx = CalculateTextX(2, 0);
        s32 ty = CalculateTextY(0);

        // Render text
        pTextWriter->SetTextColor(color.r, color.g, color.b);
        pTextWriter->PutString(tx, ty, m_pText);
    }
}

}
