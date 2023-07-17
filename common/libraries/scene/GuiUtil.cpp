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

#include "GuiUtil.h"

namespace
{
    // Renderer
    demo::RenderSystemDrawing* s_pRenderSystem = NULL;
    // Text writer
    scene::TextWriter* s_pTextWriter = NULL;
}

namespace scene
{

// Record the render object.
void RegisterDrawers(demo::RenderSystemDrawing* pRenderSystem, TextWriter* pTextWriter)
{
    s_pRenderSystem = pRenderSystem;
    s_pTextWriter   = pTextWriter;
}

// Get the renderer used by the GUI library.
demo::RenderSystemDrawing* GetRenderSystem()
{
    return s_pRenderSystem;
}

// Get the text writer used by the GUI library.
TextWriter* GetTextWriter()
{
    return s_pTextWriter;
}

// Render box with line segments.
void util::DrawBox(f32 x, f32 y, f32 width, f32 height, f32 lineWidth)
{
    // Set the line width
    s_pRenderSystem->SetLineWidth(lineWidth);
    // Upper left -> upper right
    s_pRenderSystem->DrawLine(x, y, x + width, y);
    // Upper right -> lower right
    s_pRenderSystem->DrawLine(x + width, y, x + width, y + height);
    // Lower right -> lower left
    s_pRenderSystem->DrawLine(x + width, y + height, x, y + height);
    // Lower left -> upper left
    s_pRenderSystem->DrawLine(x, y + height, x, y);
}

} // namespace scene
