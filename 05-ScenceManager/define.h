#pragma once
#include <Windows.h>
#include <iostream>
#include <d3d9.h>
#include <d3dx9.h>
#include <iostream>
#include <algorithm>
#include <stdio.h>
#include <iomanip>
#include "Camera.h"
using namespace std;
#define SCREEN_WIDTH 528
#define SCREEN_HEIGHT 480
#define GAME_TIME_LIMIT  300 // thoi gian dem nguoc khi ket thuc 1 man 
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
	
	//ENEMY 
	GOOMBA = 30,
	GOOMBA_JUMP = 31, 
	KOOPAS = 32,
	KOOPAS_JUMP = 33,
	
	//WEAPON
	FIRE = 40,

	//ITEM
	MUSHROOM = 50,
	MUSHROOM_BLUE =51,
	LEAF = 52

};
#define GAMEOVER_SELECT_CONTINUE 0
#define GAME_OVER_SELECT_END 1

#ifndef CHECK_OBJECT_IN_CAMERA // Kiểm tra GameObject có nằm trong camera ko?
#define CHECK_OBJECT_IN_CAMERA(obj) \
checkObjectInCamera(obj->GetX(), obj->GetY(), (float)obj->GetWidth(),  (float)obj->GetHeight())
#endif



