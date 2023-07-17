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

#ifndef DEMO_SPHERE_H_
#define DEMO_SPHERE_H_

#include "demo/Body/demo_Body.h"

namespace demo {
/*!
:private

@brief Represents a sphere.
*/

class Sphere : public Body
{
public:
	/*!
	@brief Constructor.
	*/
	Sphere(void);

	/*!
	@brief The destructor.
	*/
	virtual ~Sphere(void);

public:
	virtual void Initialize(void);
	void		 InitializeSphere(const u32 vertexAttributes, const f32 radius, const u32 division);

protected:
	f32 m_Radius;
	u32 m_Division;
};

}  // namespace demo

#endif
