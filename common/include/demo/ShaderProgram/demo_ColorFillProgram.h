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

#ifndef DEMO_COLOR_FILL_PROGRAM_OBJECT_H_
#define DEMO_COLOR_FILL_PROGRAM_OBJECT_H_

#include "demo/ShaderProgram/demo_ProgramObject.h"

namespace demo {
/*!
	:private

	@brief Represents a shader program for drawing polygons with the vertex color.
*/

class ColorFillProgram : public ProgramObject
{
public:
	/*!
	:private

	@brief Constructor.
	*/
	ColorFillProgram(void);

	/*!
	:private

	@brief Destructor.
	*/
	virtual ~ColorFillProgram(void);

public:
	/*!
	:private

	@brief Initializes the shader program object.
	*/
	virtual bool Initialize(const GLuint shaderId);

	/*!
	:private

	@brief Finalizes the shader program object.
	*/
	virtual bool Finalize(void);

public:
	/*!
	:private

	@brief Uses the shader program object.
	*/
	virtual bool Use(void);

protected:
	virtual void InitializeVertexAttributes(void);
};

}  // namespace demo

#endif	// NN_DEMO_CTR_DEMO_COLOR_FILL_PROGRAM_OBJECT_H_
