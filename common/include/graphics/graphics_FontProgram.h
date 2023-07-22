#ifndef GRAPHICS_FONT_PROGRAM_OBJECT_H_
#define GRAPHICS_FONT_PROGRAM_OBJECT_H_

#include "graphics/graphics_ProgramObject.h"

namespace nn { namespace graphics {

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

}}	// namespace nn::graphics

#endif
