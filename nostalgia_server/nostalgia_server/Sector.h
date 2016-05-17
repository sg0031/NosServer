#pragma once
class Sector
{
public:
	std::mutex sectorLock;
	sectorPosition sectorNum;
	D3DXVECTOR3 startSectorPosition;
	D3DXVECTOR3 endSectorPosition;
	Object arrayObject[MAX_OBJECT];
	std::unordered_set <int> playerList;
	Monster *monsterArr[MAX_MONSTER];
	Sector();
	~Sector();
};

