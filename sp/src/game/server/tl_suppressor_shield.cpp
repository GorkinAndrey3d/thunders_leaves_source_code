#include "cbase.h"
#include "Sprite.h"
#include "particle_parse.h"
#include "IEffects.h"
#include "spark.h"

#define SHIELD_IMPACT_PARTICLE "hunter_muzzle_flash_b"
class CTLSuppressorShield : public CBaseAnimating
{
public:
	DECLARE_CLASS(CTLSuppressorShield, CBaseAnimating);
	DECLARE_DATADESC();

	CTLSuppressorShield()
	{
		m_bActive = false;
	}
	void Spawn(void);
	int	OnTakeDamage_Alive(const CTakeDamageInfo &info);
	void Precache(void);
	int	OnTakeDamage(const CTakeDamageInfo &info);
	void AbsoluteAngleThink(void);
	void Hit(const CTakeDamageInfo &info);
	void InputToggle(inputdata_t &inputData);
private:
	bool m_bActive;
	float m_flNextChangeTime;
	float m_flNextChangeTime2;
	const int SF_SPARK_SILENT = 256;
};

LINK_ENTITY_TO_CLASS(npc_suppressor_shield, CTLSuppressorShield);
BEGIN_DATADESC(CTLSuppressorShield)

DEFINE_FIELD(m_bActive, FIELD_BOOLEAN),
DEFINE_FIELD(m_flNextChangeTime, FIELD_TIME),
DEFINE_INPUTFUNC(FIELD_VOID, "Toggle", InputToggle),
DEFINE_THINKFUNC(AbsoluteAngleThink),

END_DATADESC()

#define ENTITY_MODEL "models/supressor_sphere.mdl"

void CTLSuppressorShield::Precache(void)
{
	PrecacheModel(ENTITY_MODEL);
	PrecacheParticleSystem("explosion_turret_break_sparks");
	PrecacheSound("weapons/physcannon/energy_sing_flyby2.wav");
	PrecacheParticleSystem(SHIELD_IMPACT_PARTICLE);
	BaseClass::Precache();

}

void CTLSuppressorShield::Hit(const CTakeDamageInfo &info)
{

}

int CTLSuppressorShield::OnTakeDamage(const CTakeDamageInfo & info)
{
	CTakeDamageInfo tdInfo = info;
	SetThink(NULL);
	Vector vecForward, vecUp;
	Vector vecDir = vec3_origin;
	Vector vecSplortPos = GetAbsOrigin() + RandomVector(-100.0f, 100.0f);
	DoSpark(this, vecSplortPos, 1.0f, 1.0f, true, vecDir);
	EmitSound("weapons/physcannon/energy_sing_flyby2.wav");
	//DoSpark(this, dis, 100, 100, false, vecDir);

	return BaseClass::OnTakeDamage(info);
}

void CTLSuppressorShield::Spawn(void)
{
	Precache();
	SetModel(ENTITY_MODEL);
	SetSolid(SOLID_VPHYSICS);
	m_takedamage = DAMAGE_YES;
	m_iHealth = 20000;	
}

int CTLSuppressorShield::OnTakeDamage_Alive(const CTakeDamageInfo & info)
{
	BaseClass::Remove();
	return 0;
}

void CTLSuppressorShield::AbsoluteAngleThink(void)
{
	Vector vecAbsAngle = Vector(0.0f, 0.0f, 0.0f);
	QAngle absAngFacing;
	VectorAngles(vecAbsAngle, absAngFacing);
	SetAbsAngles(absAngFacing);
	SetNextThink(gpGlobals->curtime + 0.01f);
}

void CTLSuppressorShield::InputToggle(inputdata_t &inputData)
{
	if (!m_bActive)
	{
		SetThink(&CTLSuppressorShield::AbsoluteAngleThink);
		SetNextThink(gpGlobals->curtime + 0.01f);
		SetMoveType(MOVETYPE_FLY);
		m_flNextChangeTime = gpGlobals->curtime;
		m_bActive = true;
	}
	else
	{
		SetThink(NULL);
		SetAbsVelocity(vec3_origin);
		SetMoveType(MOVETYPE_NONE);
		m_bActive = false;
	}
}