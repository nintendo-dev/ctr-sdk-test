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

#ifndef NN_COMMON_SCENE_GUIUTIL_H_
#define NN_COMMON_SCENE_GUIUTIL_H_

#include <demo.h>
#include <nn.h>
#include <string.h>
#include "TextWriter.h"

namespace scene {
/*!
	@brief Record the render object.

	@param[in]  pRenderSystem  Renderer that uses the GUI library.
	@param[in]  pTextWriter    Text writer that uses the GUI library.
*/
void RegisterDrawers(demo::RenderSystemDrawing* pRenderSystem, TextWriter* pTextWriter);

/*!
	@brief Get the renderer used by the GUI library.

	@return       Renderer.
*/
demo::RenderSystemDrawing* GetRenderSystem();

/*!
	@brief Get the text writer used by the GUI library.

	@return       Text writer.
*/
TextWriter* GetTextWriter();

namespace util {
	/*!
		@brief Draws a box with line segments.

		@param[in] x       Specifies the x-coordinate of the box.
		@param[in] y        Specifies the y-coordinate of the box.
		@param[in]  width   The box's width.
		@param[in]  height    The box's height.
		@param[in]  lineWidth  The line width.
	*/
	void DrawBox(f32 x, f32 y, f32 width, f32 height, f32 lineWidth = 1.0f);
}  // namespace util
}  // namespace scene

#endif	// NN_COMMON_SCENE_GUIUTIL_H_
