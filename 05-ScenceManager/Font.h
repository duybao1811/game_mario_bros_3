#pragma once
#include "GameObject.h"
#include "Sprites.h"
#include "Game.h"
#include "Textures.h"
#define TexID_Font 45
#define TILE_FONT_WIDTH 8
#define TILE_FONT_A_HEIGHT 9
#define TILE_FONT_NUMBER_HEIGHT 7
class Font
{
public:
    LPDIRECT3DTEXTURE9 Tex;
	void Draw(float x,float y,const string &s);
	Font();

};


