#pragma once
#include "Boss.h"
#include <vector>

class Game
{
public:
	std::string gameName;
	std::vector<Boss*> bosses;

	Boss* currentBoss = NULL;

	explicit Game(std::string name);
};
