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

#ifndef DEMO_FONT_PROGRAM_OBJECT_H_
#define DEMO_FONT_PROGRAM_OBJECT_H_

#include "demo/ShaderProgram/demo_ProgramObject.h"

namespace demo {

/*!
	:private

	@brief Represents a shader program for rendering fonts.
*/

class FontProgram : public ProgramObject
{
public:
	/*!
	:private

	@brief Constructor.
	*/
	FontProgram(void);

	/*!
	:private

	@brief Destructor.
	*/
	virtual ~FontProgram(void);

public:
	/*!
	:private

	@brief Initializes the shader program object.
	*/
	virtual bool Initialize(const GLuint shaderId);

	/*!
	:private

	@brief Uses the shader program object.
	*/
	virtual bool Finalize(void);

public:
	/*!
	:private

	@brief Sets the rendering state before using the shader program object.
	*/
	virtual bool Begin(void);

	/*!
	:private

	@brief Uses the shader program object.
	*/
	virtual bool Use(void);

	/*!
	:private

	@brief Sets the rendering state after using the shader program object.
	*/
	virtual bool End(void);

protected:
	virtual void InitializeVertexAttributes(void);
	void		 InitializeFontTexture(void);

protected:
	GLuint m_FontTextureId;
};

}  // namespace demo

#endif
