#pragma once
#include "Boss.h"
#include <vector>
#include <memory>


class Game
{
public:
	std::string gameName;
	std::vector<std::shared_ptr<Boss>> bosses{};
	int deaths{};

	std::shared_ptr<Boss> currentBoss = nullptr;

	explicit Game(std::string name);


	void SetCurrentBoss(const std::string& name);
	void EndCurrentBoss();

	bool AddBoss(const std::string name);
	void RemoveBoss(const std::string& name);

	void IncrementDeaths();

	bool CheckIfExists(const std::string& name);

	bool PauseCurrentBoss();
	bool ResumeCurrentBoss();


};
