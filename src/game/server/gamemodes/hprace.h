
#include "race.h"

class CGameControllerHPRACE : public CGameControllerRACE
{
public:
	CGameControllerHPRACE(class CGameContext *pGameServer);
	virtual ~CGameControllerHPRACE();

	virtual bool IsHammerParty() const { return true; }
	virtual bool FakeCollisionTune() const { return false; }
	virtual bool FakeHookTune() const { return false; }
	virtual int GetAutoGameTeam(int ClientID);
};

