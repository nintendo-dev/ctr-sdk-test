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

#include <nn.h>
#include <nn/dbg.h>
#include <nn/types.h>

#include <nn/fnd/fnd_ExpHeap.h>
#include <nn/os/os_Memory.h>
#include <nn/os/os_MemoryTypes.h>

#include <nn/applet.h>
#include <nn/fs.h>
#include <nn/gx.h>

#include "demo.h"

#include <wchar.h>
#include <cstring>

namespace {
demo::RenderSystemDrawing s_RenderSystem;

nn::fnd::ExpHeap s_AppHeap;
uptr			 s_HeapForGx;
const u32		 s_GxHeapSize = 0x800000;

wchar_t s_kbdText[11];
}  // namespace

void
initialize(void)
{
	s_AppHeap.Initialize(nn::os::GetDeviceMemoryAddress(), nn::os::GetDeviceMemorySize());
	s_HeapForGx = reinterpret_cast<uptr>(s_AppHeap.Allocate(s_GxHeapSize));

	s_RenderSystem.Initialize(s_HeapForGx, s_GxHeapSize);
}

void
finalize(void)
{
	s_RenderSystem.Finalize();

	s_AppHeap.Free(reinterpret_cast<void*>(s_HeapForGx));
	s_AppHeap.Finalize();
}

void
drawDisplay1(void)
{
	s_RenderSystem.SetClearColor(NN_GX_DISPLAY1, 0.0f, 0.0f, 0.0f, 1.0f);
	s_RenderSystem.SetClearColor(NN_GX_DISPLAY0, 0.0f, 0.0f, 0.0f, 1.0f);
	s_RenderSystem.SetRenderTarget(NN_GX_DISPLAY1);
	s_RenderSystem.Clear();

	s_RenderSystem.SetColor(1.0f, 1.0f, 1.0f);
	s_RenderSystem.SetFontSize(8.0f);
	s_RenderSystem.DrawText(0.0f, 0.0f, "Text from keyboard: %ls", s_kbdText);

	s_RenderSystem.SwapBuffers();  // leaving this out will cause the screen to be black, other render target to "bleed" pixels and will
								   // cause applet::CloseApplication() to hang indefinitely
}

void
drawFrame(void)
{
	drawDisplay1();

	s_RenderSystem.WaitVsync(NN_GX_DISPLAY_BOTH);
}

void
validateGx()
{
	// Validate GX state
	nngxUpdateState(NN_GX_STATE_ALL);
	nngxValidateState(NN_GX_STATE_ALL, GL_TRUE);
}

AppletQueryReply
myReceiveSleepQueryNotification(uptr arg)
{
	NN_UNUSED_VAR(arg);
	return nn::applet::REPLY_ACCEPT;
}

void
myReceiveAwakeNotification(uptr arg)
{
	NN_UNUSED_VAR(arg);
	if (nn::applet::IsActive())
	{
		nngxStartLcdDisplay();
	}
}

extern "C" void
nnMain()
{
	nn::Result result;

	nn::applet::SetSleepQueryCallback(myReceiveSleepQueryNotification, 0);
	nn::applet::SetAwakeCallback(myReceiveAwakeNotification, 0);
	nn::applet::Enable();

	if (nn::applet::IsExpectedToCloseApplication())
	{
		// Quickly end the application
		nn::applet::CloseApplication();

		// Never reached
		return;
	}

	NN_UTIL_PANIC_IF_FAILED(nn::hid::Initialize());

	initialize();

	nn::hid::PadReader padReader;
	nn::hid::PadStatus ps;

	while (1)
	{
		padReader.ReadLatest(&ps);

		drawFrame();

		if (ps.trigger & nn::hid::BUTTON_A)
		{
			nn::applet::WakeupState ws;
			nn::swkbd::Parameter	kbd_param;

			nn::swkbd::InitializeConfig(&kbd_param.config);

			kbd_param.config.keyboardType												  = nn::swkbd::KEYBOARD_TYPE_QWERTY;
			kbd_param.config.upperScreenFlag											  = nn::swkbd::UPPER_SCREEN_DARK;
			kbd_param.config.bottomButtonType											  = nn::swkbd::BOTTOM_BUTTON_TYPE_2BUTTON;
			kbd_param.config.textLengthMax												  = 10;
			kbd_param.config.prediction													  = false;
			kbd_param.config.lineFeed													  = false;
			kbd_param.config.bottomButtonToFinish[nn::swkbd::BOTTOM_BUTTON_2BUTTON_RIGHT] = true;
			wcsncpy(kbd_param.config.guideText, L"Please input text.", 19);

			s32 memSize = nn::swkbd::GetSharedMemorySize(&kbd_param.config, NULL, NULL);
			// Allocate shared memory
			const u32 ALIGN		= nn::swkbd::MEMORY_ALIGNMENT;
			void*	  share_buf = std::malloc(memSize + (ALIGN - 1));
			NN_NULL_ASSERT(share_buf);

			if (share_buf)
			{
				void* aligned_share_buf = reinterpret_cast<void*>((reinterpret_cast<u32>(share_buf) + (ALIGN - 1)) & ~(ALIGN - 1));

				// Call keyboard
				nn::swkbd::StartKeyboardApplet(&ws,
											   &kbd_param,
											   aligned_share_buf,  // It must be 4096-byte aligned. Device memory cannot be specified.
											   memSize);		   // Must be a multiple of 4096
				// Recover the GPU register settings
				if (nn::applet::IsExpectedToCloseApplication())
				{
					std::free(share_buf);
					break;
				}
				validateGx();

				// Get the input string
				u8* buf = static_cast<u8*>(aligned_share_buf);
				memset(s_kbdText, 0, sizeof(s_kbdText));

				memcpy(s_kbdText, &buf[kbd_param.config.inputText], kbd_param.config.inputTextLength * sizeof(wchar_t));

				std::free(share_buf);
			}
		}

		if (ps.trigger & nn::hid::BUTTON_START)
			break;

		if (nn::applet::IsExpectedToProcessHomeButton())
		{
			nn::applet::ProcessHomeButtonAndWait();

			if (nn::applet::IsExpectedToCloseApplication())
				break;

			validateGx();
		}

		if (nn::applet::IsExpectedToProcessPowerButton())
		{
			nn::applet::ProcessPowerButtonAndWait();

			if (nn::applet::IsExpectedToCloseApplication())
				break;

			validateGx();
		}

		if (nn::applet::IsExpectedToCloseApplication())
			break;
	}

	finalize();

	nn::hid::Finalize();

	// Close application
	nn::applet::CloseApplication();
}
