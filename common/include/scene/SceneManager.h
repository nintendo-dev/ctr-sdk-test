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

#ifndef NN_COMMON_SCENE_SCENEMANAGER_H_
#define NN_COMMON_SCENE_SCENEMANAGER_H_

#include "Scene.h"
#include "SceneFactory.h"
#include "Stack.h"

namespace scene {

/*!
	@brief The scene management class.

		   Initializes, finalizes, and updates the scenes and manages the render state.
*/
class SceneManager
{
public:
	/*!
		@brief Instantiates the object (constructor).
	*/
	SceneManager();

	/*!
		@brief The destructor.
	*/
	virtual ~SceneManager();

	/*!
		@brief      Initializes the class instance.

		@param[in]  pSceneFactory   The scene factory.
		@param[in]  initialSceneId  The initial scene ID.

		@return If initialization was successful, TRUE.
	*/
	bool Initialize(SceneFactory* pSceneFactory, s32 initialSceneId);

	/*!
		@brief Finalizes the object.
	*/
	void Finalize();

	/*!
		@brief Updates the object.
	*/
	void Update();

	/*!
		@brief Renders the object.
	*/
	void Draw();

	/*!
		@brief Get whether to terminate.

		@return Returns TRUE when it is time to terminate.
	*/
	bool IsExpectedToFinish();

	/*!
		@brief Gets whether to block the HOME Menu.

		@return Returns TRUE if blocking the HOME Menu.
	*/
	bool IsRejectHome();

protected:
	// Scene factory
	SceneFactory* m_pSceneFactory;
	// Scene
	Scene* m_pScene;
	// Scene ID
	s32 m_sceneId;
	// Scene state
	s32 m_sceneState;

	// Scene stack
	Stack<s32, 16> m_sceneStack;
};

}  // namespace scene

#endif	// NN_COMMON_SCENE_SCENEMANAGER_H_
