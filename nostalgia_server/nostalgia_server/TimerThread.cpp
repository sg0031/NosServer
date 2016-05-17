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
		//std::cout << "ㅇ" << std::endl;
		GameEvent eventWork;
		eventWork = timerQueue.top();
		while (eventWork.wakeupTime <= GetTickCount())
		{
			//지금 현재 시간이 wakeuptimer 보다 클 경우 새로운 작업을 꺼내와서 진행 한다.
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
	//std::cout << "이벤트들어옴" << std::endl;
	//우선순위 큐에 이벤트 작업을 집어 넣는다.
	GameEvent gameEvent;
	gameEvent.eventType = eventType;
	gameEvent.id = id;
	gameEvent.wakeupTime = GetTickCount() + time;
	//stl 자료구조에 접근하여 메소드를 호출하고 작업하므로 락을 반드시 해준다.
	TimerLock.lock();
	timerQueue.push(gameEvent);
	TimerLock.unlock();
}
void TimerThread::EventToWorkerThread(const GameEvent& newEvent)
{
	//std::cout << "이벤트 완료" << std::endl;
	HandleManager *hManager = HandleManager::getInstance();
	OverEx *overEvent = new OverEx;

	overEvent->operationType = newEvent.eventType;

	PostQueuedCompletionStatus(hManager->gHandle, 1,
		newEvent.id, reinterpret_cast<LPOVERLAPPED>(overEvent));
}