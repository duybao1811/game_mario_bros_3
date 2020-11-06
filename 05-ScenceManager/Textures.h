#pragma once
#include <unordered_map>
#include <d3dx9.h>

using namespace std;

/*
	Manage texture database
*/
class CTextures
{
	int FrameWidth, FrameHeight;
	static CTextures * __instance;

	unordered_map<int, LPDIRECT3DTEXTURE9> textures;

public: 
	int GetFrameWidth();
	int GetFrameHeight();
	CTextures();
	void Add(int id, LPCWSTR filePath, D3DCOLOR transparentColor);
	LPDIRECT3DTEXTURE9 Get(unsigned int i);

	void Clear();
	static CTextures * GetInstance();
};