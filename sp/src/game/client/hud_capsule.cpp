#include "cbase.h"
#include "hud.h"
#include "hud_capsule.h"
#include "hud_macros.h"
#include "c_basehlplayer.h"
#include "iclientmode.h"
#include <vgui_controls/AnimationController.h>
#include <vgui/ISurface.h>
#include <vgui/ILocalize.h>

using namespace vgui;

// memdbgon must be the last include file in a .cpp file!!!
#include "tier0/memdbgon.h"

DECLARE_HUDELEMENT(CHudCapsule);

#define SUITPOWER_INIT -1

//-----------------------------------------------------------------------------
// Purpose: Constructor
//-----------------------------------------------------------------------------
CHudCapsule::CHudCapsule(const char *pElementName) : CHudElement(pElementName), BaseClass(NULL, "HudCapsule")
{
	vgui::Panel *pParent = g_pClientMode->GetViewport();
	SetParent(pParent);

	SetHiddenBits(HIDEHUD_HEALTH | HIDEHUD_PLAYERDEAD | HIDEHUD_NEEDSUIT);
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void CHudCapsule::Init(void)
{
	m_iReadyState = 0;
	m_iCapsuleNumber = 10;
	m_flSuitPower = SUITPOWER_INIT;
	m_nSuitPowerLow = -1;
	m_iActiveSuitDevices = 0;
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void CHudCapsule::Reset(void)
{
	Init();
}

//-----------------------------------------------------------------------------
// Purpose: Save CPU cycles by letting the HUD system early cull
// costly traversal.  Called per frame, return true if thinking and 
// painting need to occur.
//-----------------------------------------------------------------------------
bool CHudCapsule::ShouldDraw()
{
	bool bNeedsDraw = false;

	C_BaseHLPlayer *pPlayer = (C_BaseHLPlayer *)C_BasePlayer::GetLocalPlayer();
	if (!pPlayer)
		return false;

	// needs draw if suit power changed or animation in progress
	bNeedsDraw = ((pPlayer->m_HL2Local.m_flSuitPower != m_flSuitPower) || (m_AuxPowerColor[3] > 0));

	return (bNeedsDraw && CHudElement::ShouldDraw());
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void CHudCapsule::OnThink(void)
{
	float flCurrentPower = 0;
	C_BaseHLPlayer *pPlayer = (C_BaseHLPlayer *)C_BasePlayer::GetLocalPlayer();
	if (!pPlayer)
		return;

	flCurrentPower = pPlayer->m_HL2Local.m_flSuitPower;

	// Only update if we've changed suit power
	if (flCurrentPower == m_flSuitPower)
		return;

	if (flCurrentPower >= 100.0f && m_flSuitPower < 100.0f)
	{
		// we've reached max power
		g_pClientMode->GetViewportAnimationController()->StartAnimationSequence("SuitAuxPowerMax");
	}
	else if (flCurrentPower < 100.0f && (m_flSuitPower >= 100.0f || m_flSuitPower == SUITPOWER_INIT))
	{
		// we've lost power
		g_pClientMode->GetViewportAnimationController()->StartAnimationSequence("SuitAuxPowerNotMax");
	}

	bool flashlightActive = pPlayer->IsFlashlightActive();
	bool sprintActive = pPlayer->IsSprinting();
	bool breatherActive = pPlayer->IsBreatherActive();
	int activeDevices = (int)flashlightActive + (int)sprintActive + (int)breatherActive;

	if (activeDevices != m_iActiveSuitDevices)
	{
		m_iActiveSuitDevices = activeDevices;

		switch (m_iActiveSuitDevices)
		{
		default:
		case 3:
			g_pClientMode->GetViewportAnimationController()->StartAnimationSequence("SuitAuxPowerThreeItemsActive");
			break;
		case 2:
			g_pClientMode->GetViewportAnimationController()->StartAnimationSequence("SuitAuxPowerTwoItemsActive");
			break;
		case 1:
			g_pClientMode->GetViewportAnimationController()->StartAnimationSequence("SuitAuxPowerOneItemActive");
			break;
		case 0:
			g_pClientMode->GetViewportAnimationController()->StartAnimationSequence("SuitAuxPowerNoItemsActive");
			break;
		}
	}

	m_flSuitPower = flCurrentPower;
}

static ConVar capsule_subtract("capsule_subtract", "0", 0, "toggles beta icon in upper right corner");
static ConVar capsule_state("capsule_state", "0", 0, "toggles beta icon in upper right corner");
static ConVar capsule_num1("capsule_num_1", "0", 0, "toggles beta icon in upper right corner");
static ConVar capsule_num2("capsule_num_2", "0", 0, "toggles beta icon in upper right corner");
static ConVar capsule_num3("capsule_num_3", "0", 0, "toggles beta icon in upper right corner");
static ConVar capsule_num4("capsule_num_4", "0", 0, "toggles beta icon in upper right corner");
static ConVar capsule_num5("capsule_num_5", "0", 0, "toggles beta icon in upper right corner");
static ConVar capsule_num6("capsule_num_6", "0", 0, "toggles beta icon in upper right corner");
static ConVar capsule_num7("capsule_num_7", "0", 0, "toggles beta icon in upper right corner");
static ConVar capsule_num8("capsule_num_8", "0", 0, "toggles beta icon in upper right corner");
static ConVar capsule_num9("capsule_num_9", "0", 0, "toggles beta icon in upper right corner");
static ConVar capsule_num10("capsule_num_10", "0", 0, "toggles beta icon in upper right corner");
static ConVar capsule_num0("capsule_num_0", "0", 0, "toggles beta icon in upper right corner");

void CHudCapsule::CapsuleCheck()
{
	/*
	if (capsule_subtract.GetBool())
	{
		m_iCapsuleNumber = m_iCapsuleNumber - 1;
		capsule_subtract.SetValue(0);
	}
	else
	*/
		//int cn = capsule_num.GetInt();
	if (capsule_num1.GetBool())
	{
		m_iCapsuleNumber = 1;
		capsule_num1.SetValue(0);
	}
	else if (capsule_num2.GetBool())
	{
		m_iCapsuleNumber = 2;
		capsule_num2.SetValue(0);
	}
	else if (capsule_num3.GetBool())
	{
		m_iCapsuleNumber = 3;
		capsule_num3.SetValue(0);
	}
	else if (capsule_num4.GetBool())
	{
		m_iCapsuleNumber = 4;
		capsule_num4.SetValue(0);
	}
	else if (capsule_num5.GetBool())
	{
		m_iCapsuleNumber = 5;
		capsule_num5.SetValue(0);
	}
	else if (capsule_num6.GetBool())
	{
		m_iCapsuleNumber = 6;
		capsule_num6.SetValue(0);
	}
	else if (capsule_num7.GetBool())
	{
		m_iCapsuleNumber = 7;
		capsule_num7.SetValue(0);
	}
	else if (capsule_num8.GetBool())
	{
		m_iCapsuleNumber = 8;
		capsule_num8.SetValue(0);
	}
	else if (capsule_num9.GetBool())
	{
		m_iCapsuleNumber = 9;
		capsule_num9.SetValue(0);
	}
	else if (capsule_num10.GetBool())
	{
		m_iCapsuleNumber = 10;
		capsule_num10.SetValue(0);
	}
	else if (capsule_num0.GetBool())
	{
		m_iCapsuleNumber = 0;
		capsule_num0.SetValue(0);
	}
		return;
}

void CHudCapsule::CapsuleState()
{
	if (!capsule_state.GetBool())
		m_iReadyState = 0;
	else
		m_iReadyState = 1;
}


//-----------------------------------------------------------------------------
// Purpose: draws the power bar
//-----------------------------------------------------------------------------
void CHudCapsule::Paint()
{
	C_BaseHLPlayer *pPlayer = (C_BaseHLPlayer *)C_BasePlayer::GetLocalPlayer();
	if (!pPlayer)
		return;

	CapsuleCheck();
	CapsuleState();
	// get bar chunks
	int chunkCount = 10;
	//int chunkCount = m_flBarWidth / (m_flBarChunkWidth + m_flBarChunkGap);
	int enabledChunks = m_iCapsuleNumber;
	//int enabledChunks = (int)((float)chunkCount * (m_flSuitPower * 1.0f / 100.0f) + 0.5f);

	// see if we've changed power state
	int lowPower = 0;
	if (enabledChunks <= (chunkCount / 4))
	{
		lowPower = 1;
	}
	if (m_nSuitPowerLow != lowPower)
	{
		if (m_iActiveSuitDevices || m_flSuitPower < 100.0f)
		{
			if (lowPower)
			{
				g_pClientMode->GetViewportAnimationController()->StartAnimationSequence("SuitAuxPowerDecreasedBelow25");
			}
			else
			{
				g_pClientMode->GetViewportAnimationController()->StartAnimationSequence("SuitAuxPowerIncreasedAbove25");
			}
			m_nSuitPowerLow = lowPower;
		}
	}

	// draw the suit power bar
	surface()->DrawSetColor(m_AuxPowerColor);
	int xpos = m_flBarInsetX, ypos = m_flBarInsetY;
	for (int i = 0; i < enabledChunks; i++)
	{
		surface()->DrawFilledRect(xpos, ypos, xpos + m_flBarChunkWidth, ypos + m_flBarHeight);
		xpos += (m_flBarChunkWidth + m_flBarChunkGap);
	}
	// draw the exhausted portion of the bar.
	surface()->DrawSetColor(Color(m_AuxPowerColor[0], m_AuxPowerColor[1], m_AuxPowerColor[2], m_iAuxPowerDisabledAlpha));
	for (int i = enabledChunks; i < chunkCount; i++)
	{
		surface()->DrawFilledRect(xpos, ypos, xpos + m_flBarChunkWidth, ypos + m_flBarHeight);
		xpos += (m_flBarChunkWidth + m_flBarChunkGap);
	}

	// draw our name
	surface()->DrawSetTextFont(m_hTextFont);
	surface()->DrawSetTextColor(m_AuxPowerColor);
	surface()->DrawSetTextPos(text_xpos, text_ypos);

	wchar_t *tempString = g_pVGuiLocalize->Find("#thundersleaves_Hud_capsules");

	if (tempString)
	{
		surface()->DrawPrintText(tempString, wcslen(tempString));
	}
	else
	{
		surface()->DrawPrintText(L"AVAILABLE CAPSULES", wcslen(L"AVAILABLE CAPSULES"));
	}

	ypos = text2_ypos;

	if (m_iReadyState == 1)
	{
		tempString = g_pVGuiLocalize->Find("#thundersleaves_Hud_ready");

		surface()->DrawSetTextPos(text2_xpos, ypos);

		if (tempString)
		{
			surface()->DrawSetTextColor(m_GreenColor);
			surface()->DrawPrintText(tempString, wcslen(tempString));
		}
		else
		{
			surface()->DrawSetTextColor(m_GreenColor);
			surface()->DrawPrintText(L"READY", wcslen(L"READY"));
		}
		ypos += text2_gap;
	}

	if (m_iReadyState == 0)
	{
		tempString = g_pVGuiLocalize->Find("#thundersleaves_Hud_notready");
		surface()->DrawSetTextPos(text2_xpos, ypos);

		if (tempString)
		{
			surface()->DrawSetTextColor(m_RedColor);
			surface()->DrawPrintText(tempString, wcslen(tempString));
		}
		else
		{
			surface()->DrawSetTextColor(m_RedColor);
			surface()->DrawPrintText(L"NOT READY", wcslen(L"NOT READY"));
		}
		ypos += text2_gap;
	}
}

