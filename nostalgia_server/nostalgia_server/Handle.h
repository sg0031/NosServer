#pragma once


class HandleManager
{
public:
	HandleManager() {}
	~HandleManager() {}
	HANDLE gHandle;
	static HandleManager *getInstance()
	{
		static HandleManager instance;
		return &instance;
	}
};