/*---------------------------------------------------------------------------*
  Project:  Horizon
  File:     demo_RenderSystemExt.h

  Copyright (C)2009-2013 Nintendo Co., Ltd.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Rev: 50863 $
 *---------------------------------------------------------------------------*/

#ifndef DEMO_RENDER_SYSTEM_AA_H_
#define DEMO_RENDER_SYSTEM_AA_H_

#include "demo/Render/demo_RenderSystemExt.h"

namespace demo {
class RenderSystemAA : public RenderSystemExt
{
public:
	enum AAMode
	{
		AA_NONE,
		AA_2x1,
		AA_2x2
	};

public:
	RenderSystemAA(AAMode mode);

public:
	virtual void Initialize(
		const uptr						 fcramAddress,
		const size_t					 memorySize,
		const u32						 commandBufferSize		= 0x40000,
		const u32						 requestNum				= 128,
		const bool						 serialRunMode			= true,
		const DisplayBuffersDescription& displayBuffers0Desc	= DisplayBuffersDescription::GetDefaultDisplay0Description(),
		const DisplayBuffersDescription& displayBuffers1Desc	= DisplayBuffersDescription::GetDefaultDisplay1Description(),
		const FrameBufferDescription&	 frameBuffer0Desc		= FrameBufferDescription::GetDefaultDisplay0FrameBufferDescription(),
		const DisplayBuffersDescription& displayBuffers0ExtDesc = DisplayBuffersDescription::GetDefaultDisplay0ExtDescription(),
		const bool						 isFillBlackLCD			= true);

public:
	virtual void SetRenderTarget(const GLenum targetDisplay);
	virtual void Transfer(void);

	void SetUnitViewport(f32 Left, f32 Right, f32 Bottom, f32 Top);

protected:
	f32 m_Left;
	f32 m_Right;
	f32 m_Bottom;
	f32 m_Top;

	AAMode m_AAMode;
	NN_PADDING3;
};

}  // namespace demo

#endif
