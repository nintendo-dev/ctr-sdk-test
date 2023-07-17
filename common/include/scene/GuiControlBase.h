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

#ifndef NN_COMMON_SCENE_GUICONTROLBASE_H_
#define NN_COMMON_SCENE_GUICONTROLBASE_H_

#include <nn/util/util_SizedEnum.h>
#include "GuiTypes.h"
#include "GuiUtil.h"

namespace scene {
class ControlManager;

/*!
	@brief This is the base class for all controls.
*/
class ControlBase
{
public:
	friend class ControlManager;

public:
	/*!
		@brief Instantiates the object (constructor).

		@param[in]  type        The control type.
		@param[in]  id          The control ID
		@param[in] x  Specifies the x-coordinate.
		@param[in] y  Specifies the y-coordinate.
		@param[in] width   The width.
		@param[in]  height          The height.
		@param[in]  pText       The caption string.
		@param[in]  pExtraData  The expanded data.
	*/
	ControlBase(ControlType type, u32 id, s32 x, s32 y, u32 width, u32 height, const wchar_t* pText, void* pExtraData, f32 fontScale);

	/*!
		@brief The destructor.
	*/
	virtual ~ControlBase();

	/*!
		@brief Gets the control type.
	*/
	ControlType GetType() const { return m_type; }

	/*!
		@brief      Gets the control ID.
	*/
	u32 GetId() const { return m_id; }

	/*!
		@brief Gets the control's x-coordinate.
	*/
	s32 GetX() const { return m_x; }

	/*!
		@brief Gets the control's y-coordinate.
	*/
	s32 GetY() const { return m_y; }

	/*!
		@brief Gets the control width.
	*/
	u32 GetWidth() const { return m_width; }

	/*!
		@brief Gets the control height.
	*/
	u32 GetHeight() const { return m_height; }

	/*!
		@brief Gets the text.
	*/
	const wchar_t* GetText() const { return m_pText; }

	/*!
		@brief Gets the text length.
	*/
	s32 GetTextLength() const { return m_textLength; }

	/*!
		@brief Gets the color for the frame lines.
	*/
	nn::util::Color8 GetBorderColor(ControlState state) const
	{
		if (state < CONTROL_STATE_MAX)
		{
			return m_borderColors[state];
		}

		return nn::util::Color8();
	}

	/*!
		@brief Gets the character color.
	*/
	nn::util::Color8 GetTextColor(ControlState state) const
	{
		if (state < CONTROL_STATE_MAX)
		{
			return m_textColors[state];
		}

		return nn::util::Color8();
	}

	/*!
		@brief Gets the text placement.
	*/
	TextAlign GetTextAlign() const { return m_textAlign; }

	/*!
		@brief Gets the frame line width.
	*/
	f32 GetBorderWidth() const { return m_borderWidth; }

	/*!
		@brief Gets the control state.
	*/
	ControlState GetState() const { return m_state; }

	/*!
		@brief Gets expanded data.
	*/
	void* GetExtraData() const { return m_pExtraData; }

	/*!
		@brief Gets the control manager that will manage the local host.
	*/
	ControlManager* GetControlManager() const { return m_pManager; }

	/*!
		@brief Sets the control's x-coordinate.
	*/
	void SetX(s32 x) { m_x = x; }

	/*!
		@brief Sets the control's y-coordinate.
	*/
	void SetY(s32 y) { m_y = y; }

	/*!
		@brief Sets the control width.
	*/
	void SetWidth(u32 width)
	{
		if (width > 0)
		{
			m_width = width;
		}
	}

	/*!
		@brief Sets the control height.
	*/
	void SetHeight(u32 height)
	{
		if (height > 0)
		{
			m_height = height;
		}
	}

	/*!
		@brief Sets the text.
	*/
	void SetText(const wchar_t* pText);

	/*!
		@brief Sets the text.
	*/
	void SetFormattedText(const wchar_t* pFormat, ...);

	/*!
		@brief Sets the color for the frame lines.
	*/
	void SetBorderColor(ControlState state, nn::util::Color8 color)
	{
		if (state < CONTROL_STATE_MAX)
		{
			m_borderColors[state] = color;
		}
	}

	/*!
		@brief Sets the character color.
	*/
	void SetTextColor(ControlState state, nn::util::Color8 color)
	{
		if (state < CONTROL_STATE_MAX)
		{
			m_textColors[state] = color;
		}
	}

	/*!
		@brief Sets the text placement.
	*/
	void SetTextAlign(TextAlign textAlign) { m_textAlign = textAlign; }

	/*!
		@brief Sets the width of the frame lines.
	*/
	void SetBorderWidth(f32 borderWidth) { m_borderWidth = borderWidth; }

	/*!
		@brief Sets the control state.
	*/
	void SetState(ControlState state)
	{
		if (state < CONTROL_STATE_MAX)
		{
			m_state = state;
		}
	}

	/*!
		@brief Sets expanded data.
	*/
	void SetExtraData(void* pExtraData) { m_pExtraData = pExtraData; }

	/*!
		@brief Sets the control manager that will manage the local host.
	*/
	void SetControlManager(ControlManager* pManager) { m_pManager = pManager; }

protected:
	// Calculate the start position for text rendering.
	s32 CalculateTextX(s32 offsetL, s32 offsetR);
	// Calculate the start position for text rendering.
	s32 CalculateTextY(s32 offset);

	// Processing when pressed with the stylus
	virtual void OnPenDown() = 0;
	// Processing when the stylus is lifted
	virtual void OnPenUp(bool isIn) = 0;
	// Processing when sliding with the stylus
	virtual void OnPenSlide(bool isIn) = 0;
	// Processing when tapped with the stylus
	virtual void OnPenTouch() = 0;
	// Processing during rendering
	virtual void OnDraw() = 0;

protected:
	// Control type
	nn::util::SizedEnum4<ControlType> m_type;
	// Control ID
	u32 m_id;
	// Coordinates
	s32 m_x;
	s32 m_y;
	// Size.
	u32 m_width;
	u32 m_height;
	// Text
	wchar_t* m_pText;
	// Text length
	s32 m_textLength;
	s32 m_textBufferLength;
	// Text scale
	f32 m_TextScale;
	// Text size
	f32 m_textWidth;
	f32 m_textHeight;
	// Border color
	nn::util::Color8 m_borderColors[CONTROL_STATE_MAX];
	// Font color
	nn::util::Color8 m_textColors[CONTROL_STATE_MAX];
	// Text placement
	nn::util::SizedEnum4<TextAlign> m_textAlign;
	// Border size
	f32 m_borderWidth;
	// Control state
	nn::util::SizedEnum4<ControlState> m_state;
	// Expanded data
	void* m_pExtraData;
	// Control manager
	ControlManager* m_pManager;
};
}  // namespace scene

#endif	// NN_COMMON_SCENE_GUICONTROLBASE_H_
