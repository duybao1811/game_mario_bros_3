#pragma once
#include "Game.h"
#include "Textures.h"
#include "Scence.h"
#include "GameObject.h"
#include "Mario.h"
#include "Map.h"
class IntroScene : public CScene
{
private:
	DWORD TimeTranform = 0;
	float CamX = 0, CamY = 0;

	DWORD TimeSection2=0;
protected:
	Map* map;
	CMario* player;					// A play scene has to have player, right? 
	vector<LPGAMEOBJECT> objects;
	vector<LPGAMEOBJECT> listObjectsIntro;
	void _ParseSection_TEXTURES(string line);
	void _ParseSection_SPRITES(string line);
	void _ParseSection_ANIMATIONS(string line);
	void _ParseSection_ANIMATION_SETS(string line);
	void _ParseSection_OBJECTS(string line);
	void _ParseSection_TILEMAP(string line);

public:
	int section;
	int timegame;
	bool isGameDone;
	IntroScene(int id, LPCWSTR filePath);

	virtual void Load();
	virtual void Update(DWORD dt);
	virtual void Render();
	virtual void Unload();

	CMario* GetPlayer() { return player; }

	//friend class CPlayScenceKeyHandler;


};

class CIntroSceneKeyHandler : public CScenceKeyHandler
{
public:
	virtual void KeyState(BYTE* states);
	virtual void OnKeyDown(int KeyCode);
	virtual void OnKeyUp(int KeyCode);
	CIntroSceneKeyHandler(CScene* s) :CScenceKeyHandler(s) {};
};

