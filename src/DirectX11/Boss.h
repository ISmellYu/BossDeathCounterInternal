#pragma once
#include <memory>
#include <string>
#include <vector>

#include "BossTimeStamp.h"
#include "Pause.h"

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
	std::vector<std::shared_ptr<Pause>> pauses{};

	bool StartBoss();
	bool EndBoss();
	BossTimeStamp GetElapsedTime();
	bool PauseBoss();
	bool ResumeBoss();
	explicit Boss(std::string name);
};
