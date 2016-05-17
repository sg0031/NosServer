#include "stdafx.h"
#include "LogicServer.h"


HANDLE LogicServer::io = NULL;	//빈 포트 생성
int LogicServer::id = 0;
PlayerInfo LogicServer::player[ROOM_MAX_PLAYER];
priority_queue<FIFO, vector<FIFO>, Standard> LogicServer::playerID;
int LogicServer::count = 0;
mutex LogicServer::myLock;
Sector LogicServer::sector[SECTOR_WIDETH][SECTOR_LENGTH];
TimerThread LogicServer::timer;
HandleManager *hManager = HandleManager::getInstance();


LogicServer::LogicServer()
{
	///---------------------------------오브젝트 파일 읽어오기-----------------------
	int fileSize = 0;
	int objectKind = 0;
	ifstream fin("treeCollide.txt");
	fin >> fileSize;
	vector<Object> objcetPosition;
	objcetPosition.resize(fileSize);
	if (fin.is_open())
	{
		fin >> objectKind;
		for (auto i = 0; i < fileSize; ++i)
		{
			fin >> objcetPosition[i].objectPosition.x >> objcetPosition[i].objectPosition.y >>
				objcetPosition[i].objectPosition.z >> objcetPosition[i].max >> objcetPosition[i].min;
			objcetPosition[i].kind = objectKind;
		}
	}
	else
	{
		cout << "file open error";
	}
	fin.close();
	//------------------------몬스터읽어오기--------------------------------------------------------
	int monsterFileSize = 0;
	int monsterType = 0;
	ifstream monin("GolemData.txt");
	monin >> monsterFileSize;
	vector<MonsterPacket> monPos;
	monPos.resize(monsterFileSize);
	if (monin.is_open())
	{
		for (auto i = 0; i < monsterFileSize; ++i)
		{
			monin >> monPos[i].type >> monPos[i].monsterPos.x >> monPos[i].monsterPos.y >>
				monPos[i].monsterPos.z >> monPos[i].monsterDir.x >> monPos[i].monsterDir.y >> monPos[i].monsterDir.z;
		}
	}
	else
	{
		cout << "file open error";
	}
	monin.close();
	//-------------------------------------------------------------------------------------

	//아이디 초기화----------------------------------------------------------------------
	FIFO init;
	//FIFO que
	for (int i = 0; i < ROOM_MAX_PLAYER; ++i)
	{
		init.data = i;
		init.turn = i;
		playerID.push(init);
	}
	//섹터의 시작좌표와 끝좌표를 입력한다
	//이는 후에 오브젝트가 배치된 파일을 읽어올때 오브젝트를을 각 섹터에 배치하기 위해서 이다.
	for (int i = 0; i < SECTOR_WIDETH; ++i)
	{
		for (int j = 0; j < SECTOR_LENGTH; ++j)
		{
			sector[i][j].startSectorPosition.z = (j*DIVIDE_SECTOR_Z);
			sector[i][j].endSectorPosition.z = sector[i][j].startSectorPosition.z + DIVIDE_SECTOR_Z;
			sector[i][j].startSectorPosition.x = (i*DIVIDE_SECTOR_X);
			sector[i][j].endSectorPosition.x = sector[i][j].endSectorPosition.x + DIVIDE_SECTOR_X;
			sector[i][j].sectorNum.sectorNumX = i;
			sector[i][j].sectorNum.sectorNumZ = j;
		}
	}
	//몬스터 셋팅
	int monsterID = 500;
	for (int i = 0; i < SECTOR_WIDETH; ++i)
	{
		for (int j = 0; j < SECTOR_LENGTH; ++j)
		{
			for (auto m = 0; m < monsterFileSize; ++m)
			{
				MonsterPacket mon = monPos[m];
				if ((i == static_cast<int>(mon.monsterPos.x / DIVIDE_SECTOR_X)) &&
					(j == static_cast<int>(mon.monsterPos.z / DIVIDE_SECTOR_Z)))
				{
					for (auto mPos = 0; mPos < MAX_MONSTER; ++mPos)
					{
						if (nullptr == sector[i][j].monsterArr[mPos])
						{
							if (orcwarrior == mon.type)
							{
								sector[i][j].monsterArr[mPos] = new OrcWarrior;
								sector[i][j].monsterArr[mPos]->setPosition(mon.monsterPos);
								sector[i][j].monsterArr[mPos]->setDirection(mon.monsterDir);
								sector[i][j].monsterArr[mPos]->setMonsterID(monsterID);
								monsterID++;
								break;
							}
							else if (orcking == mon.type)
							{
								sector[i][j].monsterArr[mPos] = new OrcKing;
								sector[i][j].monsterArr[mPos]->setPosition(mon.monsterPos);
								sector[i][j].monsterArr[mPos]->setDirection(mon.monsterDir);
								sector[i][j].monsterArr[mPos]->setMonsterID(BOSS_ID);
								break;
							}
							//sector[i][j].monsterArr[mPos] = new OrcWarrior;
							//sector[i][j].monsterArr[mPos]->setPosition(mon.monsterPos);
							//sector[i][j].monsterArr[mPos]->setDirection(mon.monsterDir);
							//sector[i][j].monsterArr[mPos]->setMonsterID(monsterID);
							//monsterID++;
							//break;
						}
					}
				}
			}
		}
	}
	//이후 섹터를 전부 돌면서 오브젝트를 배치한다.
	for (int i = 0; i < SECTOR_WIDETH; ++i)
	{
		for (int j = 0; j < SECTOR_LENGTH; ++j)
		{
			for (auto o = 0; o < fileSize; ++o)
			{
				//오브젝트 임시벡터에 마지막 요소를 참조한후 그요소의 좌표값을 섹터의 값과 비교후 일치할경우 오브젝트를
				//해당섹터에 배치한다.
				Object ob = objcetPosition[o];

				if ((i == static_cast<int>(ob.objectPosition.x / DIVIDE_SECTOR_X)) &&
					(j == static_cast<int>(ob.objectPosition.z / DIVIDE_SECTOR_Z)))
					//if ((sector[i][j].startSectorPosition.x<=ob.objectPosition.x) &&
					//	(sector[i][j].startSectorPosition.z<=ob.objectPosition.z) &&
					//	(sector[i][j].endSectorPosition.x>=ob.objectPosition.x) &&
					//	(sector[i][j].endSectorPosition.z>=ob.objectPosition.z))
				{
					for (auto k = 0; k < MAX_OBJECT; ++k)
					{
						if (-1 == sector[i][j].arrayObject[k].kind)
						{
							sector[i][j].arrayObject[k] = ob;
							//cout << i << j << endl;
							break;
						}
					}
					//참조 했던 오브젝트는 벡터에서 지워준다.
					//ob=objcetPosition.erase(o);
				}
			}
		}
	}

	//create thread
	srand((unsigned)time(NULL));
	WSADATA wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
		cout << "init error" << endl;

	SYSTEM_INFO is;
	GetSystemInfo(&is);

	vector<thread*> worker;

	hManager->gHandle = CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, NULL, 0);

	for (auto i = 0; i < (int)is.dwNumberOfProcessors; ++i)
	{
		worker.push_back(new thread{ workerThread });
	}

	auto Acceptex = thread{ acceptThread };
	auto timerEx = thread{ &TimerThread::eventTimerThread,&timer };
	auto Lobby = thread{ lobbyThread };

	while (1)
	{
		Sleep(1000);
	}

	Acceptex.join();
	timerEx.join();
	Lobby.join();

	for (auto t : worker)
	{
		t->join();
	}
}
LogicServer::~LogicServer()
{
}
void LogicServer::acceptThread()
{
	cout << "ServerInit" << endl;

	SOCKET listenSock;
	SOCKADDR_IN  addr;

	listenSock = WSASocket(AF_INET, SOCK_STREAM, IPPROTO_TCP,
		NULL, 0, WSA_FLAG_OVERLAPPED);

	//주소 설정
	ZeroMemory(&addr, sizeof(addr));
	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = htonl(INADDR_ANY);
	addr.sin_port = htons(9000);

	//bind 부분
	int retval = 0;
	retval = ::bind(listenSock, reinterpret_cast<SOCKADDR*>(&addr), sizeof(SOCKADDR));
	if (retval == SOCKET_ERROR)
		cout << "Bind Error" << endl;

	//리슨 소켓 설정
	retval = listen(listenSock, SOMAXCONN);
	if (retval == INVALID_SOCKET)
	{
		cout << "listen_sock error" << endl;
	}

	SOCKET clientSock;
	SOCKADDR_IN clientAddr;

	clientSock = NULL;

	while (1)
	{
		//	myLock.lock();
		int len = sizeof(clientAddr);
		clientSock = WSAAccept(listenSock, (SOCKADDR*)&clientAddr,
			&len, NULL, NULL);

		if (clientSock == INVALID_SOCKET)
			cout << "Client Socket Error" << endl;


		for (int i = 0; i < ROOM_MAX_PLAYER; ++i)
		{
			if (player[i].getAccept() == false)
			{
				player[i].overEx->s = clientSock;
				player[i].setAccept(true);
				count = i;
				break;
			}
		}
		CreateIoCompletionPort((HANDLE)clientSock, hManager->gHandle, count, 0);

		unsigned long recvflag = 0;
		unsigned long ioByte = 0;
		int ret = 0;
		ret = WSARecv(clientSock, &player[count].overEx->buf,
			1, NULL, &recvflag,
			reinterpret_cast<LPOVERLAPPED>(&player[count].overEx->overLapped), NULL);
		if (ret == SOCKET_ERROR)
		{
			if (WSAGetLastError() != WSA_IO_PENDING)
				cout << "error code : " << WSAGetLastError() << endl;
		}
		//	myLock.unlock();
	}
}
void LogicServer::lobbyThread()
{

}
void LogicServer::workerThread()
{
	//패킷 재조립이란?
	int retval;
	unsigned long ioSize;
	unsigned long recvSize = 0;
	unsigned long objectId;
	unsigned long ioFlag = 0;
	OverEx *over = nullptr;
	double currentTime = 0.0;

	currentTime = clock();

	while (1)
	{
		GetQueuedCompletionStatus(hManager->gHandle, &ioSize, &objectId,
			reinterpret_cast<LPOVERLAPPED *>(&over), INFINITE);

		if (ioSize == 0)
		{
			//cout << iosize << endl;
			closesocket(player[static_cast<int>(objectId)].overEx->s);
			ScPacketRemoveObject remove;
			remove.packetSize = sizeof(remove);
			remove.packetType = SC_REMOVE_PLAYER;
			remove.id = player[static_cast<int>(objectId)].getID();
			for (auto i = 0; i < ROOM_MAX_PLAYER; ++i)
			{
				if (false == player[i].getPlay()) continue;
				if (static_cast<int>(objectId) == i) continue;
				sendPacket(i, &remove);
			}
			FIFO returnID;
			returnID.data = static_cast<int>(objectId);
			returnID.turn = static_cast<int>(objectId);
			player[static_cast<int>(objectId)].pLock.lock();
			playerID.push(returnID);
			player[static_cast<int>(objectId)].pLock.unlock();
			player[static_cast<int>(objectId)].setPlay(false);
			//WSAGetOverlappedResult
			//게임종료 처리
		}

		if (over->operationType == Recvtype)
		{
			//cout << iosize << endl;
			//cout << "Recvtype" << endl;
			char *buf = over->iocpBuf;	//?
			unsigned long dataToProcess = ioSize;
			over->currentSize = dataToProcess;

			while (dataToProcess > 0)
			{
				if (over->currentSize == 0)
					over->currentSize = buf[0];

				unsigned int needToBuild =
					over->currentSize - over->prevSize;

				if (dataToProcess >= needToBuild)
				{
					double newTime = clock();
					double frameTime = newTime - currentTime;
					if (frameTime > 0.25)
						frameTime = 0.25;
					currentTime = newTime;
					//accumulator += frameTime;
					//조립
					memcpy(over->packetBuf + over->prevSize, buf, needToBuild);
					//cout << static_cast<int>(Object_ID) << endl;
					processPacket(static_cast<int>(objectId), over->packetBuf, frameTime);
					//t += dt;
					//accumulator -= dt;
					over->currentSize = 0;
					over->prevSize = 0;
					buf += needToBuild;
					dataToProcess -= needToBuild;
				}
				else
				{
					//훗날을 기약해
					memcpy(over->packetBuf + over->prevSize, buf, dataToProcess);
					over->prevSize += dataToProcess;
					dataToProcess = 0;
				}

			}
			retval = WSARecv(over->s, &over->buf, 1, &recvSize, &ioFlag, &over->overLapped, NULL);
		}

		else if (over->operationType == Sendtype)
		{
			delete over;
		}
		else if (over->operationType == PlayerAttackQ || over->operationType==PlayerAttackCrush || over->operationType==PlayerAttackW)
		{
			playerActProcessPacket(over, static_cast<int>(objectId));

			delete over;
		}
		else if (over->operationType == MonsterMove)
		{
			double newTime = clock();
			double frameTime = newTime - currentTime;
			if (frameTime/1000 > 0.25)
				frameTime = 0.25;
			currentTime = newTime;
			monsterActProcessPacket(over, static_cast<int>(objectId), frameTime);

			delete over;
		}
	}
}
void LogicServer::processPacket(int id, char *ptr, double deltaTime)
{
	//cout << "processPacket 진입" << endl;
	//cout <<"id : "<< id << endl;
	BYTE *header = reinterpret_cast<BYTE*>(ptr + 4);
	FIFO idAllot;
	switch (*header)
	{
	case CS_LOGIN_REQUEST:
	{
		//cout << "Login Accept" << endl;
		player[id].pLock.lock();
		idAllot = playerID.top();
		playerID.pop();
		player[id].pLock.unlock();
		player[id].setID(idAllot.data);
		//cout << "id : " << player[id].getID() << endl;

		ScPacketMove login;
		login.packetSize = sizeof(ScPacketMove);
		login.packetType = SC_LOGIN_SUCCESS;
		login.id = player[id].getID();
		login.position = player[id].getPosition();
		login.state = waitState;
		player[id].setPlay(true);
		sendPacket(id, &login);
		setObject(id);
		viewListUpdate(id);
		break;
	}
	case CS_KEY_DOWN:
	{
		//키보드 눌렀을때 셋팅해주는부분이다 방향벡터는 비교용으로 만 필요하니 따로 셋팅 안해줘도 될듯 하다.
		//키보드 눌렀을때 위치는 셋팅해주어야 한다.
		CsPacketMove *down = reinterpret_cast<CsPacketMove*>(ptr);
		player[id].setDirection(down->direction);
		player[id].setPosition(down->position);
		player[id].setStartPosition(down->position);	//키보드 눌렀을때 시작위치를 기록해놔야한다.
		player[id].setKeyDown(clock()- deltaTime);
		player[id].setState(moveState);

		ScPacketUpdateState packet;
		packet.id = player[id].getID();
		packet.packetType = SC_STATE_UPDATE;
		packet.packetSize = sizeof(ScPacketUpdateState);
		packet.state = player[id].getState();
		sendPacket(id, &packet);

		player[id].pLock.lock();
		for (auto p : player[id].viewList) //애니메이션 동기화와 클라 좌표계산을 위해 현재 상태를 넘겨줌
		{
			sendPacket(p, &packet);
		}
		player[id].pLock.unlock();

		//std::cout << "keyDown : " << player[id].getDownTime() << std::endl;
		//std::cout << "keyDown pos : " << player[id].getStartPosition().x << "  ,  " << player[id].getStartPosition().z << std::endl;
		break;
	}
	case CS_KEY_UP:
	{
		//이동하다가 키보드에서 손을 놓았을때
		//방향벡터는 업데이트 해줄필요가 있지만 포지션벡터는 비교용이기에 따로 서버에 셋팅해 주지 않는다.
		float uptime = 0.0;
		ScPacketMove moveError;	//스택메모리를 아끼기 위해 미리 하나만 만들어둔다.
		moveError.packetSize = sizeof(ScPacketMove);
		moveError.id = player[id].getID();
		moveError.packetType = SC_MOVE_ERROR_CHECK;

		CsPacketMove *up = reinterpret_cast<CsPacketMove*>(ptr);	//받은 패킷을 해석하기 위한 형변환
		player[id].setDirection(up->direction);	//클라로 부터 받은 방향벡터를 셋팅해준다.
		player[id].setState(waitState);	//키보드에서 손을 놓았으니 정지 상태로 바꾸어준다.
		uptime = player[id].getUpTime() - player[id].getDownTime()- deltaTime;

		if (uptime < 0.0)
			uptime = 0.0;

		D3DXVECTOR3 position = player[id].getStartPosition() +
			(player[id].getVelocity()*player[id].getDirection()*(static_cast<float>(uptime / 1000)));
		//std::cout << "keyUp" << std::endl;
		//std::cout << "up받음 : " << up->position.x << " , " << up->position.z << endl;
		//std::cout << "up계산 : " << position.x << " , " << position.z << " , " << static_cast<float>(uptime / 1000) << std::endl;
		if (true != crashObject(position, id)) //충돌을 안했을경우 서버상에서 이동시킨다.
		{
			player[id].setPosition(position);
			crashMaxMapSize(id);
			searchSector(id);
			viewListUpdate(id);
			//오브젝트 충돌체크과 맵을 벗어났는지 충돌체크시에 좌표가 함수내부에서
			//변형될수 있으므로 플레이어의 좌표를 직접가져와서 비교한다.
			if (!((up->position.x - SYNCHRONIZATION_RANGE <= player[id].getPosition().x) && (player[id].getPosition().x <= (up->position.x + SYNCHRONIZATION_RANGE)) &&
				(up->position.z - SYNCHRONIZATION_RANGE <= player[id].getPosition().z) && (player[id].getPosition().z <= (up->position.z + SYNCHRONIZATION_RANGE))))
			{
				moveError.state = player[id].getState();
				moveError.position = player[id].getPosition();
				moveError.direction = player[id].getDirection();
				sendPacket(id, &moveError);
				//std::cout << "up보정이동 : " << player[id].getPositionX() << "," << player[id].getPositionZ() << std::endl;
			}
		}
		else
		{
			//충돌했을경우도 보정이 필요한지 판단해야한다.
			//충돌했을시도 보정이 필요할 경우 강제 업데이트를 해준다.
			if (!((up->position.x - SYNCHRONIZATION_RANGE <= player[id].getPosition().x) && (player[id].getPosition().x <= (up->position.x + SYNCHRONIZATION_RANGE)) &&
				(up->position.z - SYNCHRONIZATION_RANGE <= player[id].getPosition().z) && (player[id].getPosition().z <= (up->position.z + SYNCHRONIZATION_RANGE))))
			{
				moveError.state = player[id].getState();
				moveError.position = player[id].getPosition();
				moveError.direction = player[id].getDirection();
				sendPacket(id, &moveError);
				//std::cout << "up보정이동 : " << player[id].getPositionX() << "," << player[id].getPositionZ() << std::endl;
			}
		}
		//플레이어가 이동을 중지했으니 다른 플레이어들에게도 알려줘야 한다.
		ScPacketMove packet;
		packet.packetSize = sizeof(ScPacketMove);
		packet.packetType = SC_MOVE_POSITION;

		player[id].pLock.lock();
		for (auto p : player[id].viewList)
		{
			packet.id = player[p].getID();
			packet.direction = player[p].getDirection();
			packet.position = player[p].getPosition();
			packet.state = player[p].getState();
			sendPacket(id, &packet);
		}
		player[id].pLock.unlock();

		player[id].pLock.lock();
		for (auto p : player[id].viewList) //다른플레이어의 위치를 요청한 플레이어의 뷰리스트에 있는 아이디값에대한 좌표들을
										   //플레이어에게 뿌려준다.
		{
			packet.id = player[id].getID();
			packet.direction = player[id].getDirection();
			packet.position = player[id].getPosition();
			packet.state = player[id].getState();
			sendPacket(p, &packet);
		}
		player[id].pLock.unlock();

		//사용했던 시간값들은 다시 셋팅해준다.
		player[id].setKeyDown(0.0);
		player[id].setKeyUp(0.0);
		player[id].setStartPosition(D3DXVECTOR3(0.0, 0.0, 0.0));
		break;
	}
	case CS_INFO_REQUSET:
	{
		//주기적으로 업데이트요청이 왔을때 처리하는 부분 방향벡터는 업데이트 해줄 필요성을 느끼나 
		//포지션벡터의 경우 비교용으로만 사용해야한다고 생각이 든다
		float time = 0.0;
		ScPacketMove moveError;
		moveError.packetSize = sizeof(ScPacketMove);
		moveError.id = player[id].getID();
		moveError.packetType = SC_MOVE_ERROR_CHECK;

		CsPacketRequest *info = reinterpret_cast<CsPacketRequest*>(ptr);
		player[id].setDirection(info->direction);
		//player[id].setPosition(info->position);
		if (0.0 != player[id].getDownTime())	//플레이어가 움직였을 키보드 입력을 통해 움직였을 경우만 진행함
		{
			if (0.0 == player[id].getUpTime())
			{
				time = clock() - player[id].getDownTime();
				player[id].setKeyUp(clock()- deltaTime);
				//cout << "upTime 0 : " << time << endl;
			}
			else
			{
				time = clock() - player[id].getUpTime();
				player[id].setKeyUp(clock()- deltaTime);
				//cout << "upTime exist : " << time << endl;
			}
			if (time < 0.0)
				time = 0.0;
			D3DXVECTOR3 position = player[id].getPosition() +
				(player[id].getVelocity()*player[id].getDirection()*(static_cast<float>(time / 1000)));
			//std::cout << "1초에 한번오는 요청" << std::endl;
			//std::cout << "1초받음 : " << info->position.x << " , " << info->position.z << endl;
			//std::cout << "1초계산 : " << position.x << " , " << position.z << " , " << static_cast<float>(time / 1000) << std::endl;
			if (true != crashObject(position, id)) //충돌을 안했을경우 서버상에서 이동시킨다.
			{
				player[id].setPosition(position);
				crashMaxMapSize(id);
				searchSector(id);
				viewListUpdate(id);
				if (!((info->position.x - SYNCHRONIZATION_RANGE <= position.x) && (position.x <= (info->position.x + SYNCHRONIZATION_RANGE)) &&
					(info->position.z - SYNCHRONIZATION_RANGE <= position.z) && (position.z <= (info->position.z + SYNCHRONIZATION_RANGE))))
				{
					moveError.position = position;
					moveError.direction = player[id].getDirection();
					sendPacket(id, &moveError);
					//std::cout << "1초보정이동 : " << player[id].getPositionX() << "," << player[id].getPositionZ() << std::endl;
				}
			}
			else
			{
				player[id].setStartPosition(player[id].getPosition());
				player[id].setKeyDown(clock());
				moveError.position = player[id].getPosition();
				moveError.direction = player[id].getDirection();
				sendPacket(id, &moveError);
				//std::cout << "1초충돌이동 : " << player[id].getPositionX() << "," << player[id].getPositionZ() << std::endl;
			}
		}
		//else
		//	viewListUpdate(id);
		ScPacketMove packet;
		packet.packetSize = sizeof(ScPacketMove);
		packet.packetType = SC_MOVE_POSITION;

		player[id].pLock.lock();
		for (auto p : player[id].viewList)
		{
			packet.id = player[p].getID();
			packet.direction = player[p].getDirection();
			packet.position = player[p].getPosition();
			packet.state = player[p].getState();
			sendPacket(id, &packet);
		}
		player[id].pLock.unlock();

		player[id].pLock.lock();
		for (auto p : player[id].viewList) //다른플레이어의 위치를 요청한 플레이어의 뷰리스트에 있는 아이디값에대한 좌표들을
										   //플레이어에게 뿌려준다.
		{
			packet.id = player[id].getID();
			packet.direction = player[id].getDirection();
			packet.position = player[id].getPosition();
			packet.state = player[id].getState();
			sendPacket(p, &packet);
		}
		player[id].pLock.unlock();
		break;
	}
	case CS_STOP:
	{
		player[id].setState(waitState);
		ScPacektPutPlayer put;
		put.packetSize = sizeof(put);
		put.packetType = SC_PUT_PLAYER;
		put.id = id;
		put.position = player[id].getPosition();
		put.direction = player[id].getDirection();
		put.state = player[id].getState();
		sendPacket(id, &put);

		player[id].pLock.lock();
		for (auto i : player[id].viewList)
		{
			sendPacket(i, &put);
		}
		player[id].pLock.unlock();
		break;
	}
	case CS_USE_SKILL_Q:
	{
		player[id].setState(attackCutState);
		ScPacketUpdateState packet;
		packet.id = player[id].getID();
		packet.packetType = SC_STATE_UPDATE;
		packet.packetSize = sizeof(ScPacketUpdateState);
		packet.state = player[id].getState();
		sendPacket(id, &packet);

		//시야리스트에 있는 아이들에게 플레이어의 상태를 업데이트 해준다.
		player[id].pLock.lock();
		for (auto i : player[id].viewList)
		{
			sendPacket(i, &packet);
		}
		player[id].pLock.unlock();

		player[id].pLock.lock();
		timer.AddGameEvent(PlayerAttackCrush, player[id].getID(), 598);
		//timer.AddGameEvent(PlayerAttackCrush, player[id].getID(), 731);
		timer.AddGameEvent(PlayerAttackQ, player[id].getID(), ATTACK_Q_TIME);
		player[id].pLock.unlock();
		break;
	}
	case CS_USE_SKILL_W:
	{
		player[id].setState(attackThrustState);
		ScPacketUpdateState packet;
		packet.id = player[id].getID();
		packet.packetType = SC_STATE_UPDATE;
		packet.packetSize = sizeof(ScPacketUpdateState);
		packet.state = player[id].getState();
		sendPacket(id, &packet);

		//시야리스트에 있는 아이들에게 플레이어의 상태를 업데이트 해준다.
		player[id].pLock.lock();
		for (auto i : player[id].viewList)
		{
			sendPacket(i, &packet);
		}
		player[id].pLock.unlock();


		player[id].pLock.lock();
		//timer.AddGameEvent(PlayerAttackCrush, player[id].getID(), 731);
		timer.AddGameEvent(PlayerAttackCrush, player[id].getID(), 850);
		timer.AddGameEvent(PlayerAttackW, player[id].getID(), ATTACK_W_TIME);
		player[id].pLock.unlock();
		break;
	}
	}

	//매번 플레이어들의 위치값 갱신

}
void LogicServer::playerActProcessPacket(OverEx *operation, int id)
{
	if (PlayerAttackQ == operation->operationType)
	{
		//cout << "처리" << endl;
		//공격 가능하다는 패킷을 플레이에게 보내줘야 한다.
		player[id].setState(waitState);
		ScPacketUpdateState packet;
		packet.id = player[id].getID();
		packet.packetType = SC_STATE_UPDATE;
		packet.packetSize = sizeof(ScPacketUpdateState);
		packet.state = player[id].getState();
		sendPacket(id, &packet);

		//시야리스트에 있는 아이들에게 플레이어의 상태를 업데이트 해준다.
		player[id].pLock.lock();
		for (auto i : player[id].viewList)
		{
			sendPacket(i, &packet);
		}
		player[id].pLock.unlock();
	}
	if (PlayerAttackW == operation->operationType)
	{
		//cout << "처리" << endl;
		//공격 가능하다는 패킷을 플레이에게 보내줘야 한다.
		player[id].setState(waitState);
		ScPacketUpdateState packet;
		packet.id = player[id].getID();
		packet.packetType = SC_STATE_UPDATE;
		packet.packetSize = sizeof(ScPacketUpdateState);
		packet.state = player[id].getState();
		sendPacket(id, &packet);

		//시야리스트에 있는 아이들에게 플레이어의 상태를 업데이트 해준다.
		player[id].pLock.lock();
		for (auto i : player[id].viewList)
		{
			sendPacket(i, &packet);
		}
		player[id].pLock.unlock();
	}
	if (PlayerAttackCrush == operation->operationType)
	{
		//std::cout << "들어옴" << std::endl;
		int sX = player[id].getCurrentSectorX()-1;
		int sZ = player[id].getCurrentSectorZ()-1;
		int eX = player[id].getCurrentSectorX() + 2;
		int eZ = player[id].getCurrentSectorZ() + 2;

		D3DXVECTOR3 monPos;
		float distance;
		float attackRaduis;

		D3DXVECTOR3 look;
		float angle;
		float a;
		float b;

		if (sX < 0)
			sX = 0;
		if (eX >= SECTOR_LENGTH)
			eX = SECTOR_LENGTH - 1;
		if (sZ < 0)
			sZ = 0;
		if (eZ >= SECTOR_WIDETH)
			eZ = SECTOR_WIDETH - 1;
		for (auto X = sX; X < eX; ++X)
		{
			for (auto Z = sZ; Z < eZ; ++Z)
			{
				for (auto i = 0; i < MAX_MONSTER; ++i)
				{
					if (nullptr == sector[X][Z].monsterArr[i])
						continue;
					if (deadState == sector[X][Z].monsterArr[i]->getState())
						continue;
					if (waitPosState == sector[X][Z].monsterArr[i]->getState())
						continue;
					monPos = sector[X][Z].monsterArr[i]->getPosition();
					distance = ((monPos.x - player[id].getPositionX()) *(monPos.x - player[id].getPositionX()) +
						(monPos.z - player[id].getPositionZ()) *(monPos.z - player[id].getPositionZ()));
					attackRaduis = (140.0 + 60.0) * (140.0 + 60.0);
					if (distance <= attackRaduis)
					{
						look = monPos - player[id].getPosition();
						D3DXVec3Normalize(&look, &look);
						angle = D3DXVec3Dot(&player[id].getDirection(), &look);
						a = (float)acos((double)angle);
						b = D3DXToDegree(a);
						if (b <= 40.0)
							sector[X][Z].monsterArr[i]->deCreaseHealth();
					}

				}
			}
		}
	}
}
void LogicServer::monsterActProcessPacket(OverEx *operation, int id, float delTime)
{
	//cout << "몬스터이벤트" << endl;
	int startSectorX = 0, startSectorZ = 0, endSectorX = 0, endSectorZ = 0;
	startSectorX = player[id].getCurrentSectorX() - 1;
	startSectorZ = player[id].getCurrentSectorZ() - 1;
	endSectorX = player[id].getCurrentSectorX() + 2;
	endSectorZ = player[id].getCurrentSectorZ() + 2;

	if (startSectorX < 0)
		startSectorX = 0;
	if (endSectorX >= SECTOR_LENGTH)
		endSectorX = SECTOR_LENGTH-1;
	if (startSectorZ < 0)
		startSectorZ = 0;
	if (endSectorZ >= SECTOR_WIDETH)
		endSectorZ = SECTOR_WIDETH-1;

	ScPacketMonsterPos monPos;
	monPos.packetSize = sizeof(ScPacketMonsterPos);
	monPos.packetType = SC_MONSTER_POS;

	ScPacketMonsterState monState;
	monState.packetSize = sizeof(ScPacketMonsterState);
	monState.packetType = SC_MONSTER_STATE_UPDATE;

	ScPacketResetMonster reset;
	reset.packetSize = sizeof(ScPacketResetMonster);
	reset.packetType = SC_MONSTER_RESET;

	for (int i = startSectorX; i < endSectorX; ++i)
	{
		for (int j = startSectorZ; j < endSectorZ; ++j)
		{
			//if (0 == sector[i][j].playerList.size()) //현재 섹터의 플레이어 리스트가 비어있을경우
			//{
			//	//cout <<i<<" "<<j<<"플레이어없음" << endl;
			//	//현재 섹터에 플레이어가 없더라도 몬스터들이 추격했다가 돌아올수도 있으므로
			//	//플레이어가 존재하지 않는섹터의 몬스터들도 최초위치로 강제이동을 시켜줘야한다.
			//	for (auto m = 0; m < MAX_MONSTER; ++m)
			//	{
			//		if (nullptr == sector[i][j].monsterArr[m])
			//			continue;
			//		if (waitPosState!= sector[i][j].monsterArr[m]->getState() || returnPos != sector[i][j].monsterArr[m]->getState())//시야에 플레이어가 사라졌기 떄문에 상태를 바꿔줘야한다.
			//			sector[i][j].monsterArr[m]->setState(returnPos);
			//		sector[i][j].monsterArr[m]->setDealtaTime(0.02);
			//		sector[i][j].monsterArr[m]->upDate();

			//		monState.monsterState = sector[i][j].monsterArr[m]->getState();
			//		sendPacket(id, &monState);

			//		monPos.monsterID = sector[i][j].monsterArr[m]->getMonsterID();
			//		monPos.direction = sector[i][j].monsterArr[m]->getDirection();
			//		monPos.position = sector[i][j].monsterArr[m]->getPosition();
			//		sendPacket(id, &monPos);
			//	}
			//}
			//else
			//{
				for (auto m = 0; m < MAX_MONSTER; ++m)
				{
					if (nullptr == sector[i][j].monsterArr[m])
						continue;
					D3DXVECTOR3 pos = sector[i][j].monsterArr[m]->getPosition();
					if (-1 == sector[i][j].monsterArr[m]->getTarget())
					{
						sector[i][j].monsterArr[m]->setTarget(id, player[id].getPosition());
					}
					else
					{
						sector[i][j].monsterArr[m]->setTargetPosition(player[sector[i][j].monsterArr[m]->getTarget()].getPosition());
					}
					sector[i][j].monsterArr[m]->setDealtaTime(0.02);
					sector[i][j].monsterArr[m]->upDate();

					int state = sector[i][j].monsterArr[m]->getState();
					monState.monsterID = sector[i][j].monsterArr[m]->getMonsterID();
					monState.monsterState = state;
					sendPacket(id, &monState);
					if (pos != sector[i][j].monsterArr[m]->getPosition())
					{
						monPos.monsterID = sector[i][j].monsterArr[m]->getMonsterID();
						monPos.direction = sector[i][j].monsterArr[m]->getDirection();
						monPos.position = sector[i][j].monsterArr[m]->getPosition();
						sendPacket(id, &monPos);
					}
	/*			}*/
			}
		}
	}
	//몬스터 타이머 등록
	player[id].pLock.lock();
	timer.AddGameEvent(MonsterMove, player[id].getID(), 20);
	player[id].pLock.unlock();
}
void LogicServer::sendPacket(int client, void* packet)
{
	//	char* a = reinterpret_cast<char*>(packet);
	int *packetSize = reinterpret_cast<int*>(packet);
	OverEx *Send_Operation = new OverEx;
	ZeroMemory(&Send_Operation->overLapped, sizeof(WSAOVERLAPPED));
	Send_Operation->operationType = Sendtype;
	Send_Operation->buf.buf = Send_Operation->iocpBuf;
	Send_Operation->buf.len = *packetSize;
	memcpy(Send_Operation->iocpBuf, reinterpret_cast<char*>(packet), *packetSize);

	DWORD iobyte;
	DWORD sendFlag = 0;
	int retval;
	retval = WSASend(player[client].overEx->s, &Send_Operation->buf, 1,
		&iobyte, sendFlag, &Send_Operation->overLapped, NULL);
	if (retval == SOCKET_ERROR)
	{
		if (WSAGetLastError() != WSA_IO_PENDING)
		{
			//cout << "sendpacket : " <<WSAGetLastError()<< endl;
		}
	}
	//	cout << client << "data Send" << endl;

}
void LogicServer::searchSector(int id)
{
	int x = 0, z = 0;
	x = static_cast<int>(player[id].getPositionX() / DIVIDE_SECTOR_X);
	z = static_cast<int>(player[id].getPositionZ() / DIVIDE_SECTOR_Z);
	if (x >= SECTOR_LENGTH)
		x = SECTOR_LENGTH -1;
	else
		player[id].setCurrentSectorX(x);
	if (z >= SECTOR_WIDETH)
		z = SECTOR_WIDETH-1;
	else
		player[id].setCurrentSectorZ(z);

	if (x != player[id].getPreiveseSectorX() ||
		z != player[id].getPreiveseSectorZ())
	{
		//----------------Sector 갱신시 플레이어리스트도 동시에 갱신
		player[id].pLock.lock();
		//이전 섹터에서 id가 섹터리스트에 존재할 경우 id를 지워줌
		if (0 != sector[player[id].getPreiveseSectorX()][player[id].getPreiveseSectorZ()].playerList.count(id)) {
			sector[player[id].getPreiveseSectorX()][player[id].getPreiveseSectorZ()].playerList.erase(id);
			player[id].pLock.unlock();
		}
		else
			player[id].pLock.unlock();
		//그리고 현재 섹터에 갱신을 해준다.

		player[id].pLock.lock();
		if (0 == sector[player[id].getCurrentSectorX()][player[id].getCurrentSectorZ()].playerList.count(id))
			sector[player[id].getCurrentSectorX()][player[id].getCurrentSectorZ()].playerList.insert(id);
		player[id].pLock.unlock();
		//------------------

		player[id].setPreiveseSectorX(player[id].getCurrentSectorX());
		player[id].setPreiveseSectorZ(player[id].getCurrentSectorZ());

		int startSectorX = 0, startSectorZ = 0, endSectorX = 0, endSectorZ = 0;
		startSectorX = player[id].getCurrentSectorX() - 1;
		startSectorZ = player[id].getCurrentSectorZ() - 1;
		endSectorX = player[id].getCurrentSectorX() + 2;
		endSectorZ = player[id].getCurrentSectorZ() + 2;

		if (startSectorX < 0)
			startSectorX = 0;
		if (endSectorX >= SECTOR_LENGTH)
			endSectorX = SECTOR_LENGTH-1;
		if (startSectorZ < 0)
			startSectorZ = 0;
		if (endSectorZ >= SECTOR_WIDETH)
			endSectorZ = SECTOR_WIDETH-1;

		//섹터 변환시 오브젝트 셋팅
		unsigned long objectCount = 0;
		ScPacketObject sectorObjectPacket;
		ZeroMemory(&sectorObjectPacket.objects, sizeof(sectorObjectPacket.objects));
		for (int i = startSectorX; i < endSectorX; ++i)
		{
			for (int j = startSectorZ; j < endSectorZ; ++j)
			{
				memcpy_s(sectorObjectPacket.objects + objectCount,
					sizeof(sectorObjectPacket.objects) - (sizeof(sector[i][j].arrayObject)*i),
					sector[i][j].arrayObject,
					sizeof(sector[i][j].arrayObject));
				objectCount += 10;
			}
		}
		sectorObjectPacket.position = player[id].getCurrentSector();
		sectorObjectPacket.id = player[id].getID();
		sectorObjectPacket.packetSize = sizeof(ScPacketObject);
		sectorObjectPacket.packetType = SC_SECTOR_UPDATE;
		sendPacket(id, &sectorObjectPacket);

		//섹터 변환시 몬스터 셋팅
		unsigned long MonsterCount = 0;
		ScPacketMonsterList monsterPacket;
		monsterPacket.packetType = SC_MONSTER_UPDATE;
		monsterPacket.packetSize = sizeof(ScPacketMonsterList);
		for (int i = startSectorX; i < endSectorX; ++i)
		{
			for (int j = startSectorZ; j < endSectorZ; ++j)
			{
				for (auto m = 0; m < MAX_MONSTER; ++m)
				{
					if (nullptr == sector[i][j].monsterArr[m])
						continue;
					player[id].pLock.lock();
					monsterPacket.monster[MonsterCount].id = sector[i][j].monsterArr[m]->getMonsterID();
					monsterPacket.monster[MonsterCount].state = sector[i][j].monsterArr[m]->getState();
					monsterPacket.monster[MonsterCount].monsterPos = sector[i][j].monsterArr[m]->getPosition();
					monsterPacket.monster[MonsterCount].monsterDir = sector[i][j].monsterArr[m]->getDirection();
					monsterPacket.monster[MonsterCount].type = sector[i][j].monsterArr[m]->getType();
					MonsterCount++;
					player[id].pLock.unlock();
				}
			}
		}
		sendPacket(id, &monsterPacket);

	}
}
void LogicServer::viewListUpdate(int id)
{
	std::unordered_set <int> nearList;
	std::unordered_set <int> removeList;
	nearList.clear();
	removeList.clear();

	int startSectorX = 0, startSectorZ = 0, endSectorX = 0, endSectorZ = 0;
	startSectorX = player[id].getCurrentSectorX() - 1;
	startSectorZ = player[id].getCurrentSectorZ() - 1;
	endSectorX = player[id].getCurrentSectorX() + 2;
	endSectorZ = player[id].getCurrentSectorZ() + 2;

	if (startSectorX < 0)
		startSectorX = 0;
	if (endSectorX >= SECTOR_LENGTH)
		endSectorX = SECTOR_LENGTH-1;
	if (startSectorZ < 0)
		startSectorZ = 0;
	if (endSectorZ >= SECTOR_WIDETH)
		endSectorZ = SECTOR_WIDETH-1;
	//섹터에 존재하는 플레이어들을 전부 nearList에 넣는다.
	for (auto i = startSectorX; i < endSectorX; ++i)
	{
		for (auto j = startSectorZ; j < endSectorZ; ++j)
		{
			if (0 == sector[i][j].playerList.size()) continue;
			for (auto p : sector[i][j].playerList)
			{
				if (id == p) continue;//혹시나 그아이디 내아이디면 컨티뉴
				if (false == player[p].getPlay()) continue; //혹시나 게임중이 아닐경우 컨티뉴
				nearList.insert(p);
			}
		}
	}
	//id의 뷰리스트와 nearlist를 비교하여 nearlist에 없을경우 뷰리스트의 아이디를 리무브 리스트에 넣고 제거
	
	for (auto i : nearList)
	{
		player[id].pLock.lock();
		if (0 == player[id].viewList.count(i))
		{
			player[id].viewList.insert(i);
			player[id].pLock.unlock();

			player[i].pLock.lock();
			if (0 == player[i].viewList.count(id))
			{
				player[i].viewList.insert(id);
				player[i].pLock.unlock();
			}
			else
				player[i].pLock.unlock();
		}
		else
		{
			player[id].pLock.unlock();

			player[i].pLock.lock();
			if (0 == player[i].viewList.count(id))
			{
				player[i].viewList.insert(id);
				player[i].pLock.unlock();
			}
			else
				player[i].pLock.unlock();
		}
	}
	

	//뷰리스트에서 나갈경우
	player[id].pLock.lock();
	for (auto i : player[id].viewList)
	{
		if (0 != nearList.count(i)) continue;
		removeList.insert(i);
	}
	player[id].pLock.unlock();

	//nearlist에게 putplayer 패킷 뿌리기
	ScPacektPutPlayer put;
	put.packetSize = sizeof(put);
	put.packetType = SC_PUT_PLAYER;
	put.id = id;
	put.position = player[id].getPosition();
	put.direction = player[id].getDirection();
	put.state = waitState;
	for (auto i : nearList)
	{
		sendPacket(i, &put);
	}
	for (auto i : nearList)
	{
		put.id = i;
		put.position = player[i].getPosition();
		put.direction = player[i].getDirection();
		put.state = waitState;
		sendPacket(id, &put);
	}
	//removelist에게 removeplayer 패킷 뿌리기
	player[id].pLock.lock();
	for (auto i : removeList)
	{
		player[id].viewList.erase(i);
	}
	player[id].pLock.unlock();

	ScPacketRemoveObject remove;
	remove.packetSize = sizeof(remove);
	remove.packetType = SC_REMOVE_PLAYER;
	remove.id = id;
	for (auto i : removeList)
	{
		player[i].pLock.lock();
		if (0 != player[i].viewList.count(id)) {
			player[i].viewList.erase(id);
			player[i].pLock.unlock();
			sendPacket(i, &remove);
		}
		else player[i].pLock.unlock();

	}
	for (auto i : removeList)
	{
		remove.id = i;
		sendPacket(id, &remove);
	}

}
void LogicServer::setObject(int id)
{
	int x = 0, z = 0;
	x = static_cast<int>(player[id].getPositionX() / DIVIDE_SECTOR_X);
	z = static_cast<int>(player[id].getPositionZ() / DIVIDE_SECTOR_Z);
	if (x >= SECTOR_LENGTH)
		x = SECTOR_LENGTH-1;
	else
		player[id].setCurrentSectorX(x);
	if (z >= SECTOR_WIDETH)
		z = SECTOR_WIDETH-1;
	else
		player[id].setCurrentSectorZ(z);

	player[id].setPreiveseSectorX(player[id].getCurrentSectorX());
	player[id].setPreiveseSectorZ(player[id].getCurrentSectorZ());

	int startSectorX = 0, startSectorZ = 0, endSectorX = 0, endSectorZ = 0;
	startSectorX = player[id].getCurrentSectorX() - 1;
	startSectorZ = player[id].getCurrentSectorZ() - 1;
	endSectorX = player[id].getCurrentSectorX() + 2;
	endSectorZ = player[id].getCurrentSectorZ() + 2;

	x = player[id].getCurrentSectorX();
	z = player[id].getCurrentSectorZ();

	//player[id].pLock.lock();
	sector[x][z].sectorLock.lock();
	sector[x][z].playerList.insert(id);
	sector[x][z].sectorLock.unlock();
	//player[id].pLock.unlock();

	if (startSectorX < 0)
		startSectorX = 0;
	if (endSectorX >= SECTOR_LENGTH)
		endSectorX = SECTOR_LENGTH-1;
	if (startSectorZ < 0)
		startSectorZ = 0;
	if (endSectorZ >= SECTOR_WIDETH)
		endSectorZ = SECTOR_WIDETH-1;

	//초기 정적 오브젝트 셋팅
	unsigned long objectCount = 0;
	ScPacketObject sectorObjectPacket;
	ZeroMemory(&sectorObjectPacket.objects, sizeof(sectorObjectPacket.objects));
	//myLock.lock();
	for (int i = startSectorX; i < endSectorX; ++i)
	{
		for (int j = startSectorZ; j < endSectorZ; ++j)
		{
			memcpy_s(sectorObjectPacket.objects + objectCount,
				sizeof(sectorObjectPacket.objects) - (sizeof(sector[i][j].arrayObject)*i),
				sector[i][j].arrayObject,
				sizeof(sector[i][j].arrayObject));
			objectCount += 10;
		}
	}
	sectorObjectPacket.position = player[id].getCurrentSector();
	sectorObjectPacket.id = player[id].getID();
	sectorObjectPacket.packetSize = sizeof(ScPacketObject);
	sectorObjectPacket.packetType = SC_SECTOR_UPDATE;
	//myLock.unlock();
	sendPacket(id, &sectorObjectPacket);

	//초기 몬스터 셋팅
	unsigned long MonsterCount = 0;
	ScPacketMonsterList monsterPacket;
	monsterPacket.packetType = SC_MONSTER_UPDATE;
	monsterPacket.packetSize = sizeof(ScPacketMonsterList);
	for (int i = startSectorX; i < endSectorX; ++i)
	{
		for (int j = startSectorZ; j < endSectorZ; ++j)
		{
			for (auto m = 0; m < MAX_MONSTER; ++m)
			{
				if (nullptr == sector[i][j].monsterArr[m])
					continue;
				monsterPacket.monster[MonsterCount].id = sector[i][j].monsterArr[m]->getMonsterID();
				monsterPacket.monster[MonsterCount].state = sector[i][j].monsterArr[m]->getState();
				monsterPacket.monster[MonsterCount].monsterPos = sector[i][j].monsterArr[m]->getPosition();
				monsterPacket.monster[MonsterCount].monsterDir = sector[i][j].monsterArr[m]->getDirection();
				monsterPacket.monster[MonsterCount].type = sector[i][j].monsterArr[m]->getType();
				MonsterCount++;
			}
		}
	}
	sendPacket(id, &monsterPacket);

	//몬스터 타이머 등록
	player[id].pLock.lock();
	timer.AddGameEvent(MonsterMove, player[id].getID(), 100);
	player[id].pLock.unlock();
}
void LogicServer::crashMaxMapSize(int id)
{
	if (player[id].getPositionX() <= 0.0)
		player[id].setPositionX(0.0);
	if (player[id].getPositionX() >= MAP_MAX_LENGTH)
		player[id].setPositionX(MAP_MAX_LENGTH);
	if (player[id].getPositionZ() <= 0.0)
		player[id].setPositionZ(0.0);
	if (player[id].getPositionZ() >= MAP_MAX_WIDETH)
		player[id].setPositionZ(MAP_MAX_WIDETH);
}
bool LogicServer::crashObject(D3DXVECTOR3 position, int id)
{
	int startSectorX = 0, startSectorZ = 0, endSectorX = 0, endSectorZ = 0;
	startSectorX = player[id].getCurrentSectorX() - 1;
	startSectorZ = player[id].getCurrentSectorZ() - 1;
	endSectorX = player[id].getCurrentSectorX() + 2;
	endSectorZ = player[id].getCurrentSectorZ() + 2;

	if (startSectorX < 0)
		startSectorX = 0;
	if (endSectorX > SECTOR_LENGTH)
		endSectorX = SECTOR_LENGTH-1;
	if (startSectorZ < 0)
		startSectorZ = 0;
	if (endSectorZ > SECTOR_WIDETH)
		endSectorZ = SECTOR_WIDETH-1;

	for (auto sx = startSectorX; sx < endSectorX; ++sx)
	{
		for (auto sz = startSectorZ; sz < endSectorZ; ++sz)
		{
			for (auto i = 0; i < MAX_OBJECT; ++i) //max는 X   min은 Y
			{
				if ((position.x + 20.0 >
					sector[sx][sz].arrayObject[i].objectPosition.x + (-sector[sx][sz].arrayObject[i].max)) &&
					(position.x - 20.0f <
					sector[sx][sz].arrayObject[i].objectPosition.x + sector[sx][sz].arrayObject[i].max))
				{
					if ((position.z + 20.0 >
						sector[sx][sz].arrayObject[i].objectPosition.z + (-sector[sx][sz].arrayObject[i].min)) &&
						(position.z - 20.0f<
						sector[sx][sz].arrayObject[i].objectPosition.z + sector[sx][sz].arrayObject[i].min))
					{
						return true;
					}
				}
			}
		}
	}
}





////_asm mfence;
//if ((position.x + 20.0 <
//	sector[sx][sz].arrayObject[i].objectPosition.x + (-sector[sx][sz].arrayObject[i].min)) ||
//	(position.x > sector[sx][sz].arrayObject[i].objectPosition.x +
//		sector[sx][sz].arrayObject[i].max))
//{
//	continue;
//}
////if ((position.y + 20.0 < sector[sx][sz].arrayObject[i].objectPosition.y) ||
////	(position.y > sector[sx][sz].arrayObject[i].objectPosition.y +
////		sector[sx][sz].arrayObject[i].max))
////{
////	continue;
////}
//if ((position.z + 20.0 <
//	sector[sx][sz].arrayObject[i].objectPosition.z + (-sector[sx][sz].arrayObject[i].min)) ||
//	(position.z >
//		sector[sx][sz].arrayObject[i].objectPosition.z + sector[sx][sz].arrayObject[i].max))
//{
//	continue;
//}
//return true;