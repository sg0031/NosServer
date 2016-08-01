#include "stdafx.h"
#include "ClientServer.h"

ClientServer::ClientServer()
{
	currentTime = clock() + 1000;
	keyDownFlag = false;
	keyUpFlag = 1;
	flag = false;
	size = 0;
	//inPacketSize = new int;
	inPacketSize = &size;

	myId = 0;
	count = 0;
	for (int i = 0; i < SECTOR_WIDETH; ++i)
	{
		for (int j = 0; j < SECTOR_LENGTH; ++j)
		{
			sector[i][j].startSectorPosition.z = (j*80.0);
			sector[i][j].endSectorPosition.z = sector[i][j].startSectorPosition.z + 80.0;
			sector[i][j].startSectorPosition.x = (i*60.0);
			sector[i][j].endSectorPosition.x = sector[i][j].startSectorPosition.x + 60.0;
		}
	}
	ZeroMemory(&objectList, sizeof(objectList));
}
ClientServer::~ClientServer()
{
	//delete inPacketSize;
}
void ClientServer::setSocketHWND(HWND s) {
	socketHWND = s;
}
void ClientServer::setHINSTANCE(HINSTANCE g)
{
	ghInst = g;
}
int ClientServer::socketInit()
{
	WSADATA wsa;
	int ret = 0;

	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
		cout << "winsock init error" << endl;

	sock = WSASocket(AF_INET, SOCK_STREAM, IPPROTO_TCP, NULL, NULL, 0);

	SOCKADDR_IN addr;
	char arr[15];
	cout << "Ip 입력 : ";
	cin >> arr;

	ZeroMemory(&addr, sizeof(addr));
	addr.sin_family = AF_INET; //127.0.0.1
							   // 192.168.0.6
	addr.sin_addr.s_addr = inet_addr(arr);
	addr.sin_port = htons(9000);

	ret = WSAConnect(sock, (SOCKADDR*)&addr, sizeof(addr), NULL, NULL, NULL, NULL);

	WSAAsyncSelect(sock, socketHWND, WM_SOCKET, FD_READ | FD_CLOSE);

	wsaSendBuf.buf = sendBuf;
	wsaSendBuf.len = MAX_SIZE;
	wsaRecvBuf.buf = recvBuf;
	wsaRecvBuf.len = MAX_SIZE;

	DWORD iobyte;
	DWORD sendFlag = 0;

	CsPacketLogin *login =
		reinterpret_cast<CsPacketLogin*>(sendBuf);
	login->packetSize = sizeof(CsPacketLogin);
	login->packetType = CS_LOGIN_REQUEST;

	ret = WSASend(sock, &wsaSendBuf, 1, &iobyte, sendFlag, NULL, NULL);
	cout << iobyte << endl;
	if (ret == SOCKET_ERROR)
		cout << "WSASend Error" << endl;
	cout << "Login Request" << endl;
	return 1;
}

void ClientServer::readPacket()
{
	DWORD iobyte, ioflag = 0;

	int ret = WSARecv(sock, &wsaRecvBuf, 1, &iobyte, &ioflag, NULL, NULL);
	if (ret != 0)
	{
		int err_code = WSAGetLastError();
		cout << "Recv Error : " << err_code << endl;
	}
	//cout << "recv" << endl;
	char *ptr = reinterpret_cast<char*>(recvBuf);

	while (0 != iobyte)
	{
		if (size == 0)
		{
			inPacketSize = reinterpret_cast<int*>(ptr);
			size = *inPacketSize;
		}

		if (iobyte + savePacketSize >= size)
		{
			memcpy(completeBuf + savePacketSize,
				ptr,
				size - savePacketSize);

			processPacket(completeBuf);

			ptr += size - savePacketSize;
			iobyte -= size - savePacketSize;
			size = 0;
			savePacketSize = 0;
		}
		else
		{
			memcpy(completeBuf + savePacketSize, ptr, iobyte);
			savePacketSize += iobyte;
			iobyte = 0;
		}
	}
}

