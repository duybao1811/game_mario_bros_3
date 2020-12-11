#include "GameTime.h"
GameTime::GameTime()
{
	this->demkhac = 0;
	SetTime(0);
}
void GameTime::Update(DWORD dt)
{
	if (demkhac + dt < 1000)  // khắc < 1s
	{
		demkhac += dt;
	}
	else
	{
		demkhac = (demkhac + dt) % 1000;
		Time++;
	}
}
void GameTime::SetTime(int t)
{
	Time = t;
}
int GameTime::GetTime()
{
	return Time;
}