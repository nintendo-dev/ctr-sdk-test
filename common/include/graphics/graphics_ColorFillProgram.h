#ifndef GRAPHICS_COLOR_FILL_PROGRAM_OBJECT_H_
#define GRAPHICS_COLOR_FILL_PROGRAM_OBJECT_H_

#include "graphics/graphics_ProgramObject.h"

namespace nn { namespace graphics {
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

}}	// namespace nn::graphics

#endif	// NN_GRAPHICS_CTR_GRAPHICS_COLOR_FILL_PROGRAM_OBJECT_H_
