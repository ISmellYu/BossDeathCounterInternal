#include "Game.h"
#include <cassert>
#include <string>
#include <vector>
#include "SaveHandler.h"

Game::Game(std::string name)
{
	gameName = std::move(name);
}

void Game::SetCurrentBoss(const std::string& name)
{
	if (currentBoss != nullptr)
	{
		if (currentBoss->state == Started)
			PauseCurrentBoss();
	}

	for (const auto& boss : bosses)
	{
		if (boss->bossName == name)
		{
			currentBoss = boss;
			return;
		}
	}

	SaveHandler::Save(this, "save.json");
}

void Game::EndCurrentBoss()
{
	if (currentBoss == nullptr)
		return;

	currentBoss->EndBoss();
	currentBoss = nullptr;
	SaveHandler::Save(this, "save.json");
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
	SaveHandler::Save(this, "save.json");
}

bool Game::AddBoss(std::string name)
{
	for (auto& boss : bosses)
	{
		if (boss->bossName == name)
			return false;
	}

	auto boss = std::make_shared<Boss>(std::move(name));
	bosses.emplace_back(std::move(boss));
	SaveHandler::Save(this, "save.json");
	return true;
}

void Game::IncrementDeaths()
{
	if (currentBoss != nullptr)
	{
		// check if is paused
		if (currentBoss->state == Started)
		{
			currentBoss->deaths++;
		}
	}
	deaths++;
	SaveHandler::Save(this, "save.json");
}

void Game::DecrementDeaths()
{
	if (currentBoss != nullptr && currentBoss->deaths <= 0)
		currentBoss->deaths--;
	if (deaths <= 0)
		return;
	deaths--;
	SaveHandler::Save(this, "save.json");
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

bool Game::PauseCurrentBoss()
{
	if (currentBoss == nullptr)
		return false;
	auto state = currentBoss->PauseBoss();
	SaveHandler::Save(this, "save.json");
	return state;
}

bool Game::ResumeCurrentBoss()
{
	if (currentBoss == nullptr)
		return false;
	auto state = currentBoss->ResumeBoss();
	SaveHandler::Save(this, "save.json");
	return state;
}


