#pragma once
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include"protocol.h"
#include"Object.h"
using namespace std;

class LogicServer
{
	static HANDLE io;	//ºó Æ÷Æ® »ý¼º
	static int id;
	static int count;
	//static PlayerInfo player[ROOM_MAX_PLAYER];
	static map<int,PlayerInfo> player;
	static map<int, Object*> objectList;
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
	static bool crashObject(D3DXVECTOR3 position,int id);
	static void crashMaxMapSize(int id);
	static void viewListUpdate(int id);
	static void viewListObjectUpdate(int id);
	static void playerActProcessPacket(OverEx *operation,int id);
	static void monsterActProcessPacket(OverEx *operation, int id, float delTime);
};

