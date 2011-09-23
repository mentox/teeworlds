/* CSqlScore Class by Sushi */
#ifndef GAME_SERVER_SQLSCORE_H
#define GAME_SERVER_SQLSCORE_H

#include <cppconn/driver.h>
#include <cppconn/exception.h>
#include <cppconn/resultset.h>
#include <cppconn/statement.h>

#include "../score.h"

class CSqlScore : public IScore
{
	class CGameContext *m_pGameServer;
	class IServer *m_pServer;
	
	sql::Driver *m_pDriver;
	sql::Connection *m_pConnection;
	sql::Statement *m_pStatement;
	sql::ResultSet *m_pResults;
	
	// copy of config vars
	const char* m_pDatabase;
	const char* m_pPrefix;
	const char* m_pUser;
	const char* m_pPass;
	const char* m_pIp;
	char m_aMap[64];
	int m_Port;
	
	CGameContext *GameServer() { return m_pGameServer; }
	IServer *Server() { return m_pServer; }
	
	static void LoadScoreThread(void *pUser);
	static void SaveScoreThread(void *pUser);
	static void ShowRankThread(void *pUser);
	static void ShowTop5Thread(void *pUser);
	
	void Init();
	
	bool Connect();
	void Disconnect();
	
	// anti SQL injection
	void ClearString(char *pString, int Size);
	
public:
	
	CSqlScore(CGameContext *pGameServer);
	~CSqlScore();
	
	void LoadScore(int ClientID);
	void SaveScore(int ClientID, float Time, float *pCpTime, bool NewRecord);
	void ShowRank(int ClientID, const char *pName, bool Search=false);
	void ShowTop5(int ClientID, int Debut=1);
};

struct CSqlScoreData
{
	CSqlScore *m_pSqlData;
	int m_ClientID;
	char m_aName[16];
	char m_aIP[16];
	float m_Time;
	float m_aCpCurrent[NUM_CHECKPOINTS];
	int m_Num;
	bool m_Search;
	char m_aRequestingPlayer[MAX_NAME_LENGTH];
};

#endif
