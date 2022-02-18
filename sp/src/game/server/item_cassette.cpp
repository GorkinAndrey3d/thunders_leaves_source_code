#include "cbase.h"
#include "hl2_player.h"
#include "basecombatweapon.h"
#include "gamerules.h"
#include "item_cassette.h"
#include "items.h"
#include "engine/IEngineSound.h"

// memdbgon must be the last include file in a .cpp file!!!
#include "tier0/memdbgon.h"
#include "item_cassette.h"



BEGIN_DATADESC(CItemCassette)
DEFINE_OUTPUT(m_OnPlayerPickup, "OnPlayerPickup"),
END_DATADESC()

LINK_ENTITY_TO_CLASS(item_cassette, CItemCassette);
PRECACHE_REGISTER(item_cassette);

void CItemCassette::Spawn(void)
{
	Precache();
	SetModel("models/cassette.mdl");
	BaseClass::Spawn();
}

void CItemCassette::Precache(void)
{
	PrecacheModel("models/cassette.mdl");

	PrecacheScriptSound("ItemBattery.Touch");
}

void CItemCassette::OnPhysGunPickup(CBasePlayer * pPhysGunUser, PhysGunPickup_t reason)
{
	BaseClass::OnPhysGunPickup(pPhysGunUser, reason);

	IPhysicsObject *pPhysicsObject = VPhysicsGetObject();
	if (pPhysicsObject && !pPhysicsObject->IsMoveable())
	{
		if (!HasSpawnFlags(SF_PHYSPROP_ENABLE_ON_PHYSCANNON))
			return;

		//EnableMotion();

	}

	m_OnPhysGunPickup.FireOutput(pPhysGunUser, this);

	if (reason == PICKED_UP_BY_CANNON)
	{
		m_OnPhysGunOnlyPickup.FireOutput(pPhysGunUser, this);
	}

	if (reason == PUNTED_BY_CANNON)
	{
		m_OnPhysGunPunt.FireOutput(pPhysGunUser, this);
	}

	if (reason == PICKED_UP_BY_CANNON || reason == PICKED_UP_BY_PLAYER)
	{
		m_OnPlayerPickup.FireOutput(pPhysGunUser, this);
	}
}

