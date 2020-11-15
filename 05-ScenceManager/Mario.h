#pragma once
#include "GameObject.h"

#define MARIO_WALKING_SPEED		0.15f 
//0.1f
#define MARIO_JUMP_SPEED_Y		0.35f
#define MARIO_JUMP_DEFLECT_SPEED 0.2f
#define MARIO_JUMP_SPEED_RUNING_MAXSPEED 0.45f
#define MARIO_GRAVITY			0.001f
#define MARIO_DIE_DEFLECT_SPEED	 0.5f
#define MARIO_JUMP_DEFLECT_SPEED_AFTER_COLLISION 0.4f
#define MARIO_WALKING_MAXSPEED 0.1f
#define MARIO_RUNNING_MAXSPEED 0.2f
#define PULL_UP_MARIO_AFTER_SIT 10.0f
#define turn 0.006f                //vân tốc quay đầu
#define a 0.004f                  //gia tốc
#define MARIO_STATE_IDLE			0
#define MARIO_STATE_WALKING_RIGHT	100
#define MARIO_STATE_WALKING_LEFT	200
#define MARIO_STATE_WALKING         101
#define MARIO_STATE_JUMP			300
#define MARIO_STATE_DIE				400
#define MARIO_STATE_SIT             500
#define MARIO_STATE_RUN_MAXSPEED    600
#define MARIO_STATE_RUNNING         700
#define MARIO_STATE_TURN            800
#define MARIO_STATE_RACCOON_ATTACK  900
#define MARIO_STATE_SHOOT_FIRE      901
#define MARIO_STATE_FALLING         301
#define MARIO_STATE_UP              1000
#define MARIO_STATE_FLY             301
#define MARIO_STATE_KICK            1
#define MARIO_STATE_FLY_UP          2
#define MARIO_RACCOON_STATE_FALL_SLOW 3
#define MARIO_STATE_FALL_FLY         4
#define MARIO_ANI_BIG_IDLE_RIGHT		0    //400
#define MARIO_ANI_BIG_IDLE_LEFT			1   //401
#define MARIO_ANI_SMALL_IDLE_RIGHT		2   //402
#define MARIO_ANI_SMALL_IDLE_LEFT			3   //403
#define MARIO_ANI_BIG_WALKING_RIGHT			4   //500
#define MARIO_ANI_BIG_WALKING_LEFT			5   //501
#define MARIO_ANI_SMALL_WALKING_RIGHT		6   //502
#define MARIO_ANI_SMALL_WALKING_LEFT		7   //503
#define MARIO_ANI_DIE				        8   //599
#define MARIO_ANI_BIG_JUMP_RIGHT            9   //504
#define MARIO_ANI_BIG_JUMP_LEFT             10  //505
#define MARIO_ANI_SMALL_JUMP_RIGHT          11  //506
#define MARIO_ANI_SMALL_JUMP_LEFT           12   //507
#define MARIO_ANI_SIT_RIGHT                 13   //508
#define MARIO_ANI_SIT_LEFT                  14   //509
#define MARIO_ANI_BIG_FALL_RIGHT            15   //568
#define MARIO_ANI_BIG_FALL_LEFT             16   //569
#define MARIO_ANI_BIG_TURN_LEFT             17   //514
#define MARIO_ANI_BIG_TURN_RIGHT            18   //515
#define MARIO_ANI_BIG_RUNMAXSPEED_RIGHT     19   //528
#define MARIO_ANI_BIG_RUNMAXSPEED_LEFT      20   //529
#define MARIO_ANI_BIG_RUN_RIGHT             21   //530
#define MARIO_ANI_BIG_RUN_LEFT              22   //531
#define MARIO_ANI_SMALL_TURN_RIGHT          24    //517
#define MARIO_ANI_SMALL_TURN_LEFT           23   //516
#define MARIO_ANI_SMALL_RUN_RIGHT           25   //564
#define MARIO_ANI_SMALL_RUN_LEFT            26
//#define MARIO_ANI_SMALL_RUNMAXSPEED_RIGHT   26
//#define MARIO_ANI_SMALL_RUNMAXSPEED_LEFT    27
#define MARIO_ANI_RACCOON_IDLE_RIGHT        27   //508
#define MARIO_ANI_RACCOON_IDLE_LEFT         28   //509
#define MARIO_ANI_RACCOON_WALKING_RIGHT     29
#define MARIO_ANI_RACCOON_WALKING_LEFT      30
#define MARIO_ANI_RACCOON_JUMP_RIGHT        31
#define MARIO_ANI_RACCOON_JUMP_LEFT         32
#define MARIO_ANI_RACCOON_TURN_RIGHT        34
#define MARIO_ANI_RACCOON_TURN_LEFT         33
#define MARIO_ANI_RACCOON_RUN_RIGHT         35
#define MARIO_ANI_RACCOON_RUN_LEFT          36
#define MARIO_ANI_FIRE_IDLE_RIGHT           37
#define MARIO_ANI_FIRE_IDLE_LEFT            38
#define MARIO_ANI_FIRE_WALKING_RIGHT        39
#define MARIO_ANI_FIRE_WALKING_LEFT         40
#define MARIO_ANI_FIRE_JUMP_RIGHT           41
#define MARIO_ANI_FIRE_JUMP_LEFT            42
#define MARIO_ANI_FIRE_TURN_RIGHT           43
#define MARIO_ANI_FIRE_TURN_LEFT            44
#define MARIO_ANI_FIRE_RUN_RIGHT            45
#define MARIO_ANI_FIRE_RUN_LEFT             46
#define MARIO_ANI_RACCOON_SIT_RIGHT         47
#define MARIO_ANI_RACCOON_SIT_LEFT          48
#define MARIO_ANI_FIRE_SIT_RIGHT            49
#define MARIO_ANI_FIRE_SIT_LEFT             50
#define MARIO_ANI_RACCOON_ATTACK_RIGHT      51
#define MARIO_ANI_RACCOON_ATTACK_LEFT       52
#define MARIO_ANI_BIG_KICK_RIGHT            53
#define MARIO_ANI_BIG_KICK_LEFT             54
#define MARIO_ANI_RACCOON_FALL_RIGHT        55
#define MARIO_ANI_RACCOON_FALL_LEFT         56
#define MARIO_ANI_RACCOON_KICK_RIGHT        57
#define MARIO_ANI_RACCOON_KICK_LEFT         58
#define MARIO_ANI_FIRE_FALL_RIGHT           59
#define MARIO_ANI_FIRE_FALL_LEFT            60
#define MARIO_ANI_FIRE_KICK_RIGHT           61
#define MARIO_ANI_FIRE_KICK_LEFT            62
#define MARIO_ANI_BIG_FLY_RIGHT             63
#define MARIO_ANI_BIG_FLY_LEFT              64
#define MARIO_ANI_FIRE_SHOOT_RIGHT          65
#define MARIO_ANI_FIRE_SHOOT_LEFT           66
#define MARIO_ANI_FALL_SLOW_RIGHT               67
#define MARIO_ANI_FALL_SLOW_LEFT                68
#define MARIO_ANI_RACCOON_RUN_RIGHT         69
#define MARIO_ANI_RACCOON_RUN_LEFT          70
#define MARIO_ANI_RACCOON_RUNMAXSPEED_RIGHT 71
#define MARIO_ANI_RACCOON_RUNMAXSPEED_LEFT  72
#define MARIO_ANI_FIRE_RUNMAXSPEED_RIGHT    73
#define MARIO_ANI_FIRE_RUNMAXSPEED_LEFT     74
#define MARIO_ANI_FIRE_FLY_RIGHT            75
#define MARIO_ANI_FIRE_FLY_LEFT             76
#define MARIO_ANI_RACCOON_FLY_RIGHT         77
#define MARIO_ANI_RACCOON_FLY_LEFT          78
#define MARIO_ANI_RACCOON_FALL_FLY_RIGHT    79
#define MARIO_ANI_RACCOON_FALL_FLY_LEFT     80
#define	MARIO_LEVEL_SMALL	1
#define	MARIO_LEVEL_BIG		2
#define MARIO_LEVEL_RACCOON 3
#define MARIO_LEVEL_FIRE    4
#define MARIO_BIG_BBOX_WIDTH  15
#define MARIO_BIG_BBOX_HEIGHT 27

