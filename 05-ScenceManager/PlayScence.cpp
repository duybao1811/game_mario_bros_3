#include <iostream>
#include <fstream>

#include "PlayScence.h"
#include "Utils.h"
#include "Textures.h"
#include "Sprites.h"
#include "Portal.h"
#include "Mario.h"
#include "Camera.h"
#include "Map.h"
#include "Scence.h"
#include <string>
#include <cstring>
#include "Fire.h"
using namespace std;

CPlayScene::CPlayScene(int id, LPCWSTR filePath):
	CScene(id, filePath)
{
	key_handler = new CPlayScenceKeyHandler(this);
}

/*
	Load scene resources from scene file (textures, sprites, animations and objects)
	See scene1.txt, scene2.txt for detail format specification
*/

#define SCENE_SECTION_UNKNOWN -1
#define SCENE_SECTION_TEXTURES 2
#define SCENE_SECTION_SPRITES 3
#define SCENE_SECTION_ANIMATIONS 4
#define SCENE_SECTION_ANIMATION_SETS	5
#define SCENE_SECTION_OBJECTS	6
#define SCENE_SECTION_DRAWMAP 7
#define OBJECT_TYPE_MARIO	0
#define OBJECT_TYPE_BRICK	1
#define OBJECT_TYPE_GOOMBA	2
#define OBJECT_TYPE_KOOPAS	3
#define OBJECT_TYPE_FIRE     4
#define OBJECT_TYPE_PORTAL	50

#define MAX_SCENE_LINE 1024


void CPlayScene::_ParseSection_TEXTURES(string line)
{
	vector<string> tokens = split(line);

	if (tokens.size() < 5) return; // skip invalid lines

	int texID = atoi(tokens[0].c_str());
	wstring path = ToWSTR(tokens[1]);
	int R = atoi(tokens[2].c_str());
	int G = atoi(tokens[3].c_str());
	int B = atoi(tokens[4].c_str());

	CTextures::GetInstance()->Add(texID, path.c_str(), D3DCOLOR_XRGB(R, G, B));
}
void CPlayScene::_ParseSection_SPRITES(string line)
{
	vector<string> tokens = split(line);

	if (tokens.size() < 6) return; // skip invalid lines

	int ID = atoi(tokens[0].c_str());
	int l = atoi(tokens[1].c_str());
	int t = atoi(tokens[2].c_str());
	int r = atoi(tokens[3].c_str());
	int b = atoi(tokens[4].c_str());
	int texID = atoi(tokens[5].c_str());

	LPDIRECT3DTEXTURE9 tex = CTextures::GetInstance()->Get(texID);
	if (tex == NULL)
	{
		DebugOut(L"[ERROR] Texture ID %d not found!\n", texID);
		return; 
	}

	CSprites::GetInstance()->Add(ID, l, t, r, b, tex);
}

void CPlayScene::_ParseSection_ANIMATIONS(string line)
{
	vector<string> tokens = split(line);

	if (tokens.size() < 3) return; // skip invalid lines - an animation must at least has 1 frame and 1 frame time

	//DebugOut(L"--> %s\n",ToWSTR(line).c_str());

	LPANIMATION ani = new CAnimation();

	int ani_id = atoi(tokens[0].c_str());
	for (int i = 1; i < tokens.size(); i += 2)	// why i+=2 ?  sprite_id | frame_time  
	{
		int sprite_id = atoi(tokens[i].c_str());
		int frame_time = atoi(tokens[i+1].c_str());
		ani->Add(sprite_id, frame_time);
	}

	CAnimations::GetInstance()->Add(ani_id, ani);
}

