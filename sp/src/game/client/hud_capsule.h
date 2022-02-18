#define HUD_SUITPOWER_H
#ifdef _WIN32
#pragma once
#endif

#include "hudelement.h"
#include "hud_numericdisplay.h"
#include <vgui_controls/Panel.h>

//-----------------------------------------------------------------------------
// Purpose: Shows the sprint power bar
//-----------------------------------------------------------------------------
class CHudCapsule : public CHudElement, public vgui::Panel
{
	DECLARE_CLASS_SIMPLE(CHudCapsule, vgui::Panel);

public:
	CHudCapsule(const char *pElementName);
	virtual void	Init(void);
	virtual void	Reset(void);
	virtual void	OnThink(void);
	void CapsuleCheck();
	void CapsuleState();
	bool			ShouldDraw(void);

protected:
	virtual void	Paint();

private:
	CPanelAnimationVar(Color, m_AuxPowerColor, "AuxPowerColor", "255 220 0 220");
	CPanelAnimationVar(Color, m_RedColor, "RedColor", "255 0 0 220");
	CPanelAnimationVar(Color, m_GreenColor, "GreenColor", "0 255 0 220");
	CPanelAnimationVar(int, m_iAuxPowerDisabledAlpha, "AuxPowerDisabledAlpha", "70");

	CPanelAnimationVarAliasType(float, m_flBarInsetX, "BarInsetX", "8", "proportional_float");
	CPanelAnimationVarAliasType(float, m_flBarInsetY, "BarInsetY", "8", "proportional_float");
	CPanelAnimationVarAliasType(float, m_flBarWidth, "BarWidth", "80", "proportional_float");
	CPanelAnimationVarAliasType(float, m_flBarHeight, "BarHeight", "10", "proportional_float");
	CPanelAnimationVarAliasType(float, m_flBarChunkWidth, "BarChunkWidth", "10", "proportional_float");
	CPanelAnimationVarAliasType(float, m_flBarChunkGap, "BarChunkGap", "2", "proportional_float");

	CPanelAnimationVar(vgui::HFont, m_hTextFont, "TextFont", "Default");
	CPanelAnimationVarAliasType(float, text_xpos, "text_xpos", "8", "proportional_float");
	CPanelAnimationVarAliasType(float, text_ypos, "text_ypos", "20", "proportional_float");
	CPanelAnimationVarAliasType(float, text2_xpos, "text2_xpos", "30", "proportional_float");
	CPanelAnimationVarAliasType(float, text2_ypos, "text2_ypos", "40", "proportional_float");
	CPanelAnimationVarAliasType(float, text2_gap, "text2_gap", "10", "proportional_float");

	float m_flSuitPower;
	int m_nSuitPowerLow;
	int m_iActiveSuitDevices;
	int m_iCapsuleNumber;
	bool m_iReadyState;
};