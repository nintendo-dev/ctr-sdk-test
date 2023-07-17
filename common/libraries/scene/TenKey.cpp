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

#include <string.h>
#include "TenKey.h"
#include "Gui.h"
#include "Util.h"

//////////////////////////////////////////////////////////////////////////////////////////
// Constant Definitions
//////////////////////////////////////////////////////////////////////////////////////////

// Control ID
#define CONTROL_ID_OK         1000
#define CONTROL_ID_CANCEL     1001
#define CONTROL_ID_BACK       1002
#define CONTROL_ID_CLEAR      1003
#define CONTROL_ID_INPUT_BASE 2000

// Size of the input box
#define BOX_SIZE 16
// Hyphen size
#define HYPHEN_SIZE 6

//////////////////////////////////////////////////////////////////////////////////////////
// Function definitions
//////////////////////////////////////////////////////////////////////////////////////////

namespace
{
    scene::Label* CreateInputBox(u32 id, u16 x, const wchar_t* pText)
    {
        // Labels
        scene::Label* pLabel = new scene::Label(id, x, 8, BOX_SIZE, 36, pText);
        // Border color
        nn::util::Color8 borderColor = nn::util::Color8(64, 64, 64, 255);

        pLabel->SetTextAlign(scene::TEXT_ALIGN_CENTER);
        pLabel->SetBorderColor(scene::CONTROL_STATE_DEFAULT, borderColor);
        pLabel->SetBorderWidth(1.0f);

        return pLabel;
    }

    scene::Label* CreateHyphen(u16 x)
    {
        // Labels
        scene::Label* pLabel = new scene::Label(scene::CONTROL_ID_UNUSED, x, 12, HYPHEN_SIZE, 30, L"-");

        pLabel->SetTextAlign(scene::TEXT_ALIGN_CENTER);

        return pLabel;
    }
}

