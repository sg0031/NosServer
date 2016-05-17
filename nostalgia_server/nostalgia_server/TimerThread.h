#pragma once

struct GameEvent
{
	EOperationType eventType;
	int id;
	DWORD wakeupTime;
};

struct TimerStandard
{
	bool operator()(const GameEvent& lhs, const GameEvent& rhs) const
	{
		return lhs.wakeupTime > rhs.wakeupTime;
	}
};

class TimerThread
{

	std::mutex TimerLock;
	std::priority_queue <GameEvent,std::vector<GameEvent>,TimerStandard> timerQueue;
public:
	TimerThread();
	~TimerThread();
	void eventTimerThread();
	void AddGameEvent(EOperationType eventType, int id, DWORD time);
	void EventToWorkerThread(const GameEvent& new_event);
};

