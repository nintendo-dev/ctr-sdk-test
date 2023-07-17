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

#ifndef DEMO_SHADER_MANAGER_H_
#define DEMO_SHADER_MANAGER_H_

#include <nn/gx.h>
#include <nn/math.h>

#include "demo/ShaderProgram/demo_ColorFillProgram.h"
#include "demo/ShaderProgram/demo_DecalTextureProgram.h"
#include "demo/ShaderProgram/demo_FontProgram.h"
#include "demo/ShaderProgram/demo_ProgramObject.h"

namespace demo {

/*!
:private

@brief Specifies the maximum number of shader binaries in the <tt>ShaderManager</tt> class.
*/
const u32 MAX_SHADER_BINARY_NUM = 1;

/*!
:private

@brief Specifies the maximum number of shaders per shader binary in the <tt>ShaderManager</tt> class.
*/
const u32 MAX_SHADER_PROGRAM_NUM = 3;

/*!
:private

@brief Enumerates basic shaders.
*/
enum ShaderType
{
	COLOR_FILL_SHADER			   = 0,	 //!< Specifies a vertex color shader.
	FONT_SHADER					   = 1,	 //!< Specifies a font shader.
	DECAL_TEXTURE_SHADER		   = 2,	 //!< Specifies a decal texture shader.
	BASIC_SHADER_BINARY_SHADER_NUM = 3	 //!< Specifies the total number of basic shaders.
};

/*!
:private

@brief Defines the index of a basic shader binary.
*/
const u32 BASIC_SHADER_BINARY_INDEX = 0;

/*!
	:private

	@brief Contains member functions for managing shaders.
*/

class ShaderManager : private nn::util::NonCopyable<ShaderManager>
{
public:
	/*!
	:private

	@brief Constructor.
	*/
	ShaderManager(void);

	/*!
	:private

	@brief Destructor.
	*/
	virtual ~ShaderManager(void);

public:
	/*!
	:private

	@brief Initializes the class instance.
	*/
	virtual bool Initialize(void);

	/*!
	:private

	@brief Finalizes the object.
	*/
	virtual bool Finalize(void);

protected:
	virtual bool InitializeBasicShader(void);

public:
	/*!
	:private

	@brief Gets the vertex attributes of the specified shader.
	@param[in] shaderType  Specifies the shader type.
	@return Returns vertex attributes.
	*/
	u32 GetVertexAttributes(const ShaderType shaderType);

	/*!
	:private

	@brief Gets a pointer to the specified shader.

	@param[in] shaderType  Specifies the shader type.
	@return Returns a pointer to the shader program.
	*/
	ProgramObject* GetShaderProgram(const ShaderType shaderType);

protected:
	bool		   m_Initialized;
	u8			   m_Padding[3];
	GLuint		   m_ShaderProgramIdArray[MAX_SHADER_BINARY_NUM][MAX_SHADER_PROGRAM_NUM];
	ProgramObject* m_ShaderProgramPtrArray[MAX_SHADER_BINARY_NUM][MAX_SHADER_PROGRAM_NUM];

protected:
	ColorFillProgram	m_ColorFillProgram;
	FontProgram			m_FontProgram;
	DecalTextureProgram m_DecalTextureProgram;
};

}  // namespace demo

#endif
