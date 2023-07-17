/*---------------------------------------------------------------------------*
  Project:  Horizon

  Copyright (C)Nintendo Co., Ltd.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Rev: 59470 $
 *---------------------------------------------------------------------------*/

#ifndef NN_COMMON_SCENE_TEXTWRITER_H_
#define NN_COMMON_SCENE_TEXTWRITER_H_

#include <nn.h>
#include <nn/font.h>
#include <nn/pl.h>

namespace scene {

/*!
	@brief Text writer.
*/
class TextWriter
{
public:
	/*!
@brief Constructor.
	*/
	TextWriter();

	/*!
@brief Destructor.
	*/
	virtual ~TextWriter();

	/*!
		@brief      Initializes the class instance.

		@param[in]  pExpHeap     The heap. @n
								 Used to allocate the buffer used internally.
		@param[in]  pShaderPath  The shader path.
	*/
	void Initialize(nn::fnd::ExpHeap* pExpHeap);

	/*!
		@brief Finalizes the class instance.
	*/
	void Finalize();

	/*!
		@brief Starts drawing.

		@param[in]  targetDisplay  The drawing target. @n
								   Specify either NN_GX_DISPLAY0 or NN_GX_DISPLAY1.
	*/
	void BeginRender(GLenum targetDisplay);

	/*!
		@brief Stops drawing.
	*/
	void EndRender();

	/*!
		@brief Draws a string.

		@param[in] x       Specifies the render position of the x-coordinate.
		@param[in] y        Specifies the render position of the y-coordinate.
		@param[in] pFormat  Specifies the format string.
		@param[in] ...     Specifies variable length arguments.
	*/
	void Printf(f32 x, f32 y, const wchar_t* pFormat, ...);

	/*!
		@brief Draws a string.

		@param[in] x       Specifies the render position of the x-coordinate.
		@param[in] y        Specifies the render position of the y-coordinate.
		@param[in] pString  The string.
	*/
	void PutString(f32 x, f32 y, const wchar_t* pString);

	/*!
		@brief Draws a string. You can specify the maximum width to render. (Characters from the specified width until the end of the string
	   are not drawn.) Note that pString only accepts up to 127 characters. If a longer string is specified, no characters are drawn.

		@param[in] x       Specifies the render position of the x-coordinate.
		@param[in] y        Specifies the render position of the y-coordinate.
		@param[in] pString  The string.
		@param[in] limitWidth  Maximum string width to render.
	*/
	void PutString(f32 x, f32 y, const wchar_t* pString, f32 limitWidth);

	/*!
		@brief Sets the character color.

		@param[in] red  Specifies the red component.
		@param[in] green  Specifies the green component.
		@param[in] blue  Specifies the blue component.
		@param[in] alpha  Specifies the alpha component.
	*/
	void SetTextColor(u8 red, u8 green, u8 blue, u8 alpha = 255);

	/*!
		@brief Gets the font size.

		@return Returns the font size.
	*/
	f32 GetFontSize();

	/*!
		@brief Specifies the font size.

		@param[in] size  The font size.
	*/
	void SetFontSize(f32 size);

	/*!
		@brief Gets the font magnification rate.

		@return Returns the font magnification rate.
	*/
	f32 GetFontScale();

	/*!
		@brief Sets the font magnification rate.

		@param[in]  scale  The font's magnification rate.
	*/
	void SetFontScale(f32 scale);

	/*!
		@brief Gets the space between characters.

		@return  The space between characters (tracking).
	*/
	f32 GetCharSpace();

	/*!
		@brief Sets the space between characters.

		@param[in]  charSpace  The space between characters (tracking).
	*/
	void SetCharSpace(f32 charSpace);

	/*!
		@brief Calculates the width of a string.

		@param[in] pString  The string.
	*/
	f32 CalculateStringWidth(const wchar_t* pString);

	/*!
		@brief Calculates the height of the string.

		@param[in] pString  The string.
	*/
	f32 CalculateStringHeight(const wchar_t* pString);

protected:
	// Initialize font.
	bool InitializeFont();
	// Perform shader initialization.
	bool InitializeShader();
	// Allocate the string buffer.
	nn::font::DispStringBuffer* AllocateStringBuffer(s32 charMax);

protected:
	// Heap
	nn::fnd::ExpHeap* m_pExpHeap;
	// Font.
	nn::font::ResFont m_font;
	// Render object
	nn::font::RectDrawer	 m_drawer;
	nn::font::WideTextWriter m_writer;
	// String buffer.
	nn::font::DispStringBuffer* m_pStringBuffer;
	// Character color.
	nn::util::Color8 m_textColor;
	// Shader buffer
	void* m_pShaderBuffer;
	// Render target
	u32 m_targetDisplay;
};

}  // namespace scene

#endif	// NN_COMMON_SCENE_TEXTWRITER_H_
