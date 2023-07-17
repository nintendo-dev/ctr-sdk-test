/*---------------------------------------------------------------------------*
  Project:  Horizon

  Copyright (C)Nintendo Co., Ltd.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Rev: 58466 $
 *---------------------------------------------------------------------------*/

#include "applet.h"

namespace {
void
RestoreGraphicSetting()
{
	// Recover the GPU register settings
	// However, the following is the method for recovering the state set using DMPGL API.
	// If using other libraries, it is necessary to reissue all of the register  setting commands with their own methods.
	nngxUpdateState(NN_GX_STATE_ALL);

	// (Reference) Recovering settings with the GD library
	// nn::gd::System::ForceDirty(nn::gd::MODULE_ALL);

	// (Reference) Recovering settings with the GR library
	// The GR API only creates commands; it does not have a feature for recovering settings.
	// When rendering next time, run all of the necessary commands.
}
}  // namespace

bool					TransitionHandler::s_IsExitRequired = false;
volatile bool			TransitionHandler::s_IsAfterWakeUp	= false;
nn::os::LightEvent		TransitionHandler::s_AwakeEvent;
nn::os::CriticalSection TransitionHandler::s_CriticalSection;
nn::os::CriticalSection TransitionHandler::s_CriticalSectionForSleep;

TransitionHandler::TRANSITION_CALLBACK TransitionHandler::s_PrepareSleepCallback;
TransitionHandler::TRANSITION_CALLBACK TransitionHandler::s_AfterSleepCallback;
TransitionHandler::TRANSITION_CALLBACK TransitionHandler::s_PrepareHomeButtonCallback;
TransitionHandler::TRANSITION_CALLBACK TransitionHandler::s_AfterHomeButtonCallback;
TransitionHandler::TRANSITION_CALLBACK TransitionHandler::s_PreparePowerButtonCallback;

void
TransitionHandler::Initialize()
{
	// Set sleep-related callbacks
	nn::applet::SetSleepQueryCallback(SleepQueryCallback, 0);
	nn::applet::SetAwakeCallback(AwakeCallback, 0);
	//     nn::applet::SetSleepCanceledCallback(NULL, 0); // Recommend not using SleepCanceledCallback

	s_AwakeEvent.Initialize(true);
	s_CriticalSection.Initialize();
	s_CriticalSectionForSleep.Initialize();

	// During wake-up, always set to Signal state
	s_AwakeEvent.Signal();

	// Enable features related to the applet library.
	// If you set 'false' for the argument, then sleep will be rejected automatically until nn::applet::EnableSleep() is called.
	//
	nn::applet::Enable(false);

	// The Initialize function for each of the gx, snd, and dsp libraries must be called after the applet::Enable function
	// In particular, the nngxInitialize function, which initializes the GX library, should be called after determining whether to exit
	// immediately following a call to the Enable function.

	// The application terminates here if the termination conditions are already established, such as by pressing the POWER Button while
	// displaying the logo
	if (nn::applet::IsExpectedToCloseApplication())
	{
		s_IsExitRequired = true;
	}
}

void
TransitionHandler::Finalize()
{
	s_AwakeEvent.Finalize();
	s_CriticalSection.Finalize();
	s_CriticalSectionForSleep.Finalize();

	nn::applet::SetSleepQueryCallback(NULL, 0);
	nn::applet::SetAwakeCallback(NULL, 0);
}

void
TransitionHandler::EnableSleep()
{
	// Enable responses to sleep requests.
	// Check the system state. If the system has been closed, issue a sleep request.
	nn::applet::EnableSleep(true);
}

void
TransitionHandler::DisableSleep()
{
	// Reject sleep.
	// Return REJECT if a sleep request has come already.
	nn::applet::DisableSleep(true);
}

