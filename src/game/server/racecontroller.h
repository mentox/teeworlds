/* copyright (c) 2007 rajh and gregwar. Score stuff */
#ifndef GAME_SERVER_RACECONTROLLER_H
#define GAME_SERVER_RACECONTROLLER_H

#include "gamecontroller.h"

class IRaceController : public IGameController
{
public:
	enum
	{
		RACE_NONE = 0,
		RACE_STARTED,
		RACE_FINISHED,
		RACE_TEAM_STARTED,
	};

public: // TODO: fix this
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
	} m_aRace[MAX_TEAMS];

protected:
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

	int m_aPartnerWishes[MAX_CLIENTS];

public:

	IRaceController(class CGameContext *pGameServer);
	~IRaceController();

	bool IsRaceController() const { return true; }
	virtual bool IsFastCap() const { return false; }

	virtual bool IsHammerParty() const { return false; }
	virtual bool CanUsePartnerCommands() const { return false; }

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
	virtual void OnPlayerDisconnect(class CPlayer *pPlayer);

	virtual int GetAutoGameTeam(int ClientID);
	virtual int GetAutoTeam(int ClientID);

	float GetTime(int ID);

	virtual void ChatCommandWith(int ClientID, const char *pName = 0);
	virtual void ChatCommandLeaveTeam(int ClientID);

	virtual void TryCreateTeam(int ClientID, int With);
	virtual void LeaveTeam(int ClientID, bool Disconnect = false);

	virtual bool GetEmptyTeam(int Team);
	virtual int GetRandTeam();

	virtual bool CanJoinTeam(int Team, int ClientID);
};

#endif

