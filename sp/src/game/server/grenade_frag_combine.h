#ifndef GRENADE_FRAG_COMBINE_H
#define GRENADE_FRAG_COMBINE_H
#pragma once

class CBaseGrenade;
struct edict_t;

CBaseGrenade *Fraggrenade_Create_combine(const Vector &position, const QAngle &angles, const Vector &velocity, const AngularImpulse &angVelocity, CBaseEntity *pOwner, float timer, bool combineSpawned);
bool	Fraggrenade_WasPunted_combine(const CBaseEntity *pEntity);
bool	Fraggrenade_WasCreatedByCombine_combine(const CBaseEntity *pEntity);

#endif // GRENADE_FRAG_H