void CPlayScene::_ParseSection_ANIMATION_SETS(string line)
{
	vector<string> tokens = split(line);

	if (tokens.size() < 2) return; // skip invalid lines - an animation set must at least id and one animation id

	int ani_set_id = atoi(tokens[0].c_str());

	LPANIMATION_SET s = new CAnimationSet();

	CAnimations *animations = CAnimations::GetInstance();

	for (int i = 1; i < tokens.size(); i++)
	{
		int ani_id = atoi(tokens[i].c_str());
		
		LPANIMATION ani = animations->Get(ani_id);
		s->push_back(ani);
	}

	CAnimationSets::GetInstance()->Add(ani_set_id, s);
}

/*
	Parse a line in section [OBJECTS] 
*/
void CPlayScene::_ParseSection_OBJECTS(string line)
{
	vector<string> tokens = split(line);
	//DebugOut(L"--> %s\n",ToWSTR(line).c_str());

	if (tokens.size() < 3) return; // skip invalid lines - an object set must have at least id, x, y

	int object_type = atoi(tokens[0].c_str());
	float x = atof(tokens[1].c_str());
	float y = atof(tokens[2].c_str());

	int ani_set_id = atoi(tokens[3].c_str());

	CAnimationSets * animation_sets = CAnimationSets::GetInstance();

	CGameObject *obj = NULL;

	switch (object_type)
	{
	case OBJECT_TYPE_MARIO:
		if (player!=NULL) 
		{
			DebugOut(L"[ERROR] MARIO object was created before!\n");
			return;
		}
		obj = new CMario(x,y); 
		player = (CMario*)obj;  

		DebugOut(L"[INFO] Player object created!\n");
		break;
	case OBJECT_TYPE_GOOMBA: obj = new CGoomba(); break;
	case OBJECT_TYPE_BRICK: {
		float w = atof(tokens[4].c_str());
		float h = atof(tokens[5].c_str());
		obj = new CBrick(w,h); 
		break;
	}
	case OBJECT_TYPE_KOOPAS: obj = new CKoopas(); break;
	case OBJECT_TYPE_PORTAL:
		{	
			float r = atof(tokens[4].c_str());
			float b = atof(tokens[5].c_str());
			int scene_id = atoi(tokens[6].c_str());
			obj = new CPortal(x, y, r, b, scene_id);
		}
		break;

	default:
		DebugOut(L"[ERR] Invalid object type: %d\n", object_type);
		return;
	}

	// General object setup
	obj->SetPosition(x, y);

	LPANIMATION_SET ani_set = animation_sets->Get(ani_set_id);

	obj->SetAnimationSet(ani_set);
	objects.push_back(obj);
}
void CPlayScene::_ParseSection_TILEMAP(string line)
{ 
	//đọc Map từ file txt
	int ID, rowmap, columnmap, rowtile, columntile, totaltile;
	LPCWSTR path = ToLPCWSTR(line);
	ifstream f;
	f.open(path);
	f >> ID >> rowmap >> columnmap >> rowtile >> columntile >> totaltile;
	int** tilemapdata = new int* [rowmap];
	for (int i = 0; i < rowmap; i++)
	{
		tilemapdata[i] = new int[columnmap];
		for (int j = 0; j < columnmap; j++)
		{
			f >> tilemapdata[i][j];
		}
	}
	f.close();
	map = new Map(ID, rowmap, columnmap, rowtile, columntile, totaltile);
	map->GetSpriteTile();
	map->SetMap(tilemapdata);
}
void CPlayScene::Load()
{
	DebugOut(L"[INFO] Start loading scene resources from : %s \n", sceneFilePath);

	ifstream f;
	f.open(sceneFilePath);
	int section = SCENE_SECTION_UNKNOWN;					

	char str[MAX_SCENE_LINE];
	while (f.getline(str, MAX_SCENE_LINE))
	{
		string line(str);

		if (line[0] == '#') continue;	// skip comment lines	

		if (line == "[TEXTURES]") { section = SCENE_SECTION_TEXTURES; continue; }
		if (line == "[SPRITES]") {
			section = SCENE_SECTION_SPRITES; continue;
		}
		if (line == "[ANIMATIONS]") {
			section = SCENE_SECTION_ANIMATIONS; continue;
		}
		if (line == "[ANIMATION_SETS]") {
			section = SCENE_SECTION_ANIMATION_SETS; continue;
		}
		if (line == "[OBJECTS]") {
			section = SCENE_SECTION_OBJECTS; continue;
		}
		if (line == "[TILEMAP]") {
			section = SCENE_SECTION_DRAWMAP; continue;
		}
		if (line[0] == '[') { section = SCENE_SECTION_UNKNOWN; continue; }	

		//
		// data section
		//
		switch (section)
		{ 
			case SCENE_SECTION_TEXTURES: _ParseSection_TEXTURES(line); break;
			case SCENE_SECTION_SPRITES: _ParseSection_SPRITES(line); break;
			case SCENE_SECTION_ANIMATIONS: _ParseSection_ANIMATIONS(line); break;
			case SCENE_SECTION_ANIMATION_SETS: _ParseSection_ANIMATION_SETS(line); break;
			case SCENE_SECTION_OBJECTS: _ParseSection_OBJECTS(line); break;
			case SCENE_SECTION_DRAWMAP: _ParseSection_TILEMAP(line); break;
		}
	}

	f.close();

	CTextures::GetInstance()->Add(ID_TEX_BBOX, L"textures\\bbox.png", D3DCOLOR_XRGB(255, 255, 255));

	DebugOut(L"[INFO] Done loading scene resources %s\n", sceneFilePath);
}

