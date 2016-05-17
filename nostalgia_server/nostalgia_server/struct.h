#pragma once
#include "stdafx.h"
//#include <Windows.h>

#pragma pack (push, 1)
struct FIFO
{
	int data;
	int turn;
};
struct Standard
{
	bool operator()(FIFO t, FIFO u)
	{
		return t.turn > u.turn;
	}
};
enum PlayerState
{
	waitState,
	moveState,
	attackCutState,
	attackThrustState
};
enum EOperationType
{
	Sendtype = 1,
	Recvtype,
	MonsterMove,
	MonsterAttack,
	MonsterDead,
	PlayerAttackQ,
	PlayerAttackW,
	PlayerAttackCrush
};

struct OverEx	//오버렙트구조체 확장
{
	OVERLAPPED overLapped;
	SOCKET s;
	EOperationType operationType;	//패킷의 타입
	int prevSize;	//이전데이타 크기
	int currentSize;//현재데이타 크기
	WSABUF buf;
	char packetBuf[256];
	char iocpBuf[4096];
};
struct Object
{
	int kind;
	D3DXVECTOR3 objectPosition;
	float max;
	float min;
};
struct MonsterPacket
{
	int id;
	int type;
	int state;
	D3DXVECTOR3 monsterPos;
	D3DXVECTOR3 monsterDir;
};
struct sectorPosition
{
	int sectorNumX;
	int sectorNumZ;
};

//struct RoomInfo
//{
//	int roomNum;
//	PlayerInfo maxPlayer[4];
//};

#pragma pack (pop)