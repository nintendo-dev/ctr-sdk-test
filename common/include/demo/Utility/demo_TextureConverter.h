//
//------------------------------------------------------------
// Copyright(c) 2009-2010 by Digital Media Professionals Inc.
// All rights reserved.
//------------------------------------------------------------
// This source code is the confidential and proprietary
// of Digital Media Professionals Inc.
//------------------------------------------------------------
//

#ifndef DEMO_TEXTURE_CONVERTER_H_
#define DEMO_TEXTURE_CONVERTER_H_

#include <gles2/gl2ext.h>
#include <nn/config.h>
#include <nn/gx.h>

namespace demo {

/* Please see man pages for details












*/
bool ConvertGLTextureToNative(const GLenum format, const u32 width, const u32 height, void* glData, void* dmpData);

}  // namespace demo

#endif
