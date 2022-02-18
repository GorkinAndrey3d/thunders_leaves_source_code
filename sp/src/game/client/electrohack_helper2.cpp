//========= Copyright Valve Corporation, All rights reserved. ============//
//
// Purpose: 
//
// $NoKeywords: $
//
//=============================================================================//
//
// Health.cpp
//
// implementation of CHudHealth class
//
#include "cbase.h"
#include "hud.h"
#include "hud_macros.h"
#include "view.h"

#include "iclientmode.h"

#include <KeyValues.h>
#include <vgui/ISurface.h>
#include <vgui/ISystem.h>
#include <vgui_controls/AnimationController.h>
#include "ihudlcd.h"
#include <vgui/ILocalize.h>

using namespace vgui;

#include "hudelement.h"
#include "hud_numericdisplay.h"

#include "convar.h"

// memdbgon must be the last include file in a .cpp file!!!
#include "tier0/memdbgon.h"

#define INIT_HEALTH -1

//-----------------------------------------------------------------------------
// Purpose: Health panel
//-----------------------------------------------------------------------------
class CHud_Electrohackhelper2 : public CHudElement, public CHudNumericDisplay
{
	DECLARE_CLASS_SIMPLE(CHud_Electrohackhelper2, CHudNumericDisplay);

public:
	CHud_Electrohackhelper2(const char *pElementName);
	virtual void Init(void);
	virtual void VidInit(void);
	virtual void Reset(void);
	virtual void OnThink();
	void togglePrint();
	void MsgFunc_Damage(bf_read &msg);
protected:
	virtual void Paint();
private:
	// old variables
	int		m_iHealth;
	int m_nImport;
	int		m_bitsDamage;
	CPanelAnimationVar(vgui::HFont, m_hFont, "Font", "WeaponIconsSmall");
	CPanelAnimationVarAliasType(float, text_xpos, "text_xpos", "4", "proportional_float");
	CPanelAnimationVarAliasType(float, text_ypos, "text_ypos", "85", "proportional_float");
	CPanelAnimationVarAliasType(float, m_IconX, "icon_xpos", "4", "proportional_float");
	CPanelAnimationVarAliasType(float, m_IconY, "icon_ypos", "4", "proportional_float");
	CPanelAnimationVar(vgui::HFont, m_hIconFont, "IconFont", "HudNumbers");
	CPanelAnimationVar(Color, m_SquadIconColor, "SquadIconColor", "255 220 0 220");
	CPanelAnimationVarAliasType(float, m_flIconInsetX, "IconInsetX", "25", "proportional_float");
	CPanelAnimationVarAliasType(float, m_flIconInsetY, "IconInsetY", "8", "proportional_float");
	int m_iMaxOverload;
};

DECLARE_HUDELEMENT(CHud_Electrohackhelper2);
DECLARE_HUD_MESSAGE(CHud_Electrohackhelper2, Damage);

