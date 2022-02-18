//========= Copyright Valve Corporation, All rights reserved. ============//
//
// Purpose: 
//
//=============================================================================//

#include "cbase.h"
#include "hud.h"
#include "hudelement.h"
#include "hud_macros.h"
#include "iclientmode.h"
#include "c_basehlplayer.h"
#include "vgui_controls/Panel.h"
#include "vgui_controls/AnimationController.h"
#include "vgui/ISurface.h"
#include <vgui/ILocalize.h>
#include "hud_numericdisplay.h"
#include "ihudlcd.h"
#include "basecombatweapon_shared.h"

// memdbgon must be the last include file in a .cpp file!!!
#include "tier0/memdbgon.h"

ConVar player_squad_transient_commands2("player_squad_transient_commands", "1", FCVAR_REPLICATED);

//-----------------------------------------------------------------------------
// Purpose: Shows the sprint power bar
//-----------------------------------------------------------------------------
class CHudSquadStatus_test : public CHudElement, public vgui::Panel
{
	DECLARE_CLASS_SIMPLE(CHudSquadStatus_test, vgui::Panel);

public:
	CHudSquadStatus_test(const char *pElementName);
	virtual void Init(void);
	virtual void Reset(void);
	virtual void OnThink(void);
	void OverloadCheck();
	bool ShouldDraw();

	void MsgFunc_SquadMemberDied(bf_read &msg);

protected:
	virtual void Paint();

private:
	CPanelAnimationVar(vgui::HFont, m_hTextFont, "TextFont", "Default");
	CPanelAnimationVarAliasType(float, text_xpos, "text_xpos", "8", "proportional_float");
	CPanelAnimationVarAliasType(float, text_ypos, "text_ypos", "20", "proportional_float");

	CPanelAnimationVar(vgui::HFont, m_hIconFont, "IconFont", "HudNumbers");
	CPanelAnimationVarAliasType(float, m_flIconInsetX, "IconInsetX", "8", "proportional_float");
	CPanelAnimationVarAliasType(float, m_flIconInsetY, "IconInsetY", "8", "proportional_float");
	CPanelAnimationVarAliasType(float, m_flIconGap, "IconGap", "30", "proportional_float");

	CPanelAnimationVar(Color, m_SquadIconColor, "SquadIconColor", "255 220 0 160");
	CPanelAnimationVar(Color, m_LastMemberColor, "LastMemberColor", "255 220 0 0");
	CPanelAnimationVar(Color, m_SquadTextColor, "SquadTextColor", "255 220 0 160");

	int m_iSquadMembers;
	int m_iSquadMedics;
	bool m_bSquadMembersFollowing;
	bool m_bSquadMemberAdded;
	bool m_bSquadMemberJustDied;
	int		m_iHealth;
	int m_iOverloads;
};


DECLARE_HUDELEMENT(CHudSquadStatus_test);
DECLARE_HUD_MESSAGE(CHudSquadStatus_test, SquadMemberDied);

using namespace vgui;



