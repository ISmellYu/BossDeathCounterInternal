#pragma once
#include <string>

#include "BossTimeStamp.h"

enum BossState
{
	None = 0,
	NotStarted = 1,
	Started = 2,
	Paused = 3,
	Dead = 4
};


class Boss
{
public:
	std::string bossName;
	time_t startDate = NULL;
	time_t endDate = NULL;
	BossState state;
	int deaths{};

	bool StartBoss();
	bool EndBoss();
	BossTimeStamp GetElapsedTime();
	explicit Boss(std::string name);
};