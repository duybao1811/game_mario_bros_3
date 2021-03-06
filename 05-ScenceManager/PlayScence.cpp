﻿#include <iostream>
#include <fstream>

#include "PlayScence.h"
#include "Utils.h"
#include "Textures.h"
#include "Sprites.h"
#include "Portal.h"
#include "Mario.h"
#include "Map.h"
#include "Scence.h"
#include <string>
#include <cstring>
#include "Fire.h"
#include "Coin.h"
#include "Pipe.h"
#include "PiranhaPlant.h"
#include "PlatForm.h"
#include "QuestionBrick.h"
#include "BlockColor.h"
#include "Effect.h"
#include "CoinEffect.h"
#include "PointEffect.h"
#include "define.h"
#include "GoldBrick.h"
#include "P_Switch.h"
#include "Floor.h"
#include "WorldMap.h"
#include "Effect_1_UP.h"
#include "Box.h"
#include "wood.h"
#include "BoomerangBrother.h"
#include "Flower.h"
#include "TranformEffect.h"
using namespace std;

CPlayScene::CPlayScene(int id, LPCWSTR filePath):
	CScene(id, filePath)
{
	key_handler = new CPlayScenceKeyHandler(this);
	CountEnemy = 0;
	grid = new Grid;
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
#define OBJECT_TYPE_PLATFORM	1
#define OBJECT_TYPE_GOOMBA	2
#define OBJECT_TYPE_KOOPAS	3
#define OBJECT_TYPE_COIN 4
#define OBJECT_TYPE_FIRE_PLANT 5
#define OBJECT_TYPE_PIPE    7
#define OBJECT_TYPE_PLANT 8
#define OBJECT_TYPE_QUESTIONBRICK 9
#define OBJECT_TYPE_BLOCK_COLOR 10
#define OBJECT_TYPE_GOLD_BRICK 11
#define OBJECT_TYPE_FLOOR 12
#define OBJECT_TYPE_WORLD_MAP 13
#define OBJECT_TYPE_BOX 14
#define OBJECT_TYPE_WOOD 15
#define OBJECT_TYPE_BOOM_BROTHER 16
#define OBJECT_TYPE_PORTAL	50
#define MAX_SCENE_LINE 1024

#define BOARD_DEFAULT_POSITION_X 32.0f
#define BOARD_DEFAULT_POSITION_Y 440.0f
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
	/*	if (player!=NULL) 
		{
			DebugOut(L"[ERROR] MARIO object was created before!\n");
			return;
		}*/
		obj = new CMario(x,y);
		player = (CMario*)obj;  

		DebugOut(L"[INFO] Player object created!\n");
		break;
	case OBJECT_TYPE_GOOMBA:
	{
		int model = atof(tokens[4].c_str());
		int d = atof(tokens[5].c_str());
		obj = new CGoomba(x,y,model, d);
		break;
	}
	case OBJECT_TYPE_PLATFORM: {
		float w = atof(tokens[4].c_str());
		float h = atof(tokens[5].c_str());
		obj = new CPlatform(w, h);
		break;
	}
	case OBJECT_TYPE_KOOPAS: 
	{
		int model = atof(tokens[4].c_str());
		int d = atof(tokens[5].c_str());
		obj = new CKoopas(x,y,player,model,d); 
		break;
	}
	case OBJECT_TYPE_PORTAL:
		{	
			float r = atof(tokens[4].c_str());
			float b = atof(tokens[5].c_str());
			int scene_id = atoi(tokens[6].c_str());
			obj = new CPortal(x, y, r, b, scene_id);
			break;
		}
	case OBJECT_TYPE_COIN:
	{
		obj = new CCoin(x,y);
		break;
	}
	case OBJECT_TYPE_FIRE_PLANT:
	{
		int model = atof(tokens[4].c_str());
		obj = new CFirePiranhaPlant(x,y,model,&listFireEnemy);
		break;
	}
	case OBJECT_TYPE_PIPE:
	{
		int model = atof(tokens[4].c_str());
		obj = new Pipe(model);
		break;
	}
	case OBJECT_TYPE_PLANT:
	{
		int model = atof(tokens[4].c_str());
		obj = new CPiranhaPlant(x,y,model);
		break;
	}
	case OBJECT_TYPE_QUESTIONBRICK:
	{
		int model = atof(tokens[4].c_str());
		obj = new CQuestionBrick(x, y, model);
		break;
	}
	case OBJECT_TYPE_BLOCK_COLOR:
	{
		float w = atof(tokens[4].c_str());
		float h = atof(tokens[5].c_str());
		obj = new CBlockColor(w, h);
		break;
	}
	case OBJECT_TYPE_GOLD_BRICK:
	{
		int model = atof(tokens[4].c_str());
		obj = new GoldBrick(x, y,model);
		break;
	}
	case OBJECT_TYPE_WORLD_MAP:
	{
		obj = new WorldMap(x, y);
		break;
	}
	case OBJECT_TYPE_BOX:
	{
		obj = new Box();
		break;
	} 
	case OBJECT_TYPE_BOOM_BROTHER:
	{
		int d = atof(tokens[4].c_str());
		obj = new BoomerangBrother(d, &listBoomerangEnemy);
		break;
	}
	case OBJECT_TYPE_WOOD:
	{
		obj = new Wood();
		break;
	}
	default:
		DebugOut(L"[ERR] Invalid object type: %d\n", object_type);
		return;
	}
	LPANIMATION_SET ani_set = animation_sets->Get(ani_set_id);
	
	
		// General object setup
		obj->SetPosition(x, y);

		obj->SetAnimationSet(ani_set);

		objects.push_back(obj);
	grid->Insert(obj);
}
void CPlayScene::_ParseSection_TILEMAP(string line)
{ 
	
	//đọc Map từ file txt
	int ID, rowmap, columnmap, rowtile, columntile, totaltile;
	LPCWSTR path = ToLPCWSTR(line);
	ifstream f(path, ios::in);
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
void CPlayScene::CheckCollistionMarioWithItem()
{
	for (UINT i = 0; i < ListItem.size(); i++)
	{
		if (ListItem[i]->GetFinish() == false)
		{
			if (player->isCollisionWithItem(ListItem[i]) == true)
			{
				switch (ListItem[i]->GetType())
				{
				case Type::MUSHROOM_POWER:
				{
					player->isTranForm = true;
					player->SetLevel(MARIO_LEVEL_BIG);
					ListEffect.push_back(new TranformEffect(player->x, player->y, player->direction));
					ListEffect.push_back(new PointEffect(player->GetX(),player->GetY(), POINT_EFFECT_TYPE_ONE_THOUSAND));
					player->y -= 20;
					player->SetScore(player->GetScore() + 1000);
					ListItem[i]->SetFinish(true);
					break;
				}
				case Type::LEAF:
				{
					player->isTranForm = true;
					player->SetLevel(MARIO_LEVEL_RACCOON);
					ListEffect.push_back(new PointEffect(player->GetX(), player->GetY(), POINT_EFFECT_TYPE_ONE_THOUSAND));
					player->SetScore(player->GetScore() + 1000);
					ListEffect.push_back(new EffectDisappear(player->x, player->y));
					ListItem[i]->SetFinish(true);
					break;

				}
				case Type::MUSHROOM_1_UP:
				{
					player->SetLive(player->GetLive() + 1);
					ListItem[i]->SetFinish(true);
					ListEffect.push_back(new Effect_1_UP(player->GetX(), player->GetY()));
					break;
				}
				case Type::FLOWER:
				{
					player->SetLevel(MARIO_LEVEL_FIRE);
					ListEffect.push_back(new PointEffect(player->GetX(), player->GetY(), POINT_EFFECT_TYPE_ONE_THOUSAND));
					player->SetScore(player->GetScore() + 1000);
					ListItem[i]->SetFinish(true);
					break;
				}
				}
			}
		}
	}
}			
void CPlayScene::QuestionBrickDropItem(int model, float x,float y)
{
	switch (model)
	{
	case QUESTION_BRICK_MODEL_COIN:
	{	
		ListEffect.push_back(new CoinEffect(x, y));
	}
	break;
	case QUESTION_BRICK_MODEL_POWER_UP:
	{
		if (player->level == MARIO_LEVEL_SMALL)
		{	
			ListItem.push_back(new CMushRoom(x, y+10, MUSHROOM_RED));
		}
		if(player->level == MARIO_LEVEL_BIG)
		{
			ListItem.push_back(new Leaf(x, y));
		}
		if (player->level == MARIO_LEVEL_RACCOON || player->level == MARIO_LEVEL_FIRE)
		{
			ListItem.push_back(new Flower(x, y));
		}
	}
	break;
	}
}
void CPlayScene::GoldBrickDestroy(int model, float x, float y)
{
	switch (model)
	{
	case GB_CONTAIN_PSWITCH:
	{
		objects.push_back(new P_Switch(x, y));
		break;
	}
	case GB_CONTAIN_MUSHROOM_1_UP:
	{
		ListItem.push_back(new CMushRoom(x, y + 10, MUSHROOM_GREEN));
		break;
	}
	case GB_CONTAIN_POWER_UP:
	{
		if (player->level == MARIO_LEVEL_SMALL)
		{
			ListItem.push_back(new CMushRoom(x, y + 10, MUSHROOM_RED));
		}
		if (player->level == MARIO_LEVEL_BIG)
		{
			ListItem.push_back(new Leaf(x, y));
		}
		if (player->level == MARIO_LEVEL_RACCOON || player->level == MARIO_LEVEL_FIRE)
		{
			ListItem.push_back(new Flower(x, y));
		}
	}
	break;

	}
}
void CPlayScene::CheckCollision()
{
	CheckCollistionMarioWithItem();
	//CheckCollisionMarioWithEnemy();
}
void CPlayScene::Update(DWORD dt)
{
	CGameObject* obj = NULL;

	vector<LPGAMEOBJECT> coObjects;

	grid->GetListObject(objects);
	for (size_t i = 1; i < objects.size(); i++)
	{
		coObjects.push_back(objects[i]);
	}
	for (size_t i = 0; i < objects.size(); i++)
	{
		LPGAMEOBJECT e = objects[i];
		if (objects[i]->checkObjInCamera(objects[i]) == true)
		{
			objects[i]->Update(dt, &coObjects);
		}

		if (e->GetType() == QUESTION_BRICK)
		{
			CQuestionBrick* questionbrick = dynamic_cast<CQuestionBrick*>(e);
			if (questionbrick->isUnbox)
			{
				QuestionBrickDropItem(questionbrick->GetModel(), questionbrick->GetX(), questionbrick->GetY());
				questionbrick->isUnbox = false;
			}
		}
		if (e->GetType() == GOLD_BRICK)
		{
			GoldBrick* goldbrick = dynamic_cast<GoldBrick*>(e);
			if (goldbrick->isUnbox)
			{
				GoldBrickDestroy(goldbrick->GetModel(), goldbrick->GetX(), goldbrick->GetY() - GB_BBOX_HEIGHT);
				goldbrick->isUnbox = false;
			}
		}

		if (e->GetType() == P_SWITCH)
		{
			P_Switch* pswitch = dynamic_cast<P_Switch*>(e);
			if (pswitch->GetState()==PSWITCH_STATE_USED && pswitch->isUsed)
			{
				for (UINT i = 0; i < objects.size(); i++)
				{
					if (objects[i]->GetType() == GOLD_BRICK)
					{
						GoldBrick* goldbrick = dynamic_cast<GoldBrick*>(objects[i]);
						if (goldbrick->GetModel() == GB_CONTAIN_COIN)
						{
							if (goldbrick->checkObjInCamera(goldbrick))
							{
								goldbrick->SetState(GB_STATE_TRANFORM);
							}
						}
					}
				}
				pswitch->isUsed = false;

			}
		}
	}

	for (int i = 0; i < ListEffect.size(); i++)
	{
		ListEffect[i]->Update(dt);
	}
	for (int i = 0; i < ListEffect.size(); i++)
	{
		CGameObject* effect = ListEffect[i];
		if (effect->GetFinish())
		{
			if (effect->GetType() == COIN_EFFECT)
			{
				ListEffect.erase(ListEffect.begin() + i);
				ListEffect.push_back(new PointEffect(effect->GetX(), effect->GetY(), POINT_EFFECT_TYPE_ONE_HUNDRED));
				player->SetScore(player->GetScore() + 100);
				player->SetCoinCollect(player->GetCoinCollect() + 1);
			}
			else
				ListEffect.erase(ListEffect.begin() + i);
		}
	}
	for (int i = 0; i < ListItem.size(); i++)
	{
		ListItem[i]->Update(dt,&coObjects);
	}
	for (UINT i = 0; i < ListItem.size(); i++)
	{
		if (ListItem[i]->GetFinish() == true)
		{
			ListItem.erase(ListItem.begin() + i);
		}
	}
	for (UINT i = 0; i < listFireEnemy.size(); i++)
	{
		listFireEnemy[i]->Update(dt, &coObjects);
	}
	for (UINT i = 0; i < listFireEnemy.size(); i++)
	{
		if (listFireEnemy[i]->GetFinish() == true)
		{
			listFireEnemy.erase(listFireEnemy.begin() + i);
		}
	}
	for (UINT i = 0; i < listBoomerangEnemy.size(); i++)
	{
		listBoomerangEnemy[i]->Update(dt, &coObjects);
	}
	for (UINT i = 0; i < listBoomerangEnemy.size(); i++)
	{
		if (listBoomerangEnemy[i]->GetFinish() == true)
		{
			listBoomerangEnemy.erase(listBoomerangEnemy.begin() + i);
		}
	}
	if (!player->GetIsDeadth())
	{
		CheckCollision();
	}

	if (section == 1)
	{
		player->vx = 0.1f;
		player->SetState(MARIO_STATE_WALKING_RIGHT);
	}
	if (section == 1)
	{
		if (abs(player->x-player->BackupX)>114)
		{
			section = 2;
		}
	}
	if (section == 2)
	{
		player->vx = 0.1f;
		TimeSwitchScene += dt;
		if (TimeSwitchScene > 3000)
		{
			CGame::GetInstance()->SwitchScene(WORLD_MAP);
			section = 0;
			TimeSwitchScene = 0;
		}
	}
	gametime->Update(dt);
	if (gametime->GetTime() >= GAME_TIME_LIMIT || player->GetHealth() <= 0)
	{
			isGameOver = true;
			player->SetIsDeadth(true);
			player->SetState(MARIO_STATE_DIE);
			TimeSwitchScene1 += dt;
	}
	if (isGameOver)
	{
		if (TimeSwitchScene1 > 1000)
		{
			CGame::GetInstance()->SwitchScene(WORLD_MAP);
			isGameOver = false;
			TimeSwitchScene1 = 0;
		}
	}
	if (CGame::GetInstance()->GetScene() == WORLD_1_4)
	{
		float CamVx;
		if (CGame::GetInstance()->GetCamX() + SCREEN_WIDTH< map->GetMapWidth() + 16)
		{
			CamVx = 0.05f;
		}
		else
		{
			CamVx = 0;
		}
		CamX += CamVx * dt;
		CGame::GetInstance()->SetCamPos(CamX, 0);
		map->SetCamPos(CamX, 0);
	}
	if (CGame::GetInstance()->GetScene()==WORLD_MAP)
	{
		CGame::GetInstance()->SetCamPos(0, 0);
		player->SetState(MARIO_STATE_WORLDMAP);
		player->vy = 0;
	}
	if (CGame::GetInstance()->GetScene() == 5)
	{

		CGame::GetInstance()->SetCamPos(player->x-(SCREEN_WIDTH/2),16);
		map->SetCamPos(player->x - (SCREEN_WIDTH / 2),16);
	}
	if(CGame::GetInstance()->GetScene() == WORLD_1_1 || CGame::GetInstance()->GetScene() == WORLD_1_1_1)
	{
		// skip the rest if scene was already unloaded (Mario::Update might trigger PlayScene::Unload)
		if (player == NULL) return;

		// Update camera to follow mario
		float cx, cy, sw, sh, mw, mh, mx, my;
		player->GetPosition(cx, cy);
		CGame* game = CGame::GetInstance();
		sw = game->GetScreenWidth();
		sh = game->GetScreenHeight();
		mw = map->GetMapWidth();
		mh = map->GetMapHeight();
		bool isTopSide = false;
		if (cy < mh / 2)
			isTopSide = true;
		if (cx >= sw / 2 //Left Edge
			&& cx + sw / 2 <= mw) //Right Edge
			cx -= sw / 2;
		else if (cx < sw / 2)
			cx = 0;
		else if (cx + sw / 2 > mw)
			cx = mw - sw + 1;
		if (CGame::GetInstance()->GetScene() == WORLD_1_1_1)
		{
			cy = 0;
		}
		else
		{
			if (cy - sh / 4 <= 0)//Top Edge
				cy = 0;
			else if (cy > mh / 2)//Bottom Edge
			{
				cy = mh - sh;
			}
			else cy -= sh / 4;
		}
		CGame::GetInstance()->SetCamPos((int)cx, (int)cy);
		map->SetCamPos((int)cx, (int)cy);
	}

}

void CPlayScene::Render()
{

	map->DrawMap();

	for (int i = 0; i < objects.size(); i++)
		objects[i]->Render();
	for (int i = 0; i < ListEffect.size(); i++)
	{
		ListEffect[i]->Render();
	}
	for (int i = 0; i < ListItem.size(); i++)
	{
		ListItem[i]->Render();
	}
	for (int i = 0; i < listFireEnemy.size(); i++)
	{
		listFireEnemy[i]->Render();
	}
	
	for (int i = 0; i < listBoomerangEnemy.size(); i++)
	{
		listBoomerangEnemy[i]->Render();
	}
	board = new Board(CGame::GetInstance()->GetCamX(), CGame::GetInstance()->GetCamY() + SCREEN_HEIGHT - DISTANCE_FROM_BOTTOM_CAM_TO_TOP_BOARD);
	board->Render(player, GAME_TIME_LIMIT - gametime->GetTime());
	
	if (((CPlayScene*)game->GetCurrentScene())->section ==2)
	{
		Text = new Font();
		Text->Draw(2635, 275, "COURSE CLEAR !");
		Text->Draw(2615, 300, "YOU GOT A CARD");
	}
}

/*
	Unload current scene
*/
void CPlayScene::Unload()
{
	objects.clear();
	ListPointEffect.clear();
	listFireEnemy.clear();
	listBoomerangEnemy.clear();
	ListEffect.clear();
	
	DebugOut(L"[INFO] Scene %s unloaded! \n", sceneFilePath);
}

void CPlayScene::CheckCollisionMarioWithEnemy()
{
	if (GetTickCount64() - player->untouchable_start > MARIO_UNTOUCHABLE_TIME)
	{
		player->untouchable_start = 0;
		player->untouchable = false;
	}
	if (player->untouchable == false)
	{
		for (UINT i = 0; i < listFireEnemy.size(); i++)
		{
			CGameObject* gameobj = dynamic_cast<CGameObject*>(listFireEnemy[i]);
			if (gameobj->GetFinish()!=true)
			{
				LPCOLLISIONEVENT e = player->SweptAABBEx(gameobj);
				if (e->nx != 0 || e->ny != 0)
				{
				player->StartUntouchable();
				player->StartUntouchable();
				if (player->level == MARIO_LEVEL_BIG)
				{
					player->level--;
					player->SetHealth(2);
				}
				if (player->level == MARIO_LEVEL_RACCOON)
				{
					player->level -= 1;
				}
				if (player->level == MARIO_LEVEL_FIRE)
				{
					player->level == MARIO_LEVEL_BIG;
				}
				else if (player->level == MARIO_LEVEL_SMALL)
				{
					player->SubHealth(1);
				}
				}
			}
		}
		for (UINT i = 0; i < objects.size(); i++)
		{
			if (objects[i]->GetObjType() == ENEMY)
			{
				CGameObject* gameobj = dynamic_cast<CGameObject*>(objects[i]);
				if (gameobj->GetHealth() > 0)
				{
					LPCOLLISIONEVENT e = player->SweptAABBEx(gameobj);
					bool isCollision = false;
					if (e->nx != 0 || e->ny >0)
					{
						if (!(objects[i]->GetType()==KOOPAS && objects[i]->GetState()==KOOPAS_STATE_DEFEND))
						{
							player->StartUntouchable();
							if (player->level == MARIO_LEVEL_BIG)
							{
								player->level--;
								player->SetHealth(2);
							}
							if ( player->level == MARIO_LEVEL_RACCOON)
							{
								player->level-=1;
							}
							if (player->level == MARIO_LEVEL_FIRE)
							{
								player->level == MARIO_LEVEL_BIG;
							}
							else if (player->level == MARIO_LEVEL_SMALL)
							{
								player->SubHealth(1);
							}
							isCollision = true;
						}
					}
					if (e->ny < 0)
					{
						if ((objects[i]->GetType() == PIRANHA_GREEN && objects[i]->GetState() != PLANT_STATE_HIDDING || objects[i]->GetType() == FIRE_PIRANHA && objects[i]->GetState() != PLANT_STATE_HIDDING))
						{

							player->StartUntouchable();
							if (player->level == MARIO_LEVEL_BIG)
							{
								player->level--;
								player->SetHealth(2);
							}
							if (player->level == MARIO_LEVEL_RACCOON)
							{
								player->level -= 1;
							}
							if (player->level == MARIO_LEVEL_FIRE)
							{
								player->level == MARIO_LEVEL_BIG;
							}
							else if (player->level == MARIO_LEVEL_SMALL)
							{
								player->SubHealth(1);
							}
							isCollision = true;
						}
					}
				}
			}
		}
	}
}
void CPlayScenceKeyHandler::OnKeyDown(int KeyCode)
{
	//DebugOut(L"[INFO] KeyDown: %d\n", KeyCode);
	CGame* game = CGame::GetInstance();
	if (((CPlayScene*)game->GetCurrentScene())->section == 0)
	{
		CMario* mario = ((CPlayScene*)scence)->GetPlayer();
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
		case DIK_A:
		{
			if (mario->level == MARIO_LEVEL_RACCOON)
			{
				mario->isJumping = false;
				mario->SetState(MARIO_STATE_RACCOON_ATTACK);
				mario->TimeAttack = GetTickCount64();
				break;

			}
			else if (mario->level == MARIO_LEVEL_FIRE)
			{
				mario->isJumping = false;
				mario->SetState(MARIO_STATE_SHOOT_FIRE);
				mario->TimeAttack = GetTickCount64();
			}
			break;
		}
		case DIK_S:
		{
			mario->SetState(MARIO_STATE_JUMP);
			if (CGame::GetInstance()->GetScene() == WORLD_1_4)
			{
				mario->isIntoWorld = true;
			}
			break;
		}
		}
	}
}

