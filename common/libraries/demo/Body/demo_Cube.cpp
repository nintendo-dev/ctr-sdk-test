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

#include "demo/Body/demo_Cube.h"

namespace demo {

/* ------------------------------------------------------------------------
	<tt>Cube</tt> Class Member Function
	------------------------------------------------------------------------ */

Cube::Cube(void) : Body()
{
	for (u32 i = 0; i < 3; i++)
	{
		m_HalfEdges[i] = 0.5f;
	}
}

Cube::~Cube()
{
}

void
Cube::Initialize(void)
{
	u32 vertexAttributes = demo::VERTEX_POSITION_ATTRIBUTE | demo::VERTEX_COLOR_ATTRIBUTE | demo::VERTEX_NORMAL_ATTRIBUTE;
	InitializeCube(vertexAttributes, 0.5f, 0.5f, 0.5f);
}

void
Cube::InitializeCube(const u32 vertexAttributes, const f32 halfEdgeX, const f32 halfEdgeY, const f32 halfEdgeZ)
{
	TrianglesRenderData::Initialize();
	DEMO_ASSERT_GL_ERROR();

	m_HalfEdges[0] = halfEdgeX;
	m_HalfEdges[1] = halfEdgeY;
	m_HalfEdges[2] = halfEdgeZ;

	u32 verticesNum	 = 24;
	u32 trianglesNum = 12;
	InitializeVertexBuffers(vertexAttributes, GL_TRIANGLES, verticesNum, trianglesNum);

	SetPackedVerticesNum(verticesNum);
	SetPackedTrianglesNum(trianglesNum);
	SetPackedArraysSize();

	if (m_VertexAttributes & demo::VERTEX_POSITION_ATTRIBUTE)
	{
		// -Y face.
		SetPosition(0, (-m_HalfEdges[0]), (-m_HalfEdges[1]), (-m_HalfEdges[2]));
		SetPosition(1, (-m_HalfEdges[0]), (-m_HalfEdges[1]), (+m_HalfEdges[2]));
		SetPosition(2, (+m_HalfEdges[0]), (-m_HalfEdges[1]), (+m_HalfEdges[2]));
		SetPosition(3, (+m_HalfEdges[0]), (-m_HalfEdges[1]), (-m_HalfEdges[2]));

		// +Y face.
		SetPosition(4, (-m_HalfEdges[0]), (+m_HalfEdges[1]), (-m_HalfEdges[2]));
		SetPosition(5, (+m_HalfEdges[0]), (+m_HalfEdges[1]), (-m_HalfEdges[2]));
		SetPosition(6, (+m_HalfEdges[0]), (+m_HalfEdges[1]), (+m_HalfEdges[2]));
		SetPosition(7, (-m_HalfEdges[0]), (+m_HalfEdges[1]), (+m_HalfEdges[2]));

		// -Z face.
		SetPosition(8, (-m_HalfEdges[0]), (-m_HalfEdges[1]), (-m_HalfEdges[2]));
		SetPosition(9, (+m_HalfEdges[0]), (-m_HalfEdges[1]), (-m_HalfEdges[2]));
		SetPosition(10, (+m_HalfEdges[0]), (+m_HalfEdges[1]), (-m_HalfEdges[2]));
		SetPosition(11, (-m_HalfEdges[0]), (+m_HalfEdges[1]), (-m_HalfEdges[2]));

		// +Z face.
		SetPosition(12, (-m_HalfEdges[0]), (-m_HalfEdges[1]), (+m_HalfEdges[2]));
		SetPosition(13, (-m_HalfEdges[0]), (+m_HalfEdges[1]), (+m_HalfEdges[2]));
		SetPosition(14, (+m_HalfEdges[0]), (+m_HalfEdges[1]), (+m_HalfEdges[2]));
		SetPosition(15, (+m_HalfEdges[0]), (-m_HalfEdges[1]), (+m_HalfEdges[2]));

		// -X face.
		SetPosition(16, (-m_HalfEdges[0]), (-m_HalfEdges[1]), (-m_HalfEdges[2]));
		SetPosition(17, (-m_HalfEdges[0]), (+m_HalfEdges[1]), (-m_HalfEdges[2]));
		SetPosition(18, (-m_HalfEdges[0]), (+m_HalfEdges[1]), (+m_HalfEdges[2]));
		SetPosition(19, (-m_HalfEdges[0]), (-m_HalfEdges[1]), (+m_HalfEdges[2]));

		// +X face.
		SetPosition(20, (+m_HalfEdges[0]), (-m_HalfEdges[1]), (-m_HalfEdges[2]));
		SetPosition(21, (+m_HalfEdges[0]), (-m_HalfEdges[1]), (+m_HalfEdges[2]));
		SetPosition(22, (+m_HalfEdges[0]), (+m_HalfEdges[1]), (+m_HalfEdges[2]));
		SetPosition(23, (+m_HalfEdges[0]), (+m_HalfEdges[1]), (-m_HalfEdges[2]));
	}

	if (m_VertexAttributes & demo::VERTEX_COLOR_ATTRIBUTE)
	{
		demo::TrianglesRenderData::SetColor(1.0f, 0.0f, 0.0f, 1.0f);
	}

	if (m_VertexAttributes & demo::VERTEX_TEXCOORD_ATTRIBUTE)
	{
		// -Y face.
		SetTexcoord(0, 0.0f, 0.0f);
		SetTexcoord(1, 1.0f, 0.0f);
		SetTexcoord(2, 1.0f, 1.0f);
		SetTexcoord(3, 0.0f, 1.0f);

		// +Y face.
		SetTexcoord(4, 0.0f, 1.0f);
		SetTexcoord(5, 1.0f, 1.0f);
		SetTexcoord(6, 1.0f, 0.0f);
		SetTexcoord(7, 0.0f, 0.0f);

		// -Z face.
		SetTexcoord(8, 1.0f, 0.0f);
		SetTexcoord(9, 0.0f, 0.0f);
		SetTexcoord(10, 0.0f, 1.0f);
		SetTexcoord(11, 1.0f, 1.0f);

		// +Z face.
		SetTexcoord(12, 0.0f, 0.0f);
		SetTexcoord(13, 0.0f, 1.0f);
		SetTexcoord(14, 1.0f, 1.0f);
		SetTexcoord(15, 1.0f, 0.0f);

		// -X face.
		SetTexcoord(16, 0.0f, 0.0f);
		SetTexcoord(17, 0.0f, 1.0f);
		SetTexcoord(18, 1.0f, 1.0f);
		SetTexcoord(19, 1.0f, 0.0f);

		// +X face.
		SetTexcoord(20, 1.0f, 0.0f);
		SetTexcoord(21, 0.0f, 0.0f);
		SetTexcoord(22, 0.0f, 1.0f);
		SetTexcoord(23, 1.0f, 1.0f);
	}

	if (m_VertexAttributes & demo::VERTEX_NORMAL_ATTRIBUTE)
	{
		// -Y face.
		u32 faceIndex = 0;
		for (u32 i = 0; i < 4; i++)
		{
			SetNormal(4 * faceIndex + i, 0.0f, (-1.0f), 0.0f);
		}

		// +Y face.
		faceIndex = 1;
		for (u32 i = 0; i < 4; i++)
		{
			SetNormal(4 * faceIndex + i, 0.0f, 1.0f, 0.0f);
		}

		// -Z face.
		faceIndex = 2;
		for (u32 i = 0; i < 4; i++)
		{
			SetNormal(4 * faceIndex + i, 0.0f, 0.0f, (-1.0f));
		}

		// +Z face.
		faceIndex = 3;
		for (u32 i = 0; i < 4; i++)
		{
			SetNormal(4 * faceIndex + i, 0.0f, 0.0f, 1.0f);
		}

		// -X face.
		faceIndex = 4;
		for (u32 i = 0; i < 4; i++)
		{
			SetNormal(4 * faceIndex + i, (-1.0f), 0.0f, 0.0f);
		}

		// +X face.
		faceIndex = 5;
		for (u32 i = 0; i < 4; i++)
		{
			SetNormal(4 * faceIndex + i, 1.0f, 0.0f, 0.0f);
		}
	}

	if (m_VertexAttributes & demo::VERTEX_TANGENT_ATTRIBUTE)
	{
		NN_TPANIC_("Tangent attribute is not implemented.");
	}

	// -Y face.
	SetIndex(0, 0, 2, 1);
	SetIndex(1, 0, 3, 2);

	// +Y face.
	SetIndex(2, 4, 6, 5);
	SetIndex(3, 4, 7, 6);

	// -Z face.
	SetIndex(4, 8, 10, 9);
	SetIndex(5, 8, 11, 10);

	// +Z face.
	SetIndex(6, 12, 14, 13);
	SetIndex(7, 12, 15, 14);

	// -X face.
	SetIndex(8, 16, 18, 17);
	SetIndex(9, 16, 19, 18);

	// +X face.
	SetIndex(10, 20, 22, 21);
	SetIndex(11, 20, 23, 22);

	UpdateBuffers();
	SetUpdateBufferBeforeDraw(false);

	DEMO_ASSERT_GL_ERROR();
}

}  // namespace demo
