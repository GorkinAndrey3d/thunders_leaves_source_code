#include "cbase.h"
#include "hl2_player.h" 
#include "basecombatweapon.h"
#include "gamerules.h" 
#include "items.h" 
#include "engine/IEngineSound.h" 
#include "tier0/memdbgon.h"

class CItemArmor : public CBaseAnimating
{
public:
	DECLARE_CLASS(CItemArmor, CBaseAnimating);
	DECLARE_DATADESC();
	void Spawn(void);
	void Precache(void);
	bool MyTouch(CBasePlayer *pPlayer);
};

LINK_ENTITY_TO_CLASS(tl_armor, CItemArmor);

BEGIN_DATADESC(CItemArmor)
DEFINE_ENTITYFUNC(MyTouch),
END_DATADESC()

#define ENTITY_MODEL "models/props_silo/welding_helmet.mdl"


void CItemArmor::Spawn(void)
{
	Precache();
	SetModel(ENTITY_MODEL);
	
	BaseClass::Spawn();
	
}

void CItemArmor::Precache(void)
{
	PrecacheModel(ENTITY_MODEL);

	PrecacheScriptSound("ItemArmor.Touch");

}

bool CItemArmor::MyTouch(CBasePlayer *pPlayer)
{
	CHL2_Player *pHL2Player = dynamic_cast<CHL2_Player *>(pPlayer);
	return (pHL2Player && pHL2Player->GetHealth());
	this->Remove();
}

