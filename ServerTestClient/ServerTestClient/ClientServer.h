#pragma once

#include"ServerPlayer.h"

using namespace std;

#pragma pack()

class ClientServer
{
	HWND socketHWND;
	HINSTANCE ghInst;

	SOCKET sock;
	WSABUF wsaSendBuf;
	char sendBuf[MAX_SIZE];
	WSABUF wsaRecvBuf;
	char recvBuf[MAX_SIZE];
	WSABUF wsaCompleteBuf;
	char completeBuf[MAX_SIZE];

	int *inPacketSize;
	int size;

	int savePacketSize;

	int count;
	int myId;
public:
	DWORD dirFlag;
	float currentTime;
	bool keyDownFlag;
	int keyUpFlag;
	bool flag;
	double time;
	Sector sector[SECTOR_WIDETH][SECTOR_LENGTH];
	ServerPlayer Player[ROOM_MAX_PLAYER];
	Object objectList[100];
	MonsterPacket monsterList[100];
	//LPCTSTR IpszClass = "Test Client";
	//--------------------------------------------
	void setSocketHWND(HWND s);
	void setHINSTANCE(HINSTANCE g);
	//--------------------------------------
	static ClientServer* getInstangce()
	{
		static ClientServer inst;
		return &inst;
	}
	//---------------------------------------
	ClientServer();
	int socketInit();
	void readPacket();

	void keyUp();
	void keyDownAttacket(DWORD key);
	void keyDownAttackTrust(DWORD key);
	void processPacket(char* buf);
	void sendPacket(SOCKET s, void* buf);
	void keyDown(D3DXVECTOR3 dir, D3DXVECTOR3 pos);
	void dirKeyUp(D3DXVECTOR3 dir, D3DXVECTOR3 pos);
	void infoRequest(D3DXVECTOR3 dir, D3DXVECTOR3 pos);
	int getMyId();
	~ClientServer();
};

