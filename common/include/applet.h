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

#ifndef NN_SAMPLE_DEMOS_DEMO1_APPLET_H_
#define NN_SAMPLE_DEMOS_DEMO1_APPLET_H_

#include <nn/applet.h>
#include <nn/os.h>

class TransitionHandler
{
public:
	typedef void (*TRANSITION_CALLBACK)();

public:
	TransitionHandler() {}
	~TransitionHandler() {}

	/*!
		@brief Initializes the applet.
				Permission for sleep mode occurs with @ref EnableSleep.
	 */
	static void Initialize();
	/*!
		@brief Finalizes the applet.
	 */
	static void Finalize();
	/*!
		@brief Authorizes sleep mode.
	 */
	static void EnableSleep();
	/*!
		@brief Denies sleep mode.
	 */
	static void DisableSleep();
	/*!
		@brief  Performs processes specific to sleep mode, the HOME Button and the POWER Button.

	 */
	static void Process();
	/*!
		@brief  Checks whether the application should be terminated.
	 */
	static bool IsExitRequired();
	/*!
		@brief  Performs an exclusion lock specific to transitions.
	 */
	static void Lock();
	/*!
		@brief  Attempts an exclusion lock specific to transitions.
	 */
	static bool TryLock();
	/*!
		@brief  Releases an exclusion lock specific to transitions.
	 */
	static void Unlock();
	/*!
		@brief  Performs an exclusion lock specific to sleep mode.
	 */
	static void LockForSleep();
	/*!
		@brief  Attempts an exclusion lock specific to sleep mode.
	 */
	static bool TryLockForSleep();
	/*!
		@brief  Releases an exclusion lock specific to sleep mode.
	 */
	static void UnlockForSleep();

	/*!
		@brief  Sets processes to occur before transitioning into sleep mode.
	 */
	static void SetPrepareSleepCallback(TRANSITION_CALLBACK f) { s_PrepareSleepCallback = f; }

	/*!
		@brief  Sets processes to occur after waking up from sleep mode.
	 */
	static void SetAfterSleepCallback(TRANSITION_CALLBACK f) { s_AfterSleepCallback = f; }

	/*!
		@brief  Sets processes to occur before transitioning to the HOME Menu.
	 */
	static void SetPrepareHomeButtonCallback(TRANSITION_CALLBACK f) { s_PrepareHomeButtonCallback = f; }

	/*!
		@brief  Sets processes to occur after returning from the HOME Menu.
	 */
	static void SetAfterHomeButtonCallback(TRANSITION_CALLBACK f) { s_AfterHomeButtonCallback = f; }

	/*!
		@brief  Sets processes to occur before displaying the POWER Menu.
	 */
	static void SetPreparePowerButtonCallback(TRANSITION_CALLBACK f) { s_PreparePowerButtonCallback = f; }

private:
	/*!
		@brief  The callback for SleepQuery.
	 */
	static AppletQueryReply SleepQueryCallback(uptr arg);
	/*!
		@brief  The callback for Awake.
	 */
	static void AwakeCallback(uptr arg);

private:
	static bool s_IsExitRequired;

	// Flag to enable LCD after waiting for one-frame worth of rendering after recovering from sleep
	static volatile bool s_IsAfterWakeUp;

	// Event used for Sleep Mode
	static nn::os::LightEvent s_AwakeEvent;

	// CriticalSection used for mutual exclusion related to transitions and Sleep Mode
	static nn::os::CriticalSection s_CriticalSection;
	static nn::os::CriticalSection s_CriticalSectionForSleep;

	// Process before sleep transition
	static TRANSITION_CALLBACK s_PrepareSleepCallback;
	// Process after recovery from sleep
	static TRANSITION_CALLBACK s_AfterSleepCallback;

	// Process before HOME Menu transition
	static TRANSITION_CALLBACK s_PrepareHomeButtonCallback;
	// Process after return from HOME Menu.
	static TRANSITION_CALLBACK s_AfterHomeButtonCallback;

	// Process before display of POWER Menu
	static TRANSITION_CALLBACK s_PreparePowerButtonCallback;
};

#endif	// NN_SAMPLE_DEMOS_DEMO1_APPLET_H_