void
TransitionHandler::Process()
{
	// When waking up from sleep, enable LCD after waiting for one-frame worth of rendering to complete
	if (TransitionHandler::s_IsAfterWakeUp)
	{
		nn::gx::StartLcdDisplay();
		TransitionHandler::s_IsAfterWakeUp = false;
	}

	// If the consistency of the GPU register settings is taken into account, the responses to various transitions and sleep requests should
	// be done after rendering has ended.
	//

	// Determination for System Sleep Mode
	if (nn::applet::IsExpectedToReplySleepQuery())
	{
		// If you any reason REJECT is returned for SleepQuery, make the determination here.
		// Return REJECT and exit from this function.

		// Do not enter sleep during file system processing
		if (TryLockForSleep())
		{
			// Perform the Sleep Mode preprocessing here as required
			if (s_PrepareSleepCallback)
			{
				s_PrepareSleepCallback();
			}

			// Clears AwakeEvent immediately before permitting Sleep Mode
			s_AwakeEvent.ClearSignal();

			// Permits Sleep Mode, and stops the main thread immediately
			nn::applet::ReplySleepQuery(nn::applet::REPLY_ACCEPT);
			s_AwakeEvent.Wait();

			// Perform the processing when recovering from Sleep Mode here as required
			if (s_AfterSleepCallback)
			{
				s_AfterSleepCallback();
			}

			UnlockForSleep();
		}
	}

	// Check termination request
	if (nn::applet::IsExpectedToCloseApplication())
	{
		s_IsExitRequired = true;
		return;
	}

	// Performs application transition processing

	// HOME Button processing (the graphics library must already be initialized)
	if (nn::applet::IsExpectedToProcessHomeButton())
	{
		// If exiting the block by determining with flags or other things here, the HOME Button prohibition interval can be implemented
		// HOME Button flag must be deleted with nn::applet::ClearHomeButtonState function

		if (TryLock())
		{
			if (s_PrepareHomeButtonCallback)
			{
				s_PrepareHomeButtonCallback();
			}

			nn::applet::ProcessHomeButtonAndWait();

			Unlock();

			// Caused by exiting from the HOME Menu or low batteries
			// If it is determined to end the application, exit from the application main loop and go to termination processing
			if (nn::applet::IsExpectedToCloseApplication())
			{
				// No render rights are passed
				s_IsExitRequired = true;

				if (s_AfterHomeButtonCallback)
				{
					s_AfterHomeButtonCallback();
				}
				return;
			}

			if (s_AfterHomeButtonCallback)
			{
				s_AfterHomeButtonCallback();
			}

			// Restore graphics settings
			RestoreGraphicSetting();
		}
	}

	// POWER Button processing (the graphics library must already be initialized)
	if (nn::applet::IsExpectedToProcessPowerButton())
	{
		// Quickly handle the POWER Button, but as for the other tasks that should be done before the application ends, (such as dealing
		// with save data) should be done after nn::applet::ProcessPowerButtonAndWait() returns.
		//
		//
		if (s_PreparePowerButtonCallback)
		{
			s_PreparePowerButtonCallback();
		}

		nn::applet::ProcessPowerButtonAndWait();

		// If it is determined to end the application, exit from the application main loop and go to termination processing
		if (nn::applet::IsExpectedToCloseApplication())
		{
			// No render rights are passed
			s_IsExitRequired = true;
			return;
		}

		// Restore graphics settings
		RestoreGraphicSetting();
	}
}

bool
TransitionHandler::IsExitRequired()
{
	return s_IsExitRequired;
}

void
TransitionHandler::Lock()
{
	s_CriticalSection.Enter();
}

bool
TransitionHandler::TryLock()
{
	return s_CriticalSection.TryEnter();
}

void
TransitionHandler::Unlock()
{
	s_CriticalSection.Leave();
}

void
TransitionHandler::LockForSleep()
{
	s_CriticalSectionForSleep.Enter();
}

bool
TransitionHandler::TryLockForSleep()
{
	return s_CriticalSectionForSleep.TryEnter();
}

void
TransitionHandler::UnlockForSleep()
{
	s_CriticalSectionForSleep.Leave();
}

/*------------------------------------------------------------------------*
	Callback called when there is a sleep query
 *------------------------------------------------------------------------*/
AppletQueryReply
TransitionHandler::SleepQueryCallback(uptr arg)
{
	NN_UNUSED_VAR(arg);

	if (!nn::applet::IsActive())
	{
		// When Inactive, the main thread is stopped on the applet::WaitForStarting function,
		// and other threads also should be stopped. (Implementation-dependent on the application side.)
		return nn::applet::REPLY_ACCEPT;
	}
	else
	{
		// The main thread is suspended here because it will sleep after the necessary processing completes
		//
		return nn::applet::REPLY_LATER;
	}
}

/*------------------------------------------------------------------------*
	Callback called when recovering from sleep
 *------------------------------------------------------------------------*/
void
TransitionHandler::AwakeCallback(uptr arg)
{
	NN_UNUSED_VAR(arg);
	s_AwakeEvent.Signal();

	s_IsAfterWakeUp = true;
}
