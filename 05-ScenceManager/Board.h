#pragma once
#include "GameObject.h"
#include "Mario.h"
#include "Font.h"
#include "Textures.h"
#include "Power.h"
#include "FullPower.h"
#define ANI_BOARD 0
#define DISTANCE_FROM_BOTTOM_CAM_TO_TOP_BOARD 86.0f
#define TexID_Board 35
class Board
{
	float x, y;
	Power power;
	Font font;
	FullPower fpower;
	static Font* __instance;
	LPDIRECT3DTEXTURE9 Tex;
	LPSPRITE Sprite;
	DWORD timeDraw;
	DWORD now;
public:
	Board(float X,float Y);
	void GetBoundingBox(float& left, float& top, float& right, float& bottom) {}
	void Render(CMario* mario, int RemainingTime);
	string FillNumber(string s, UINT amoumt_of_number);
	~Board(){}
};