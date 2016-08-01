#include "stdafx.h"
using namespace std;
#ifdef _DEBUG
#pragma comment(linker, "/entry:WinMainCRTStartup /subsystem:console")
#endif
#pragma comment (lib, "ws2_32.lib")

LRESULT CALLBACK WndProc(HWND hwnd, UINT iMsg,
	WPARAM wParam, LPARAM lParam);
HINSTANCE hInst;
HWND hWnd;
D3DXVECTOR3 myDir,myPos;
float currectTime;
float newTime;
float deltaTIme;
bool flagUP;

WPARAM keyValue;

ClientServer *s = ClientServer::getInstangce();
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
	LPSTR lpszCmdLine, int nCmdShow)
{
	HWND 	 hwnd;
	MSG 	 msg;
	WNDCLASS WndClass;
	hInst = hInstance;
	WndClass.style = CS_HREDRAW | CS_VREDRAW;
	WndClass.lpfnWndProc = WndProc;
	WndClass.cbClsExtra = 0;
	WndClass.cbWndExtra = 0;
	WndClass.hInstance = hInstance;
	WndClass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	WndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
	WndClass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	WndClass.lpszMenuName = NULL;
	WndClass.lpszClassName = L"Window Class Name";
	RegisterClass(&WndClass);
	hwnd = CreateWindow(L"Window Class Name",
		L"Window Title Name",
		WS_OVERLAPPEDWINDOW,
		0,
		0,
		800,
		600,
		NULL,
		NULL,
		hInstance,
		NULL
		);
	ShowWindow(hwnd, nCmdShow);
	UpdateWindow(hwnd);

	//서버 초기화
	s->setSocketHWND(hwnd);
	s->setHINSTANCE(hInst);
	s->socketInit();
	myDir = s->Player[0].getPlayerDirection();
	myPos = s->Player[0].getPlayerPosition();
	//----------
	newTime = clock();
	while (GetMessage(&msg, 0, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	return msg.wParam;
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT iMsg,
	WPARAM wParam, LPARAM lParam)
{
	if (s->flag == true) {
		myDir = s->Player[0].getPlayerDirection();
		myPos = s->Player[0].getPlayerPosition();
		s->flag = false;
	}
	currectTime = clock();
	HWND hDlg = NULL;
	hWnd = hDlg;
	HDC hdc;
	PAINTSTRUCT ps;

	switch (iMsg)
	{
	case WM_SOCKET:
	{
		if (WSAGETSELECTERROR(lParam)) {
			closesocket((SOCKET)wParam);
			break;
		}
		switch (WSAGETSELECTEVENT(lParam)) {
		case FD_READ:
			s->readPacket();
			//	InvalidateRect(hWnd, NULL, TRUE);
			break;
		case FD_CLOSE:
			closesocket((SOCKET)wParam);
			break;
		}
		break;
	}
	case WM_CREATE:
		SetTimer(hwnd, 1, 16, NULL);
		keyValue = NULL;
		flagUP = false;
		break;
	case WM_KEYDOWN:
		if (VK_RETURN == wParam) break;
		if (NULL == keyValue) {
			keyValue = wParam;
			if (VK_DOWN == wParam)
				myDir = D3DXVECTOR3(0, 0, 1);
			if (VK_UP == wParam)
				myDir = D3DXVECTOR3(0, 0, -1);
			if (VK_LEFT == wParam)
				myDir = D3DXVECTOR3(-1, 0, 0);
			if (VK_RIGHT == wParam)
				myDir = D3DXVECTOR3(1, 0, 0);
			s->keyDown(myDir, myPos);
			//myPos = myPos + (VELOCITY*myDir*static_cast<float>(deltaTIme));
			cout << "keyDown" << static_cast<float>(deltaTIme)<< endl;
			myPos = myPos + (VELOCITY*myDir*(deltaTIme));
		}
		else
		{
			myPos = myPos + (VELOCITY*myDir*(deltaTIme));
			//cout << myPos.x << "," << myPos.z << endl;
		}
		break;
	case WM_KEYUP:
		cout << "up" << endl;
		keyValue = NULL;
		s->dirKeyUp(myDir, myPos);
		break;
	case WM_TIMER:
		InvalidateRect(hwnd, NULL, true);
		break;
	case WM_PAINT:
		deltaTIme = currectTime - newTime;
		hdc = BeginPaint(hwnd, &ps);
		for (auto i = 0; i < 100; ++i) {
			Rectangle(hdc, s->objectList[i].objectPosition.x, s->objectList[i].objectPosition.z,
				s->objectList[i].objectPosition.x + s->objectList[i].radius, s->objectList[i].objectPosition.z + s->objectList[i].radius);
		}
		for (auto i = 0; i < 100; ++i) {
			Rectangle(hdc, s->monsterList[i].monsterPos.x, s->monsterList[i].monsterPos.z,
				s->monsterList[i].monsterPos.z + 40, s->monsterList[i].monsterPos.z + 40);
		}
		for (auto i = 1; i < ROOM_MAX_PLAYER; ++i)
		{
			if (true == s->Player[i].getPlay()) {
				if (1 == s->Player[i].getState()) {
					D3DXVECTOR3 pos = s->Player[i].getPlayerPosition() +
						(VELOCITY* s->Player[i].getPlayerDirection() *(deltaTIme));
					Rectangle(hdc, pos.x, pos.z,
						pos.x + 40, pos.z + 40);
				}
				else
				{
					Rectangle(hdc, s->Player[i].getPlayerPosition().x, s->Player[i].getPlayerPosition().z,
						s->Player[i].getPlayerPosition().x + 40, s->Player[i].getPlayerPosition().z + 40);

				}
			}
		}
		Rectangle(hdc, myPos.x, myPos.z,
			myPos.x + 40, myPos.z + 40);
		EndPaint(hwnd, &ps);
		break;

	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	}
	newTime = clock();
	return DefWindowProc(hwnd, iMsg, wParam, lParam);
}
