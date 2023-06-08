#pragma once
#include <string>

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

	bool StartBoss();
	bool EndBoss();
	explicit Boss(std::string name);
};