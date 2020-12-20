#pragma once
#include "GameObject.h"
#include "Sprites.h"
#include "Game.h"
#include "Textures.h"
#include "define.h"
#define POWER_ANI 0
class Power
{
	LPSPRITE Sprite;
public:
	LPDIRECT3DTEXTURE9 Tex;
	void Draw(float x, float y);
	Power();
};

