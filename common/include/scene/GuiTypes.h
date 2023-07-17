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

#ifndef NN_COMMON_SCENE_GUITYPES_H_
#define NN_COMMON_SCENE_GUITYPES_H_

#include <nn/types.h>

namespace scene {
/*!
	@brief Unused control ID
*/
static const u32 CONTROL_ID_UNUSED = 0xFFFFFFFF;

/*!
	@brief Control type
*/
enum ControlType
{
	CONTROL_TYPE_LABEL,	  //!< Labels
	CONTROL_TYPE_BUTTON,  //!< Button

	CONTROL_TYPE_MAX
};

/*!
	@brief Control state
*/
enum ControlState
{
	CONTROL_STATE_DEFAULT,	  //!< Normal
	CONTROL_STATE_DISABLED,	  //!< Disabled.
	CONTROL_STATE_SELECTING,  //!< Selecting
	CONTROL_STATE_HIDE,		  //!< Hide

	CONTROL_STATE_MAX
};

/*!
	@brief Control event
*/
enum ControlEvent
{
	CONTROL_EVENT_PENDOWN,		 //!< Pressed with the stylus
	CONTROL_EVENT_PENUP,		 //!< The stylus was released
	CONTROL_EVENT_SLIDE,		 //!< Slid
	CONTROL_EVENT_TOUCH,		 //!< Touched
	CONTROL_EVENT_CHANGE_STATE,	 //!< The status changed

	CONTROL_EVENT_MAX
};

/*!
	@brief Text placement
*/
enum TextAlign
{
	TEXT_ALIGN_LEFT,	//!< Left aligned
	TEXT_ALIGN_RIGHT,	//!< Right aligned
	TEXT_ALIGN_CENTER,	//!< Center aligned

	TEXT_ALIGN_MAX
};

//! @brief Font scale default values
const f32 DEFAULT_FONT_SCALE = 0.6f;
}  // namespace scene

#endif	// NN_COMMON_SCENE_GUITYPES_H_
