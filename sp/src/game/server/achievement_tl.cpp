
#include "cbase.h"
#include "achievementmgr.h"
#include "baseachievement.h"
#include "iservervehicle.h"
#include "npc_combine.h"
#include "npc_hunter.h"
#include "npc_scanner.h"




// Ep2-specific macro that sets game dir filter.  We need this because Ep1/Ep2/... share a binary so we need runtime check against running game.
#define DECLARE_EP2_MAP_EVENT_ACHIEVEMENT( achievementID, achievementName, iPointValue )					\
	DECLARE_MAP_EVENT_ACHIEVEMENT_( achievementID, achievementName, "thundersleaves", iPointValue, false )

#define DECLARE_EP2_MAP_EVENT_ACHIEVEMENT_HIDDEN( achievementID, achievementName, iPointValue )					\
	DECLARE_MAP_EVENT_ACHIEVEMENT_( achievementID, achievementName, "thundersleaves", iPointValue, true )

class CAchievementTlFindAllCassette : public CBaseAchievement
{
	virtual void Init()
	{
		static const char *szComponents[] =
		{
			"TL_CASSETTE_01", "TL_CASSETTE_02", "TL_CASSETTE_03", "TL_CASSETTE_04", "TL_CASSETTE_05",
			"TL_CASSETTE_06", "TL_CASSETTE_07", "TL_CASSETTE_08", "TL_CASSETTE_09", "TL_CASSETTE_10",
			"TL_CASSETTE_11", "TL_CASSETTE_12"
		};
		SetFlags(ACH_HAS_COMPONENTS | ACH_LISTEN_COMPONENT_EVENTS | ACH_SAVE_GLOBAL);
		m_pszComponentNames = szComponents;
		m_iNumComponents = ARRAYSIZE(szComponents);
		SetComponentPrefix("TL_CASSETTE");
		SetGameDirFilter("thundersleaves");
		SetGoal(m_iNumComponents);
	}

	// don't show progress notifications for this achievement, it's distracting
	virtual bool ShouldShowProgressNotification() { return true; }
};
DECLARE_ACHIEVEMENT(CAchievementTlFindAllCassette, ACHIEVEMENT_TL_CASSETTE, "TL_ACHIEVEMENT_CASSETTE", 5);


class CAchievementTlSuppressor : public CBaseAchievement
{
	virtual void Init()
	{
		static const char *szComponents[] =
		{
			"TL_SUPPRESSOR_01", "TL_SUPPRESSOR_02"
		};
		SetFlags(ACH_HAS_COMPONENTS | ACH_LISTEN_COMPONENT_EVENTS | ACH_SAVE_GLOBAL);
		m_pszComponentNames = szComponents;
		m_iNumComponents = ARRAYSIZE(szComponents);
		SetComponentPrefix("TL_SUPPRESSOR");
		SetGameDirFilter("thundersleaves");
		SetGoal(m_iNumComponents);
	}

	// don't show progress notifications for this achievement, it's distracting
	virtual bool ShouldShowProgressNotification() { return true; }
};
DECLARE_ACHIEVEMENT(CAchievementTlSuppressor, ACHIEVEMENT_TL_SUPPRESSOR, "TL_ACHIEVEMENT_SUPPRESSOR", 5);






class CAchievementTlKillHeavy : public CBaseAchievement
{
protected:

	virtual void Init()
	{
		SetVictimFilter("npc_combine_s");
		SetFlags(ACH_LISTEN_PLAYER_KILL_ENEMY_EVENTS);
		SetGameDirFilter("thundersleaves");
		SetGoal(10);
	}

	virtual void Event_EntityKilled(CBaseEntity *pVictim, CBaseEntity *pAttacker, CBaseEntity *pInflictor, IGameEvent *event)
	{
		CNPC_Combine *pCombine = dynamic_cast<CNPC_Combine *>(pVictim);
		if (pCombine && pCombine->IsHeavy() )
		{
			IncrementCount();
		}
	}
};
DECLARE_ACHIEVEMENT(CAchievementTlKillHeavy, ACHIEVEMENT_TL_HEAVY, "TL_ACHIEVEMENT_HEAVY", 5);

class CAchievementTlCapsule : public CBaseAchievement
{
protected:

	virtual void Init()
	{
		SetFlags(ACH_LISTEN_MAP_EVENTS);
		SetGameDirFilter("thundersleaves");
		SetGoal(10);
	}
	virtual void FireGameEvent(IGameEvent *event)
	{
		IncrementCount();
	}
};
DECLARE_ACHIEVEMENT(CAchievementTlCapsule, ACHIEVEMENT_TL_CAPSULE, "TL_ACHIEVEMENT_CAPSULE", 5);

class CAchievementTlDrone : public CBaseAchievement
{
protected:

	virtual void Init()
	{
		SetVictimFilter("npc_clawscanner");
		SetFlags(ACH_LISTEN_PLAYER_KILL_ENEMY_EVENTS );
		SetGameDirFilter("thundersleaves");
		SetGoal(50);
	}

	virtual void Event_EntityKilled(CBaseEntity *pVictim, CBaseEntity *pAttacker, CBaseEntity *pInflictor, IGameEvent *event)
	{
		CNPC_CScanner *pDrone = dynamic_cast<CNPC_CScanner *>(pVictim);
		if (pDrone)
		{
			IncrementCount();
		}
	}
};
DECLARE_ACHIEVEMENT(CAchievementTlDrone, ACHIEVEMENT_TL_DRONE, "TL_ACHIEVEMENT_DRONE", 5);

class CAchievementTlElectrohack : public CBaseAchievement
{
protected:

	virtual void Init()
	{
		SetFlags(ACH_LISTEN_MAP_EVENTS);
		SetGameDirFilter("thundersleaves");
		SetGoal(7);
	}
	virtual void FireGameEvent(IGameEvent *event)
	{
		IncrementCount();
	}
	virtual bool ShouldShowProgressNotification() { return true; }
};
DECLARE_ACHIEVEMENT(CAchievementTlElectrohack, ACHIEVEMENT_TL_ELECTROHACK, "TL_ACHIEVEMENT_ELECTROHACK", 5);


class CAchievementTlElectrozombie : public CBaseAchievement
{
protected:

	virtual void Init()
	{
		SetFlags(ACH_LISTEN_MAP_EVENTS);
		SetGameDirFilter("thundersleaves");
		SetGoal(10);
	}
	virtual void FireGameEvent(IGameEvent *event)
	{
		IncrementCount();
	}
	virtual bool ShouldShowProgressNotification() { return true; }
};
DECLARE_ACHIEVEMENT(CAchievementTlElectrozombie, ACHIEVEMENT_TL_ELECTROZOMBIE, "TL_ACHIEVEMENT_ELECTROZOMBIE", 5);



// achievements which are won by a map event firing once
DECLARE_EP2_MAP_EVENT_ACHIEVEMENT(ACHIEVEMENT_TL_CAR, "TL_ACHIEVEMENT_CAR", 1);
DECLARE_EP2_MAP_EVENT_ACHIEVEMENT(ACHIEVEMENT_TL_TRUE_ENDING, "TL_ACHIEVEMENT_TRUE_ENDING", 5);
DECLARE_EP2_MAP_EVENT_ACHIEVEMENT(ACHIEVEMENT_TL_PLANT, "TL_ACHIEVEMENT_PLANT", 10);
DECLARE_EP2_MAP_EVENT_ACHIEVEMENT(ACHIEVEMENT_TL_OVERLOAD1, "TL_ACHIEVEMENT_OVERLOAD1", 5);
DECLARE_EP2_MAP_EVENT_ACHIEVEMENT(ACHIEVEMENT_TL_OVERLOAD2, "TL_ACHIEVEMENT_OVERLOAD2", 5);
DECLARE_EP2_MAP_EVENT_ACHIEVEMENT(ACHIEVEMENT_TL_OVERLOAD3, "TL_ACHIEVEMENT_OVERLOAD3", 5);
DECLARE_EP2_MAP_EVENT_ACHIEVEMENT(ACHIEVEMENT_TL_OVERLOAD4, "TL_ACHIEVEMENT_OVERLOAD4", 10);
DECLARE_EP2_MAP_EVENT_ACHIEVEMENT(ACHIEVEMENT_TL_TRAINING, "TL_ACHIEVEMENT_TRAINING", 30);
DECLARE_EP2_MAP_EVENT_ACHIEVEMENT(ACHIEVEMENT_TL_SQUAD, "TL_ACHIEVEMENT_SQUAD", 5);
DECLARE_EP2_MAP_EVENT_ACHIEVEMENT(ACHIEVEMENT_TL_ANTLIONS, "TL_ACHIEVEMENT_ANTLIONS", 35);
DECLARE_EP2_MAP_EVENT_ACHIEVEMENT(ACHIEVEMENT_TL_COLBERT, "TL_ACHIEVEMENT_COLBERT", 35);
DECLARE_EP2_MAP_EVENT_ACHIEVEMENT(ACHIEVEMENT_TL_FELIX, "TL_ACHIEVEMENT_FELIX", 35);
DECLARE_EP2_MAP_EVENT_ACHIEVEMENT(ACHIEVEMENT_TL_SECTORD, "TL_ACHIEVEMENT_SECTORD", 35);
DECLARE_EP2_MAP_EVENT_ACHIEVEMENT(ACHIEVEMENT_TL_TURRET, "TL_ACHIEVEMENT_TURRET", 35);
DECLARE_EP2_MAP_EVENT_ACHIEVEMENT(ACHIEVEMENT_TL_WIND, "TL_ACHIEVEMENT_WIND", 35);