//////////////////////////////////////////////////////////////////////////////////////////
namespace scene
{

// Constructor.
TenKey::TenKey()
{
    m_pInput         = NULL;
    m_length         = 0;
    m_split          = 0;
    m_numLowestChars = 1;
    m_cursor         = 0;
    m_state          = STATE_NORMAL;
}

// Destructor.
TenKey::~TenKey()
{
    Finalize();
}

// Performs initialization.
void TenKey::Initialize(u32 length, u32 split, const wchar_t* pInitial, u32 numLowestChars, bool isPassword)
{
    NN_ASSERT(m_pInput == NULL);

    m_pInput = new wchar_t[length + 1];
    NN_ASSERT(m_pInput != NULL);

    memset(m_pInput, 0, (length + 1) * sizeof (m_pInput[0]));

    m_length         = length;
    m_split          = split;
    m_numLowestChars = numLowestChars;
    m_isPassword     = isPassword;

    // Return icon
    wchar_t backIcon[] = {0xE072, '\0'};

    scene::AddYesNoButtons(&m_controlManager, CONTROL_ID_OK, CONTROL_ID_CANCEL, L"OK", L"Cancel.");

    m_controlManager.Add(new scene::Button(CONTROL_ID_BACK,   258,  58,  60, 30, backIcon));
    m_controlManager.Add(new scene::Button(CONTROL_ID_CLEAR,    2,  58,  60, 30, L"CLR"));
    m_controlManager.Add(new scene::Button(1,                  66,  58,  60, 30, L"1"));
    m_controlManager.Add(new scene::Button(2,                 130,  58,  60, 30, L"2"));
    m_controlManager.Add(new scene::Button(3,                 194,  58,  60, 30, L"3"));
    m_controlManager.Add(new scene::Button(4,                  66,  92,  60, 30, L"4"));
    m_controlManager.Add(new scene::Button(5,                 130,  92,  60, 30, L"5"));
    m_controlManager.Add(new scene::Button(6,                 194,  92,  60, 30, L"6"));
    m_controlManager.Add(new scene::Button(7,                  66, 126,  60, 30, L"7"));
    m_controlManager.Add(new scene::Button(8,                 130, 126,  60, 30, L"8"));
    m_controlManager.Add(new scene::Button(9,                 194, 126,  60, 30, L"9"));
    m_controlManager.Add(new scene::Button(0,                 130, 160,  60, 30, L"0"));

    // Input information
    {
        // Margin
        const u16 margin = 2;
        // Number of hyphens
        u16 hyphenNum = split > 0 ? ((length - 1) / split) : 0;
        // Size of field where the input box is placed
        u16 fieldWidth = BOX_SIZE * length + HYPHEN_SIZE * hyphenNum + (margin * (length + hyphenNum - 1));
        // Position of the input box
        u16 x = (NN_GX_DISPLAY1_HEIGHT - fieldWidth) / 2;

        for (u32 i = 0; i < length; i++)
        {
            u32 id = CONTROL_ID_INPUT_BASE + i;

            m_controlManager.Add(CreateInputBox(id, x, L""));
            x += BOX_SIZE + margin;

            if (hyphenNum > 0 && i != (length - 1) && ((i + 1) % split) == 0)
            {
                m_controlManager.Add(CreateHyphen(x));
                x += HYPHEN_SIZE + margin;
            }
        }
    }

    if (pInitial)
    {
        Reset(pInitial);
    }

    m_controlManager.RegisterControlEventCallback(TenKey::MyControlEventCallback, this);
}

// Performs shutdown.
void TenKey::Finalize()
{
    if (m_pInput)
    {
        delete[] m_pInput;
    }

    m_pInput         = NULL;
    m_length         = 0;
    m_split          = 0;
    m_numLowestChars = 1;
    m_cursor         = 0;
    m_state          = STATE_NORMAL;

    m_controlManager.Clear();
}

// Perform update processing.
void TenKey::Update()
{
    m_controlManager.Update();
}

// Performs rendering.
void TenKey::Draw()
{
    m_controlManager.Draw();
}

// Resets the state.
void TenKey::Reset(const wchar_t* pInitial)
{
    m_cursor = 0;
    m_state  = STATE_NORMAL;

    if (m_pInput)
    {
        memset(m_pInput, 0, (m_length + 1) * sizeof (m_pInput[0]));

        if (pInitial)
        {
            m_cursor = scene::wstrlen(pInitial);
            memcpy(m_pInput, pInitial, (m_cursor > m_length ? m_length : m_cursor) *  sizeof (m_pInput[0]));
        }

        if (m_cursor >= m_numLowestChars)
        {
            // Enable the OK button
            m_controlManager.FindBy(CONTROL_ID_OK)->SetState(scene::CONTROL_STATE_DEFAULT);
        }
        else
        {
            // Disable the OK button
            m_controlManager.FindBy(CONTROL_ID_OK)->SetState(scene::CONTROL_STATE_DISABLED);
        }

        for (u32 i = 0; i < m_length; i++)
        {
            if (m_pInput[i] != '\0' && m_isPassword)
            {
                // *
                m_controlManager.FindBy(CONTROL_ID_INPUT_BASE + i)->SetText(L"*");
            }
            else
            {
                // Character
                wchar_t text[] = {m_pInput[i], '\0'};

                m_controlManager.FindBy(CONTROL_ID_INPUT_BASE + i)->SetText(text);
            }
        }
    }
}

// Get values that were input.
u32 TenKey::GetInputInteger() const
{
    return scene::wtoi(m_pInput);
}

// Event callback
void TenKey::MyControlEventCallback(scene::ControlEvent event, scene::ControlBase* pControl, s16 x, s16 y, void* pParam)
{
    (void)x;
    (void)y;

    if (event == scene::CONTROL_EVENT_TOUCH)
    {
        // Numeric keypad
        TenKey* pTenKey = (TenKey*)pParam;
        // ID of the tapped control
        u16 controlId = pControl->GetId();

        switch (controlId)
        {
        case CONTROL_ID_OK:
            {
                pTenKey->m_state = STATE_OK;
            }
            break;
        case CONTROL_ID_CANCEL:
            {
                pTenKey->m_state = STATE_CANCELLED;
            }
            break;
        case CONTROL_ID_CLEAR:
            {
                pTenKey->Reset(NULL);
            }
            break;
        case CONTROL_ID_BACK:
            {
                if (pTenKey->m_cursor > 0)
                {
                    // Makes the text at the cursor position blank
                    pTenKey->m_pInput[--pTenKey->m_cursor] = '\0';
                    pTenKey->m_controlManager.FindBy(CONTROL_ID_INPUT_BASE + pTenKey->m_cursor)->SetText(L"");

                    if (pTenKey->m_cursor < pTenKey->m_numLowestChars)
                    {
                        // Disable the OK button
                        pTenKey->m_controlManager.FindBy(CONTROL_ID_OK)->SetState(scene::CONTROL_STATE_DISABLED);
                    }
                }
            }
            break;
        default:
            {
                if (controlId < 10)
                {
                    if (pTenKey->m_cursor < pTenKey->m_length)
                    {
                        if (pTenKey->m_isPassword)
                        {
                            pTenKey->m_controlManager.FindBy(CONTROL_ID_INPUT_BASE + pTenKey->m_cursor)->SetText(L"*");
                        }
                        else
                        {
                            // Character
                            wchar_t text[] = {'0' + controlId, '\0'};

                            pTenKey->m_controlManager.FindBy(CONTROL_ID_INPUT_BASE + pTenKey->m_cursor)->SetText(text);
                        }

                        pTenKey->m_pInput[pTenKey->m_cursor++] = '0' + controlId;
                    }

                    if (pTenKey->m_cursor >= pTenKey->m_numLowestChars)
                    {
                        // Enable the OK button
                        pTenKey->m_controlManager.FindBy(CONTROL_ID_OK)->SetState(scene::CONTROL_STATE_DEFAULT);
                    }
                }
            }
        }
    }
}

} // namespace scene
