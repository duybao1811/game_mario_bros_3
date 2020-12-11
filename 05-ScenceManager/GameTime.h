#pragma once
#include <d3dx9.h>
class GameTime
{
	DWORD demkhac; // đếm khắc
	int Time;
public:
	GameTime();
	void Update(DWORD dt);
	void SetTime(int t);
	int GetTime();

};

