#include "cbase.h"
#include "Sprite.h"
#include "particle_parse.h"
#include "baseanimating.h"
#include "IEffects.h"
#include "spark.h"

class CTLtestOne2 : public CBaseAnimating
{
public:
	DECLARE_CLASS(CTLtestOne2, CBaseAnimating);
	DECLARE_DATADESC();

	CTLtestOne2()
	{
		m_bActive = false;
	}
	void Spawn(void);
	void Precache(void);
	void MoveThink(void);
	void AnotherThink(void);
	void AbsoluteAngleThink(void);
	void	CreateGlow(void);
	virtual int		OnTakeDamage(const CTakeDamageInfo &info);
	void Hit(const CTakeDamageInfo &info);
	void InputToggle(inputdata_t &inputData);
	void InputToggle2(inputdata_t &inputData);
	void InputToggle3(inputdata_t &inputData);
	virtual void	TraceAttack(const CTakeDamageInfo &info, const Vector &vecDir, trace_t *ptr);
private:
	bool m_bActive;
	bool m_bShieldActive;
	float m_flNextChangeTime;
	float m_flNextChangeTime2;
	const int SF_SPARK_SILENT = 256;
	CHandle<CSprite>	m_hGlowSprite;
	
	
};

class CTLSuppressorShieldAttach : public CBaseAnimating
{
public:
	DECLARE_CLASS(CTLSuppressorShieldAttach, CBaseAnimating);
	DECLARE_DATADESC();
	void Spawn(void);
	virtual void Precache(void);
	
};
LINK_ENTITY_TO_CLASS(npc_suppressor_shield_attach2, CTLSuppressorShieldAttach);

BEGIN_DATADESC(CTLSuppressorShieldAttach)

END_DATADESC()

LINK_ENTITY_TO_CLASS(npc_tltestOne2, CTLtestOne2);

BEGIN_DATADESC(CTLtestOne2)

DEFINE_FIELD(m_bActive, FIELD_BOOLEAN),
DEFINE_FIELD(m_flNextChangeTime, FIELD_TIME),
DEFINE_INPUTFUNC(FIELD_VOID, "Toggle", InputToggle),
DEFINE_INPUTFUNC(FIELD_VOID, "Toggle2", InputToggle2),
DEFINE_INPUTFUNC(FIELD_VOID, "Toggle3", InputToggle3),
DEFINE_THINKFUNC(MoveThink),

END_DATADESC()

#define ENTITY_MODEL "models/gibs/airboat_broken_engine.mdl"
#define ENTITY_MODEL_SUP "models/supressor_sphere.mdl"


void CTLtestOne2::Precache(void)
{
	PrecacheModel(ENTITY_MODEL);
	PrecacheModel(ENTITY_MODEL_SUP);
	PrecacheParticleSystem("explosion_turret_break_sparks");
	BaseClass::Precache();

}

void CTLSuppressorShieldAttach::Precache(void)
{
	PrecacheModel(ENTITY_MODEL_SUP);
}

void CTLtestOne2::Hit(const CTakeDamageInfo &info)
{
	
}

void CTLSuppressorShieldAttach::Spawn(void)
{
	Precache();
	SetModel(ENTITY_MODEL_SUP);
	SetSolid(SOLID_NONE);
	m_takedamage = DAMAGE_YES;
	m_iHealth = 20000;
}


void CTLtestOne2::Spawn(void)
{
	Precache();
	SetModel(ENTITY_MODEL);
	SetSolid(SOLID_BBOX);
	CreateGlow();
	UTIL_SetSize(this, -Vector(10, 10, 10), Vector(10, 10, 10));
	m_takedamage = DAMAGE_YES;
	m_iHealth = 200;
}




void CTLtestOne2::AnotherThink(void)
{
	
	if (m_flNextChangeTime < gpGlobals->curtime)
	{
		
		Vector vecNewAngle = RandomVector(-256.0f, 256.0f);
		QAngle angFacing;
		VectorAngles(vecNewAngle, angFacing);
		SetAbsAngles(angFacing);
		m_flNextChangeTime = gpGlobals->curtime + float(3.0f);
		Vector vecForward, vecUp;
		Vector vecDir = vec3_origin;
		Vector vecSplortPos = GetAbsOrigin() + vecForward * 14.0f;
		DoSpark(this, vecSplortPos, 1.0f, 1.0f, true, vecDir);
	}
	SetNextThink(gpGlobals->curtime + 0.05f);
}

void CTLtestOne2::AbsoluteAngleThink(void)
{
	Vector vecAbsAngle = Vector(0.0f, 0.0f, 0.0f);
	QAngle absAngFacing;
	VectorAngles(vecAbsAngle, absAngFacing);
	SetAbsAngles(absAngFacing);
	SetNextThink(gpGlobals->curtime + 0.05f);
}


