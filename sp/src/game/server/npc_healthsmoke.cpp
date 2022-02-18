#include "cbase.h"
#include "particle_parse.h"
#include "IEffects.h"
#include "hl2_player.h" 
#include "basecombatweapon.h"
#include "gamerules.h" 
#include "items.h" 
#include "engine/IEngineSound.h" 
#include "tier0/memdbgon.h"
#include "entityoutput.h"
#include "player_pickup.h"
#include "vphysics/constraints.h"
#include "player.h"
#include "gamerules.h"
#include "iservervehicle.h"
#include "physics_saverestore.h"
#include "world.h"

class HealthSmoke : public  CBaseAnimating, public CDefaultPlayerPickupVPhysics
{
public:
	DECLARE_CLASS(HealthSmoke, CBaseAnimating);
	DECLARE_DATADESC();

	HealthSmoke()
	{
		m_bActive = false;
	}

	void Spawn(void);
	void Precache(void);

	unsigned int PhysicsSolidMaskForEntity(void) const;

	void MoveThink(void);
	void Toggle(void);

	virtual void BrushTouch(CBaseEntity *pother);
	//virtual bool MyTouch(CBasePlayer *pPlayer) { return false; };

	virtual bool	tlItemCanBeTouchedByPlayer(CBasePlayer *pPlayer);
	virtual bool MyTouch(CBasePlayer *pPlayer) { return false; };


private:

	bool	m_bActive;
	float	m_flNextChangeTime;
	float   m_flFadeTime;
};

LINK_ENTITY_TO_CLASS(npc_HealthSmoke, HealthSmoke);

// Start of our data description for the class
BEGIN_DATADESC(HealthSmoke)

// Save/restore our active state
DEFINE_FIELD(m_bActive, FIELD_BOOLEAN),
DEFINE_FIELD(m_flNextChangeTime, FIELD_TIME),

//DEFINE_FUNCTION(BrushTouch),

// Links our input name from Hammer to our input member function

// Declare our think function
DEFINE_THINKFUNC(MoveThink),

END_DATADESC()

// Name of our entity's model
#define	ENTITY_MODEL	"models/gibs/airboat_broken_engine.mdl"

//-----------------------------------------------------------------------------
// Purpose: Precache assets used by the entity
//-----------------------------------------------------------------------------
void HealthSmoke::Precache(void)
{
	PrecacheModel(ENTITY_MODEL);
	PrecacheParticleSystem("tl_smoke_healthh");
	BaseClass::Precache();
}

//-----------------------------------------------------------------------------
// Purpose: Sets up the entity's initial state
//-----------------------------------------------------------------------------
void HealthSmoke::Spawn(void)
{
	Precache();
	SetSolid(SOLID_BBOX);
	SetBlocksLOS(false);
	UTIL_SetSize(this, -Vector(10, 10, 10), Vector(10, 10, 10));
	AddEFlags(EFL_NO_ROTORWASH_PUSH);
	SetCollisionGroup(COLLISION_GROUP_WEAPON);
	CollisionProp()->UseTriggerBounds(true, 200);
	SetTouch(&HealthSmoke::BrushTouch);
	//SetTouch(&HealthSmoke::BrushTouch);
	SetMoveType(MOVETYPE_FLYGRAVITY, MOVECOLLIDE_FLY_BOUNCE);

	SetGravity(UTIL_ScaleForGravity(20));
	Toggle();
	
}

unsigned int HealthSmoke::PhysicsSolidMaskForEntity(void) const
{
	return BaseClass::PhysicsSolidMaskForEntity() | CONTENTS_PLAYERCLIP;
}

//-----------------------------------------------------------------------------
// Purpose: Think function to randomly move the entity
//-----------------------------------------------------------------------------
void HealthSmoke::MoveThink(void)
{

	// See if we should change direction again
	if (m_flNextChangeTime < gpGlobals->curtime)
	{
		// Randomly take a new direction and speed
		DispatchParticleEffect("tl_smoke_healthh", GetAbsOrigin(), GetAbsAngles());
		// Randomly change it again within one to three seconds
		m_flNextChangeTime = gpGlobals->curtime + random->RandomFloat(0.1f, 0.1f);

	}
	if (m_flFadeTime < gpGlobals->curtime)
	{
		this->Remove();
	}

	/*
	// Snap our facing to where we're heading
	Vector velFacing = Vector(-90.0f);
	QAngle angFacing;
	VectorAngles(velFacing, angFacing);
	velFacing.x = 0.0f;
	velFacing.y = 0.0f;
	velFacing.z = -20.0f;
	angFacing.x = 0.0f;
	angFacing.y = 0.0f;
	angFacing.z = 90.0f;
	SetAbsAngles(angFacing);
	SetAbsVelocity(velFacing);
	// Think every 20Hz
	*/

	SetNextThink(gpGlobals->curtime + 0.05f);
}

//-----------------------------------------------------------------------------
// Purpose: Toggle the movement of the entity
//-----------------------------------------------------------------------------



void HealthSmoke::Toggle()
{
	if (!m_bActive)
	{
		m_flFadeTime = gpGlobals->curtime + float(2.0f);
		// Start thinking
		SetThink(&HealthSmoke::MoveThink);
		SetNextThink(gpGlobals->curtime + 0.05f);
		m_flNextChangeTime = gpGlobals->curtime;
		m_bActive = true;
	}
	else
	{
		SetThink(NULL);
		m_bActive = false;
	}
}


void HealthSmoke::BrushTouch(CBaseEntity *pOther)
{
	UTIL_Remove(this);
}

bool UTIL_tlItemCanBeTouchedByPlayer(CBaseEntity *pItem, CBasePlayer *pPlayer)
{
	if (pItem == NULL || pPlayer == NULL)
		return false;

	// For now, always allow a vehicle riding player to pick up things they're driving over
	if (pPlayer->IsInAVehicle())
		return true;

	// Get our test positions
	Vector vecStartPos;
	IPhysicsObject *pPhysObj = pItem->VPhysicsGetObject();
	if (pPhysObj != NULL)
	{
		// Use the physics hull's center
		QAngle vecAngles;
		pPhysObj->GetPosition(&vecStartPos, &vecAngles);
	}
	else
	{
		// Use the generic bbox center
		vecStartPos = pItem->CollisionProp()->WorldSpaceCenter();
	}

	Vector vecEndPos = pPlayer->EyePosition();

	// FIXME: This is the simple first try solution towards the problem.  We need to take edges and shape more into account
	//		  for this to be fully robust.

	// Trace between to see if we're occluded
	trace_t tr;
	CTraceFilterSkipTwoEntities filter(pPlayer, pItem, COLLISION_GROUP_PLAYER_MOVEMENT);
	UTIL_TraceLine(vecStartPos, vecEndPos, MASK_SOLID, &filter, &tr);

	// Occluded
	// FIXME: For now, we exclude starting in solid because there are cases where this doesn't matter
	if (tr.fraction < 1.0f)
		return false;

	return true;
}

bool HealthSmoke::tlItemCanBeTouchedByPlayer(CBasePlayer *pPlayer)
{
	return UTIL_tlItemCanBeTouchedByPlayer(this, pPlayer);
}


