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

#ifndef NN_COMMON_SCENE_GUICONTROLMANAGER_H_
#define NN_COMMON_SCENE_GUICONTROLMANAGER_H_

#include "GuiControlBase.h"
#include "List.h"

namespace scene {
/*!
	@brief Control event callback
*/
typedef void (*ControlEventCallback)(ControlEvent event, ControlBase* pControl, s16 x, s16 y, void* pParam);

/*!
	@brief Control list
*/
typedef List<ControlBase*> ControlList;

/*!
	@brief Contains member functions for managing controls.
*/
class ControlManager
{
public:
	/*!
		@brief Constructor.
	*/
	ControlManager();

	/*!
		@brief Destructor.
	*/
	virtual ~ControlManager();

	/*!
		@brief Adds controls.

		@param[in]  pControl  A control.
	*/
	void Add(ControlBase* pControl);

	/*!
		@brief Deletes all controls.
	*/
	void Clear();

	/*!
		@brief Searches for controls.

		@param [in] id ID to search for.

		@return Returns the control.
	*/
	ControlBase* FindBy(u32 id);

	/*!
		@brief Gets the control list.

		@return Returns the control list.
	*/
	List<ControlBase*>* GetList();

	/*!
		@brief Registers the control event callback.

		@param[in]  pCallback  The callback function.
		@param[in]  pParam     Parameters.
	*/
	void RegisterControlEventCallback(ControlEventCallback pCallback, void* pParam)
	{
		m_pCallback = pCallback;
		m_pParam	= pParam;
	}

	/*!
		@brief Updates the object.
	*/
	void Update();

	/*!
		@brief Renders the object.
	*/
	void Draw();

protected:
	// vtable 4byte exists in this position (keep in mind when adjusting padding)
	// ID of active control
	u32 m_activeId;
	// Callbacks
	ControlEventCallback m_pCallback;
	// Callback parameters
	void* m_pParam;

	// Control list
	ControlList m_controlList;
	// Touch panel reader
	nn::hid::TouchPanelReader m_touchReader;
	// Touch panel input information
	nn::hid::TouchPanelStatus m_touchStatus;
	nn::hid::TouchPanelStatus m_touchStatusOld;
};
}  // namespace scene

#endif	// NN_COMMON_SCENE_GUICONTROLMANAGER_H_
