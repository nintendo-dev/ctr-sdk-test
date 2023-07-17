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

#ifndef DEMO_DECAL_TEXTURE_PROGRAM_OBJECT_H_
#define DEMO_DECAL_TEXTURE_PROGRAM_OBJECT_H_

#include "demo/ShaderProgram/demo_ProgramObject.h"

namespace demo {

/*!
:private

@brief Represents a shader program for placing texture decals on polygons.
*/

class DecalTextureProgram : public ProgramObject
{
public:
	/*!
	:private

	@brief Constructor.
	*/
	DecalTextureProgram(void);

	/*!
	:private

	@brief Destructor.
	*/
	virtual ~DecalTextureProgram(void);

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

public:
	/*!
	:private

	@brief Sets the handle of a texture object.

	@param[in] textureId  Specifies the texture object handle.
	*/
	virtual void SetTextureId(const GLuint textureId);

	/*!
	:private

	@brief Updates the state of the shader program object.
	*/
	virtual void Update(void);

protected:
	virtual void InitializeVertexAttributes(void);

protected:
	GLuint m_TextureId;
};

}  // namespace demo

#endif
