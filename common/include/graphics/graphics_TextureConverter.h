#ifndef GRAPHICS_TEXTURE_CONVERTER_H_
#define GRAPHICS_TEXTURE_CONVERTER_H_

#include <nn/config.h>
#include <nn/gx.h>
#include <gles2/gl2ext.h>

namespace nn { namespace graphics {

	/* Please see man pages for details */
	bool ConvertGLTextureToNative(const GLenum format, const u32 width, const u32 height, void* glData, void* dmpData);

}}	// namespace nn::graphics

#endif
