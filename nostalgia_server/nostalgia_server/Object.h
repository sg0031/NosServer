#pragma once
enum objectType
{
	orcwarrior = 200,
	orcarchor,
	orcmaster,
	orcking,
	tree,
	stone,
	river
};
enum objectState
{
	waitPosState = 300,
	attackTargetState,
	chaseTargetState,
	returnPos,
	deadState,
	smashState,
	revengeState
};
class Object
{
	bool active;
	float max;
	float min;
protected:
	int ID;
	int type;
	objectState myState;
	int health;
	int defence;
	int attack;
	int target;
	int attackRange;
	int chaseRange;
	D3DXVECTOR3 targetPosition;
	D3DXVECTOR3 objectPosition;
	D3DXVECTOR3 objectDirection;
	D3DXVECTOR3 objectReturnPos;
	float objectVelocity;
	float dealtaTime;
public:
	Object();
	~Object();
	void setType(int ty) { type = ty; }
	void setMax(float mx) { max = mx; }
	void setMin(float mi) { min = mi; }
	float getMax() { return max; }
	float getMin() { return min; }
	void setActive(bool act) { active = act; }
	bool getActive() { return active; }
	void setMonsterID(int id) { ID = id; }
	void setHealth(int h) { health = h; }
	void setDefence(int d) { defence = d; }
	void setAttack(int a) { attack = a; }
	void setVelocity(double v) { objectVelocity = v; }
	void setPosition(D3DXVECTOR3 p) { objectPosition = p; objectReturnPos = p;}
	D3DXVECTOR3 getPosition() { return objectPosition; }
	void setDirection(D3DXVECTOR3 d) { objectDirection = d; }
	D3DXVECTOR3 getDirection() { return objectDirection; }
	void setDealtaTime(float deal) { dealtaTime = deal; }
	D3DXVECTOR3 getReturnPos() {  return objectReturnPos;	}

	virtual int getState() { return myState; }
	virtual int getMonsterID() { return ID; }
	virtual int getType() { return type; }
	virtual int getTarget() { return target; }
	virtual bool getAlive() { return health > 0; }
	virtual void setState(objectState state) { myState = state; }
	virtual void deCreaseHealth(int playerAttack){
		health = health - (playerAttack - defence);
	}
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
class Stone :public Object
{
public:
	Stone() {}
	~Stone() {}
};
class Tree :public Object
{
public:
	Tree() {};
	~Tree() {};
};
class River : public Object
{
public:
	River() {};
	~River() {};
};
class OrcWarrior : public Object
{
public:
	OrcWarrior() 
	{
		type = orcwarrior;
		health=700;
		defence=20;
		attack = 20;
		target = -1;
		objectVelocity =50.0;
		myState = waitPosState;
		attackRange = 140;
		chaseRange = 300;
	};
	~OrcWarrior() {};
};
class OrcArchor :public Object
{
public:
	OrcArchor() 
	{
		type = orcarchor;
		health = 500;
		defence = 10;
		attack = 40;
		target = -1;
		objectVelocity = 0.3;
		myState = waitPosState;
		attackRange = 2;
		chaseRange = 20;
	};
	~OrcArchor() {};
};
class OrcMaster : public Object
{
public:
	OrcMaster() 
	{
		type = orcmaster;
		health = 1000;
		defence = 30;
		attack = 50;
		target = -1;
		objectVelocity = 0.5;
		myState = waitPosState;
		attackRange = 2;
		chaseRange = 20;
	};
	~OrcMaster() {};
};
class OrcKing : public Object
{
public:
	OrcKing() 
	{
		type = orcking;
		health = 3000;
		defence = 50;
		attack = 100;
		target = -1;
		objectVelocity = 50.0;
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
		attack = attack * 3; defence = defence * 3; objectVelocity = objectVelocity * 2;
	}
};
