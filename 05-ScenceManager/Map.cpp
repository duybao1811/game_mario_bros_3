#include "Map.h"
#include <fstream>
#include <iostream>
#include "Textures.h"
Map::Map(int TexID, int RowMap, int ColumnMap, int RowTile, int ColumnTile, int TotalTile)
{
	Tex = CTextures::GetInstance()->Get(TexID);
	this->RowMap = RowMap;
	this->ColumnMap = ColumnMap;
	this->RowTile = RowTile;
	this->ColumnTile = ColumnTile;
	this->TotalTile = TotalTile;
	CamX = CamY = 0;
//	TileMap = NULL;
}
Map::~Map(){}
void Map::SetMap(int** tilemap)
{
	TileMap = tilemap;
}
void Map::DrawMap()
{
/*	int row = floor (CamY / TILE_HEIGHT);
	int column = floor(CamX / TILE_WIDTH);
	int TotalHeight = ceil((CGame::GetInstance()->GetScreenHeight()) / TILE_HEIGHT);
	int TotalWidth = ceil((CGame::GetInstance()->GetScreenWidth()) / TILE_WIDTH);
	float x = -(float)((int)(CamX) % TILE_HEIGHT);
	float y = -(float)((int)(CamY) % TILE_HEIGHT);
	for (int i = 0; i < RowMap; i++)
		for (int j = column; j <=TotalWidth; j++)
		{
			int IDFrame = TileMap[i][j] - 1;
			if (!(row + i < 0 || row + i >= RowMap || j + column < 0 || j + column > ColumnMap))
				Tiles.at(IDFrame)->Draw(i*TILE_WIDTH, j * TILE_HEIGHT);
		}*/
	int FirstColumn = floor(CamX / TILE_WIDTH);
	int LastColumn = ceil((CamX + CGame::GetInstance()->GetScreenWidth()) / TILE_WIDTH);
	if (LastColumn >= RowMap)
		LastColumn = ColumnMap- 1;
	for (int CurrentRow = 0; CurrentRow < RowMap; CurrentRow++)
		for (int CurrentColumn = 0; CurrentColumn <= LastColumn; CurrentColumn++)
		{
			int index = TileMap[CurrentRow][CurrentColumn] - 1;
			if (index < TotalTile)
				Tiles.at(index)->Draw(CurrentColumn * TILE_WIDTH, CurrentRow * TILE_HEIGHT);
		}
}
void Map::GetSpriteTile()
{
	//DebugOut(L"%d %d \n", FrameW, FrameH);
	for (int id = 0; id < TotalTile; id++)
	{
		{
			int left = id % ColumnTile * TILE_WIDTH;
			int top = id / ColumnTile * TILE_HEIGHT;
			int right = left + TILE_WIDTH;
			int bottom = top + TILE_HEIGHT;
			LPSPRITE SpriteTile = new CSprite(id, left, top, right, bottom, Tex);
			this->Tiles.push_back(SpriteTile);
			//CGame::GetInstance()->Draw(j * TILE_WIDTH, i * TILE_HEIGHT, Tex, r.left, r.top, r.right, r.bottom);
		}
		//DebugOut(L"\n");
	}
}
int Map::GetMapHeight()
{
	return RowMap * TILE_HEIGHT;
}
int Map::GetMapWidth()
{
	return ColumnMap * TILE_WIDTH;
}


