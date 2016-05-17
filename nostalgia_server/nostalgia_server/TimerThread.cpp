#include "stdafx.h"
#include "TimerThread.h"


TimerThread::TimerThread()
{
	
}

TimerThread::~TimerThread()
{
}

void TimerThread::eventTimerThread()
{
	//timerThread
	int count = 0;
	std::cout << "timer create" << std::endl;
	while (1)
	{
		Sleep(0);
		TimerLock.lock();
		if (0 == timerQueue.size())
		{	
			TimerLock.unlock();
			continue;
		}
		//std::cout << "��" << std::endl;
		GameEvent eventWork;
		eventWork = timerQueue.top();
		while (eventWork.wakeupTime <= GetTickCount())
		{
			//���� ���� �ð��� wakeuptimer ���� Ŭ ��� ���ο� �۾��� �����ͼ� ���� �Ѵ�.
			timerQueue.pop();
			TimerLock.unlock();
			EventToWorkerThread(eventWork);
			TimerLock.lock();
			if (0 == timerQueue.size())
			{
				//TimerLock.unlock();
				break;
			}
			
			eventWork = timerQueue.top();
		}
		//std::cout << "dd" << std::endl;
		TimerLock.unlock();
	}
}
void TimerThread::AddGameEvent(EOperationType eventType, int id, DWORD time)
{
	//std::cout << "�̺�Ʈ����" << std::endl;
	//�켱���� ť�� �̺�Ʈ �۾��� ���� �ִ´�.
	GameEvent gameEvent;
	gameEvent.eventType = eventType;
	gameEvent.id = id;
	gameEvent.wakeupTime = GetTickCount() + time;
	//stl �ڷᱸ���� �����Ͽ� �޼ҵ带 ȣ���ϰ� �۾��ϹǷ� ���� �ݵ�� ���ش�.
	TimerLock.lock();
	timerQueue.push(gameEvent);
	TimerLock.unlock();
}
void TimerThread::EventToWorkerThread(const GameEvent& newEvent)
{
	//std::cout << "�̺�Ʈ �Ϸ�" << std::endl;
	HandleManager *hManager = HandleManager::getInstance();
	OverEx *overEvent = new OverEx;

	overEvent->operationType = newEvent.eventType;

	PostQueuedCompletionStatus(hManager->gHandle, 1,
		newEvent.id, reinterpret_cast<LPOVERLAPPED>(overEvent));
}