void CTLtestOne2::InputToggle(inputdata_t &inputData)
{
	if (!m_bActive)
	{	
		
		///////////////SPAWN SHIELD////////
		
		
		
		
		///////////START THINK///////////////
		SetThink(&CTLtestOne2::MoveThink);
		SetNextThink(gpGlobals->curtime + 0.05f);
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

void CTLtestOne2::MoveThink(void)
{
	Vector SupvecAbsAngle = Vector(0.0f, 0.0f, 0.0f);
	QAngle SupabsAngFacing;
	VectorAngles(SupvecAbsAngle, SupabsAngFacing);

	CTLSuppressorShieldAttach *shield = (CTLSuppressorShieldAttach *)CreateEntityByName("npc_suppressor_shield_attach2");
	if (shield == NULL)
		return;
	Vector vecOrigin;
	Vector SupOrigin = GetAbsOrigin();
	if (m_bShieldActive == false)
	{
		shield->SetAbsOrigin(vecOrigin);
		shield->SetAbsAngles(SupabsAngFacing);
		DispatchSpawn(shield);//
		shield->SetAbsOrigin(SupOrigin);
		shield->SetParent(this);
	}
	m_bShieldActive = true;


	if (m_flNextChangeTime < gpGlobals->curtime)
	{

		Vector vecNewVelocity = RandomVector(0.0f, 100.0f);
		vecNewVelocity.z = 0.0f;
		SetAbsVelocity(vecNewVelocity);
		m_flNextChangeTime = gpGlobals->curtime + random->RandomFloat(0.0f, 4.0f);
		shield->SetAbsAngles(SupabsAngFacing);
		
		
		
	}
	shield->SetAbsAngles(SupabsAngFacing);
	Vector velFacing = GetAbsVelocity();
	velFacing.z = 0.0f;
	QAngle angFacing;
	VectorAngles(velFacing, angFacing);
	angFacing.z = 0.0f;
	SetAbsAngles(angFacing);
	Vector vecAbsAngle = Vector(0.0f, 0.0f, 0.0f);
	QAngle absAngFacing;
	VectorAngles(vecAbsAngle, absAngFacing);
	Quaternion SupQuat;
	AxisAngleQuaternion(vecAbsAngle, 0, SupQuat);
	
	shield->SetAbsAngles(SupabsAngFacing);


	SetNextThink(gpGlobals->curtime + 0.05f);

}

void CTLtestOne2::InputToggle2(inputdata_t &inputData)
{
	if (!m_bActive)
	{
		SetThink(&CTLtestOne2::AnotherThink);
		SetNextThink(gpGlobals->curtime + 0.05f);
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

void CTLtestOne2::InputToggle3(inputdata_t &inputData)
{
	if (!m_bActive)
	{
		SetThink(&CTLtestOne2::AbsoluteAngleThink);
		SetNextThink(gpGlobals->curtime + 0.05f);
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

void CTLtestOne2::CreateGlow(void)
{
	m_hGlowSprite = CSprite::SpriteCreate("sprites/grubflare1.vmt", GetLocalOrigin(), false);

	Assert(m_hGlowSprite);
	if (m_hGlowSprite == NULL)
	{
		return;
	}
	m_hGlowSprite->TurnOn();
	m_hGlowSprite->SetTransparency(kRenderWorldGlow, 156, 169, 121, 255, kRenderFxNoDissipation);
	m_hGlowSprite->SetScale(2.0f);
	m_hGlowSprite->SetGlowProxySize(16.0f);
	int nAttachment = LookupAttachment("glow");
	m_hGlowSprite->SetParent(this, nAttachment);
	m_hGlowSprite->SetLocalOrigin(vec3_origin);
	m_hGlowSprite->SetThink(NULL);
	m_hGlowSprite->SetNextThink(TICK_NEVER_THINK);
}

void CTLtestOne2::TraceAttack(const CTakeDamageInfo &info, const Vector &vecDir, trace_t *ptr)
{
	QAngle vecAngles;
	VectorAngles(-vecDir, vecAngles);
	DispatchParticleEffect("GrubBlood", ptr->endpos, vecAngles);
	BaseClass::TraceAttack(info, vecDir, ptr);
	m_hGlowSprite->TurnOff();
	return BaseClass::TraceAttack(info, vecDir, ptr);
}

int CTLtestOne2::OnTakeDamage(const CTakeDamageInfo &info)
{
	if (m_iHealth < 150)
	{
		m_hGlowSprite->TurnOff();
		Vector vecForward, vecUp;
		Vector vecDir = vec3_origin;
		Vector vecSplortPos = GetAbsOrigin() + vecForward * 14.0f;
		DispatchParticleEffect("explosion_turret_break_sparks", vecSplortPos, GetAbsAngles());
		DoSpark(this, vecSplortPos, 100, 100, false, vecDir);
	}
	return BaseClass::OnTakeDamage(info);
}