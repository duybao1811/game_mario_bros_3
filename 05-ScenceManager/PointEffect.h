#pragma once
#include "Effect.h"
#define POINT_TYPE_ONE_HUNDRED 1 //100 điểm
#define POINT_TYPE_TWO_HUNDRED 2	//200 điểm 
#define	POINT_TYPE_FOUR_HUNDRED 3	//400 điểm 
#define POINT_TYPE_EIGHT_HUNDRED 4   //800 điểm 
#define	POINT_TYPE_ONE_THOUSAND 5   // 1000 điểm
#define	POINT_TYPE_TWO_THOUSAND 6	//2000 điểm 
#define	POINT_TYPE_FOUR_THOUSAND 7	//4000 điểm 
#define	POINT_TYPE_EIGHT_THOUSAND 8	//8000 điểm
#define POINT_EFFECT_SPEED_Y 0.05f
#define POINT_EFFECT_MIN_Y 25
#define ID_LOAD_POINT_EFFECT 17
#define POINT_TYPE_ONE_HUNDRED_ANI 0 //100 điểm
#define POINT_TYPE_TWO_HUNDRED_ANI 1	//200 điểm 
#define	POINT_TYPE_FOUR_HUNDRED_ANI 2	//400 điểm 
#define POINT_TYPE_EIGHT_HUNDRED_ANI 3  //800 điểm 
#define	POINT_TYPE_ONE_THOUSAND_ANI 4   // 1000 điểm
#define	POINT_TYPE_TWO_THOUSAND_ANI 5	//2000 điểm 
#define	POINT_TYPE_FOUR_THOUSAND_ANI 6	//4000 điểm 
#define	POINT_TYPE_EIGHT_THOUSAND_ANI 7	//8000 điểm
class PointEffect : public Effect
{
public:
	int model;
	float minY;
	PointEffect(float X, float Y,int Model);
	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	void Render();
	int GetModel();
};

