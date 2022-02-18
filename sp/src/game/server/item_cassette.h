#pragma once

#include "cbase.h"
#include "hl2_player.h"
#include "basecombatweapon.h"
#include "gamerules.h"
#include "items.h"
#include "engine/IEngineSound.h"
#include "props_shared.h"
#include "tier0/memdbgon.h"

class CItemCassette : public CItem
{
public:
	DECLARE_CLASS(CItemCassette, CItem);

	void Spawn(void);
	void Precache(void);
	bool MyTouch(CBasePlayer *pPlayer)
	{
		//CHL2_Player *pHL2Player = dynamic_cast<CHL2_Player *>(pPlayer);
		//return (pHL2Player && pHL2Player->);
		return false;
	}
	void OnPhysGunPickup(CBasePlayer *pPhysGunUser, PhysGunPickup_t reason);
	DECLARE_DATADESC();
private:
	COutputEvent m_OnPlayerPickup;
	COutputEvent m_OnPhysGunPickup;
	COutputEvent m_OnPhysGunOnlyPickup;
	COutputEvent m_OnPhysGunPunt;
};