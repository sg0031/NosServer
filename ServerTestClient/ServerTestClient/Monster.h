#pragma once
#include"stdafx.h"
enum monsterType
{
	orcwarrior = 200,
	orcarchor,
	orcmaster,
	orcking
};

class Monster
{
protected:
	monsterType type;
	int health;
	int defence;
	int attack;
	int target;
	D3DXVECTOR3 monsterPosition;
	D3DXVECTOR3 monsterDirection;
	double monsterVelocity;
public:
	Monster();
	~Monster();
	void setHealth(int h) { health = h; }
	void setDefence(int d) { defence = d; }
	void setAttack(int a) { attack = a; }
	void setVelocity(double v) { monsterVelocity = v; }
	void setPosition(D3DXVECTOR3 p) { monsterPosition = p; }
	void setDirection(D3DXVECTOR3 d) { monsterDirection = d; }
	//virtual void decreaseHealth() = 0;
	//virtual void changeTarget() = 0;
	//virtual void attackPlayer()=0;
	//virtual D3DXVECTOR3 roamingMap()=0;
};
class OrcWarrior : public Monster
{
public:
	OrcWarrior()
	{
		type = orcwarrior;
		target = -1;
	};
	~OrcWarrior() {};
};
class OrcArchor :public Monster
{
public:
	OrcArchor()
	{
		type = orcarchor;
		target = -1;
	};
	~OrcArchor() {};
};
class OrcMaster : public Monster
{
public:
	OrcMaster()
	{
		type = orcmaster;
		target = -1;
	};
	~OrcMaster() {};
};
class OrcKing : public Monster
{
public:
	OrcKing()
	{
		type = orcwarrior;
		target = -1;
	};
	~OrcKing() {};
};
