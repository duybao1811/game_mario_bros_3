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
using namespace std;

CPlayScene::CPlayScene(int id, LPCWSTR filePath):
	CScene(id, filePath)
{
	key_handler = new CPlayScenceKeyHandler(this);
	gametime = new GameTime();
	CountEnemy = 0;
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
		if (player!=NULL) 
		{
			DebugOut(L"[ERROR] MARIO object was created before!\n");
			return;
		}
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
	case OBJECT_TYPE_FLOOR:
	{
		obj = new Floor(x, y);
		break;
	}
	case OBJECT_TYPE_WORLD_MAP:
	{
		obj = new WorldMap(x, y);
		break;
	}
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
					player->SetLevel(MARIO_LEVEL_BIG);
					ListEffect.push_back(new PointEffect(player->GetX(),player->GetY(), POINT_EFFECT_TYPE_ONE_THOUSAND));
					player->SetHealth(player->GetHealth() + 1);
					player->y -= 20;
					player->SetScore(player->GetScore() + 1000);
					ListItem[i]->SetFinish(true);
					break;
				}
				case Type::LEAF:
				{
					//player->GetLeaf();
					player->SetLevel(MARIO_LEVEL_RACCOON);
					ListEffect.push_back(new PointEffect(player->GetX(), player->GetY(), POINT_EFFECT_TYPE_ONE_THOUSAND));
					player->SetScore(player->GetScore() + 1000);
					if (player->GetLevel() == MARIO_LEVEL_BIG)
					{
						player->SetHealth(player->GetHealth()+1);
					}
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
		else
		{
			ListItem.push_back(new Leaf(x, y));
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
			objects[i]->isInCam = true;
		}


#pragma region cơ chế hồi sinh của enemy khi ra khỏi cam không phải bị giết 


			if (objects[i]->GetType() == GOOMBA || objects[i]->GetType() == KOOPAS)
			{
				if (player->GetX() > objects[i]->GetStartX() + SCREEN_WIDTH / 2 || player->GetX() + SCREEN_WIDTH / 2 < objects[i]->GetStartX())
				{
					if (objects[i]->isFinish)
					{
						if (objects[i]->GetStartX() == 512 || objects[i]->GetStartX() == 208 || objects[i]->GetStartX() == 816 || objects[i]->GetStartX() == 864 || objects[i]->GetStartX()==928 || objects[i]->GetStartX()==560 || objects[i]->GetStartX() == 1456||objects[i]->GetStartX() == 1312|| objects[i]->GetStartX() == 1360|| objects[i]->GetStartX() == 1406)
						{
							objects[i]->SetFinish(false);
							objects[i]->SetPosition(objects[i]->GetStartX(), objects[i]->GetStartY());
							objects[i]->SetDirection(player->nx);
							objects[i]->SetHealth(objects[i]->fullhealth);
						}
					}
				}
				else
				{
					if (objects[i]->checkObjInCamera(objects[i]) == false && objects[i]->isInCam == true)
					{
						objects[i]->SetFinish(true);
					}
				}
			}
#pragma endregion		
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
				GoldBrickDestroy(goldbrick->GetModel(), goldbrick->GetX(), goldbrick->GetY()-GB_BBOX_HEIGHT);
				goldbrick->isUnbox = false;
			}
		}
		if (e->GetType() == P_SWITCH)
		{
			P_Switch* pswitch = dynamic_cast<P_Switch*>(e);
			if (pswitch->isUsed)
			{
				for (UINT i = 0; i < objects.size(); i++)
				{
					if (objects[i]->GetType() == GOLD_BRICK)
					{
						GoldBrick* goldbrick = dynamic_cast<GoldBrick*>(objects[i]);
						if (goldbrick->GetModel()==GB_CONTAIN_COIN)
						{
							if (goldbrick->checkObjInCamera(objects[i]))
							{
								goldbrick->Transform();
							}
						//	objects.push_back(new CCoin(goldbrick->GetX(), goldbrick->GetY()));
						}
					}
				}
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
	if (!player->GetIsDeadth())
	{
		CheckCollision();
	}

	gametime->Update(dt);
	if (gametime->GetTime() >= GAME_TIME_LIMIT || player->GetHealth() <= 0)
	{
		if (player->GetIsDeadth())
		{
			return;
		}
		else
		{
			isGameOver = true;
			player->SetIsDeadth(true);
			player->SetState(MARIO_STATE_DIE);
		}
	}
	if (CGame::GetInstance()->GetScene() ==1)
	{
		CGame::GetInstance()->SetCamPos(0, 0);
		player->SetState(MARIO_STATE_WORLDMAP);
	}
	else
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
		if (cy - sh / 4 <= 0)//Top Edge
			cy = 0;
		else if (cy > mh / 2)//Bottom Edge
		{
			cy = mh - sh;
		}
		else cy -= sh / 4;
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
	if (CGame::GetInstance()->GetScene() != 1)
	{
		board = new Board(CGame::GetInstance()->GetCamX(), CGame::GetInstance()->GetCamY() + SCREEN_HEIGHT - DISTANCE_FROM_BOTTOM_CAM_TO_TOP_BOARD);
		board->Render(player, GAME_TIME_LIMIT - gametime->GetTime());
	}
}

/*
	Unload current scene
*/
void CPlayScene::Unload()
{
	for (UINT i = 0; i < objects.size(); i++)
	{
		delete objects[i];
	}
	for (UINT i = 0; i < ListItem.size(); i++)
	{
		delete ListItem[i];
	}
	for (UINT i = 0; i < ListEffect.size(); i++)
	{
		delete ListEffect[i];
	}
	for (UINT i = 0; i < ListPointEffect.size(); i++)
	{
		delete ListPointEffect[i];
	}
	for (UINT i = 0; i < listFireEnemy.size(); i++)
	{
		delete listFireEnemy[i];
	}
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
							player->SetHurt(e);
							player->SetLevel(player->level -= 1);
							isCollision = true;
						}
					}
					if (e->ny < 0)
					{
						if ((objects[i]->GetType() == PIRANHA_GREEN && objects[i]->GetState() != PLANT_STATE_HIDDING || objects[i]->GetType() == FIRE_PIRANHA && objects[i]->GetState() != PLANT_STATE_HIDDING))
						{
							player->SetHurt(e);
							player->SetLevel(player->level -= 1);
							isCollision = true;
						}
					}
					if (isCollision == false && player->checkAABB(gameobj) == true)
					{
						//LPCOLLISIONEVENT e = new CCollisionEvent(1.0f, player->GetDirection(), 0.0f, 0.1f, 0.1f, NULL);
						player->SetHurt(e);
						isCollision = true;
					}
				}
			}
		}
	}
}
void CPlayScenceKeyHandler::OnKeyDown(int KeyCode)
{
	//DebugOut(L"[INFO] KeyDown: %d\n", KeyCode);

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
			mario->SetState(MARIO_STATE_RACCOON_ATTACK);
			mario->TimeAttack = GetTickCount64();
			break;

		}
		else if (mario->level == MARIO_LEVEL_FIRE)
		{
			mario->SetState(MARIO_STATE_SHOOT_FIRE);
			mario->TimeAttack = GetTickCount64();
		}
		break;
	}
	case DIK_S:
	{
		mario->SetState(MARIO_STATE_JUMP);
		break;
	}
	}


}

void CPlayScenceKeyHandler::KeyState(BYTE* states)
{
	CGame* game = CGame::GetInstance();
	CMario* mario = ((CPlayScene*)scence)->GetPlayer();
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
	else if (game->IsKeyDown(DIK_RIGHT) || (game->IsKeyDown(DIK_DOWN)&& game->IsKeyDown(DIK_RIGHT)))
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
	else if (game->IsKeyDown(DIK_DOWN) && mario->GetLevel()!=MARIO_LEVEL_SMALL)
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
	if (CGame::GetInstance()->GetScene() == 1)
	{
		if (game->IsKeyDown(DIK_UP))
		{
			mario->vy = -0.05f;
		}
		if (game->IsKeyDown(DIK_DOWN))
		{
			mario->vy = 0.05f;
		}
	}

}
void  CPlayScenceKeyHandler::OnKeyUp(int KeyCode)
{
	//DebugOut(L"[INFO] KeyUp: %d\n", KeyCode);

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