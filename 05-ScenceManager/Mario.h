#pragma once
#include "GameObject.h"
#include "Item.h"
#include "define.h"
#include "Coin.h"
#include "Tail.h"
#include "Camera.h"
#define MARIO_WALKING_SPEED		0.00015f 
#define MARIO_RUNNING_SPEED     0.00015f
#define MARIO_DECELERATE_SPEED 0.0005f
#define MARIO_JUMP_SLOW_SPEED_Y 0.2f
#define MARIO_JUMP_SPEED_Y		0.37f
#define MARIO_JUMP_DEFLECT_SPEED 0.2f
#define MARIO_JUMP_SPEED_RUNNING_MAXSPEED 0.45f
#define MARIO_GRAVITY			0.001f
#define MARIO_DIE_DEFLECT_SPEED	 0.5f
#define MARIO_JUMP_DEFLECT_SPEED_AFTER_COLLISION 0.3f
#define MARIO_WALKING_MAXSPEED 0.12f
#define MARIO_RUNNING_MAXSPEED 0.2f
#define PULL_UP_MARIO_AFTER_SIT 10.0f
#define MARIO_STATE_IDLE			0
#define MARIO_STATE_WALKING_RIGHT	100
#define MARIO_STATE_WALKING_LEFT	200
#define MARIO_STATE_DIE				400
#define MARIO_STATE_SIT             500
#define MARIO_STATE_RUN_MAXSPEED    600
#define MARIO_STATE_RUN_LEFT        700
#define MARIO_STATE_RUN_RIGHT       750
#define MARIO_STATE_TURN            800
#define MARIO_STATE_RACCOON_ATTACK  900
#define MARIO_STATE_SHOOT_FIRE      901
#define MARIO_STATE_FLY             300
#define MARIO_STATE_JUMP            350
#define MARIO_STATE_FLY_UP          450
#define MARIO_STATE_FALLING         650
#define MARIO_RACCOON_STATE_FALL_SLOW 550
#define MARIO_STATE_WORLDMAP        850
#define MARIO_STATE_KICK            1
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
#define MARIO_ANI_RACCOON_FLY_UP_RIGHT 81
#define MARIO_ANI_RACCOON_FLY_UP_LEFT 82
#define MARIO_RACCOON_ANI_HOLD_WALK_RIGHT 83
#define MARIO_RACCOON_ANI_HOLD_WALK_LEFT 84
#define MARIO_RACCOON_ANI_HOLD_JUMP_RIGHT 85
#define MARIO_RACCOON_ANI_HOLD_JUMP_LEFT 86
#define MARIO_RACCOON_ANI_HOLD_IDLE_RIGHT 87
#define MARIO_RACCOON_ANI_HOLD_IDLE_LEFT 88
#define MARIO_RACCOON_ANI_HOLD_RUN_MAX_RIGHT 89
#define MARIO_RACCOON_ANI_HOLD_RUN_MAX_LEFT 90
#define MARIO_SMALL_ANI_HOLD_WALK_RIGHT 91
#define MARIO_SMALL_ANI_HOLD_WALK_LEFT 92
#define MARIO_SMALL_ANI_HOLD_JUMP_RIGHT 93
#define MARIO_SMALL_ANI_HOLD_JUMP_LEFT 94
#define MARIO_SMALL_ANI_HOLD_IDLE_RIGHT 95
#define MARIO_SMALL_ANI_HOLD_IDLE_LEFT 96
#define MARIO_SMALL_ANI_HOLD_RUN_MAX_RIGHT 97
#define MARIO_SMALL_ANI_HOLD_RUN_MAX_LEFT 98
#define MARIO_BIG_ANI_HOLD_WALK_RIGHT 99
#define MARIO_BIG_ANI_HOLD_WALK_LEFT 100
#define MARIO_BIG_ANI_HOLD_JUMP_RIGHT 101
#define MARIO_BIG_ANI_HOLD_JUMP_LEFT 102
#define MARIO_BIG_ANI_HOLD_IDLE_RIGHT 103
#define MARIO_BIG_ANI_HOLD_IDLE_LEFT 104
#define MARIO_BIG_ANI_HOLD_RUN_MAX_RIGHT 105
#define MARIO_BIG_ANI_HOLD_RUN_MAX_LEFT 106
#define MARIO_FIRE_ANI_HOLD_WALK_RIGHT 107
#define MARIO_FIRE_ANI_HOLD_WALK_LEFT 108
#define MARIO_FIRE_ANI_HOLD_JUMP_RIGHT 109
#define MARIO_FIRE_ANI_HOLD_JUMP_LEFT 110
#define MARIO_FIRE_ANI_HOLD_IDLE_RIGHT 111
#define MARIO_FIRE_ANI_HOLD_IDLE_LEFT 112
#define MARIO_FIRE_ANI_HOLD_RUN_MAX_RIGHT 113
#define MARIO_FIRE_ANI_HOLD_RUN_MAX_LEFT 114
#define MARIO_SMALL_INTRO 115
#define MARIO_BIG_INTRO 116
#define MARIO_RACCOON_INTRO 117
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
#define MARIO_FLY_SPEED_Y 0.01f
#define TIME_CAN_FLY 5000
#define MARIO_DEFAULT_SCORE 0
#define MARIO_DEFAULT_COIN 0
#define MARIO_DEFAULT_LIVE 4
#define MARIO_RACCOON_STATE_FLY_UP 1000

class CMario : public CGameObject
{

	float start_x;			// initial position of Mario at scene
	float start_y;
	float MarioGravity;
	int CoinCollect;
	int score;
	int Live;
	CCoin* coin;
	Tail* tail;
	int ani = -1;
	Camera* camera;
public:
	DWORD TimeFly;
	DWORD now;
	int level;
	bool isWalkingR, isWalkingL;
	bool isJumping;
	bool isOnGround;
	bool isFalling;
	bool isSitting;
	bool isDie;
	bool isFlying;
	bool isRunning;
	bool isFallSlow;
	bool isAttack;
	bool isKick;
	bool isFallFly;
	bool isTrampleEnemy;
	float last_vy;
	bool isDeadth;
	bool isFlyup;
	bool isReadytoFly;
	bool isHoldTurtle;
	bool isOnAir;
public:
	bool untouchable;
	DWORD untouchable_start;
	DWORD Time;
	DWORD StartFly;
	DWORD TimeAttack;
	DWORD TimeKick;

	void ResetSit();
	vector<LPGAMEOBJECT> ListFire;
	vector<LPGAMEOBJECT> ListEffect;
	vector<LPGAMEOBJECT> ListTail;
	CMario(float x, float y, Camera *camera);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* colliable_objects = NULL);
	virtual void Render();
	
	void SetState(int state);
	void SetLevel(int l);
	void StartUntouchable() { untouchable = 1; untouchable_start = GetTickCount(); }
	

	void Decelerate();
	void Sit();
	void Fly();
	void WalkRight();
	void WalkLeft();
	void JumpSlow();
	void JumpHight();
	void FallSlow();

	int GetAni() { return ani; }
	bool CheckCollision(CGameObject* obj);
	void SetHurt(LPCOLLISIONEVENT e);
	void Reset();
	int GetLevel();
	bool isCollisionWithItem(Item* objItem);
	void ShootFire();
	void TailAttack();
	void SetCoinCollect(int c);
	int GetCoinCollect();
	void SetScore(int s);
	int GetScore();
	
	bool GetIsDeadth();
	void SetIsDeadth(bool b);
	void SetLive(int l);
	int GetLive();
	void Init();
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
};