void ClientServer::processPacket(char* ptr)
{
	BYTE *header = reinterpret_cast<BYTE*>(ptr + 4);
	//cout << "process" << endl;
	switch (*header)
	{
	case SC_LOGIN_SUCCESS:
	{
		ScPacketMove *login =
			reinterpret_cast<ScPacketMove*>(ptr);
		myId = login->id;
		Player[0].setPlayerID(myId);
		Player[0].setPlayerPosition(login->position);
		Player[0].setState(login->state);
		Player[0].setPlay(true);
		//cout << "myId : " << Player[0].getPlayerID() << endl;
		break;
	}
	case SC_PLAYER_LIST:
	{
		ScPacketAcceptPlayerList *list =
			reinterpret_cast<ScPacketAcceptPlayerList*>(ptr);
		for (int i = 0; i < ROOM_MAX_PLAYER; ++i)
		{
			if (list->id == Player[0].getPlayerID())
			{
				Player[i].setPlayerPosition(list->position);
				break;
			}
			else if (-1 == Player[i].getPlayerID())
			{
				Player[i].setPlayerID(list->id);
				Player[i].setPlayerPosition(list->position);
				Player[i].setPlay(true);
				break;
			}
		}
		break;
	}
	case SC_MOVE_POSITION:
	{
		//		cout << "movePacket" << endl;
		ScPacketMove *move =
			reinterpret_cast<ScPacketMove*>(ptr);
		time = move->time;
		//cout << p->id << "," << p->x << "," << p->y << endl;
		for (int i = 0; i < ROOM_MAX_PLAYER; ++i)
		{
			if (Player[i].getPlayerID() == move->id)
			{
				Player[i].setPlayerDirection(move->direction);
				Player[i].setPlayerPosition(move->position);
				Player[i].setState(move->state);
				break;
			}
		}
		//Player[move->id].setPlayerPosition(move->position);
		break;
	}

	case SC_MOVE_ERROR_CHECK:
	{
		//cout << "이동 동기화 체크" << endl;
		ScPacketMove *check = reinterpret_cast<ScPacketMove*>(ptr);
		Player[0].setPlayerPosition(check->position);
		Player[0].setPlayerDirection(check->direction);
		Player[0].setState(check->state);
		flag = true;
		break;
	}
	case SC_SECTOR_UPDATE:
	{
		ScPacketObject *object = reinterpret_cast<ScPacketObject*>(ptr);
		ZeroMemory(&objectList, sizeof(objectList));
		memcpy_s(&objectList, sizeof(objectList), &object->objects, sizeof(object->objects));
		break;
	}
	case SC_PUT_PLAYER:
	{
		ScPacektPutPlayer *put = reinterpret_cast<ScPacektPutPlayer*>(ptr);
		for (int i = 0; i < ROOM_MAX_PLAYER; ++i)
		{
			if (put->id == Player[i].getPlayerID())
			{
				Player[i].setPlayerPosition(put->position);
				//Player[i].setPlayerDirection(put->direction);
				Player[i].setState(put->state);
				Player[i].setPlay(true);
				break;
			}
			else if (-1 == Player[i].getPlayerID())
			{
				Player[i].setPlayerPosition(put->position);
				//Player[i].setPlayerDirection(put->direction);
				Player[i].setPlayerID(put->id);
				Player[i].setState(put->state);
				Player[i].setPlay(true);
				break;
			}
		}
		break;
	}
	case SC_REMOVE_PLAYER:
	{
		ScPacketRemoveObject *remove = reinterpret_cast<ScPacketRemoveObject*>(ptr);
		for (int i = 0; i < ROOM_MAX_PLAYER; ++i)
		{
			if (remove->id == Player[i].getPlayerID())
			{
				Player[i].setPlayerPosition(D3DXVECTOR3(-10.0, 0, -10.0));
				Player[i].setPlay(false);
				//cout << Player[i].getPlayerPosition().x <<" "<< Player[i].getPlayerPosition().z << endl;
				break;
			}
		}
		break;
	}
	case SC_STATE_UPDATE:
	{
		ScPacketUpdateState *stateup = reinterpret_cast<ScPacketUpdateState*>(ptr);
		for (int i = 0; i < ROOM_MAX_PLAYER; ++i)
		{
			if (stateup->id == Player[i].getPlayerID())
			{
				Player[i].setState(stateup->state);
				//std::cout <<i<<"의 상태 : "<<Player[i].getState() << std::endl;
				break;
			}
		}
		break;
	}
	case SC_MONSTER_UPDATE:
	{
		ScPacketMonsterList *mon = reinterpret_cast<ScPacketMonsterList*>(ptr);
		ZeroMemory(&monsterList, sizeof(monsterList));
		memcpy_s(&monsterList, sizeof(monsterList), &mon->monster, sizeof(mon->monster));
		break;
	}
	case SC_MONSTER_POS:
	{
		ScPacketMonsterPos *monMove = reinterpret_cast<ScPacketMonsterPos*>(ptr);
		for (int i = 0; i < 100; ++i)
		{
			if (monMove->monsterID == monsterList[i].id)
			{
				monsterList[i].monsterPos = monMove->position;
				monsterList[i].monsterDir = monMove->direction;
				break;
			}
		}
		break;
	}
	case SC_MONSTER_STATE_UPDATE:
	{
		ScPacketMonsterState *monState = reinterpret_cast<ScPacketMonsterState*>(ptr);
		for (int i = 0; i < 100; ++i)
		{
			if (monState->monsterID == monsterList[i].id)
			{
				monsterList[i].state = monState->monsterState;
				break;
			}
		}
		break;
	}
	case SC_MONSTER_RESET:
	{
		ScPacketResetMonster *reset = reinterpret_cast<ScPacketResetMonster*>(ptr);
		for (auto i = 0; i < 100; ++i)
		{
			if (monsterList[i].id == reset->monsterID)
			{
				monsterList[i].state = waitPosState;
				monsterList[i].monsterPos = reset->returnPos;
				break;
			}
		}
		break;
	}
	}
}


