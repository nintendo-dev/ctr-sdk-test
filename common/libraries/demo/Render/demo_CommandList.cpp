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

#include "demo/Render/demo_CommandList.h"

namespace demo {

/* ------------------------------------------------------------------------
	 <tt>CommandList</tt> Class Member Function
------------------------------------------------------------------------ */

CommandList::CommandList(void)
: m_InitializeFlag(false)
, m_SerialRunFlag(false)
, m_StoreCommandListIndex(0)
, m_RunningCommandListIndex(0)
, m_BufferSize(0)
, m_RequestNum(0)
{
	for (u32 index = 0; index < DEMO_COMMAND_LIST_NUM; index++)
	{
		m_CommandListIdArray[index] = 0;
	}
}

CommandList::~CommandList(void)
{
	Finalize();
}

void
CommandList::Initialize(const GLsizei bufferSize, const GLsizei requestNum, const bool serialRunMode)
{
	if (!m_InitializeFlag)
	{
		DEMO_ASSERT_GL_ERROR();

		m_StoreCommandListIndex	  = 0;
		m_RunningCommandListIndex = 0;

		m_BufferSize = bufferSize;
		m_RequestNum = requestNum;

		nngxGenCmdlists(DEMO_COMMAND_LIST_NUM, &m_CommandListIdArray[0]);
		for (u32 index = 0; index < DEMO_COMMAND_LIST_NUM; index++)
		{
			GLuint id = m_CommandListIdArray[index];
			nngxBindCmdlist(id);
			nngxCmdlistStorage(m_BufferSize, m_RequestNum);
			DEMO_ASSERT_GL_ERROR();

			nngxSetCmdlistParameteri(NN_GX_CMDLIST_RUN_MODE, NN_GX_CMDLIST_SERIAL_RUN);
		}
		nngxBindCmdlist(m_CommandListIdArray[m_StoreCommandListIndex]);
		DEMO_ASSERT_GL_ERROR();

		m_SerialRunFlag = serialRunMode;
		if (m_SerialRunFlag)
		{
			nngxRunCmdlist();
		}
		else
		{
			nngxStopCmdlist();
		}
		DEMO_ASSERT_GL_ERROR();

		m_InitializeFlag = true;
	}
}

void
CommandList::Finalize(void)
{
	if (m_InitializeFlag)
	{
		DEMO_ASSERT_GL_ERROR();

		Clear();
		DEMO_ASSERT_GL_ERROR();

		nngxDeleteCmdlists(DEMO_COMMAND_LIST_NUM, &m_CommandListIdArray[0]);
		for (u32 index = 0; index < DEMO_COMMAND_LIST_NUM; index++)
		{
			m_CommandListIdArray[index] = 0;
		}

		DEMO_ASSERT_GL_ERROR();

		m_InitializeFlag = false;
	}
}

void
CommandList::Run(void)
{
	RunSingle();

	DEMO_ASSERT_GL_ERROR();
}

void
CommandList::RunSingle(void)
{
	nngxBindCmdlist(m_CommandListIdArray[m_StoreCommandListIndex]);
	nngxSplitDrawCmdlist();
	nngxRunCmdlist();
	nngxWaitCmdlistDone();
}

void
CommandList::Swap(void)
{
	Clear();

	if (m_SerialRunFlag)
	{
		nngxRunCmdlist();
	}
	else
	{
		nngxStopCmdlist();
	}

	m_RunningCommandListIndex = 0;
	m_StoreCommandListIndex	  = 0;
}

void
CommandList::Stop(void)
{
	nngxBindCmdlist(m_CommandListIdArray[m_RunningCommandListIndex]);
}

void
CommandList::Wait(void)
{
	nngxBindCmdlist(m_CommandListIdArray[m_RunningCommandListIndex]);
	nngxWaitCmdlistDone();

	if (m_SerialRunFlag)
	{
		nngxRunCmdlist();
	}
	else
	{
		nngxStopCmdlist();
	}
}

void
CommandList::Clear(void)
{
	nngxBindCmdlist(m_CommandListIdArray[m_RunningCommandListIndex]);
	nngxClearCmdlist();
}

void
CommandList::SetSerialRun(const bool flag)
{
	m_SerialRunFlag = flag;
}

}  // namespace demo
