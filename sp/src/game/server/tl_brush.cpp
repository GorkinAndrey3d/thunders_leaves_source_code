#include "cbase.h"


class TLBrush : public CBaseToggle
{
public:
	DECLARE_CLASS(TLBrush, CBaseToggle);
	DECLARE_DATADESC();
	void Spawn(void);
	bool CreateVPhysics(void);
	void BrushTouch(CBaseEntity *pother);
	virtual int OnTakeDamage(const CTakeDamageInfo &info);
};

LINK_ENTITY_TO_CLASS(tl_brush, TLBrush);

BEGIN_DATADESC(TLBrush)
DEFINE_ENTITYFUNC(BrushTouch),
END_DATADESC()

void TLBrush::Spawn(void)
{
	SetTouch(&TLBrush::BrushTouch);
	SetSolid(SOLID_VPHYSICS);
	SetMoveType(MOVETYPE_PUSH);
	CollisionProp()->UseTriggerBounds(true, 50);
	SetModel(STRING(GetModelName()));
	CreateVPhysics();
}

bool TLBrush::CreateVPhysics(void)
{
	VPhysicsInitShadow(false, false);
	return true;
}

void TLBrush::BrushTouch(CBaseEntity *pother)
{
	const trace_t &tr = GetTouchTrace();
	Vector vecPushDir = tr.plane.normal;
	vecPushDir.Negate();
	vecPushDir.z = 0.0f;
	LinearMove(GetAbsOrigin() + (vecPushDir*256.0f), 600.0f);
}

int TLBrush::OnTakeDamage(const CTakeDamageInfo &info)
{
	Vector	vecTemp;

	CTakeDamageInfo subInfo = info;

	// Check our damage filter

	vecTemp = subInfo.GetInflictor()->GetAbsOrigin() - WorldSpaceCenter();


	// HACK: slam health back to what it was so UpdateHealth can do its thing

	// Make a shard noise each time func breakable is hit, if it's capable of taking damage
	if (m_takedamage == DAMAGE_YES)
	{
		BaseClass::Remove();
	}

	return 1;
}