//-----------------------------------------------------------------------------
// Purpose: Constructor
//-----------------------------------------------------------------------------
CHudSquadStatus_test::CHudSquadStatus_test(const char *pElementName) : CHudElement(pElementName), BaseClass(NULL, "HudSquadStatus_test")
{
	vgui::Panel *pParent = g_pClientMode->GetViewport();
	SetParent(pParent);

	SetHiddenBits(HIDEHUD_HEALTH | HIDEHUD_PLAYERDEAD | HIDEHUD_NEEDSUIT);

}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void CHudSquadStatus_test::Init(void)
{
	HOOK_HUD_MESSAGE(CHudSquadStatus_test, SquadMemberDied);
	m_iSquadMembers = 1;
	m_iSquadMedics = 0;
	m_iOverloads = -1;
	hudlcd->SetGlobalStat("(overload_num)", "2");
	m_bSquadMemberAdded = true;
	m_bSquadMembersFollowing = true;
	m_bSquadMemberJustDied = false;
	SetAlpha(0);
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void CHudSquadStatus_test::Reset(void)
{
	Init();
}

//-----------------------------------------------------------------------------
// Purpose: Save CPU cycles by letting the HUD system early cull
// costly traversal.  Called per frame, return true if thinking and 
// painting need to occur.
//-----------------------------------------------------------------------------
bool CHudSquadStatus_test::ShouldDraw(void)
{
	bool bNeedsDraw = true;

	C_BaseHLPlayer *pPlayer = (C_BaseHLPlayer *)C_BasePlayer::GetLocalPlayer();
	if (!pPlayer)
		return false;
	/*
	bNeedsDraw = (pPlayer->m_HL2Local.m_iSquadMemberCount > 0 ||
		(pPlayer->m_HL2Local.m_iSquadMemberCount != m_iSquadMembers) ||
		(pPlayer->m_HL2Local.m_fSquadInFollowMode != m_bSquadMembersFollowing) ||
		(m_iSquadMembers > 0) ||
		(m_LastMemberColor[3] > 0));
	*/

	return (bNeedsDraw && CHudElement::ShouldDraw());
}

//-----------------------------------------------------------------------------
// Purpose: updates hud icons
//-----------------------------------------------------------------------------
void CHudSquadStatus_test::OnThink(void)
{
	C_BaseHLPlayer *pPlayer = (C_BaseHLPlayer *)C_BasePlayer::GetLocalPlayer();
	if (!pPlayer)
		return;
	
	int squadMembers = 2;
	bool following = 1;
	m_iSquadMedics = 0;
	OverloadCheck();
	//int squadMembers = pPlayer->m_HL2Local.m_iSquadMemberCount;
	//bool following = pPlayer->m_HL2Local.m_fSquadInFollowMode;
	//m_iSquadMedics = pPlayer->m_HL2Local.m_iSquadMedicCount;


	// Only update if we've changed vars
	if (squadMembers == m_iSquadMembers && following == m_bSquadMembersFollowing)
		return;

	// update status display
	if (squadMembers > 0)
	{
		// we have squad members, show the display
		g_pClientMode->GetViewportAnimationController()->RunAnimationCommand(this, "alpha", 255.0f, 0.0f, 0.4f, AnimationController::INTERPOLATOR_LINEAR);
	}
	else
	{
		// no squad members, hide the display
		g_pClientMode->GetViewportAnimationController()->RunAnimationCommand(this, "alpha", 0.0f, 0.0f, 0.4f, AnimationController::INTERPOLATOR_LINEAR);
	}

	if (squadMembers > m_iSquadMembers)
	{
		// someone is added
		// reset the last icon color and animate
		m_LastMemberColor = m_SquadIconColor;
		m_LastMemberColor[3] = 0;
		m_bSquadMemberAdded = true;
		g_pClientMode->GetViewportAnimationController()->StartAnimationSequence("SquadMemberAdded");
	}
	else if (squadMembers < m_iSquadMembers)
	{
		// someone has left
		// reset the last icon color and animate
		m_LastMemberColor = m_SquadIconColor;
		m_bSquadMemberAdded = false;
		if (m_bSquadMemberJustDied)
		{
			g_pClientMode->GetViewportAnimationController()->StartAnimationSequence("SquadMemberDied");
			m_bSquadMemberJustDied = false;
		}
		else
		{
			g_pClientMode->GetViewportAnimationController()->StartAnimationSequence("SquadMemberLeft");
		}
	}

	if (following != m_bSquadMembersFollowing)
	{
		if (following)
		{
			// flash the squad area to indicate they are following
			g_pClientMode->GetViewportAnimationController()->StartAnimationSequence("SquadMembersFollowing");
		}
		else
		{
			// flash the crosshair to indicate the targeted order is in effect
			g_pClientMode->GetViewportAnimationController()->StartAnimationSequence("SquadMembersStationed");
		}
	}

	m_iSquadMembers = 1;
	m_bSquadMembersFollowing = following;

	int newHealth = 0;
	C_BasePlayer *local = C_BasePlayer::GetLocalPlayer();
	if (local)
	{
		// Never below zero
		newHealth = MAX(local->GetHealth(), 0);
	}

	// Only update the fade if we've changed health
	if (newHealth == m_iHealth)
	{
		return;
	}

	m_iHealth = newHealth;

	
	
	
}

static ConVar overload_subtract("overload_subtract", "0", 0, "toggles beta icon in upper right corner");
static ConVar overload_3("overload_3", "0", 0, "toggles beta icon in upper right corner");
static ConVar overload_2("overload_2", "0", 0, "toggles beta icon in upper right corner");
static ConVar overload_1("overload_1", "0", 0, "toggles beta icon in upper right corner");
static ConVar overload_0("overload_0", "0", 0, "toggles beta icon in upper right corner");

void CHudSquadStatus_test::OverloadCheck()
{
	if (overload_subtract.GetBool())
	{
		m_iOverloads = m_iOverloads - 1;
		overload_subtract.SetValue(0);
	}
	else
		if (overload_3.GetBool())
		{
			m_iOverloads = 3;
			overload_3.SetValue(0);
		}
		else
			if (overload_2.GetBool())
			{
				m_iOverloads = 2;
				overload_2.SetValue(0);
			}
			else
				if (overload_1.GetBool())
				{
					m_iOverloads = 1;
					overload_1.SetValue(0);
				}
				else
					if (overload_0.GetBool())
					{
						m_iOverloads = 0;
						overload_1.SetValue(0);
					}
		return;
}

//-----------------------------------------------------------------------------
// Purpose: Notification of squad member being killed
//-----------------------------------------------------------------------------
void CHudSquadStatus_test::MsgFunc_SquadMemberDied(bf_read &msg)
{
	m_bSquadMemberJustDied = true;
}

//-----------------------------------------------------------------------------
// Purpose: draws the power bar
//-----------------------------------------------------------------------------
void CHudSquadStatus_test::Paint()
{

	C_BaseHLPlayer *pPlayer = (C_BaseHLPlayer *)C_BasePlayer::GetLocalPlayer();
	if (!pPlayer)
		return;

	// draw the suit power bar
	surface()->DrawSetTextColor(m_SquadIconColor);
	surface()->DrawSetTextFont(m_hIconFont);
	int xpos = m_flIconInsetX, ypos = m_flIconInsetY;
	for (int i = -1; i < m_iSquadMembers; i++)
	{
		if (i == m_iSquadMembers - 1)
		{
			// draw the last added squad member specially
			surface()->DrawSetTextColor(m_LastMemberColor);
		}
		surface()->DrawSetTextPos(xpos, ypos);

		surface()->DrawUnicodeChar('C');
		xpos += 30;
		surface()->DrawSetTextPos(xpos, ypos);
		if (m_iOverloads == 3)
		{
			surface()->DrawUnicodeChar('3');
		}
		if (m_iOverloads == 2)
		{
			surface()->DrawUnicodeChar('2');
		}
		if (m_iOverloads == 1)
		{
			surface()->DrawUnicodeChar('1');
		}
		if (m_iOverloads == 0)
		{
			surface()->DrawUnicodeChar('$');
		}
		
	}
	

		// draw the last one in the special color
		//surface()->DrawSetTextColor(m_LastMemberColor);
		//surface()->DrawSetTextPos(xpos, ypos);
		//surface()->DrawUnicodeChar('3');
	

	// draw our squad status
	wchar_t *text = NULL;
	if (m_bSquadMembersFollowing)
	{
		//text = g_pVGuiLocalize->Find("#Valve_Hud_SQUAD_FOLLOWING");
		text = L"3";
		
		if (!text)
		{
			//text = L"SQUAD FOLLOWING";
		}
	}
	else
	{
		if (!player_squad_transient_commands2.GetBool())
		{
			//text = g_pVGuiLocalize->Find("#Valve_Hud_SQUAD_STATIONED");

			if (!text)
			{
				//text = L"SQUAD STATIONED";
			}
		}
	}

	
		surface()->DrawSetTextFont(m_hTextFont);
		surface()->DrawSetTextColor(m_SquadTextColor);
		surface()->DrawSetTextPos(text_xpos, text_ypos);
		surface()->DrawPrintText(text, wcslen(text));
	
}


