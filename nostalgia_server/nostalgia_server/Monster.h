#pragma once
enum monsterType
{
	orcwarrior = 200,
	orcarchor,
	orcmaster,
	orcking
};
enum monsterState
{
	waitPosState = 300,
	attackTargetState,
	chaseTargetState,
	returnPos,
	deadState,
	smashState,
	revengeState
};
class Monster
{
protected:
	int ID;
	monsterType type;
	monsterState myState;
	int health;
	int defence;
	int attack;
	int target;
	int attackRange;
	int chaseRange;
	D3DXVECTOR3 targetPosition;
	D3DXVECTOR3 monsterPosition;
	D3DXVECTOR3 monsterDirection;
	D3DXVECTOR3 monsterReturnPos;
	float monsterVelocity;
	float dealtaTime;
public:
	Monster();
	~Monster();
	void setMonsterID(int id) { ID = id; }
	void setHealth(int h) { health = h; }
	void setDefence(int d) { defence = d; }
	void setAttack(int a) { attack = a; }
	void setVelocity(double v) { monsterVelocity = v; }
	void setPosition(D3DXVECTOR3 p) { monsterPosition = p; monsterReturnPos = p;}
	D3DXVECTOR3 getPosition() { return monsterPosition; }
	void setDirection(D3DXVECTOR3 d) { monsterDirection = d; }
	D3DXVECTOR3 getDirection() { return monsterDirection; }
	void setDealtaTime(float deal) { dealtaTime = deal; }
	D3DXVECTOR3 getReturnPos() {  return monsterReturnPos;	}

	virtual int getState() { return myState; }
	virtual int getMonsterID() { return ID; }
	virtual int getType() { return type; }
	virtual int getTarget() { return target; }
	virtual bool getAlive() { return health > 0; }
	virtual void setState(monsterState state) { myState = state; }
	//void deCreaseHealth(int playerAttack){
	//	health = health - (playerAttack - defence);
	//}
	virtual void deCreaseHealth(){
		health = health -400;
	}
	virtual void setTarget(int id, D3DXVECTOR3 playerPosition);
	virtual void setTargetPosition(D3DXVECTOR3 playerPosition) {targetPosition = playerPosition;}
	virtual void setTargetID(int id) { target = id; }
	virtual bool searchChasePlayer();
	virtual bool attackPlayer();
	virtual void move();
	virtual void chaseTarget();
	virtual void attackTarget();
	virtual void upDate();
	virtual void returnMove();
};
class OrcWarrior : public Monster
{
public:
	OrcWarrior() 
	{
		type = orcwarrior;
		health=700;
		defence=20;
		attack = 20;
		target = -1;
		monsterVelocity=50.0;
		myState = waitPosState;
		attackRange = 140;
		chaseRange = 300;
	};
	~OrcWarrior() {};
};
class OrcArchor :public Monster
{
public:
	OrcArchor() 
	{
		type = orcarchor;
		health = 500;
		defence = 10;
		attack = 40;
		target = -1;
		monsterVelocity = 0.3;
		myState = waitPosState;
		attackRange = 2;
		chaseRange = 20;
	};
	~OrcArchor() {};
};
class OrcMaster : public Monster
{
public:
	OrcMaster() 
	{
		type = orcmaster;
		health = 1000;
		defence = 30;
		attack = 50;
		target = -1;
		monsterVelocity = 0.5;
		myState = waitPosState;
		attackRange = 2;
		chaseRange = 20;
	};
	~OrcMaster() {};
};
class OrcKing : public Monster
{
public:
	OrcKing() 
	{
		type = orcking;
		health = 3000;
		defence = 50;
		attack = 100;
		target = -1;
		monsterVelocity = 50.0;
		myState = waitPosState;
		attackRange = 140;
		chaseRange = 300;
	};
	~OrcKing() {};
	virtual void upDate();
	virtual void deCreaseHealth() {
		health = health -1000;
	}
	void revengeMode() {
		attack = attack * 3; defence = defence * 3; monsterVelocity= monsterVelocity* 2;
	}
};
