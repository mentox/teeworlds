
#include "race.h"

class CGameControllerHPRACE : public CGameControllerRACE
{
public:
	CGameControllerHPRACE(class CGameContext *pGameServer);
	virtual ~CGameControllerHPRACE();

	virtual bool IsHammerParty() const { return true; }
	virtual int GetAutoGameTeam(int ClientID);
};

