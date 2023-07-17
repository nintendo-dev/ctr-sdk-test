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

#include "demo/Body/demo_Body.h"

namespace demo {
/* ------------------------------------------------------------------------
	<tt>Body</tt> Class Member Function
	------------------------------------------------------------------------ */

Body::Body(void) : TrianglesRenderData()
{
}

Body::~Body()
{
}

void
Body::SetColor(const GLfloat red, const GLfloat green, const GLfloat blue)
{
	demo::TrianglesRenderData::SetColor(red, green, blue);
	UpdateBuffers();
}

void
Body::SetColor(const GLfloat red, const GLfloat green, const GLfloat blue, const GLfloat alpha)
{
	demo::TrianglesRenderData::SetColor(red, green, blue, alpha);
	UpdateBuffers();
}

}  // namespace demo
