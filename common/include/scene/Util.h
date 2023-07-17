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

#ifndef NN_COMMON_SCENE_UTIL_H_
#define NN_COMMON_SCENE_UTIL_H_

#include <nn.h>
#include "Gui.h"
#include "TextWriter.h"

#define AOC_PANIC_IF_FAILED(result) NN_UTIL_PANIC_IF_FAILED(result)

namespace scene {
/*!
	@brief Get the length of a wide string.
*/
u32 wstrlen(const wchar_t* pText);

/*!
	@brief Convert a wide string to a numerical value.
*/
u32 wtoi(const wchar_t* pText);

/*!
	@brief Initializes memory.
*/
void SetupMemory(u32 deviceMemorySize);

/*!
	@brief Frees memory.
*/
void ReleaseMemory();

/*!
	@brief Gets the heap.
*/
nn::fnd::ExpHeap* GetHeap();

/*!
	@brief Allocates memory.
*/
void* Allocate(size_t size, s32 alignment = 4);

/*!
	@brief Frees memory.
*/
void Free(void* pBuffer);

/*!
	@brief Draws a title.
*/
void SetTitle(const wchar_t* pTitle, size_t length);
void DrawTitle(TextWriter* pTextWriter);
void DrawTitleForApplet(TextWriter* pTextWriter);
void DrawTitleForApplication(TextWriter* pTextWriter);

/*!
	@brief Render the scene ID.
*/
void DrawSceneId(TextWriter* pTextWriter, const wchar_t* pSceneId);

/*!
	@brief Draws the result.
*/
void DrawResult(TextWriter* pTextWriter, const nn::Result& result);

/*!
	@brief Renders the busy icon.
*/
void DrawBusyIcon(TextWriter* pTextWriter, u16 x, u16 y, u32 counter);

/*!
	@brief Render the confirmation message.
*/
void DrawConfirmMessage(TextWriter* pTextWriter, const wchar_t* pMessage1, const wchar_t* pMessage2);

/*!
	@brief Get the DateTime from the UNIX time.
*/
nn::fnd::DateTime GetDateTimeFromUnixTime(s64 unixTime);

/*!
	@brief Add the left button.
*/
void AddLButton(scene::ControlManager* pManager, u32 id, const wchar_t* pCaption);

/*!
	@brief Add the right button.
*/
void AddRButton(scene::ControlManager* pManager, u32 id, const wchar_t* pCaption);

/*!
	@brief Add the YES/NO button.
*/
void AddYesNoButtons(scene::ControlManager* pManager,
					 u32					idYes,
					 u32					idNo,
					 const wchar_t*			pCaptionYes = L"Yes",
					 const wchar_t*			pCaptionNo	= L"No");

}  // namespace scene

#endif	// NN_COMMON_SCENE_UTIL_H_
