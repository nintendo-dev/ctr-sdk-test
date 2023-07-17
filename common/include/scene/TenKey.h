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

#ifndef NN_COMMON_SCENE_TENKEY_H_
#define NN_COMMON_SCENE_TENKEY_H_

#include <nn.h>
#include <nn/util/util_SizedEnum.h>
#include "GuiControlManager.h"

namespace scene {

class TenKey
{
public:
	enum State
	{
		STATE_NORMAL,	  // Normal
		STATE_OK,		  // OK
		STATE_CANCELLED,  // Cancel.

		STATE_MAX
	};

public:
	/*!
		@brief Instantiates the object (constructor).
	*/
	TenKey();

	/*!
		@brief The destructor.
	*/
	virtual ~TenKey();

	/*!
		@brief      Initializes the class instance.

		@param[in] length       The number of characters.
		@param[in]  split           The split position. @n
									A hyphen (-) will be inserted at the specified value's position.
		@param[in]  pInitial        The initial value. @n
									When the length is longer than length, the exceeding part will be thrown away.
		@param[in]  numLowestChars  The minimum number of characters. @n
									You will not be able to press the OK button until at least numLowestChars number of characters have been
	   entered.
		@param[in]  isPassword      Hide the password? @n
									When TRUE, entered characters will be displayed as asterisks (*).
	*/
	void Initialize(u32 length, u32 split = 0, const wchar_t* pInitial = NULL, u32 numLowestChars = 1, bool isPassword = false);

	/*!
		@brief Finalizes the object.
	*/
	void Finalize();

	/*!
		@brief Updates the object.
	*/
	void Update();

	/*!
		@brief Renders the object.
	*/
	void Draw();

	/*!
		@brief Resets the state.

		@param[in]  pInitial        The initial value. @n
							  When the length is longer than the size of the input characters, the exceeding part will be thrown away.
	*/
	void Reset(const wchar_t* pInitial = NULL);

	/*!
		@brief Sets the state.

		@param[in] state  Status.
	*/
	void SetState(State state) { m_state = state; }

	/*!
		@brief Gets whether the OK button has been pressed.

		@return Returns TRUE if the OK button has been pressed.
	*/
	bool IsOK() const { return (m_state == STATE_OK); }

	/*!
		@brief Gets whether the Cancel button has been pressed.

		@return Returns TRUE if the Cancel button has been pressed.
	*/
	bool IsCancelled() const { return (m_state == STATE_CANCELLED); }

	/*!
		@brief Gets the entered string.

		@return Returns the entered string.
	*/
	const wchar_t* GetInput() const { return m_pInput; }

	/*!
		@brief Gets the entered numerical value.

		@return Returns the entered numerical value.
	*/
	u32 GetInputInteger() const;

	/*!
		@brief Gets the cursor position.

		@return Returns the cursor position.
	*/
	u32 GetCursor() const { return m_cursor; }

protected:
	// Event callback
	static void MyControlEventCallback(scene::ControlEvent event, scene::ControlBase* pControl, s16 x, s16 y, void* pParam);

protected:
	// vtable 4byte exists in this position (keep in mind when adjusting padding)

	// Input characters
	wchar_t* m_pInput;
	// Number of characters
	u32 m_length;
	// Separation position
	u32 m_split;
	// Minimum number of characters
	u32 m_numLowestChars;
	// Cursor position.
	u32 m_cursor;
	// Status.
	nn::util::SizedEnum4<State> m_state;
	// Password?
	bool m_isPassword;
	NN_PADDING3;
	// GUI manager
	scene::ControlManager m_controlManager;
};

}  // namespace scene

#endif	// NN_COMMON_SCENE_TENKEY_H_
