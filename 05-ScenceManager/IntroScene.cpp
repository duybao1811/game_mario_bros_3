#include "IntroScene.h"
#include "Utils.h"
#include "Textures.h"
#include "Sprites.h"
#include "Portal.h"
#include "Mario.h"
#include <string>
#include <cstring>
#include <iostream>
#include <fstream>
#include "Map.h"
#include "Floor.h"
#include "Logo.h"
#include "BackGroundIntro.h"
#include "Number3.h"
#include "GroundIntro.h"
#include "Cursor.h"
using namespace std;

IntroScene::IntroScene(int id, LPCWSTR filePath) :
	CScene(id, filePath)
{
	key_handler = new CIntroSceneKeyHandler(this);
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
void IntroScene::_ParseSection_TEXTURES(string line)
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
void IntroScene::_ParseSection_SPRITES(string line)
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

void IntroScene::_ParseSection_ANIMATIONS(string line)
{
	vector<string> tokens = split(line);

	if (tokens.size() < 3) return; // skip invalid lines - an animation must at least has 1 frame and 1 frame time

	//DebugOut(L"--> %s\n",ToWSTR(line).c_str());

	LPANIMATION ani = new CAnimation();

	int ani_id = atoi(tokens[0].c_str());
	for (int i = 1; i < tokens.size(); i += 2)	// why i+=2 ?  sprite_id | frame_time  
	{
		int sprite_id = atoi(tokens[i].c_str());
		int frame_time = atoi(tokens[i + 1].c_str());
		ani->Add(sprite_id, frame_time);
	}

	CAnimations::GetInstance()->Add(ani_id, ani);
}

void IntroScene::_ParseSection_ANIMATION_SETS(string line)
{
	vector<string> tokens = split(line);

	if (tokens.size() < 2) return; // skip invalid lines - an animation set must at least id and one animation id

	int ani_set_id = atoi(tokens[0].c_str());

	LPANIMATION_SET s = new CAnimationSet();

	CAnimations* animations = CAnimations::GetInstance();

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
void IntroScene::_ParseSection_OBJECTS(string line)
{
	vector<string> tokens = split(line);
	//DebugOut(L"--> %s\n",ToWSTR(line).c_str());

	if (tokens.size() < 3) return; // skip invalid lines - an object set must have at least id, x, y

	int object_type = atoi(tokens[0].c_str());
	float x = atof(tokens[1].c_str());
	float y = atof(tokens[2].c_str());
	int ani_set_id = atoi(tokens[3].c_str());


	CAnimationSets* animation_sets = CAnimationSets::GetInstance();

	CGameObject* obj = NULL;
	switch (object_type)
	{
	case OBJECT_TYPE_MARIO:
		/*	if (player!=NULL)
			{
				DebugOut(L"[ERROR] MARIO object was created before!\n");
				return;
			}*/
		obj = new CMario(x, y);
		player = (CMario*)obj;

		DebugOut(L"[INFO] Player object created!\n");
		break;



	case OBJECT_TYPE_FLOOR:
	{
		obj = new Floor(x, y);
		break;
	}
	case 17:
	{
		obj = new Logo(x, y);
		break;
	}
	case 18:
	{
		obj = new GroundIntro(x, y);
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
}
void IntroScene::_ParseSection_TILEMAP(string line)
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
void IntroScene::Load()
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
void IntroScene::Update(DWORD dt)
{
	CGame* game = CGame::GetInstance();
	CGameObject* obj = NULL;

	vector<LPGAMEOBJECT> coObjects;
	for (size_t i = 1; i < objects.size(); i++)
	{
		coObjects.push_back(objects[i]);
	}
	for (size_t i = 0; i < objects.size(); i++)
	{
		objects[i]->Update(dt, &coObjects);
	}
	for (size_t i = 1; i < listObjectsIntro.size(); i++)
	{
		coObjects.push_back(listObjectsIntro[i]);
	}
	for (size_t i = 0; i < listObjectsIntro.size(); i++)
	{
		listObjectsIntro[i]->Update(dt, &coObjects);
	}

	for (size_t i = 0; i < objects.size(); i++)
	{
		if (objects[i]->GetType() == FLOOR)
		{
			Floor* floor = dynamic_cast<Floor*>(objects[i]);
			if (floor->y + FLOOR_HEIGHT >= CGame::GetInstance()->GetCamY())
			{
				floor->vy = -FLOOR_SPEED_Y;
			}
			else
				section = 2;
		}
	}
	if (section == 2)
	{
		for (size_t i = 0; i < objects.size(); i++)
		{
			if (objects[i]->GetType() == LOGO)
			{
				Logo* logo = dynamic_cast<Logo*>(objects[i]);
				logo->vy = LOGO_SPEED;
				if (logo->y >= POSITION_LOGO_IN_INTRO)
				{
					logo->vy = 0;
					logo->y = POSITION_LOGO_IN_INTRO;
					TimeSection2 += dt;
				}
			}
		}
	}
	if (TimeSection2 >500 )
	{
		section = 3;
	}
	if (section == 3)
	{
		listObjectsIntro.push_back(new BackGroundIntro(0, 0));
		listObjectsIntro.push_back(new Number3(112, 96));
		listObjectsIntro.push_back(new Cursor(70, 144));
		for (size_t i = 0; i < objects.size(); i++)
		{
			objects.erase(objects.begin() + i);
		}
	}
	
}

void IntroScene::Render()
{

	map->DrawMap();

	for (int i = 0; i < objects.size(); i++)
	{
		if (objects[i]->GetType() != MARIO)
		{
			objects[i]->Render();
		}
	}
	for (int i = 0; i < listObjectsIntro.size(); i++)
	{
		listObjectsIntro[i]->Render();
	}
}

/*
	Unload current scene
*/
void IntroScene::Unload()
{
	objects.clear();

	DebugOut(L"[INFO] Scene %s unloaded! \n", sceneFilePath);
}
void CIntroSceneKeyHandler::OnKeyDown(int KeyCode)
{
	//DebugOut(L"[INFO] KeyDown: %d\n", KeyCode);

	switch (KeyCode)
	{
	}
}

void CIntroSceneKeyHandler::KeyState(BYTE* states)
{
	CGame* game = CGame::GetInstance();
	CMario* mario = ((IntroScene*)scence)->GetPlayer();
	// disable control key when Mario die 
	if (((IntroScene*)CGame::GetInstance()->GetCurrentScene())->section != 3)
	{
		if (game->IsKeyDown(DIK_S))
		{
			((IntroScene*)CGame::GetInstance()->GetCurrentScene())->section = 3;
		}
	}
	if (((IntroScene*)CGame::GetInstance()->GetCurrentScene())->section == 3)
	{
		if (game->IsKeyDown(DIK_X))
		{
			CGame::GetInstance()->SwitchScene(WORLD_MAP);
		}
	}
}
void  CIntroSceneKeyHandler::OnKeyUp(int KeyCode)
{
	//DebugOut(L"[INFO] KeyUp: %d\n", KeyCode);

	CMario* mario = ((IntroScene*)scence)->GetPlayer();
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