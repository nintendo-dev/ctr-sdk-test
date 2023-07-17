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

#ifndef NN_COMMON_SCENE_SCENEFACTORY_H_
#define NN_COMMON_SCENE_SCENEFACTORY_H_

#include "Scene.h"

namespace scene {

/*!
	@brief Scene factory's interface class.

		   Inherit this class, and implement the @ref Generate function to register it to @ref SceneManager.
*/
class SceneFactory
{
public:
	/*!
		@brief Creates a scene.

		@return The scene ID of the scene you want to create.
	*/
	virtual Scene* Generate(s32 sceneId) = 0;
};

}  // namespace scene

#endif	// NN_COMMON_SCENE_SCENEFACTORY_H_