void CPlayScene::Update(DWORD dt)
{
	// We know that Mario is the first object in the list hence we won't add him into the colliable object list
	// TO-DO: This is a "dirty" way, need a more organized way 

	vector<LPGAMEOBJECT> coObjects;
	for (size_t i = 1; i < objects.size(); i++)
	{
		coObjects.push_back(objects[i]);
	}

	for (size_t i = 0; i < objects.size(); i++)
	{
		objects[i]->Update(dt, &coObjects);
	}

	// skip the rest if scene was already unloaded (Mario::Update might trigger PlayScene::Unload)
	if (player == NULL) return;

	// Update camera to follow mario
/*	float cx, cy;
	player->GetPosition(cx, cy);

	CGame* game = CGame::GetInstance();
	cx -= game->GetScreenWidth() / 2;
	cy -= game->GetScreenHeight() / 2;

	CGame::GetInstance()->SetCamPos(cx, 280 cy);*/
	float cx, cy, sw, sh, mw, mh, mx, my;
	player->GetPosition(cx, cy);
	CGame* game = CGame::GetInstance();
	sw = game->GetScreenWidth();
	sh = game->GetScreenHeight();
	mw = map->GetMapWidth();
	mh = map->GetMapHeight();

	// Update camera to follow mario
	if (cx >= sw / 2 //Left Edge
		&& cx + sw / 2 <= mw) //Right Edge
		cx -= sw / 2;
	else if (cx < sw / 2)
		cx = 0;
	else if (cx + sw / 2 > mw)
		cx = mw - sw + 1;
	if (cy - sh / 2 <= 0)//Top Edge
		cy = 0;
	else if (cy + sh / 2 >= mh)//Bottom Edge
		cy = mh - sh;
	else cy -= sh / 2;
	CGame::GetInstance()->SetCamPos((int)cx, (int)cy);
	//TileMap->SetCamPos((int)cx, (int)cy);
	map->SetCamPos((int)cx, (int)cy);
}

void CPlayScene::Render()
{
	map->DrawMap();
	for (int i = 0; i < objects.size(); i++)
		objects[i]->Render();
}

/*
	Unload current scene
*/
void CPlayScene::Unload()
{
	for (int i = 0; i < objects.size(); i++)
		delete objects[i];

	objects.clear();
	player = NULL;

	DebugOut(L"[INFO] Scene %s unloaded! \n", sceneFilePath);
}

