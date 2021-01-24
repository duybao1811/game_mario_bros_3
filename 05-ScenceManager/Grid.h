#pragma once
#include "GameObject.h"
#define GRID_CELL_WIDTH (SCREEN_WIDTH/4.0f)
#define GRID_CELL_HEIGHT (SCREEN_HEIGHT/4.0f)

#define GRID_CELL_MAX_ROW 100 // số dòng tối đa;
#define GRID_CELL_MAX_COLUMN 100 // số cột tối đa

class Grid
{

public:
	Grid();
	vector<CGameObject*> cells[GRID_CELL_MAX_ROW][GRID_CELL_MAX_COLUMN];
	void Insert(LPGAMEOBJECT obj); //Thêm object vào grid
	void GetListObject(vector<LPGAMEOBJECT>);
};

