#include"stdafx.h"
#include "ServerPlayer.h"


ServerPlayer::ServerPlayer()
{
	position = D3DXVECTOR3(20.0, 700.0, 20.0);
	direction = D3DXVECTOR3(0, 0, 1);
	velocity = 0.5;
	character_type = -1;
	id = -1;
	play = false;

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


ServerPlayer::~ServerPlayer()
{
}

bool ServerPlayer::getPlay()
{
	return play;
}
D3DXVECTOR3 ServerPlayer::getPlayerPosition()
{
	return position;
}

D3DXVECTOR3 ServerPlayer::getPlayerDirection()
{
	return direction;
}
int ServerPlayer::getPlayerID()
{
	return id;
}
double ServerPlayer::getPlayerVelocity()
{
	return velocity;
}
SOCKET ServerPlayer::getPlayerSocket()
{
	return overEx->s;
}



void ServerPlayer::setPlayerVelocity(double velocity)
{
	this->velocity = velocity;
}
void ServerPlayer::setPlay(bool p)
{
	play = p;
}
void ServerPlayer::setPlayerID(int id)
{
	this->id = id;
}
void ServerPlayer::setPlayerPosition(D3DXVECTOR3 position)
{
	this->position = position;
}
void ServerPlayer::setPlayerDirection(D3DXVECTOR3 direction)
{
	this->direction = direction;
}