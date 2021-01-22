#pragma once
#include "Game.h"
#include "Textures.h"
#include "Scence.h"
#include "GameObject.h"
#include "Mario.h"
#include "Goomba.h"
#include "Koopas.h"
#include "Map.h"
#include "Item.h"
#include "MushRoom.h"
#include "Leaf.h"
#include "FirePiranhaPlant.h"
#include "Board.h"
#include "QuestionBrick.h"
#include "FirePlant.h"
#include "Boomerang.h"
#include "GameTime.h"
class CPlayScene : public CScene
{
private:
	vector <Item*> ListItem;
	bool isGameOver;
	GameTime* gametime = new GameTime();
	int CountEnemy;
	float CamX = 0, CamY = 0;
	Font* Text;
	bool isSwitchScene;
	DWORD TimeSwitchScene =0;
	DWORD TimeSwitchScene1 = 0;
protected:
	CMario* player;					// A play scene has to have player, right?
	CGame* game = CGame::GetInstance();
	Map* map;
	Board* board;
	vector<LPGAMEOBJECT> objects;
	vector<LPGAMEOBJECT> ListEffect;
	vector<LPGAMEOBJECT> ListPointEffect;
	vector<FirePlant*> listFireEnemy;
	vector<Boomerang*>listBoomerangEnemy;
	void _ParseSection_TEXTURES(string line);
	void _ParseSection_SPRITES(string line);
	void _ParseSection_ANIMATIONS(string line);
	void _ParseSection_ANIMATION_SETS(string line);
	void _ParseSection_OBJECTS(string line);
	void _ParseSection_TILEMAP(string line);

public:
	int section;
	CPlayScene(int id, LPCWSTR filePath);

	virtual void Load();
	virtual void Update(DWORD dt);
	virtual void Render();
	virtual void Unload();

	CMario* GetPlayer() { return player; }

	//friend class CPlayScenceKeyHandler;


	//Check collision
	void CheckCollision();
	void CheckCollistionMarioWithItem();
	void QuestionBrickDropItem(int model, float x, float y);
	void GoldBrickDestroy(int model, float x, float y);
	void CheckCollisionMarioWithEnemy();
};

class CPlayScenceKeyHandler : public CScenceKeyHandler
{
public:
	virtual void KeyState(BYTE* states);
	virtual void OnKeyDown(int KeyCode);
	virtual void OnKeyUp(int KeyCode);
	CPlayScenceKeyHandler(CScene* s) :CScenceKeyHandler(s) {};
};

