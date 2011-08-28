
#include <game/server/racecontroller.h>

class CGameControllerHPRACE : public IRaceController
{
public:
	CGameControllerHPRACE(class CGameContext *pGameServer);
	virtual ~CGameControllerHPRACE();

	virtual bool IsHammerParty() const { return true; }
	virtual bool FakeCollisionTune() const { return false; }
	virtual bool FakeHookTune() const { return false; }
	virtual bool CanUsePartnerCommands() const { return true; }
};

