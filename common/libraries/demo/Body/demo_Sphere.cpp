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

#include "demo/Body/demo_Sphere.h"

namespace demo {
/* ------------------------------------------------------------------------
	<tt>Sphere</tt> Class Member Function
------------------------------------------------------------------------ */

Sphere::Sphere(void) : Body(), m_Radius(0.5f), m_Division(3)
{
}

Sphere::~Sphere()
{
}

void
Sphere::Initialize(void)
{
	u32 vertexAttributes = demo::VERTEX_POSITION_ATTRIBUTE | demo::VERTEX_COLOR_ATTRIBUTE | demo::VERTEX_NORMAL_ATTRIBUTE;
	InitializeSphere(vertexAttributes, 0.5f, 10);
}

void
Sphere::InitializeSphere(const u32 vertexAttributes, const f32 radius, const u32 division)
{
	if (division == 0)
	{
		NN_TPANIC_("division is 0.\n");
	}

	TrianglesRenderData::Initialize();
	DEMO_ASSERT_GL_ERROR();

	m_Radius   = radius;
	m_Division = division;

	u32 circleVerticesNum = 4 * m_Division;
	u32 verticesNum		  = circleVerticesNum * circleVerticesNum;
	u32 trianglesNum	  = 2 * verticesNum;
	InitializeVertexBuffers(vertexAttributes, GL_TRIANGLES, verticesNum, trianglesNum);

	SetPackedVerticesNum(verticesNum);
	SetPackedTrianglesNum(trianglesNum);
	SetPackedArraysSize();

	if (m_VertexAttributes & demo::VERTEX_POSITION_ATTRIBUTE)
	{
		u32 vertexIndex = 0;
		f32 deltaTheta	= 360.0f / (m_Division * 4.0f);
		f32 deltaPhi	= 360.0f / (m_Division * 4.0f);

		for (u32 phiIndex = 0; phiIndex < circleVerticesNum; phiIndex++)
		{
			f32 phi = deltaPhi * phiIndex;
			for (u32 thetaIndex = 0; thetaIndex < circleVerticesNum; thetaIndex++)
			{
				f32 sinPhi = 0.0f;
				f32 cosPhi = 0.0f;
				nn::math::SinCosDeg(&sinPhi, &cosPhi, phi);

				f32 sinTheta = 0.0f;
				f32 cosTheta = 0.0f;
				f32 theta	 = deltaTheta * thetaIndex;
				nn::math::SinCosDeg(&sinTheta, &cosTheta, theta);

				nn::math::VEC3 position(m_Radius * cosPhi * sinTheta, m_Radius * cosTheta, m_Radius * sinPhi * sinTheta);

				if (m_VertexAttributes & demo::VERTEX_NORMAL_ATTRIBUTE)
				{
					nn::math::VEC3 normal(position);
					nn::math::VEC3Normalize(&normal, &normal);
					SetNormal(vertexIndex, normal.x, normal.y, normal.z);
				}
				SetPosition(vertexIndex, position.x, position.y, position.z);

				vertexIndex += 1;
			}
		}
	}

	if (m_VertexAttributes & demo::VERTEX_COLOR_ATTRIBUTE)
	{
		demo::TrianglesRenderData::SetColor(1.0f, 0.0f, 0.0f, 1.0f);
	}

	if (m_VertexAttributes & demo::VERTEX_TEXCOORD_ATTRIBUTE)
	{
		u32 vertexIndex = 0;
		f32 deltaST		= 1.0f / circleVerticesNum;
		for (u32 phiIndex = 0; phiIndex < circleVerticesNum; phiIndex++)
		{
			f32 s = deltaST * phiIndex;

			for (u32 thetaIndex = 0; thetaIndex < circleVerticesNum; thetaIndex++)
			{
				f32 t = deltaST * thetaIndex;

				SetTexcoord(vertexIndex, s, t);
				vertexIndex += 1;
			}
		}
	}

	if (m_VertexAttributes & demo::VERTEX_TANGENT_ATTRIBUTE)
	{
		NN_TPANIC_("Tangent attribute is not implemented.");
	}

	u32 vertexIndex = 0;
	for (u32 phiIndex = 0; phiIndex < circleVerticesNum; phiIndex++)
	{
		for (u32 thetaIndex = 0; thetaIndex < circleVerticesNum; thetaIndex++)
		{
			u32 index0 = thetaIndex + phiIndex * circleVerticesNum;
			u32 index1 = (thetaIndex + 1) % circleVerticesNum + phiIndex * circleVerticesNum;
			u32 index2 = (thetaIndex + (phiIndex + 1) * circleVerticesNum) % verticesNum;
			u32 index3 = (thetaIndex + 1 + (phiIndex + 1) * circleVerticesNum) % verticesNum;
			SetIndex(vertexIndex, index0, index1, index2);
			vertexIndex += 1;
			SetIndex(vertexIndex, index1, index3, index2);
			vertexIndex += 1;
		}
	}

	UpdateBuffers();
	SetUpdateBufferBeforeDraw(false);

	DEMO_ASSERT_GL_ERROR();
}

}  // namespace demo
