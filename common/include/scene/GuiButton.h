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

#ifndef NN_COMMON_SCENE_GUIBUTTON_H_
#define NN_COMMON_SCENE_GUIBUTTON_H_

#include "GuiControlBase.h"

namespace scene {
/*!
	@brief The button control class.
*/
class Button : public ControlBase
{
public:
	/*!
		@brief Instantiates the object (constructor).

		@param[in]  id          The control ID
		@param[in] x  Specifies the x-coordinate.
		@param[in] y  Specifies the y-coordinate.
		@param[in] width   The width.
		@param[in]  height          The height.
		@param[in]  pText       The caption string.
		@param[in]  pExtraData  The expanded data.
	*/
	Button(u32			  id,
		   s32			  x,
		   s32			  y,
		   u32			  width,
		   u32			  height,
		   const wchar_t* pText		 = NULL,
		   void*		  pExtraData = NULL,
		   f32			  fontScale	 = DEFAULT_FONT_SCALE);

	/*!
		@brief The destructor.
	*/
	virtual ~Button();

protected:
	// Processing when pressed with the stylus
	virtual void OnPenDown();
	// Processing when the stylus is lifted
	virtual void OnPenUp(bool isIn);
	// Processing when sliding with the stylus
	virtual void OnPenSlide(bool isIn);
	// Processing when tapped with the stylus
	virtual void OnPenTouch();
	// Processing during rendering
	virtual void OnDraw();
};
}  // namespace scene

#endif	// NN_COMMON_SCENE_GUIBUTTON_H_
