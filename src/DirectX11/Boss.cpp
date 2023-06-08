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
	startDate = std::time(nullptr);
	state = Started;
	return true;
}

bool Boss::EndBoss()
{
	if (startDate == NULL || endDate != NULL)
		return false;
	endDate = std::time(nullptr);
	state = Dead;
	return true;
}

BossTimeStamp Boss::GetElapsedTime()
{
	if (state == NotStarted)
		return {};

	if (state == Dead)
	{
		return {static_cast<int>(difftime(endDate, startDate))};
	}


	if (state == Started)
	{
		return {static_cast<int>(difftime(std::time(0), startDate))};
	}


	// just in case xD
	if (startDate == NULL)
		return {};

	if (endDate == NULL)
	{
		return {static_cast<int>(difftime(std::time(0), startDate))};
	}


	return {};
}