void ClientServer::sendPacket(SOCKET s, void* buf)
{
	DWORD iobyte;
	DWORD ioflag = 0;
	SOCKET Send_socket = s;
	int *packet_size = reinterpret_cast<int*>(buf);
	OverEx *Send_Operation = new OverEx;
	ZeroMemory(Send_Operation, sizeof(OverEx));

	Send_Operation->buf.buf = Send_Operation->packetBuf;
	Send_Operation->buf.len = *packet_size;

	//wsaCompleteBuf.buf = completeBuf;
	//wsaCompleteBuf.len = packet_size;

	memcpy(Send_Operation->packetBuf, reinterpret_cast<char*>(buf), *packet_size);

	int retval = WSASend(Send_socket, &Send_Operation->buf, 1, &iobyte, ioflag, NULL, NULL);
	//cout << iobyte << endl;
	if (retval == SOCKET_ERROR)
	{
		cout << "WSASend() x Error" << endl;
		cout << WSAGetLastError() << endl;
	}
	delete Send_Operation;
	//cout << "client data send" << endl;
}
int ClientServer::getMyId()
{
	return myId;
}

void ClientServer::keyUp()
{
	CsPacketKeyUp upPacket;
	upPacket.packetSize = sizeof(CsPacketKeyUp);
	upPacket.packetType = CS_STOP;
	sendPacket(sock, &upPacket);
}
void ClientServer::keyDown(D3DXVECTOR3 dir, D3DXVECTOR3 pos)
{
	CsPacketMove myPacket;
	myPacket.packetSize = sizeof(CsPacketMove);
	myPacket.packetType = CS_KEY_DOWN;
	myPacket.position = pos;
	myPacket.direction = dir;
	sendPacket(sock, &myPacket);
}
void ClientServer::keyDownAttacket(DWORD key)
{
	CspacketAttack attack;
	attack.packetSize = sizeof(CspacketAttack);
	if (key == ATTACK01)
	{
		attack.packetType = CS_USE_SKILL_Q;
	}
	else if (key == ATTACK02)
	{
		attack.packetType = CS_USE_SKILL_W;
	}
	sendPacket(sock, &attack);
}
void ClientServer::dirKeyUp(D3DXVECTOR3 dir, D3DXVECTOR3 pos)
{
	CsPacketMove myPacket;
	myPacket.packetSize = sizeof(CsPacketMove);
	myPacket.packetType = CS_KEY_UP;
	myPacket.position = pos;
	myPacket.direction = dir;
	sendPacket(sock, &myPacket);
}
void ClientServer::infoRequest(D3DXVECTOR3 dir, D3DXVECTOR3 pos)
{
	CsPacketRequest request;
	request.packetSize = sizeof(CsPacketRequest);
	request.packetType = CS_INFO_REQUSET;
	request.direction = dir;
	request.position = pos;
	sendPacket(sock, &request);
}