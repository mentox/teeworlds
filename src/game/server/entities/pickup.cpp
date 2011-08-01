/* (c) Magnus Auvinen. See licence.txt in the root of the distribution for more information. */
/* If you are missing that file, acquire a complete release at teeworlds.com.                */
#include <engine/shared/config.h>
#include <game/generated/protocol.h>
#include <game/server/gamecontext.h>
#include "pickup.h"

CPickup::CPickup(CGameWorld *pGameWorld, int Type, int SubType)
: CEntity(pGameWorld, CGameWorld::ENTTYPE_PICKUP, -1)
{
	m_Type = Type;
	m_Subtype = SubType;
	m_ProximityRadius = PickupPhysSize;

	Reset();

	GameWorld()->InsertEntity(this);
}

void CPickup::Reset()
{
	for(int i = 0; i < MAX_CLIENTS; i++)
	{
		if (g_pData->m_aPickups[m_Type].m_Spawndelay > 0)
			m_aSpawnTick[i] = Server()->Tick() + Server()->TickSpeed() * g_pData->m_aPickups[m_Type].m_Spawndelay;
		else
			m_aSpawnTick[i] = -1;
	}
}

void CPickup::Respawn(int Team)
{
	m_aSpawnTick[Team] = -1;
}

void CPickup::Tick()
{
	// wait for respawn
	for(int i = 0; i < MAX_CLIENTS; i++)
	{
		if(m_aSpawnTick[i] > 0)
		{
			if(Server()->Tick() > m_aSpawnTick[i] && g_Config.m_SvPickupRespawn > -1)
			{
				// respawn
				m_aSpawnTick[i] = -1;

				if(m_Type == POWERUP_WEAPON)
					GameServer()->CreateSound(m_Pos, SOUND_WEAPON_SPAWN, GameServer()->PlayermaskAllTeam(i));
			}
		}
	}
	
	// Check if a player intersected us
	CCharacter *apChrs[MAX_CLIENTS];
	int Num = GameServer()->m_World.FindEntities(m_Pos, 20.0f, (CEntity**)apChrs, MAX_CLIENTS, CGameWorld::ENTTYPE_CHARACTER, -1);
	for(int j = 0; j < Num; j++)
	{
		int PlayerTeam = apChrs[j]->GetPlayer()->GetGameTeam();
		int ClientID = apChrs[j]->GetPlayer()->GetCID();
		if(apChrs[j]->IsAlive() && m_aSpawnTick[PlayerTeam] == -1)
		{
			// player picked us up, is someone was hooking us, let them go
			int RespawnTime = -1;
			switch (m_Type)
			{
				case POWERUP_HEALTH:
					if(apChrs[j]->IncreaseHealth(1))
					{
						GameServer()->CreateSound(m_Pos, SOUND_PICKUP_HEALTH, CmaskOne(ClientID));
						RespawnTime = g_pData->m_aPickups[m_Type].m_Respawntime;
					}
					break;
					
				case POWERUP_ARMOR:
					if(apChrs[j]->IncreaseArmor(1))
					{
						GameServer()->CreateSound(m_Pos, SOUND_PICKUP_ARMOR, CmaskOne(ClientID));
						RespawnTime = g_pData->m_aPickups[m_Type].m_Respawntime;
					}
					break;

				case POWERUP_WEAPON:
					if(m_Subtype >= 0 && m_Subtype < NUM_WEAPONS)
					{
						if(apChrs[j]->GiveWeapon(m_Subtype, 10))
						{
							RespawnTime = g_pData->m_aPickups[m_Type].m_Respawntime;

							if(m_Subtype == WEAPON_GRENADE)
								GameServer()->CreateSound(m_Pos, SOUND_PICKUP_GRENADE, CmaskOne(ClientID));
							else if(m_Subtype == WEAPON_SHOTGUN)
								GameServer()->CreateSound(m_Pos, SOUND_PICKUP_SHOTGUN, CmaskOne(ClientID));
							else if(m_Subtype == WEAPON_RIFLE)
								GameServer()->CreateSound(m_Pos, SOUND_PICKUP_SHOTGUN, CmaskOne(ClientID));

							if(apChrs[j]->GetPlayer())
								GameServer()->SendWeaponPickup(apChrs[j]->GetPlayer()->GetCID(), m_Subtype);
						}
					}
					break;

				case POWERUP_NINJA:
					{
						// activate ninja on target player
						apChrs[j]->GiveNinja();
						RespawnTime = g_pData->m_aPickups[m_Type].m_Respawntime;

						// loop through all players, setting their emotes
						/*CCharacter *pC = static_cast<CCharacter *>(GameServer()->m_World.FindFirst(CGameWorld::ENTTYPE_CHARACTER));
						for(; pC; pC = (CCharacter *)pC->TypeNext())
						{
							if (pC != pChr)
							pC->SetEmote(EMOTE_SURPRISE, Server()->Tick() + Server()->TickSpeed());
						}*/

						apChrs[j]->SetEmote(EMOTE_ANGRY, Server()->Tick() + 1200 * Server()->TickSpeed() / 1000);
						break;
					}
						
				default:
					break;
			};

			if(RespawnTime >= 0)
			{
				char aBuf[256];
				str_format(aBuf, sizeof(aBuf), "pickup player='%d:%s' item=%d/%d", apChrs[j]->GetPlayer()->GetCID(), Server()->ClientName(apChrs[j]->GetPlayer()->GetCID()), m_Type, m_Subtype);
				GameServer()->Console()->Print(IConsole::OUTPUT_LEVEL_DEBUG, "game", aBuf);

				if(g_Config.m_SvPickupRespawn > -1)
					m_aSpawnTick[PlayerTeam] = Server()->Tick() + Server()->TickSpeed() * g_Config.m_SvPickupRespawn;
				else
					m_aSpawnTick[PlayerTeam] = 1;
			}
		}
	}
}

void CPickup::Snap(int SnappingClient)
{
	if(SnappingClient != -1)
		if(m_aSpawnTick[GameServer()->m_apPlayers[SnappingClient]->GetGameTeam()] != -1 || NetworkClipped(SnappingClient))
			return;

	CNetObj_Pickup *pP = static_cast<CNetObj_Pickup *>(Server()->SnapNewItem(NETOBJTYPE_PICKUP, m_ID, sizeof(CNetObj_Pickup)));
	if(!pP)
		return;

	pP->m_X = (int)m_Pos.x;
	pP->m_Y = (int)m_Pos.y;
	pP->m_Type = m_Type;
	pP->m_Subtype = m_Subtype;
}
