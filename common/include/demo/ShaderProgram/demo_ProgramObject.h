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

#ifndef DEMO_PROGRAM_OBJECT_H_
#define DEMO_PROGRAM_OBJECT_H_

#include <nn/gx.h>
#include <nn/math.h>

#include "demo/RenderData/demo_TrianglesRenderData.h"
#include "demo/Utility/demo_Utility.h"

namespace demo {

/*!
:private

@brief Sets the maximum number of shaders that the <tt>ProgramObject</tt> class can hold.
*/
const u32 PROGRAM_OBJECT_MAX_SHADERS = 1;

/*!
:private

@brief Represents the base class for shader programs.
*/

class ProgramObject : private nn::util::NonCopyable<ProgramObject>
{
public:
	/*!
	:private

	@brief Constructor.
	*/
	ProgramObject(void);

	/*!
	:private

	@brief Destructor.
	*/
	virtual ~ProgramObject(void);

public:
	/*!
	:private

	@brief Initializes the shader program object.
	*/
	virtual bool Initialize(const GLuint shaderId) = 0;

	/*!
	:private

	@brief Finalizes the shader program object.
	*/
	virtual bool Finalize(void) = 0;

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
	virtual bool Use(void) = 0;

	/*!
	:private

	@brief Sets the rendering state after using the shader program object.
	*/
	virtual bool End(void);

public:
	/*!
	:private

	@brief Gets vertex attributes set in the shader program object.

	@return Returns vertex attributes.
	*/
	virtual u32 GetVertexAttributes(void);

	/*!
	:private

	@brief Sets whether to convert the three-dimensional vertex position coordinates to clip coordinates using the projection matrix and
	model view matrix.

	@param[in] use3d  Specifies whether to convert to the clip coordinate system (<tt>true</tt> or <tt>false</tt>).
	*/
	virtual void SetUse3d(const bool use3d);

	/*!
	:private

	@brief Sets the projection matrix for the shader program object.

	@param[in] projectionMatrix  Specifies the projection matrix.
	*/
	virtual void SetProjectionMatrix(const nn::math::MTX44& projectionMatrix);

	/*!
	:private

	@brief Sets the model view matrix for the shader program object.

	@param[in] modelViewMatrix  Specifies the model view matrix.
	*/
	virtual void SetModelViewMatrix(const nn::math::MTX44& modelViewMatrix);

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
	virtual void InitializeUniforms(void);
	virtual void UpdateModelViewProjectionMatrix(void);

protected:
	bool m_Initialized;
	bool m_Padding0[3];

	u32	 m_VertexAttributes;
	bool m_Use3d;
	u8	 m_Padding[3];

	GLuint m_ProgramId;
	GLuint m_ShaderIds[PROGRAM_OBJECT_MAX_SHADERS];
	GLint  m_UniformLocation[MAX_UNIFORM_LOCATIONS_NUM];

	nn::math::MTX44 m_ProjectionMatrix;
	nn::math::MTX44 m_ModelViewMatrix;
};

}  // namespace demo

#endif