#define MARIO_SMALL_BBOX_WIDTH  13
#define MARIO_SMALL_BBOX_HEIGHT 15
#define MARIO_SIT_BBOX_HEIGHT   17
#define MARIO_UNTOUCHABLE_TIME 5000
#define MARIO_GRAVITY_JUMPING 0.0001f
#define PULL_UP_MARIO_AFTER_SIT 10.0f
#define MARIO_FALL_SLOW 0.04f
#define MARIO_FLY_SPEED_Y 0.15f
class CMario : public CGameObject
{
	int untouchable;
	DWORD untouchable_start;

	float start_x;			// initial position of Mario at scene
	float start_y; 
	float MarioGravity;
public: 
	Camera* camera;
	DWORD Time;
	DWORD StartFly;
	DWORD Attack;
	DWORD Kick;
	int TimeFly;
	int level;
	void Sit();
	void Jump();
	void Stop();
	void GoRight();
	void GoLeft();
	void Falling();
	bool isJumping;
	bool isOnGround;
	bool isWalking;
	bool isFalling;
	bool isSitting;
	bool isDie;
	bool isFlying;
	bool isWalkingR;
	bool isWalkingL;
	bool isRunning;
	bool isFlyup;
	bool isFallSlow;
	bool isAttack;
	bool isKick;
	bool isFallFly;
	bool isOnAir;
	bool isReadyToFly;
	void ResetSit();
	vector<LPGAMEOBJECT> ListFire;
	CMario(float x = 0.0f, float y = 0.0f);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT> *colliable_objects = NULL);
	virtual void Render();

	void SetState(int state);
	void SetLevel(int l) { level = l; }
	void StartUntouchable() { untouchable = 1; untouchable_start = GetTickCount(); }

	void Reset();

	virtual void GetBoundingBox(float &left, float &top, float &right, float &bottom);
};