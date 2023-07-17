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

#ifndef DEMO_CUBE_H_
#define DEMO_CUBE_H_

#include "demo/Body/demo_Body.h"

namespace demo {
/*!
:private

@brief Represents a cube.
*/

class Cube : public Body
{
public:
	/*!
	@brief Constructor.
	*/
	Cube(void);

	/*!
	@brief The destructor.
	*/
	virtual ~Cube(void);

public:
	virtual void Initialize(void);
	void		 InitializeCube(const u32 vertexAttributes, const f32 halfEdgeX, const f32 halfEdgeY, const f32 halfEdgeZ);

protected:
	f32 m_HalfEdges[3];
};

}  // namespace demo

#endif
