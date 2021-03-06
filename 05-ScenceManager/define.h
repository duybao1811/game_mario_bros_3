﻿#pragma once
#include <Windows.h>
#include <iostream>
#include <d3d9.h>
#include <d3dx9.h>
#include <iostream>
#include <algorithm>
#include <stdio.h>
#include <iomanip>
using namespace std;
#define GAME_TIME_LIMIT 300 // thoi gian dem nguoc khi ket thuc 1 man 
#define SCREEN_WIDTH 278
#define SCREEN_HEIGHT 280
#define TexID_BOARD 25
enum Type {
	// MARIO 
	MARIO = 00,
	MARIO_SMALL = 01,
	MARIO_BIG = 02,
	MARIO_RACCOON = 03,
	MARIO_FIRE = 04,

	//OBJECT nền 
	PLATFORM = 21,
	QUESTION_BRICK = 22,
	PORTAL = 23,
	PORAL_SWITCH = 24,
	BLOCK_COLOR = 25,
	ESHORT_PIPE = 26,
	EMEDIUM_PIPE = 27,
	GOLD_BRICK = 28,
	WOOD = 29,

	//ENEMY 
	GOOMBA = 30,
	GOOMBA_JUMP = 31,
	KOOPAS = 32,
	KOOPAS_JUMP = 33,
	FIRE_PIRANHA = 34,
	PIRANHA_GREEN = 35,
	FIRE_ENEMY = 36,
	BOOMERANG = 37,
	BOOM_BROTHER = 38,
	//WEAPON
	FIRE = 40,


	//ITEM
	MUSHROOM_POWER = 50,
	MUSHROOM_1_UP = 51,
	LEAF = 52,
	COIN = 53,
	P_SWITCH=54,
	BOX=55,
	FLOWER = 56,
	//EFFECT
	COIN_EFFECT = 1,

	// intro

	FLOOR = 60,
	LOGO = 99,
	GROUND_INTRO = 100,
	CURSOR = 101,
};
enum ObjectType
{
	MISC =1,
	ENEMY=2,
	ITEM=3,
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
	RIGHT_BOTTOM_SIDE_FAR = 8,
	LEFT_SIDE = 9,
	RIGHT_SIDE = 10
};


#define FPLANT_BBOX_WIDTH 16
#define FPLANT_RED_BBOX_HEIGHT 32
#define FPLANT_GREEN_BBOX_HEIGHT 24
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
#define DISTANCE_SAFE_ZONE 25


//############################# MARIO #####################


//############################# PLANT ###############################

//                          QUESTION BRICK
#define QB_BBOX_WIDTH 16
#define QB_BBOX_HEIGHT 16
#define QB_ANI 0
#define QB_ANI_EMPTY 1
#define	QB_MODEL_1UP_ANI 2
#define QUESTION_BRICK_MODEL_COIN 1
#define QUESTION_BRICK_MODEL_POWER_UP 2
#define QB_MINY 10
#define QB_SPEED_DOWN 0.1f
#define QB_SPEED_UP 0.1f
#define QB_STATE_UNBOX 100
#define QB_STATE_EMPTY 200


#define GB_BBOX_WIDTH 16
#define GB_BBOX_HEIGHT 16
#define GB_ANI 0
#define GB_ANI_UNBOX 1
#define IDLE_COIN_ANI 2
#define GB_CONTAIN_COIN 1
#define GB_CONTAIN_PSWITCH 2
#define GB_CONTAIN_MUSHROOM_1_UP 3
#define GB_MODEL_MANY_COIN 4
#define GB_CONTAIN_POWER_UP 5
#define GB_STATE_EMPTY 100
#define GB_STATE_TRANFORM 200
#define GB_STATE_BACK 300
#define TIME_BACK 8000
// Enemy
//GOOMBA 
#define GOOMBA_WALKING_SPEED 0.025f
#define GOOMBA_RED_WALKING_SPEED 0.02f
#define GOOMBA_BBOX_WIDTH 16
#define GOOMBA_BBOX_HEIGHT 16
#define GOOMBA_RED_BBOX_WIDTH 20
#define GOOMBA_RED_BBOX_HEIGHT 21 
#define GOOMBA_BBOX_HEIGHT_DIE 9
#define GOOMBA_JUMP_SPEED_Y 0.3f
#define GOOMBA_STATE_WALKING 100
#define GOOMBA_STATE_DIE 200
#define GOOMBA_STATE_ATTACKED 300
#define GOOMBA_STATE_JUMP 400
#define GOOMBA_RED_PARA_STATE_JUMP_SLOW 500
#define GOOMBA_RED_PARA_STATE_FALLING 600
#define GOOMBA_RED_STATE_WALKING 700
#define GOOMBA_RED_PARA_STATE_DIE 800
#define GOOMBA_ANI_WALKING 0
#define GOOMBA_ANI_DIE 1
#define GOOMBA_ANI_ATTACKED 2
#define GOOMBA_RED_ANI_WALKING 3
#define GOOMBA_RED_PARA_ANI_DIE 4
#define GOOMBA_RED_PARA_ANI_WALKING 5
#define GOOMBA_RED_PARA_ANI_JUMP 6
#define GOOMBA_RED_PARA_ANI_FALL 7
#define GOOMBA_RED_PARA_ANI_ATTACKED 8
#define GOOMBA_ATTACKED_SPEED_X 0.1f
#define GOOMBA_ATTACKED_SPEED_Y 0.35f
#define GOOMBA_BASE 1
#define GOOMBA_RED_PARA 2
#define GOOMBA_GRAVITY 0.001f
#define ENEMY_ATTACKED 10 // trạng thái của enemy khi bị bắn lửa, bị quẩy đuôi và bị đá rùa tấn công
#define ENEMY_STATE_FIRE_ATTACK 20
// ################### TAIL
#define TAIL_BBOX_WIDTH 10
#define TAIL_BBOX_HEIGHT 4
//##########################	EFFECT 

//################### EFFECT TAIL HIT
#define ID_LOAD_TAIL_HIT_EFFECT 19
#define ANI_TAIL_HIT_EFFECT 0
#define PULL_UP_EFFECT_TAIL_HIT 10
#define EFFECT_TAIL_HIT_RIGHT_SIDE 10
#define EFFECT_TAIL_HIT_LEFT_SIDE 15

#define BOX_ANI_RANDOM 0

#define GAMEOVER_SELECT_CONTINUE 0
#define GAME_OVER_SELECT_END 1
#define WORLD_MAP 1
#define WORLD_1_1 2
#define WORLD_1_1_1 3
#define WORLD_1_4 4
#define WORLD_1_4_1 5
/*#ifndef CHECK_OBJECT_IN_CAMERA // Kiểm tra GameObject có nằm trong camera ko?
#define CHECK_OBJECT_IN_CAMERA(obj) \
checkObjectInCamera(obj->GetX(), obj->GetY(),16.0f, 16.0f)
#endif*/



