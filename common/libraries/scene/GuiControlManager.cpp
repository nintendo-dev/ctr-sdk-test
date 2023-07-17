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

#include "GuiControlManager.h"

namespace scene
{

// Constructor.
ControlManager::ControlManager()
{
    m_touchStatusOld.x     = 0;
    m_touchStatusOld.y     = 0;
    m_touchStatusOld.touch = false;

    m_activeId  = CONTROL_ID_UNUSED;
    m_pCallback = NULL;
    m_pParam    = NULL;
}

// Destructor.
ControlManager::~ControlManager()
{
    Clear();
}

// Adds controls.
void ControlManager::Add(ControlBase* pControl)
{
    if (pControl)
    {
        if (pControl->GetId() != CONTROL_ID_UNUSED)
        {
            for (ControlList::iterator it = m_controlList.begin(); it != m_controlList.end(); it++)
            {
                // ID duplication check
                NN_ASSERT((*it)->GetId() != pControl->GetId());
            }
        }

        // Set the control manager
        pControl->SetControlManager(this);

        m_controlList.push_front(pControl);
    }
}

// Delete all controls.
void ControlManager::Clear()
{
    for (ControlList::iterator it = m_controlList.begin(); it != m_controlList.end(); it++)
    {
        delete *it;
    }

    m_controlList.clear();
}

// Search for controls.
ControlBase* ControlManager::FindBy(u32 id)
{
    if (id == CONTROL_ID_UNUSED)
    {
        return NULL;
    }

    for (ControlList::iterator it = m_controlList.begin(); it != m_controlList.end(); it++)
    {
        if ((*it)->GetId() == id)
        {
            return (*it);
        }
    }

    return NULL;
}

// Get the control list.
ControlList* ControlManager::GetList()
{
    return &m_controlList;
}

// Perform update processing.
void ControlManager::Update()
{
    // Get the input
    m_touchReader.ReadLatest(&m_touchStatus);

    // Current input
    u16  nowX     = m_touchStatus.x;
    u16  nowY     = m_touchStatus.y;
    bool nowTouch = m_touchStatus.touch;
    // Input for 1 frame prior
    u16  prevX     = m_touchStatusOld.x;
    u16  prevY     = m_touchStatusOld.y;
    bool prevTouch = m_touchStatusOld.touch;

    for (ControlList::iterator it = m_controlList.begin(); it != m_controlList.end(); it++)
    {
        if ((*it)->GetId() == CONTROL_ID_UNUSED)
        {
            continue;
        }

        s32 x = (*it)->GetX();
        s32 y = (*it)->GetY();

        u32 width  = (*it)->GetWidth();
        u32 height = (*it)->GetHeight();

        // Current status
        ControlState nowState = (*it)->GetState();

        // Do nothing when disabled or hidden
        if (nowState == CONTROL_STATE_DISABLED || nowState == CONTROL_STATE_HIDE)
        {
            continue;
        }

        // In control
        if (nowX >= x && nowX <= x + width && nowY >= y && nowY <= y + height)
        {
            // Not tapped last time & tapped this time -> stylus down
            if (!prevTouch && nowTouch && m_activeId == CONTROL_ID_UNUSED)
            {
                (*it)->OnPenDown();
                m_activeId = (*it)->GetId();

                if (m_pCallback)
                {
                    m_pCallback(CONTROL_EVENT_PENDOWN, *it, nowX - x, nowY - y, m_pParam);
                }
            }
            // Tapped last time & tapped this time -> stylus slide
            if (prevTouch && nowTouch && m_activeId == (*it)->GetId())
            {
                (*it)->OnPenSlide(true);

                if (m_pCallback)
                {
                    m_pCallback(CONTROL_EVENT_SLIDE, *it, nowX - x, nowY - y, m_pParam);
                }
            }

            // Call the callback if state is changed
            if (nowState != (*it)->GetState() && m_pCallback)
            {
                m_pCallback(CONTROL_EVENT_CHANGE_STATE, *it, nowX - x, nowY - y, m_pParam);
            }
        }
        else
        {
            if (m_activeId == (*it)->GetId())
            {
                // If the previous stylus position was inside the control
                bool isIn = (prevX >= x && prevX <= x + width && prevY >= y && prevY <= y + height);

                // Still being tapped -> stylus slide
                if (nowTouch)
                {
                    (*it)->OnPenSlide(false);

                    if (m_pCallback)
                    {
                        m_pCallback(CONTROL_EVENT_SLIDE, *it, nowX - x, nowY - y, m_pParam);
                    }
                }
                else
                {
                    (*it)->OnPenUp(isIn);

                    if (m_pCallback)
                    {
                        m_pCallback(CONTROL_EVENT_PENUP, *it, nowX - x, nowY - y, m_pParam);
                    }

                    // Stylus released inside the control -> stylus tap
                    if (isIn)
                    {
                        (*it)->OnPenTouch();

                        if (m_pCallback)
                        {
                            m_pCallback(CONTROL_EVENT_TOUCH, *it, nowX - x, nowY - y, m_pParam);
                        }
                    }

                    m_activeId = CONTROL_ID_UNUSED;
                }

                // Call the callback if state is changed
                if (nowState != (*it)->GetState() && m_pCallback)
                {
                    m_pCallback(CONTROL_EVENT_CHANGE_STATE, *it, nowX - x, nowY - y, m_pParam);
                }
            }
        }
    }

    if (nowTouch)
    {
        // If nowTouch is not valid, nowX and nowY values are also not valid
        m_touchStatusOld.x = nowX;
        m_touchStatusOld.y = nowY;
    }

    m_touchStatusOld.touch = nowTouch;
}

// Performs rendering.
void ControlManager::Draw()
{
    for (ControlList::iterator it = m_controlList.begin(); it != m_controlList.end(); it++)
    {
        if ((*it)->GetState() != CONTROL_STATE_HIDE)
        {
            (*it)->OnDraw();
        }
    }
}

}
