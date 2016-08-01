#pragma once
#include"stdafx.h"

class ServerPlayer
{
	OverEx* overEx;
	int id;
	sectorPosition currentSector;
	int character_type;
	D3DXVECTOR3 position;
	D3DXVECTOR3 direction;
	double velocity;
	int attack;
	int health;
	int mana;
	int defence;
	char item_q;
	char item_w;
	int the_number_of_player;
	double clear_time;
	bool play;
	PlayerState myState;

public:
	ServerPlayer();
	~ServerPlayer();
	void setState(PlayerState state) { myState = state; }
	PlayerState getState() { return myState; }

	bool getPlay();
	D3DXVECTOR3 getPlayerPosition();
	D3DXVECTOR3 getPlayerDirection();
	double getPlayerVelocity();
	int getPlayerID();
	SOCKET getPlayerSocket();
	sectorPosition getPlayerSecotr() { return currentSector; }

	void setPlayerVelocity(double velocity);
	void setPlay(bool p);
	void setPlayerID(int id);
	void setPlayerPosition(D3DXVECTOR3 position);
	void setPlayerDirection(D3DXVECTOR3 direction);
	void setPlayerSector(sectorPosition sec) { currentSector = sec; }
};

