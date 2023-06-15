#include "SaveHandler.h"

#include <fstream>

#include <json/json.h>


Json::Value getPauseList(Boss* boss)
{
	auto& pauses = boss->pauses;

	Json::Value pauseList(Json::arrayValue);

	Json::Value pauseJson(Json::objectValue);

	for (auto& pause : pauses)
	{
		pauseJson["start"] = pause->start;
		pauseJson["end"] = pause->end;
		pauseJson["duration"] = pause->duration;
		pauseList.append(pauseJson);
	}

	return pauseList;
}

Json::Value getBossesList(Game* game)
{
	auto& bosses = game->bosses;
	Json::Value bossList(Json::arrayValue);

	Json::Value bossJson(Json::objectValue);

	for (auto& boss : bosses)
	{
		bossJson["bossName"] = boss->bossName;
		bossJson["deaths"] = boss->deaths;
		bossJson["start"] = boss->startDate;
		bossJson["end"] = boss->endDate;
		bossJson["state"] = boss->state;
		bossJson["pauses"] = getPauseList(boss.get());
		bossList.append(bossJson);
	}
	return bossList;
}

std::vector<std::shared_ptr<Pause>> getPauseListFromJson(Json::Value pausesJson)
{
	std::vector<std::shared_ptr<Pause>> pauseList = {};

	for (auto& pauseJson : pausesJson)
	{
		auto pause = std::make_shared<Pause>();
		pause->start = pauseJson["start"].asInt64();
		pause->end = pauseJson["end"].asInt64();
		pause->duration = pauseJson["duration"].asDouble();
		pauseList.push_back(std::move(pause));
	}
	return pauseList;
}

std::vector<std::shared_ptr<Boss>> getBossListFromJson(Json::Value bossesJson)
{
	std::vector<std::shared_ptr<Boss>> bossList = {};

	for (auto& bossJson : bossesJson)
	{
		std::string bossName = bossJson["bossName"].asString();
		auto boss = std::make_shared<Boss>(bossName);
		boss->state = (BossState)bossJson["state"].asInt();
		boss->startDate = bossJson["start"].asInt64();
		boss->endDate = bossJson["end"].asInt64();
		boss->deaths = bossJson["deaths"].asInt();
		boss->pauses = getPauseListFromJson(bossJson["pauses"]);
		bossList.push_back(std::move(boss));
	}
	return bossList;
}

void SaveHandler::Save(Game* game, const char* filename)
{
	if (game == nullptr)
		return;

	Json::Value root;
	

	root["gameName"] = game->gameName;
	root["deaths"] = game->deaths;
	if (game->currentBoss == nullptr)
	{
		root["currentBoss"] = Json::Value::null;
	}
	else
	{
		root["currentBoss"] = game->currentBoss->bossName;
	}
	
	root["bosses"] = getBossesList(game);

	Json::StreamWriterBuilder writerBuilder;
	std::string jsonString = writeString(writerBuilder, root);

	std::ofstream file(filename);
	file << jsonString;
	file.close();
}

std::unique_ptr<Game> SaveHandler::Load(const char* filename)
{
	std::ifstream f(filename);
	Json::Value js;
	f >> js;
	std::string gameName = js["gameName"].asCString();
	auto game = std::make_unique<Game>(gameName);
	game->deaths = js["deaths"].asInt();
	game->bosses = getBossListFromJson(js["bosses"]);


	// check if current boss is present
	if (js["currentBoss"] == Json::Value::null)
	{
		game->currentBoss = nullptr;
	}
	else
	{
		std::string currentBossName = js["currentBoss"].asString();

		for (auto& boss : game->bosses)
		{
			if (boss->bossName == currentBossName)
			{
				game->currentBoss = boss;
				break;
			}
		}
	}

	return std::move(game);
}

std::string SaveHandler::ReadFileIntoString(const std::string& path)
{
	std::ifstream inputFile(path);
	return std::string{(std::istreambuf_iterator<char>(inputFile)), std::istreambuf_iterator<char>()};
}

