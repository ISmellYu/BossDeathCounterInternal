#pragma once


namespace DarkSouls3
{
	using IfnAddDeathCountDS3 = __int64(__fastcall*)(void* rcx_t);
	inline IfnAddDeathCountDS3 oIfnAddDeathCountDS3;
	bool Hook();
}
