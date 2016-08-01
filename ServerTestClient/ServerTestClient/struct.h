#include"Monster.h"
#include"Protocoltype.h"
#pragma pack (push, 1)
struct sectorPosition
{
	int sectorNumX;
	int sectorNumZ;
};
enum PlayerState
{
	waitState,
	moveState,
	attackCutState,
	attackThrustState
};

enum monsterState
{
	waitPosState = 300,
	attackTargetState,
	chaseTargetState,
	returnPos,
};
struct MonsterPacket
{
	int id;
	int type;
	int state;
	D3DXVECTOR3 monsterPos;
	D3DXVECTOR3 monsterDir;
};
enum EOperationType
{
	Sendtype = 1,
	Recvtype,
	MonsterMove,
	MonsterAttack,
	MonsterDead,
	PlayerAttack
};
struct OverEx//오버렙트구조체 확장
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
	double radius;
};
struct Sector
{
	D3DXVECTOR3 startSectorPosition;
	D3DXVECTOR3 endSectorPosition;
	Object arrayObject[MAX_OBJECT];
	OrcArchor archorArr[MAX_MONSTER];
	OrcWarrior warriorArr[MAX_MONSTER];
	OrcMaster masterArr[MAX_MONSTER];
	OrcKing king;
	Sector()
	{
		startSectorPosition.x = 0.0;
		startSectorPosition.y = 0.0;
		startSectorPosition.z = 0.0;
		endSectorPosition.x = 0.0;
		endSectorPosition.y = 0.0;
		endSectorPosition.z = 0.0;
	}
};

struct RoomInfo
{
	int roomNum;
	//	PlayerInfo maxPlayer[4];
};

#pragma pack (pop)