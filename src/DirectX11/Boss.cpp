#include "Boss.h"
#include <cassert>
#include <ctime>
#include <string>
#include <utility>


Boss::Boss(std::string name)
{
	bossName = std::move(name);
	state = NotStarted;
}

bool Boss::StartBoss()
{
	if (startDate != NULL)
		return false;

	if (state == Dead)
		return false;

	if (state == Paused)
		return false;

	startDate = std::time(nullptr);
	state = Started;
	return true;
}

bool Boss::EndBoss()
{
	if (startDate == NULL || endDate != NULL)
		return false;

	if (state == Paused)
		ResumeBoss();

	endDate = std::time(nullptr);
	state = Dead;
	return true;
}

BossTimeStamp Boss::GetElapsedTime()
{
	if (state == NotStarted)
		return {};

	double pauseTimeElapsed = 0;

	for (auto& pause : pauses)
	{
		if (pause->end == NULL)
			continue;
		pauseTimeElapsed += pause->duration;
	}

	if (state == Dead)
		return {static_cast<int>(difftime(endDate, startDate)) - static_cast<int>(pauseTimeElapsed)};

	if (state == Started )
		return {static_cast<int>(difftime(std::time(0), startDate)) - static_cast<int>(pauseTimeElapsed)};

	if (state == Paused)
	{
		Pause* pLatestPause = nullptr;

		for (auto& pause : pauses)
		{
			if (pause->end == NULL)
				pLatestPause = pause.get();
		}

		return {static_cast<int>(difftime(pLatestPause->start, startDate)) - static_cast<int>(pauseTimeElapsed)};
	}

	return {};
}

bool Boss::PauseBoss()
{
	if (state == NotStarted)
		return false;

	if (state == Paused)
		return false;

	if (state == Dead)
		return false;

	// check if any pauses are currently in progress
	for (auto& pause : pauses)
	{
		if (pause->end == NULL)
			return false;
	}


	auto pPause = std::make_shared<Pause>();
	pauses.emplace_back(std::move(pPause));
	state = Paused;
	return true;
}

bool Boss::ResumeBoss()
{
	if (state == NotStarted)
		return false;

	if (state == Started)
		return false;

	if (state == Dead)
		return false;


	Pause* pPause = nullptr;
	// check if any pauses are 'resumable'
	for (auto& pause : pauses)
	{
		if (pause->end == NULL)
			pPause = pause.get();
	}

	if (pPause == nullptr)
		return false;

	pPause->EndPause();

	state = Started;
}

