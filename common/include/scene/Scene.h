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

#ifndef NN_COMMON_SCENE_SCENE_H_
#define NN_COMMON_SCENE_SCENE_H_

#include <nn.h>

// Pop from the scene stack
#define SCENE_ID_POP (-3)
// Undefined scene
#define SCENE_ID_UNDEFINED (-2)
// Empty scene
#define SCENE_ID_NULL (-1)

namespace scene {

/*!
	@brief Scene's interface class.

		   Inherit this class and implement the virtual functions. @n
		   Scene transitions occur by calling the @ref SetNextSceneId function within the @ref Update function.
*/
class Scene
{
public:
	/*!
		@brief Instantiates the object (constructor).
	*/
	Scene() : m_nextSceneId(SCENE_ID_UNDEFINED), m_rejectHome(false) {}

	/*!
		@brief The destructor.
	*/
	virtual ~Scene(){};

	/*!
		@brief Initialize a scene.

		@return If initialization was successful, TRUE.
	*/
	virtual bool Initialize() = 0;

	/*!
		@brief Finalize the scene.
	*/
	virtual void Finalize() = 0;

	/*!
		@brief Update the scene.
	*/
	virtual void Update() = 0;

	/*!
		@brief Render the scene.
	*/
	virtual void Draw() = 0;

	/*!
		@brief Sets the ID for the next scene.

			   When finalizing the application, specify SCENE_ID_NULL.

		@param[in]  nextSceneId  The next scene's ID.
		@param[in]  willSave     Whether to save the current scene ID on the scene stack.
	*/
	void SetNextSceneId(s32 nextSceneId, bool willSave = false)
	{
		m_nextSceneId = nextSceneId;
		m_willSave	  = willSave;
	}

	/*!
		@brief Gets the ID for the next scene.

		@return Returns the next scene ID.
	*/
	s32 GetNextSceneId() const { return m_nextSceneId; }

	bool WillSaveSceneIdToStack() const { return m_willSave; }
	bool IsRejectHome() const { return m_rejectHome; }

protected:
	// Next scene ID
	s32 m_nextSceneId;

	bool m_willSave;
	bool m_rejectHome;
	NN_PADDING2;
};

}  // namespace scene

#endif	// NN_COMMON_SCENE_SCENE_H_
