#pragma once
struct BossTimeStamp
{
	int time_s = 0;

	int GetHours()
	{
		return time_s / 3600;
	}

	int GetMinutes()
	{
		int hours = GetHours();
		return (time_s - hours * 3600) / 60;
	}

	int GetSeconds()
	{
		int minutes = GetMinutes();
		int hours = GetHours();
		return time_s - (minutes * 60) - (hours * 3600);
	}
};