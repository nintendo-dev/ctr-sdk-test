#ifndef GRAPHICS_DECAL_TEXTURE_PROGRAM_OBJECT_H_
#define GRAPHICS_DECAL_TEXTURE_PROGRAM_OBJECT_H_

#include "graphics/graphics_ProgramObject.h"

namespace nn { namespace graphics {

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

}}	// namespace nn::graphics

#endif
