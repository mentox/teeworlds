/* copyright (c) 2007 rajh and gregwar. Score stuff */
#ifndef GAME_SERVER_GAMEMODES_RACE_H
#define GAME_SERVER_GAMEMODES_RACE_H

#include <game/server/gamecontext.h>
#include <game/server/gamecontroller.h>

class CGameControllerRACE : public IGameController
{
public:
	enum
	{
		RACE_NONE = 0,
		RACE_STARTED,
		RACE_FINISHED,
		RACE_TEAM_STARTED,
	};

	struct CRaceData
	{
		int m_RaceState;
		int m_StartTime;
		int m_RefreshTime;

		float m_aCpCurrent[25];
		int m_CpTick;

		float m_StartAddTime;

		void Reset()
		{
			m_RaceState = RACE_NONE;
			m_StartTime = -1;
			m_RefreshTime = -1;
			for(unsigned i = 0; i < sizeof(m_aCpCurrent) / sizeof(m_aCpCurrent[0]); i++)
				m_aCpCurrent[i] = 0.0f;
			m_CpTick = -1;
			m_StartAddTime = 0.0f;
		}
	} m_aRace[16];

	struct CPlayerRaceData
	{
		int m_State;
		float m_CpDiff;

		void Reset()
		{
			m_State = RACE_NONE;
			m_CpDiff = 0.0f;
		}
	} m_aPlayerRace[MAX_CLIENTS];
	
	CGameControllerRACE(class CGameContext *pGameServer);
	~CGameControllerRACE();

	virtual bool FakeCollisionTune() const { return true; }
	virtual bool FakeHookTune() const { return true; }
	
	vec2 *m_pTeleporter;
	
#if defined(CONF_TEERACE)
	int m_aStopRecordTick[MAX_CLIENTS];
#endif
	
	void InitTeleporter();

	virtual void DoWincheck();
	virtual void Tick();
	virtual int OnCharacterDeath(class CCharacter *pVictim, class CPlayer *pKiller, int Weapon);

	virtual bool OnCheckpoint(int ID, int z);
	virtual bool OnRaceStart(int ID, float StartAddTime, bool Check=true);
	virtual bool OnRaceEnd(int ID, float FinishTime);

	virtual int GetAutoGameTeam(int ClientID);

	float GetTime(int ID);
};

#endif
