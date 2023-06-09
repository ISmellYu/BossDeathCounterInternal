#pragma once
#include <ctime>

struct Pause
{
	time_t start = NULL;
	double duration = 0;
	time_t end = NULL;

	Pause(time_t startTime)
	{
		start = startTime;
	}

	Pause(bool currentTime = true)
	{
		if (currentTime)
		{
			start = std::time(nullptr);
		}
	}

	bool StartPause(time_t startTime = NULL)
	{
		if (start != NULL)
			return false;

		if (startTime == NULL)
			start = std::time(nullptr);
		else
			start = startTime;
		return true;
	}

	bool EndPause()
	{
		if (start == NULL)
			return false;

		if (end != NULL)
			return false;

		end = std::time(nullptr);
		duration = difftime(end, start);
		return true;
	}
};
