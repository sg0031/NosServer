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

struct OverEx	//������Ʈ����ü Ȯ��
{
	OVERLAPPED overLapped;
	SOCKET s;
	EOperationType operationType;	//��Ŷ�� Ÿ��
	int prevSize;	//��������Ÿ ũ��
	int currentSize;//���絥��Ÿ ũ��
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