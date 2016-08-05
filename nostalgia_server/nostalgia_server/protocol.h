#pragma once
#include "stdafx.h"
//#include <WinSock2.h>
//#include <D3DX10Math.h>
#include "struct.h"

#pragma pack (push, 1)
// Game Logic Server 
// SC_Packet
struct ScPacketUpdateState
{
	int packetSize;
	BYTE packetType;
	int id;
	PlayerState state;
};
struct ScpacketAttackPossible
{
	int packetSize;
	BYTE packetType;
	int id;
	PlayerState state;
};
struct ScPacektPutPlayer
{
	int packetSize;
	BYTE packetType;
	int id;
	D3DXVECTOR3 position;
	D3DXVECTOR3 direction;
	PlayerState state;
};
struct ScPacketRemoveObject
{
	int packetSize;
	BYTE packetType;
	int id;
};
struct ScPacketMove
{
	int packetSize;
	BYTE packetType;
	int id;
	double time;
	D3DXVECTOR3 direction;
	D3DXVECTOR3 position;
	PlayerState state;
};
struct ScPacketSector
{
	int packetSize;
	BYTE packetType;
	int id;
	Sector area[3][3];
};
//struct ScPacketObject
//{
//	int packetSize;
//	BYTE packetType;
//	int id;
//	sectorPosition position;
//	Object objects[100];
//};
//struct ScPacketMonsterList
//{
//	int packetSize;
//	BYTE packetType;
//	MonsterRead monster[100];
//};
struct ScPacketMonsterPos
{
	int packetSize;
	BYTE packetType;
	int monsterID;
	D3DXVECTOR3 direction;
	D3DXVECTOR3 position;
};
struct ScPacketResetMonster
{
	int packetSize;
	BYTE packetType;
	int monsterID;
	D3DXVECTOR3 returnPos;
};
struct ScPacketMonsterState
{
	int packetSize;
	BYTE packetType;
	int monsterID;
	int monsterState;
};
struct ScPacketMonsterRoaming
{
	int packetSize;
	BYTE packetType;
	int kind;
	D3DXVECTOR3 position;
};
struct ScPacketAcceptPlayerList
{
	int packetSize;
	BYTE packetType;
	int id;
	D3DXVECTOR3 position;
};
//  CS_Packet
struct CsPacketRequest
{
	int packetSize;
	BYTE packetType;
	D3DXVECTOR3 position;
	D3DXVECTOR3 direction;
};
struct CsPacketKeyUp
{
	int packetSize;
	BYTE packetType;
};
struct CspacketAttack
{
	int packetSize;
	BYTE packetType;
};
struct CsPacketMove
{
	int packetSize;
	BYTE packetType;
	int id;
	D3DXVECTOR3 direction;
	D3DXVECTOR3 position;
};
struct CsPacketAttack
{
	int packetSize;
	BYTE packetType;
	int id;
	//이때 충돌체크 여부 판별
};
struct CsPacketUseSkill
{
	int packetSize;
	BYTE packetType;
	int id;
	char keyboardSkill;
};
struct CsPacketUseItem
{
	int packetSize;
	BYTE packetType;
	int id;
	int itemNum;
};
//		Login & Lobby Server

//		SC_Packet
struct ScPacketLogin
{
	int packetSize;
	BYTE packetType;
	int id;
};

struct ScPacketReadyPlayer
{
	int packetSize;
	BYTE packetType;
	int id;
};

struct ScPacketRoomList
{
	int packetSize;
	BYTE packetType;
	int roomNum;
};
//  CS_Packet

struct CsPacketLogin
{
	int packetSize;
	BYTE packetType;
	//char ID[20];
	//char Password[20];
};
struct CsPacketCreateRoom
{
	int packetSize;
	BYTE packetType;
	BYTE limitPlayerNum;
};
struct CsPacketInputReady
{
	int packetSize;
	BYTE packetType;
	int roomNum;
};
struct CsPacketInputStart
{
	int packetSize;
	BYTE packetType;
};
struct CsPacketInviteShop
{
	int packetSize;
	BYTE packetType;
};
struct CsPacketBuyPortion
{
	int packetSize;
	BYTE packetType;
	int amount;
};
struct CsPacketRegisterPortion
{
	int packetSize;
	BYTE packetType;
	char keyboardS;
	char keyboardD;
};
struct CsPacketSelectCharacter
{
	int packetSize;
	BYTE packetType;
};
//	Server->DB Server

//	[SDB_Packet]

//sdb_packet_create_id

//	sdb_packet_buy_portion

//	sdb_packet_clear_time

//	sdb_packet_gold

//	sdb_packet_

//	[CDB_Packet]

//cdb_packet_rank_info
#pragma pack (pop)


//#define WM_SOCKET WM_USER+1
//#define MAX_SIZE 1024
//
//#define SC_POS 1
//#define SC_LOGIN 2
//
//#define CS_LEFT 1
//#define CS_RIGHT 2
//#define CS_UP 3
//#define CS_DOWN 4
//#define CS_LOGIN 5
//
//#pragma pack (push, 1)
////서버 -> 클라
//struct ScPacketMove
//{
//	BYTE size;
//	BYTE type;
//	int id;
//	int x;
//	int y;
//	int Sectorx;
//	int Sectory;
//};
//
//struct sc_packet_putplayer
//{
//	BYTE size;
//	BYTE type;
//	int id;
//};
//
//
////클라 -> 서버
//struct CsPacketMove
//{
//	BYTE size;
//	BYTE type;
//};
//
//struct cs_packet_accept
//{
//	BYTE size;
//	BYTE type;
//};
//
//#pragma pack (pop)