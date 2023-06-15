#pragma once


namespace DarkSouls3
{
	using IfnAddDeathCountDS3 = __int64(__fastcall*)(void* rcx_t);
	inline IfnAddDeathCountDS3 oIfnAddDeathCountDS3;

	using fnGetKeyStateDS3 = __int64(__fastcall*)(int key);
	inline fnGetKeyStateDS3 ofnGetKeyStateDS3;

	using fnSaveRequestDS3 = __int64(__fastcall*)();
	inline fnSaveRequestDS3 ofnSaveRequestDS3;

	using fnReportBossAreaDS3 = void(__fastcall*)(__int64 a1, __int64 a2, __int64 a3);
	inline fnReportBossAreaDS3 ofnReportBossAreaDS3;

	bool Hook();
}
