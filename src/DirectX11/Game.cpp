#include "Game.h"
#include <cassert>
#include <string>
#include <vector>

Game::Game(std::string name)
{
	gameName = std::move(name);
}

void Game::SetCurrentBoss(const std::string& name)
{
	for (const auto& boss : bosses)
	{
		if (boss->bossName == name)
		{
			currentBoss = boss;
			return;
		}
	}
}

void Game::EndCurrentBoss()
{
	if (currentBoss == nullptr)
		return;

	currentBoss->EndBoss();
	currentBoss = nullptr;
}

void Game::RemoveBoss(const std::string& name)
{
	if (currentBoss != nullptr && currentBoss->bossName == name)
	{
		currentBoss = nullptr;
	}

	for (int i = 0; i < bosses.size(); i++)
	{
		if (bosses[i]->bossName == name)
		{
			bosses.erase(bosses.begin() + i);
			return;
		}
	}
}

void Game::IncrementDeaths()
{
	if (currentBoss != nullptr)
		currentBoss->deaths++;

	deaths++;
}

bool Game::CheckIfExists(const std::string& name)
{
	if (bosses.empty())
		return false;

	for (auto& boss : bosses)
	{
		if (boss->bossName == name)

			return true;
	}
	return false;
}
