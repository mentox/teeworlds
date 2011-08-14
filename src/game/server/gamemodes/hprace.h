
#include "race.h"

class CGameControllerHPRACE : public CGameControllerRACE
{
private:
	int m_aPartnerWishes[MAX_CLIENTS];
public:
	CGameControllerHPRACE(class CGameContext *pGameServer);
	virtual ~CGameControllerHPRACE();

	virtual bool CanJoinTeam(int Team, int ClientID);

	virtual bool IsHammerParty() const { return true; }
	virtual bool FakeCollisionTune() const { return false; }
	virtual bool FakeHookTune() const { return false; }
	virtual bool CanUsePartnerCommands() const { return true; }

	virtual void ChatCommandWith(int ClientID, const char *pName = 0);
	virtual void TryCreateTeam(int ClientID, int With);

	virtual int GetEmptyTeam();

	virtual int GetAutoTeam(int ClientID);
	virtual int GetAutoGameTeam(int ClientID);

	virtual void OnPlayerDisconnect(CPlayer *pPlayer);
};

