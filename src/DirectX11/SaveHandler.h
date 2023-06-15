#pragma once
#include <memory>

#include "Game.h"


namespace SaveHandler
{
	void Save(Game* game, const char* filename);
	std::unique_ptr<Game> Load(const char* filename);
	std::string ReadFileIntoString(const std::string& path);
}
