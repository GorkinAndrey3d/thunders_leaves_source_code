#include "hud.h"
#include "cbase.h"
#include "hud_test2.h"
#include "iclientmode.h"
#include "hud_macros.h"
#include "vgui_controls/controls.h"
#include "vgui/ISurface.h"
#include "c_baseplayer.h" 


#include "tier0/memdbgon.h"

using namespace vgui;

DECLARE_HUDELEMENT(CHudImport);

CHudImport::CHudImport(const char *pElementName) : CHudElement(pElementName), vgui::Panel(NULL, "HudImport")
{
	Panel *pParent = g_pClientMode->GetViewport();
	SetParent(pParent);

	SetVisible(false);
	SetAlpha(255);
	int xpos = m_flIconInsetX, ypos = m_flIconInsetY;
	//AW Create Texture for Looking around
	m_nImport = surface()->CreateNewTextureID();
	surface()->DrawSetTextureFile(m_nImport, "vgui/hud/crouch_infected", true, true);
	surface()->DrawSetTextPos(xpos, ypos);
	surface()->DrawUnicodeChar('C');
	


	SetHiddenBits(HIDEHUD_PLAYERDEAD | HIDEHUD_NEEDSUIT);
}

void CHudImport::Paint()
{
	wchar_t pState = 169;

	m_testValue = 50;
	SetPaintBorderEnabled(false);
	surface()->DrawSetTexture(m_nImport);
	surface()->DrawTexturedRect(2, 2, 128, 128);
	//surface()->DrawSetColor(Color(244, 231, 64, 255));
	surface()->DrawSetTextFont(m_hTextFont);
	surface()->DrawSetTextColor(m_HullColor);
	surface()->DrawUnicodeChar(pState);
}

static ConVar show_beta("show_beta", "0", 0, "toggles beta icon in upper right corner");

void CHudImport::togglePrint()
{
	if (!show_beta.GetBool())
		this->SetVisible(false);
	else
		this->SetVisible(true);
	//SetLabelText(L"AAAAAA");
}



void CHudImport::OnThink()
{
	
	togglePrint();
	
	//SetDisplayValue(m_testValue);
	BaseClass::OnThink();
}