void CPlayScenceKeyHandler::KeyState(BYTE* states)
{
	CGame* game = CGame::GetInstance();
	CMario* mario = ((CPlayScene*)scence)->GetPlayer();
	if (((CPlayScene*)game->GetCurrentScene())->section == 0)
	{
		// disable control key when Mario die 

		if (mario->GetState() == MARIO_STATE_DIE) return;
		else if (game->IsKeyDown(DIK_A) && game->IsKeyDown(DIK_RIGHT))
		{
			mario->isRunning = true;
			mario->SetState(MARIO_STATE_RUN_RIGHT);
		}
		else if (game->IsKeyDown(DIK_A) && game->IsKeyDown(DIK_LEFT))
		{
			mario->isRunning = true;
			mario->SetState(MARIO_STATE_RUN_LEFT);
		}
		else if (game->IsKeyDown(DIK_RIGHT) || (game->IsKeyDown(DIK_DOWN) && game->IsKeyDown(DIK_RIGHT)))
		{
			mario->SetState(MARIO_STATE_WALKING_RIGHT);
			if (mario->vx < 0)
			{
				mario->SetState(MARIO_STATE_TURN);
			}
			mario->WalkRight();
		}
		else if (game->IsKeyDown(DIK_LEFT) || (game->IsKeyDown(DIK_DOWN) && game->IsKeyDown(DIK_LEFT)))
		{
			mario->SetState(MARIO_STATE_WALKING_LEFT);
			if (mario->vx > 0)
			{
				mario->SetState(MARIO_STATE_TURN);
			}
			mario->WalkLeft();
		}
		else if (game->IsKeyDown(DIK_DOWN) && mario->GetLevel() != MARIO_LEVEL_SMALL)
		{
			mario->Sit();
		}

		else if (game->IsKeyDown(DIK_X))
		{
			if (mario->isOnGround)
			{
				mario->JumpSlow();
			}

		}
		else
		{
			if (mario->isOnGround)
				mario->SetState(MARIO_STATE_IDLE);
		}
		if (CGame::GetInstance()->GetScene() == WORLD_MAP)
		{
			if (game->IsKeyDown(DIK_UP))
			{
				mario->vy = -0.05f;
			}
			if (game->IsKeyDown(DIK_DOWN))
			{
				mario->vy = 0.05f;
			}
			if (game->IsKeyDown(DIK_S))
			{
				mario->isIntoWorld = true;
			}
		}
		if (CGame::GetInstance()->GetScene() == WORLD_1_1)
		{
			if (game->IsKeyDown(DIK_5))
			{
				mario->SetPosition(687, 357);
			}
			if (game->IsKeyDown(DIK_6))
			{
				mario->SetPosition(1224, 377);
			}
			if (game->IsKeyDown(DIK_7))
			{
				mario->SetPosition(1419, 155);
			}
			if (game->IsKeyDown(DIK_8))
			{
				mario->SetPosition(2265, 72);
			}
			if (game->IsKeyDown(DIK_9))
			{
				mario->SetPosition(2627, 357);
			}
			if (game->IsKeyDown(DIK_0))
			{
				CGame::GetInstance()->SwitchScene(3);
			}
		}
		if (CGame::GetInstance()->GetScene() == WORLD_1_1_1)
		{
			if (game->IsKeyDown(DIK_5))
			{
				CGame::GetInstance()->SwitchScene(2);
				mario->SetPosition(2627, 357);
			}
		}
		if (CGame::GetInstance()->GetScene() == WORLD_1_4)
		{
			if (game->IsKeyDown(DIK_5))
			{
				CGame::GetInstance()->SwitchScene(WORLD_1_4_1);
			}
			if (game->IsKeyDown(DIK_0))
			{
				CGame::GetInstance()->SwitchScene(WORLD_1_1);
			}
		}

	}
}
void  CPlayScenceKeyHandler::OnKeyUp(int KeyCode)
{
	//DebugOut(L"[INFO] KeyUp: %d\n", KeyCode);
	CGame* game = CGame::GetInstance();
	if (((CPlayScene*)game->GetCurrentScene())->section == 0)
	{
		CMario* mario = ((CPlayScene*)scence)->GetPlayer();
		switch (KeyCode)
		{
		case DIK_A:
		{
			mario->isRunning = false;
			mario->isHoldTurtle = false;
			break;
		}
		case DIK_DOWN:
			if (mario->isSitting)
			{
				mario->ResetSit();
				float sx, sy;
				mario->GetPosition(sx, sy);
				mario->SetState(MARIO_STATE_IDLE);
			}
			if (CGame::GetInstance()->GetScene() == 1)
			{
				mario->vy = 0;
			}
			break;
		case DIK_S:

			if (mario->level == MARIO_LEVEL_RACCOON) {
				if (mario->isOnAir)
				{
					if (!mario->isFlying && mario->vy >= 0)
					{
						mario->FallSlow();
						break;
					}
					if (mario->isFlying)
					{
						if (mario->isFlyup)
						{
							mario->Fly();
							break;
						}
					}
				}
			}
			break;
		case DIK_UP:
			if (CGame::GetInstance()->GetScene() == 1)
			{
				mario->vy = 0;
			}
			break;
		case DIK_RIGHT:
		case DIK_LEFT:
			if (CGame::GetInstance()->GetScene() == 1)
			{
				mario->vx = 0;
			}
			break;
		}
	}
}