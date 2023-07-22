#ifndef GRAPHICS_TEXTS_RENDER_DATA_H_
#define GRAPHICS_TEXTS_RENDER_DATA_H_

#include "graphics/graphics_TrianglesRenderData.h"

namespace nn { namespace graphics {

	/*!
	:private

	S-direction offset value for font texture.
	*/
	const f32 FONT_OFFSET_S = 0.125f;

	/*!
	:private

	T-direction offset value for font texture.
	*/
	const f32 FONT_OFFSET_T = 0.0625f;

	/*!
	:private

	Base size for font texture.
	*/
	const u32 FONT_BASE_SIZE = 8;

	/*!
	:private

	Maximum number of characters that can be output on one line.
	*/
	const u32 MAX_CHAR_LENGTH_PER_LINE = 256;

	/*!
	:private

	Maximum number of characters that can be output.
	*/
	const u32 MAX_TEXT_LENGTH = MAX_TRIANGLES_NUM / 2;

	/*!
		:private

		@brief Represents rendering data. Contains member functions for rendering text.
	*/

	class TextsRenderData : public graphics::TrianglesRenderData
	{
	public:
		/*!
		:private

		@brief Constructor.
		*/
		TextsRenderData(void);

		/*!
		:private

		@brief Destructor.
		*/
		virtual ~TextsRenderData(void);

	public:
		/*!
		:private

		@brief Initializes the rendering data.
		*/
		virtual void Initialize(void);

		/*!
		:private

		@brief Initializes the rendering data.
		*/
		virtual void Finalize(void);

	public:
		/*!
		:private

		@brief Initializes the vertex buffers.

		@param[in] vertexAttributes  Specifies the logical sum of vertex attributes.
		@param[in] triangleType  Specifies the type of triangle primitive to render.
		*/
		void InitializeVertexBuffers(const u32 vertexAttributes, const GLenum triangleType, const u32 maxTextLength);

	protected:
		void SetMaxTextLength(const u32 maxTextLength);

	public:
		/*!
		:private

		@brief Adds the text to render to the rendering data.

		@param[in] windowCoordinateX  Specifies the x-coordinate of the top-left of the rectangle in the window coordinate system where the
		text is rendered (<tt>f32</tt> range).
		@param[in] windowCoordinateY  Specifies the y-coordinate of the top-left of the rectangle in the window coordinate system where the
		text is rendered (<tt>f32</tt> range).
		@param[in] text  Specifies the string for rendering text.
		@param[in] textLength  Specifies the length of string for rendering text.
		@param[in] normalizedDeviceCoordinateZ  Specifies the z-coordinate in normalized device coordinates for text.
		*/
		void AddText(const f32	 windowCoordinateX,
					 const f32	 windowCoordinateY,
					 const char* text,
					 const u32	 textLength,
					 const f32	 normalizedDeviceCoordinateZ);

	public:
		/*!
		:private

		@brief Clears the packed text rendering data.
		*/
		void ClearPackedTextLength(void);

	protected:
		void SetPackedTextLength(const u32 packedTextLength);

	public:
		/*!
		:private

		@brief Gets the length of the packed text.
		*/
		u32 GetPackedTextLength(void) const;

	public:
		/*!
		:private

		@brief Sets the size of the font to render.

		@param[in] fontSize  Specifies the font size.
		*/
		void SetFontSize(const f32 fontSize);

		/*!
		:private

		@brief Sets the color of the font to render.

		@param[in] red  Specifies the red component of the font color.
		@param[in] green  Specifies the green component of the font color.
		@param[in] blue  Specifies the blue component of the font color.
		@param[in] alpha  Specifies the alpha component of the font color.
		*/
		void SetFontColor(const f32 red, const f32 green, const f32 blue, const f32 alpha);

	protected:
		u32 m_PackedTextLength;
		u32 m_MaxTextLength;

		f32 m_FontSize;
		f32 m_Color[4];

	protected:
		u32 m_FontBaseSize;
		f32 m_FontOffsetS;
		f32 m_FontOffsetT;
	};

}}	// namespace nn::graphics

#endif
