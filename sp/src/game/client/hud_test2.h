#include "hudelement.h"
#include <vgui/ILocalize.h>

#include "hud_numericdisplay.h"
#include <vgui_controls/Panel.h>

using namespace vgui;

class CHudImport : public CHudElement, public vgui::Panel
{
	DECLARE_CLASS_SIMPLE(CHudImport, vgui::Panel);

public:
	CHudImport(const char *pElementName);
	void togglePrint();
	virtual void OnThink();

protected:
	virtual void Paint();
	int m_nImport;
	int m_nIcon;
private:
	CPanelAnimationVar(Color, m_HullColor, "HullColor", "255 0 0 255");
	CPanelAnimationVar(vgui::HFont, m_hTextFont, "TextFont", "HUDBarText");
	CPanelAnimationVarAliasType(float, m_flIconInsetX, "IconInsetX", "8", "proportional_float");
	CPanelAnimationVarAliasType(float, m_flIconInsetY, "IconInsetY", "8", "proportional_float");
	int m_testValue;
};