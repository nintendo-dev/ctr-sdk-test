#ifndef GRAPHICS_H_
#define GRAPHICS_H_

#if defined(NN_PLATFORM_CTR)
#	ifdef __cplusplus
#		include "graphics/graphics_MemoryManager.h"
#		include "graphics/graphics_TextureConverter.h"
#		include "graphics/graphics_Utility.h"

#		include "graphics/graphics_Light.h"
#		include "graphics/graphics_RenderSystem.h"
#		include "graphics/graphics_RenderSystemDrawing.h"
#		include "graphics/graphics_RenderSystemExt.h"

#		include "graphics/graphics_CommandCache.h"
#	endif	// __cplusplus

#else
#	error no platform selected
#endif

#endif