//-----------------------------------------------------------------------------
// Purpose: Constructor
//-----------------------------------------------------------------------------
CHud_Electrohackhelper2::CHud_Electrohackhelper2(const char *pElementName) : CHudElement(pElementName), CHudNumericDisplay(NULL, "CHud_Electrohackhelper2")
{
	vgui::Panel *pParent = g_pClientMode->GetViewport();
	SetParent(pParent);
	vgui::GetAnimationController();
	SetHiddenBits(HIDEHUD_HEALTH | HIDEHUD_PLAYERDEAD | HIDEHUD_NEEDSUIT);
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void CHud_Electrohackhelper2::Init()
{
	HOOK_HUD_MESSAGE(CHud_Electrohackhelper2, Damage);
	Reset();
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void CHud_Electrohackhelper2::Reset()
{
	m_iHealth = INIT_HEALTH;
	m_bitsDamage = 0;
	Color clrFlashlight;
	clrFlashlight[3] = 255;

	wchar_t *tempString = g_pVGuiLocalize->Find("#thundersleaves_hud_overload");


	if (tempString)
	{
		//SetLabelText(tempString);
	}
	else
	{
		//SetLabelText(L"OVERLOADS");
	}
	//surface()->DrawSetTextFont(m_hFont);
	//surface()->DrawSetTextColor(clrFlashlight);
	//surface()->DrawSetTextPos(m_IconX, m_IconY);
	SetDisplayValue(m_iHealth);

	//surface()->DrawSetTextFont(m_hFont);
	//wchar_t unicode[6];
	//swprintf(unicode, L"%02dM%02d");
	//m_nImport = surface()->CreateNewTextureID();
	//surface()->DrawSetTextureFile(m_nImport, "vgui/hud/crouch_infected", true, true);
	//wchar_t *ch = unicode;
	//surface()->DrawUnicodeChar(*ch);


	//m_nImport = surface()->CreateNewTextureID();
	//surface()->DrawSetTextureFile(m_nImport, "HUD/import", true, true);

}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void CHud_Electrohackhelper2::VidInit()
{
	Reset();
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void CHud_Electrohackhelper2::OnThink()
{
	togglePrint();
	int newHealth = 0;
	m_iMaxOverload = 3;
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

	if (m_iHealth >= 20)
	{
		g_pClientMode->GetViewportAnimationController()->StartAnimationSequence("HealthIncreasedAbove20");
	}
	else if (m_iHealth > 0)
	{
		g_pClientMode->GetViewportAnimationController()->StartAnimationSequence("HealthIncreasedBelow20");
		g_pClientMode->GetViewportAnimationController()->StartAnimationSequence("HealthLow");
	}

	//SetDisplayValue(m_iMaxOverload);


	surface()->DrawSetTextFont(m_hFont);
	wchar_t unicode['M'];
	wchar_t *ch = unicode;
	surface()->DrawUnicodeChar(*ch);
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void CHud_Electrohackhelper2::MsgFunc_Damage(bf_read &msg)
{

	int armor = msg.ReadByte();	// armor
	int damageTaken = msg.ReadByte();	// health
	long bitsDamage = msg.ReadLong(); // damage bits
	bitsDamage; // variable still sent but not used

	Vector vecFrom;

	vecFrom.x = msg.ReadBitCoord();
	vecFrom.y = msg.ReadBitCoord();
	vecFrom.z = msg.ReadBitCoord();

	// Actually took damage?
	if (damageTaken > 0 || armor > 0)
	{
		if (damageTaken > 0)
		{
			// start the animation
			g_pClientMode->GetViewportAnimationController()->StartAnimationSequence("HealthDamageTaken");
		}
	}
}

static ConVar show_overloadhelp("show_electrohackmode2", "0", 0, "toggles beta icon in upper right corner");

void CHud_Electrohackhelper2::togglePrint()
{

	if (!show_overloadhelp.GetBool())
		this->SetVisible(false);
	else
	{
		this->SetVisible(true);
		g_pClientMode->GetViewportAnimationController()->RunAnimationCommand(this, "alpha", 255.0f, 0.0f, 0.4f, AnimationController::INTERPOLATOR_LINEAR);

		//int xpos = m_flIconInsetX, ypos = m_flIconInsetY;
		//surface()->DrawSetTextColor(m_SquadIconColor);
		//surface()->DrawSetTextFont(m_hIconFont);
		//surface()->DrawSetTextPos(xpos, ypos);
		//surface()->DrawUnicodeChar('M');
	}
}

void CHud_Electrohackhelper2::Paint()
{
	surface()->DrawSetTextColor(m_SquadIconColor);
	surface()->DrawSetTextFont(m_hIconFont);
	//SetLabelText(L"OVERLOADS");
	wchar_t *text = g_pVGuiLocalize->Find("#thundersleaves_hud_electrohack_helper");
	if (text)
	{
		surface()->DrawSetTextPos(text_xpos, text_ypos);
		surface()->DrawSetTextFont(m_hTextFont);
		surface()->DrawPrintText(text, wcslen(text));
	}
	else
	{
		surface()->DrawSetTextPos(text_xpos, text_ypos);
		surface()->DrawSetTextFont(m_hTextFont);
		surface()->DrawPrintText(L"Electrohack Mode", wcslen(L"Electrohack Mode"));
	}
}