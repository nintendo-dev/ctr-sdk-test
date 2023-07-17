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

#include "demo/Render/demo_Light.h"

namespace demo {

Light::Light(void)
{
	Initialize();
}

Light::~Light()
{
}

void
Light::Initialize(void)
{
	m_Ambient[0] = 0.0f;
	m_Ambient[1] = 0.0f;
	m_Ambient[2] = 0.0f;
	m_Ambient[3] = 0.0f;

	m_Diffuse[0] = 0.0f;
	m_Diffuse[1] = 0.0f;
	m_Diffuse[2] = 0.0f;
	m_Diffuse[3] = 0.0f;

	m_Specular0[0] = 0.0f;
	m_Specular0[1] = 0.0f;
	m_Specular0[2] = 0.0f;
	m_Specular0[3] = 0.0f;

	m_Specular1[0] = 0.0f;
	m_Specular1[1] = 0.0f;
	m_Specular1[2] = 0.0f;
	m_Specular1[3] = 0.0f;

	m_Position[0] = 0.0f;
	m_Position[1] = 0.0f;
	m_Position[2] = 0.0f;
	m_Position[3] = 1.0f;
}

}  // namespace demo
