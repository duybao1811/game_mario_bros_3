#pragma once
#include "GameObject.h"
#include "Sprites.h"
#include "Game.h"
#include "Textures.h"
#define FULL_POWER_ANI 0
#define ID_LOAD_FULL_POWER 25
class FullPower
{
	LPSPRITE Sprite;
public:
	LPDIRECT3DTEXTURE9 Tex;
	void Draw(float x, float y);
	FullPower();
};

