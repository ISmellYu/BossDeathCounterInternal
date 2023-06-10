#pragma once
#include "cstdint"
namespace Offsets
{
	namespace DS3
	{
		static uintptr_t oSaveInfo = 0x47572B8;

		namespace SaveInfo
		{
			static uintptr_t deaths = 0x98;
		}

		namespace Functions
		{
			static uintptr_t AddDeathCount = 0x47CEF0;
		}
		
	}
}
