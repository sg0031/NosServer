#pragma once

class PlayerInfo
{
	int id;
	sectorPosition currentSector;
	sectorPosition preivseSector;
	int characterType;
	D3DXVECTOR3 playerPosition;
	D3DXVECTOR3 preivsePosition;
	D3DXVECTOR3 playerDirection;
	D3DXVECTOR3 startPosition;
	double playerVelocity;
	int attack;
	int health;
	int mana;
	int deFence;
	char itemQ;
	char itemW;
	int theNumberOfPlayer;
	double clearTime;
	bool play;
	bool acceptPlayer;
	PlayerState myState;
	float keyDownTime;
	float keyUpTime;
public:
	std::mutex pLock;
	std::unordered_set <int> viewList;
	OverEx* overEx;
	PlayerInfo();
	~PlayerInfo();

	void setStartPosition(D3DXVECTOR3 pos) { startPosition = pos; }
	D3DXVECTOR3 getStartPosition() { return startPosition; }

	void setKeyDown(float time) { keyDownTime = time; }
	void setKeyUp(float time) { keyUpTime = time; }
	float getDownTime() { return keyDownTime; }
	float getUpTime() { return keyUpTime; }

	void setState(PlayerState state) { myState = state; }
	PlayerState getState() { return myState; }

	void setID(int ID) { id = ID; }
	int getID() { return id; }

	void setPlay(bool p) { play = p; }
	bool getPlay() { return play; }

	void setAccept(bool ac) { acceptPlayer = ac; }
	bool getAccept() { return acceptPlayer; }

	void setVelocity(double v) { playerVelocity = v; }
	double getVelocity() { return playerVelocity; }

	void setAttack(int a) { attack = a; }
	int getAttack() { return attack; }

	void setHealth(int h) { health = h; }
	int getHealth() { return health; }

	void setMana(int m) { mana = m; }
	int getMana() { return mana; }

	void setprevisePosition(D3DXVECTOR3 p) { preivsePosition = p; }
	D3DXVECTOR3 getprevisePosition() { return preivsePosition; }
	float getprevisePositionX() { return preivsePosition.x; }
	float getprevisePositionY() { return preivsePosition.y; }
	float getprevisePositionZ() { return preivsePosition.z; }
	void setprevisePositionX(float x) { preivsePosition.x = x; }
	void setprevisePositionY(float y) { preivsePosition.y = y; }
	void setprevisePositionZ(float z) { preivsePosition.z = z; }

	void setPosition(D3DXVECTOR3 p) { playerPosition = p; }
	void setDirection(D3DXVECTOR3 d) { playerDirection = d; }
	D3DXVECTOR3 getPosition() { return playerPosition; }
	D3DXVECTOR3 getDirection() { return playerDirection; }

	float getPositionX() { return playerPosition.x; }
	float getPositionY() { return playerPosition.y; }
	float getPositionZ() { return playerPosition.z; }
	void setPositionX(float x) { playerPosition.x = x; }
	void setPositionY(float y) { playerPosition.y = y; }
	void setPositionZ(float z) { playerPosition.z = z; }

	void setCurrentSectorX(int x) { currentSector.sectorNumX = x; }
	void setCurrentSectorZ(int z) { currentSector.sectorNumZ = z; }
	int getCurrentSectorX() { return currentSector.sectorNumX; }
	int getCurrentSectorZ() { return currentSector.sectorNumZ; }

	sectorPosition getCurrentSector() { return currentSector; }

	void setPreiveseSectorX(int x) { preivseSector.sectorNumX = x; }
	void setPreiveseSectorZ(int z) { preivseSector.sectorNumZ = z; }
	int getPreiveseSectorX() { return preivseSector.sectorNumX; }
	int getPreiveseSectorZ() { return preivseSector.sectorNumZ; }
	
};

