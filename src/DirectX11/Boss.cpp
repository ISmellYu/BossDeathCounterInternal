#include "Boss.h"

#include <cassert>
#include <ctime>
#include <string>
#include <utility>

Boss::Boss(std::string name)
{
	bossName = std::move(name);
}

bool Boss::StartBoss()
{
	assert(startDate != NULL && "Start date already occupied, someting went wrong!");
	startDate = std::time(0);
	return true;
}

bool Boss::EndBoss()
{
	assert(startDate == NULL && "Start date is null! cant end boss, something went wrong");
	assert(endDate != NULL && "End date is already occupied, something went wrong!");
	endDate = std::time(0);
	return true;
}




