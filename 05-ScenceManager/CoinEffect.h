#pragma once
#include "Effect.h"
#include "PointEffect.h"
#define COIN_EFFECT_ANI 0
#define ID_LOAD_COIN_EFFECT 16
#define TIME_EFFECT 300
#define COIN_EFFECT_SPEED_Y 0.14f
#define SET_EFFECT_IN_BRICK_X 4
#define SET_EFFECT_IN_BRICK_Y 14
#define COIN_EFFECT_MIN_Y 65
#define COIN_EFFECT_MAX_Y 20

class CoinEffect : public Effect
{
private:
	float minY;
	float maxY;
	bool isFalling;
	vector<LPGAMEOBJECT> ListPointEffect;
	bool effectPoint;
	bool isDisappear;
	PointEffect* effect;

public:
	CoinEffect(float X, float Y);
	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	void Render();
};

