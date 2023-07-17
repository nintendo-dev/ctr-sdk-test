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

#include "demo/Body/demo_Cylinder.h"

namespace demo {
/* ------------------------------------------------------------------------
	<tt>Cylinder</tt> Class Member Function
------------------------------------------------------------------------ */

Cylinder::Cylinder(void) : Body(), m_Radius(0.5f), m_Height(0.5f), m_Division(3)
{
}

Cylinder::~Cylinder()
{
}

void
Cylinder::Initialize(void)
{
	u32 vertexAttributes = demo::VERTEX_POSITION_ATTRIBUTE | demo::VERTEX_COLOR_ATTRIBUTE | demo::VERTEX_NORMAL_ATTRIBUTE;
	InitializeCylinder(vertexAttributes, 0.5f, 1.5f, 10);
}

void
Cylinder::InitializeCylinder(const u32 vertexAttributes, const f32 radius, const f32 height, const u32 division)
{
	TrianglesRenderData::Initialize();
	DEMO_ASSERT_GL_ERROR();

	m_Radius   = radius;
	m_Height   = height;
	m_Division = division;

	f32 halfHeight = 0.5f * m_Height;

	u32 circleVerticesNum = 4 * m_Division;
	u32 verticesNum		  = 6 * circleVerticesNum + 2;
	u32 trianglesNum	  = 4 * circleVerticesNum;
	InitializeVertexBuffers(vertexAttributes, GL_TRIANGLES, verticesNum, trianglesNum);

	SetPackedVerticesNum(verticesNum);
	SetPackedTrianglesNum(trianglesNum);
	SetPackedArraysSize();

	u32 bottomVertexIndex = verticesNum - 2;
	u32 topVertexIndex	  = verticesNum - 1;

	if (m_VertexAttributes & demo::VERTEX_POSITION_ATTRIBUTE)
	{
		f32 deltaPhi = 360.0f / (m_Division * 4.0f);
		for (u32 vertexIndex = 0; vertexIndex < circleVerticesNum; vertexIndex++)
		{
			f32 phi		 = deltaPhi * vertexIndex;
			f32 sinValue = 0.0f;
			f32 cosValue = 0.0f;
			nn::math::SinCosDeg(&sinValue, &cosValue, phi);
			f32 x = m_Radius * sinValue;
			f32 y = (-halfHeight);
			f32 z = m_Radius * cosValue;

			// -Y face.
			SetPosition(vertexIndex, x, y, z);
			// Bottom vertices of the side face.
			SetPosition(2 * circleVerticesNum + vertexIndex, x, y, z);
			SetPosition(3 * circleVerticesNum + vertexIndex, x, y, z);

			// +Y face.
			y = halfHeight;
			SetPosition(circleVerticesNum + vertexIndex, x, y, z);
			// Top vertices of the side face.
			SetPosition(4 * circleVerticesNum + vertexIndex, x, y, z);
			SetPosition(5 * circleVerticesNum + vertexIndex, x, y, z);
		}

		SetPosition(bottomVertexIndex, 0.0f, (-halfHeight), 0.0f);
		SetPosition(topVertexIndex, 0.0f, halfHeight, 0.0f);
	}

	if (m_VertexAttributes & demo::VERTEX_COLOR_ATTRIBUTE)
	{
		demo::TrianglesRenderData::SetColor(1.0f, 0.0f, 0.0f, 1.0f);
	}

	if (m_VertexAttributes & demo::VERTEX_NORMAL_ATTRIBUTE)
	{
		f32 deltaPhi = 360.0f / (m_Division * 4.0f);
		for (s32 vertexIndex = 0; vertexIndex < circleVerticesNum; vertexIndex++)
		{
			f32 phi		  = static_cast<u32>((deltaPhi * (vertexIndex - 1) + 360.0f)) % 360;
			f32 sinValue0 = 0.0f;
			f32 cosValue0 = 0.0f;
			nn::math::SinCosDeg(&sinValue0, &cosValue0, phi);

			phi			  = deltaPhi * vertexIndex;
			f32 sinValue1 = 0.0f;
			f32 cosValue1 = 0.0f;
			nn::math::SinCosDeg(&sinValue1, &cosValue1, phi);

			phi			  = static_cast<u32>(deltaPhi * (vertexIndex + 1)) % 360;
			f32 sinValue2 = 0.0f;
			f32 cosValue2 = 0.0f;
			nn::math::SinCosDeg(&sinValue2, &cosValue2, phi);

			nn::math::VEC3 normal0(m_Radius * (sinValue0 + sinValue1), 0.0f, m_Radius * (cosValue0 + cosValue1));
			nn::math::VEC3Normalize(&normal0, &normal0);

			nn::math::VEC3 normal1(m_Radius * (sinValue1 + sinValue2), 0.0f, m_Radius * (cosValue1 + cosValue2));
			nn::math::VEC3Normalize(&normal1, &normal1);

			// -Y face.
			SetNormal(vertexIndex, 0.0f, -1.0f, 0.0f);
			// Bottom vertices of the side face.
			SetNormal(2 * circleVerticesNum + vertexIndex, normal0.x, normal0.y, normal0.z);
			SetNormal(3 * circleVerticesNum + vertexIndex, normal1.x, normal1.y, normal1.z);

			// +Y face.
			SetNormal(circleVerticesNum + vertexIndex, 0.0f, 1.0f, 0.0f);
			// Top vertices of the side face.
			SetNormal(4 * circleVerticesNum + vertexIndex, normal0.x, normal0.y, normal0.z);
			SetNormal(5 * circleVerticesNum + vertexIndex, normal1.x, normal1.y, normal1.z);
		}

		SetNormal(bottomVertexIndex, 0.0f, -1.0f, 0.0f);
		SetNormal(topVertexIndex, 0.0f, 1.0f, 0.0f);
	}

	if (m_VertexAttributes & demo::VERTEX_TEXCOORD_ATTRIBUTE)
	{
		NN_TPANIC_("Texcoord attribute is not implemented.");
	}

	if (m_VertexAttributes & demo::VERTEX_TANGENT_ATTRIBUTE)
	{
		NN_TPANIC_("Tangent attribute is not implemented.");
	}

	for (u32 vertexIndex = 0; vertexIndex < circleVerticesNum; vertexIndex++)
	{
		// -Y face.
		u32 vertexIndex0 = vertexIndex;
		u32 vertexIndex1 = (vertexIndex + 1) % circleVerticesNum;
		SetIndex(vertexIndex, bottomVertexIndex, vertexIndex1, vertexIndex0);

		// +Y face.
		vertexIndex0 = vertexIndex + circleVerticesNum;
		vertexIndex1 = (vertexIndex + 1) % circleVerticesNum + circleVerticesNum;
		SetIndex(vertexIndex + circleVerticesNum, topVertexIndex, vertexIndex0, vertexIndex1);

		// Side face.
		vertexIndex0	 = 3 * circleVerticesNum + vertexIndex;
		vertexIndex1	 = 5 * circleVerticesNum + vertexIndex;
		u32 vertexIndex2 = 2 * circleVerticesNum + (vertexIndex + 1) % circleVerticesNum;
		u32 vertexIndex3 = 4 * circleVerticesNum + (vertexIndex + 1) % circleVerticesNum;

		u32 vertexOffset = 2 * circleVerticesNum;
		SetIndex(vertexOffset + vertexIndex, vertexIndex0, vertexIndex3, vertexIndex1);

		vertexOffset = 3 * circleVerticesNum;
		SetIndex(vertexOffset + vertexIndex, vertexIndex0, vertexIndex2, vertexIndex3);
	}

	UpdateBuffers();
	SetUpdateBufferBeforeDraw(false);

	DEMO_ASSERT_GL_ERROR();
}

}  // namespace demo
