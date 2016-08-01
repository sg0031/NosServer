#pragma once
class Sector
{
public:
	std::mutex sectorLock;
	std::unordered_set<int> objectList;  //이섹터에 존재하는 오브젝트 아이디
	std::unordered_set <int> playerList;//이섹터에 존재하는 플레이어 아이디
	Sector();
	~Sector();
};

