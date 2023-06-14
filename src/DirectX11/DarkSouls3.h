#pragma once


namespace DarkSouls3
{
	using IfnAddDeathCountDS3 = __int64(__fastcall*)(void* rcx_t);
	inline IfnAddDeathCountDS3 oIfnAddDeathCountDS3;

	using fnGetKeyStateDS3 = __int64(__fastcall*)(int key);
	inline fnGetKeyStateDS3 ofnGetKeyStateDS3;
	bool Hook();
}
