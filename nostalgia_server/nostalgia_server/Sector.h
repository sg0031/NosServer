#pragma once
class Sector
{
public:
	std::mutex sectorLock;
	std::unordered_set<int> objectList;  //�̼��Ϳ� �����ϴ� ������Ʈ ���̵�
	std::unordered_set <int> playerList;//�̼��Ϳ� �����ϴ� �÷��̾� ���̵�
	Sector();
	~Sector();
};

