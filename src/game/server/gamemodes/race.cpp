
#include "race.h"
#include <game/server/gamecontext.h>

CGameControllerRACE::CGameControllerRACE(CGameContext *pGameContext)
	: IRaceController(pGameContext)
{
	m_pGameType = "Race";
}

CGameControllerRACE::~CGameControllerRACE()
{
}
