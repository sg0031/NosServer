#pragma once
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include"protocol.h"
#include"Monster.h"
using namespace std;

class LogicServer
{
	static HANDLE io;	//ºó Æ÷Æ® »ý¼º
	static int id;
	static int count;
	static PlayerInfo player[ROOM_MAX_PLAYER];
	static priority_queue<FIFO,vector<FIFO>,Standard> playerID;
	static mutex myLock;
	static Sector sector[SECTOR_WIDETH][SECTOR_LENGTH];
	static TimerThread timer;
public:
	LogicServer();
	~LogicServer();
	static void acceptThread();
	static void lobbyThread();
	static void workerThread();
	static void processPacket(int id, char *ptr,double deltTime);
	static void sendPacket(int client, void* packet);
	static void searchSector(int id);
	static void setObject(int id);
	static bool crashObject(D3DXVECTOR3 position,int id);
	static void crashMaxMapSize(int id);
	static void viewListUpdate(int id);
	static void playerActProcessPacket(OverEx *operation,int id);
	static void monsterActProcessPacket(OverEx *operation, int id, float delTime);
};

