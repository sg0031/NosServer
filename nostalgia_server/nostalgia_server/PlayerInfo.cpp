#include "stdafx.h"
#include "PlayerInfo.h"

PlayerInfo::PlayerInfo()
{
	//playerPosition.x = (rand() % 60) * 10;
	//playerPosition.y = 0;
	//playerPosition.z = (rand() % 60) * 10;
	playerPosition.x = 100.0 ;
	playerPosition.y =266.0;
	playerPosition.z = 100.0  ;
	playerDirection.x = 0;
	playerDirection.y = 0;
	playerDirection.z = 0;
	playerVelocity = 10.0;
	characterType = -1;
	id = -1;
	play = false;
	acceptPlayer = false;
	myState = waitState;
	keyDownTime=0.0;
	keyUpTime=0.0;
	
	viewList.clear();

	currentSector.sectorNumX = 0;
	currentSector.sectorNumZ = 0;

	overEx = new OverEx;
	ZeroMemory(&overEx->iocpBuf, sizeof(overEx->iocpBuf));
	ZeroMemory(&overEx->packetBuf, sizeof(overEx->packetBuf));
	overEx->s = NULL;
	overEx->operationType = Recvtype;
	overEx->prevSize = 0;
	overEx->currentSize = 0;
	overEx->buf.buf = overEx->iocpBuf;
	overEx->buf.len = sizeof(overEx->iocpBuf);
	ZeroMemory(&overEx->overLapped, sizeof(overEx->overLapped));
}


PlayerInfo::~PlayerInfo()
{
}
