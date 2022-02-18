#include "cbase.h"

#include "c_vguiscreen.h"
#include <vgui/IVGui.h>
#include <vgui_controls\Controls.h>
#include <vgui_controls\Label.h>
#include <clientmode_hlnormal.h>

#include "tier0\memdbgon.h"

class CRPGScreen : public CVGuiScreenPanel
{
	DECLARE_CLASS(CRPGScreen, CVGuiScreenPanel);

public:
	CRPGScreen(vgui::Panel *parent, const char *panelName);

	virtual bool Init(KeyValues* pKeyValues, VGuiScreenInitData_t* pInitData);
	virtual void OnTick();

private:
	vgui::Label *m_pAmmoCount;
};

DECLARE_VGUI_SCREEN_FACTORY(CRPGScreen, "rpg_screen");


//-----------------------------------------------------------------------------
// Constructor:
//-----------------------------------------------------------------------------
CRPGScreen::CRPGScreen(vgui::Panel *parent, const char *panelName)
	: BaseClass(parent, panelName, g_hVGuiCombineScheme)
{
}

bool CRPGScreen::Init(KeyValues* pKeyValues, VGuiScreenInitData_t* pInitData)
{
	// Load all of the controls in
	if (!BaseClass::Init(pKeyValues, pInitData))
		return false;

	// Make sure we get ticked...
	vgui::ivgui()->AddTickSignal(GetVPanel());

	// Ask: Here we find a pointer to our AmmoCountReadout Label and store it in m_pAmmoCount
	m_pAmmoCount = dynamic_cast<vgui::Label*>(FindChildByName("AmmoCountReadout"));

	return true;
}

void CRPGScreen::OnTick()
{
	BaseClass::OnTick();

	// Get our player
	CBasePlayer *pPlayer = C_BasePlayer::GetLocalPlayer();
	if (!pPlayer)
		return;

	// Get the players active weapon
	CBaseCombatWeapon *pWeapon = pPlayer->GetActiveWeapon();

	// If pWeapon is NULL or it doesn't use primary ammo, don't update our screen
	if (!pWeapon || !pWeapon->UsesPrimaryAmmo())
		return;

	// Our RPG isn't clip-based, so we need to check the player's arsenal of rockets
	int ammo1 = pPlayer->GetAmmoCount(pWeapon->GetPrimaryAmmoType());

	// If our Label exist
	if (m_pAmmoCount)
	{
		char buf[32];
		Q_snprintf(buf, sizeof(buf), "%d", ammo1);
		// Set the Labels text to the number of missiles we have left.
		m_pAmmoCount->SetText(buf);
	}
}