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

#include "SceneManager.h"
#include "SceneFactory.h"

//////////////////////////////////////////////////////////////////////////////////////////
// Constant Definitions
//////////////////////////////////////////////////////////////////////////////////////////

namespace
{
    enum SceneState
    {
        SCENE_STATE_INITIALIZE = 0, // Initializing
        SCENE_STATE_UPDATE     = 1, // Updating
        SCENE_STATE_FINALIZE   = 2, // Finalizing

        SCENE_STATE_MAX
    };
}

//////////////////////////////////////////////////////////////////////////////////////////
namespace scene
{

// Constructor.
SceneManager::SceneManager()
{
    m_pScene        = NULL;
    m_pSceneFactory = NULL;
    m_sceneId       = SCENE_ID_UNDEFINED;
    m_sceneState    = SCENE_STATE_INITIALIZE;
}

// Destructor.
SceneManager::~SceneManager()
{
    Finalize();
}

// Performs initialization.
bool SceneManager::Initialize(SceneFactory* pSceneFactory, s32 initialSceneId)
{
    if (!pSceneFactory || initialSceneId == SCENE_ID_UNDEFINED)
    {
        return false;
    }

    NN_ASSERT(m_pScene == NULL);

    // Generate the scene.
    m_pScene = pSceneFactory->Generate(initialSceneId);

    if (m_pScene)
    {
        m_pSceneFactory = pSceneFactory;
        m_sceneId       = initialSceneId;
        m_sceneState    = SCENE_STATE_INITIALIZE;
    }

    // Insert SCENE_ID_UNDEFINED in the stack for shutdown determination
    m_sceneStack.push(SCENE_ID_UNDEFINED);

    return (m_pScene != NULL);
}

// Performs shutdown.
void SceneManager::Finalize()
{
    if (m_pScene)
    {
        m_pScene->Finalize();
        delete m_pScene;
    }

    m_pScene     = NULL;
    m_sceneId    = SCENE_ID_UNDEFINED;
    m_sceneState = SCENE_STATE_INITIALIZE;
}

// Perform update processing.
void SceneManager::Update()
{
    if (!m_pScene)
    {
        return;
    }

    switch (m_sceneState)
    {
    case SCENE_STATE_INITIALIZE:
        {
            if (m_pScene->Initialize())
            {
                m_sceneState = SCENE_STATE_UPDATE;
            }
            else
            {
                Finalize();
            }
        }
        break;
    case SCENE_STATE_UPDATE:
        {
            m_pScene->Update();

            if (m_pScene->GetNextSceneId() != SCENE_ID_UNDEFINED)
            {
                m_sceneState = SCENE_STATE_FINALIZE;
            }
        }
        break;
    case SCENE_STATE_FINALIZE:
        {
            // Next scene
            s32 nextSceneId = m_pScene->GetNextSceneId();
            if(m_pScene->WillSaveSceneIdToStack()){
                m_sceneStack.push(m_sceneId);
                NN_LOG("Scene depth: %d\n", m_sceneStack.size());
            }

            Finalize();

            if(nextSceneId == SCENE_ID_POP){
                NN_ASSERT(m_sceneStack.empty()==false);
                nextSceneId=m_sceneStack.top();
                m_sceneStack.pop();
                NN_LOG("Scene depth: %d\n", m_sceneStack.size());
            }
            if (nextSceneId != SCENE_ID_UNDEFINED)
            {
                // Generate the scene.
                m_pScene = m_pSceneFactory->Generate(nextSceneId);

                if (m_pScene)
                {
                    m_sceneId = nextSceneId;
                }
            }
        }
        break;
    }
}

// Performs rendering.
void SceneManager::Draw()
{
    if (!m_pScene || m_sceneState != SCENE_STATE_UPDATE)
    {
        return;
    }
    m_pScene->Draw();
}

// Get whether to terminate.
bool SceneManager::IsExpectedToFinish()
{
    return (m_pScene == NULL);
}
bool SceneManager::IsRejectHome()
{
    return (m_pScene == NULL) ? false : m_pScene->IsRejectHome();
}

} // namespace scene

