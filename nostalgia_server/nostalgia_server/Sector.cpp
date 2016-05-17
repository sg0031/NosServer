#include "stdafx.h"
#include "Sector.h"


Sector::Sector()
{
	startSectorPosition.x = 0.0;
	startSectorPosition.y = 0.0;
	startSectorPosition.z = 0.0;
	endSectorPosition.x = 0.0;
	endSectorPosition.y = 0.0;
	endSectorPosition.z = 0.0;
	sectorNum.sectorNumX = 0;
	sectorNum.sectorNumZ = 0;
	playerList.clear();
	for (int i = 0; i < MAX_OBJECT; ++i)
	{
		arrayObject[i].kind = -1;
	}
	for (int i = 0; i < MAX_MONSTER; ++i)
	{
		monsterArr[i] = nullptr;
	}
}


Sector::~Sector()
{
}