void CPlayScenceKeyHandler::OnKeyDown(int KeyCode)
{
	//DebugOut(L"[INFO] KeyDown: %d\n", KeyCode);

	CMario *mario = ((CPlayScene*)scence)->GetPlayer();
	if (mario->GetState() == MARIO_STATE_DIE)
		return;
	switch (KeyCode)
	{

	case DIK_R:
		mario->Reset();
		break;
	case DIK_1:
		mario->SetLevel(MARIO_LEVEL_SMALL);
		break;
	case DIK_2:
		mario->SetLevel(MARIO_LEVEL_BIG);
		break;
	case DIK_3:
		mario->SetLevel(MARIO_LEVEL_RACCOON);
		break;
	case DIK_4:
		mario->SetLevel(MARIO_LEVEL_FIRE);
		break;
	case DIK_Z:
	{
		if (mario->level == MARIO_LEVEL_RACCOON)
		{
			mario->SetState(MARIO_STATE_RACCOON_ATTACK);
			mario->Attack = GetTickCount64();
			break;

		}
		else if (mario->level == MARIO_LEVEL_FIRE)
		{
			mario->SetState(MARIO_STATE_SHOOT_FIRE);
			mario->Attack = GetTickCount64();
		}
		break;
	}
	case DIK_X:
	{
		mario->SetState(MARIO_STATE_JUMP);
		mario->isFallSlow = false;
		break;
	}
	}
	

}

void CPlayScenceKeyHandler::KeyState(BYTE *states)
{
	CGame *game = CGame::GetInstance();
	CMario *mario = ((CPlayScene*)scence)->GetPlayer();
	// disable control key when Mario die 
	if (mario->GetState() == MARIO_STATE_DIE) return;
	if (game->IsKeyDown(DIK_RIGHT))
	{
		mario->nx = 1;
		mario->SetState(MARIO_STATE_WALKING_RIGHT);
	}
	if (game->IsKeyDown(DIK_LEFT))
	{
		mario->nx = -1;
		mario->SetState(MARIO_STATE_WALKING_LEFT);

	}
	if (game->IsKeyDown(DIK_DOWN))
	{
		if ((!mario->isWalking) || !mario->isRunning) {
			mario->SetState(MARIO_STATE_SIT);
		}
	}
	if (game->IsKeyDown(DIK_Z))
	{
		if (mario->isWalking)
		{
			mario->SetState(MARIO_STATE_RUNNING);
		}
	}

}
void  CPlayScenceKeyHandler::OnKeyUp(int KeyCode)
{
	//DebugOut(L"[INFO] KeyUp: %d\n", KeyCode);

	CMario* mario = ((CPlayScene*)scence)->GetPlayer();
	switch (KeyCode)
	{
	case DIK_RIGHT:
		mario->isWalkingR = false;
		break;
	case DIK_LEFT:
		mario->isWalkingL = false;
		break;
	case DIK_Z:
		mario->isRunning = false;
		break;
	case DIK_DOWN:
		if (mario->isSitting)
		{
			mario->ResetSit();
			float sx, sy;
			mario->GetPosition(sx, sy);
			mario->SetState(MARIO_STATE_IDLE);
			break;
		}
	case DIK_X:
		float cVx, cVy;
		mario->GetSpeed(cVx, cVy);
		if (mario->level == MARIO_LEVEL_RACCOON) {
			if (mario->isOnAir)
			{
				if (!mario->isFlying && mario->vy>=0)
				{
						mario->SetSpeed(cVx, -MARIO_FALL_SLOW);
						mario->isFallSlow = true;
						break;
				}
				if (mario->isFlying)
				{
					if (mario->isFlyup)
					{
						mario->SetSpeed(cVx,-MARIO_FLY_SPEED_Y);
						mario->isFallFly = true;						
					}
					break;
				}
			}
			else if (mario->isOnGround)
			{
				mario->isFallSlow = false;
			}
		}
		break;
	}
}