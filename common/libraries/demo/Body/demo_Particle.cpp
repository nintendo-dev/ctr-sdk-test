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

#include "demo/Body/demo_Particle.h"

namespace demo {
/* ------------------------------------------------------------------------
	<tt>Particle</tt> Class Member Function
	------------------------------------------------------------------------ */

bool Particle::s_Initialize = false;

u32				  Particle::s_InitialLifeTime = 30;
f32				  Particle::s_InitialMass	  = 1.0f;
nn::math::Vector3 Particle::s_InitialPosition(0.0f, 0.0f, 0.0f);
nn::math::Vector3 Particle::s_InitialVelocity(0.0f, 0.0f, 0.0f);
nn::math::Vector3 Particle::s_RandomVelocityScale(1.0f, 0.0f, 1.0f);
nn::math::Vector3 Particle::s_Gravity(0.0f, 0.0f, 0.0f);
f32				  Particle::s_InitialRadius = 0.4f;

Particle::Particle(void)
: m_LifeTime(0.0f)
, m_Mass(s_InitialMass)
, m_Position(s_InitialPosition)
, m_Velocity(s_InitialVelocity)
, m_Acceleration(s_Gravity)
, m_Color(1.0f, 0.0f, 0.0f)
, m_Sphere()
{
	if (!s_Initialize)
	{
		::std::srand(0x1234);

		s_Initialize = true;
	}
}

Particle::~Particle()
{
}

void
Particle::Initialize(void)
{
	u32 vertexAttributes =
		demo::VERTEX_POSITION_ATTRIBUTE | demo::VERTEX_COLOR_ATTRIBUTE | demo::VERTEX_NORMAL_ATTRIBUTE | demo::VERTEX_TEXCOORD_ATTRIBUTE;
	InitializeParticle(vertexAttributes, s_InitialRadius, 2);
}

void
Particle::InitializeParticle(const u32 vertexAttributes, const f32 radius, const u32 division)
{
	m_Sphere.InitializeSphere(vertexAttributes, radius, division);

	f32 red	  = static_cast<f32>(::std::rand() % 256) / 256.0f;
	f32 green = static_cast<f32>(::std::rand() % 256) / 256.0f;
	f32 blue  = static_cast<f32>(::std::rand() % 256) / 256.0f;
	m_Sphere.SetColor(red, green, blue);

	m_Acceleration = s_Gravity;

	Spawn();
}

void
Particle::Finalize(void)
{
	m_Sphere.Finalize();
}

void
Particle::Update(const f32 time)
{
	m_LifeTime -= time;
	if (m_LifeTime < 0)
	{
		Spawn();
	}
	else
	{
		for (u32 index = 0; index < 3; index++)
		{
			nn::math::Vector3 deltaVec;
			nn::math::VEC3Scale(&deltaVec, &m_Acceleration, time / m_Mass);
			m_Velocity += deltaVec;
			nn::math::VEC3Scale(&deltaVec, &m_Velocity, time);
			m_Position += deltaVec;
		}
		m_Sphere.SetWorldPosition(m_Position.x, m_Position.y, m_Position.z);
	}
}

void
Particle::Spawn(void)
{
	m_LifeTime = static_cast<f32>(::std::rand() % s_InitialLifeTime);

	m_Position	 = s_InitialPosition;
	m_Velocity.x = s_RandomVelocityScale.x * (static_cast<f32>(::std::rand() % 256) / 256.0f - 0.5f) + s_InitialVelocity.x;
	m_Velocity.y = s_RandomVelocityScale.y * (static_cast<f32>(::std::rand() % 256) / 256.0f - 0.5f) + s_InitialVelocity.y;
	m_Velocity.z = s_RandomVelocityScale.z * (static_cast<f32>(::std::rand() % 256) / 256.0f - 0.5f) + s_InitialVelocity.z;
}

nn::math::MTX44
Particle::GetWorldMatrix(void) const
{
	return m_Sphere.GetWorldMatrix();
}

void
Particle::Draw(void)
{
	m_Sphere.Draw();
}

}  // namespace demo
