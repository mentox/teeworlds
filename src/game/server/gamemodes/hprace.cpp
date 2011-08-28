
#include <game/server/gamecontext.h>

#include "hprace.h"

CGameControllerHPRACE::CGameControllerHPRACE(class CGameContext *pGameServer)
	: IRaceController(pGameServer)
{
	m_pGameType = "HpRace";
	for(int i = 0; i < MAX_CLIENTS; i++)
		m_aPartnerWishes[i] = -1;
}

CGameControllerHPRACE::~CGameControllerHPRACE()
{
}

