#pragma once
#include <Windows.h>
#include <iostream>
#include <d3d9.h>
#include <d3dx9.h>
#include <iostream>
#include <algorithm>
#include <stdio.h>
#include <iomanip>
using namespace std;
#define SCREEN_WIDTH 528
#define SCREEN_HEIGHT 480
#define GAME_TIME_LIMIT  300 // thoi gian dem nguoc khi ket thuc 1 man 
#define SCREEN_WIDTH 320
#define SCREEN_HEIGHT 240
enum Type {
	// MARIO 
	MARIO_SMALL = 01,
	MARIO_BIG = 02,
	MARIO_RACCOON = 03,
	MARIO_FIRE = 04,

	//OBJECT nền 
	BRICK = 21,
	QUESTION_BRICK = 22,
	PORTAL = 23,
	PORAL_SWITCH = 24,
	BLOCK_COLOR = 25,
	ESHORT_PIPE =26,
	EMEDIUM_PIPE=27,

	//ENEMY 
	GOOMBA = 30,
	GOOMBA_JUMP = 31,
	KOOPAS = 32,
	KOOPAS_JUMP = 33,
	FIRE_PIRANHA_RED = 34,
	PIRANHA_GREEN = 35,
	FIRE_ENEMY=36,
	//WEAPON
	FIRE = 40,

	//ITEM
	MUSHROOM_POWER = 50,
	MUSHROOM_1_UP =51,
	LEAF = 52,
	COIN = 53

};
enum Range
{
	LEFT_TOP_SIDE_NEAR = 1,
	LEFT_TOP_SIDE_FAR=2,
	LEFT_BOTTOM_SIDE_NEAR = 3,
	LEFT_BOTTOM_SIDE_FAR = 4,
	RIGHT_TOP_SIDE_NEAR = 5,
	RIGHT_TOP_SIDE_FAR = 6,
	RIGHT_BOTTOM_SIDE_NEAR = 7,
	RIGHT_BOTTOM_SIDE_FAR = 8	
};


#define FPLANT_BBOX_WIDTH 16
#define FPLANT_BBOX_HEIGHT 32
#define	FPLANT_ANI_LEFT_TOP 0
#define FPLANT_ANI_LEFT_BOTTOM 1
#define FPLANT_ANI_RIGHT_TOP 2
#define FPLANT_ANI_RIGHT_BOTTOM 3
#define FPLANT_ANI_ATTACK_LEFT_TOP 4
#define FPLANT_ANI_ATTACK_LEFT_BOTTOM 5
#define FPLANT_ANI_ATTACK_RIGHT_TOP 6
#define FPLANT_ANI_ATTACK_RIGHT_BOTTOM 7
#define PLANT_STATE_GROW_UP 100
#define PLANT_STATE_HIDDING 200
#define PLANT_STATE_ATTACK    300
#define PLANT_SPEED_GROW_UP 0.002f
#define PLANT_SPEED_HIDDING 0.002f
#define TIME_DELAY_GROW_UP 1500
#define TIME_ATTACK_DELAY 1000
#define TIME_ATTACK 1500
#define DISTANCE_FROM_MARIO_TO_PLANT 75
#define DISTANCE_SAFE_ZONE 24


//############################# MARIO #####################


//############################# PLANT ###############################



#define GAMEOVER_SELECT_CONTINUE 0
#define GAME_OVER_SELECT_END 1

/*#ifndef CHECK_OBJECT_IN_CAMERA // Kiểm tra GameObject có nằm trong camera ko?
#define CHECK_OBJECT_IN_CAMERA(obj) \
checkObjectInCamera(obj->GetX(), obj->GetY(),16.0f, 16.0f)
#endif*/



