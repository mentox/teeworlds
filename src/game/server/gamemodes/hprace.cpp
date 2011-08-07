
#include "hprace.h"

CGameControllerHPRACE::CGameControllerHPRACE(class CGameContext *pGameServer)
	: CGameControllerRACE(pGameServer)
{
	m_pGameType = "HpRace";
}

CGameControllerHPRACE::~CGameControllerHPRACE()
{
}

int CGameControllerHPRACE::GetAutoGameTeam(int ClientID)
{
	return -1;
}

