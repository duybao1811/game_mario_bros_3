#pragma once
#include "Game.h"
#include "Textures.h"
#include "Scence.h"
#include "GameObject.h"
#include "Mario.h"
#include "Goomba.h"
#include "Koopas.h"
#include "Camera.h"
#include "Map.h"
#include "Item.h"
#include "MushRoom.h"
#include "Leaf.h"
#include "FirePiranhaPlant.h"
#include "Board.h"
class CPlayScene: public CScene
{
private:
	vector <Item*> ListItem;
	CMario* mario;
	bool isGameOver;
protected: 
	CMario *player;					// A play scene has to have player, right? 
	Map* map;
	Camera* camera;
	CMushRoom* mushroom;
	vector<CGameObject*> listObj;
	vector<LPGAMEOBJECT> objects;
	vector<CGameObject*> ListEnemy;
	void _ParseSection_TEXTURES(string line);
	void _ParseSection_SPRITES(string line);
	void _ParseSection_ANIMATIONS(string line);
	void _ParseSection_ANIMATION_SETS(string line);
	void _ParseSection_OBJECTS(string line);
	void _ParseSection_TILEMAP(string line);
	
public: 
	CPlayScene(int id, LPCWSTR filePath);

	virtual void Load();
	virtual void Update(DWORD dt);
	virtual void Render();
	virtual void Unload();

	CMario * GetPlayer() { return player; } 
	
	//friend class CPlayScenceKeyHandler;


	//Check collision
	void CheckCollision();
	void CheckCollistionMarioWithItem();
//	void CheckCollisionMarioWithEnemy();
};

class CPlayScenceKeyHandler : public CScenceKeyHandler
{
public: 
	virtual void KeyState(BYTE *states);
	virtual void OnKeyDown(int KeyCode);
	virtual void OnKeyUp(int KeyCode) ;
	CPlayScenceKeyHandler(CScene *s) :CScenceKeyHandler(s) {};
};

