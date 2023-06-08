#pragma once
#include <memory>

#include "Game.h"

namespace State
{
	inline std::unique_ptr<Game> currentGame = nullptr;
}
