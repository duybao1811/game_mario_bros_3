#include "Grid.h"

Grid::Grid()
{
}
void Grid::GetListObject(vector<LPGAMEOBJECT> ListObjects)
{
	ListObjects.clear();

	unordered_map<int, CGameObject*> mapObject;

	int bottom = (int)((CGame::GetInstance()->GetCamY() + SCREEN_HEIGHT - 1) / GRID_CELL_HEIGHT);
	int top = (int)((CGame::GetInstance()->GetCamY() + 1) / GRID_CELL_HEIGHT);

	int left = (int)((CGame::GetInstance()->GetCamX() + 1) / GRID_CELL_WIDTH);
	int right = (int)((CGame::GetInstance()->GetCamX() + SCREEN_WIDTH - 1) / GRID_CELL_WIDTH);

	for (int i = top; i <= bottom; i++)
		for (int j = left; j <= right; j++)
			for (UINT k = 0; k < cells[i][j].size(); k++)
			{
				if (cells[i][j].at(k)->isFinish == false) // còn tồn tại
				{
					ListObjects.push_back(cells[i][j][k]);
				}
			}

	for (auto& x : mapObject)
	{
		ListObjects.push_back(x.second);
	}
}

void Grid::Insert(LPGAMEOBJECT listobj)
{
	float l, t, r, b;
	listobj->GetBoundingBox(l, t, r, b);
	int top = (int)(t / GRID_CELL_HEIGHT);
	int bottom = (ceil)(b / GRID_CELL_HEIGHT);
	int left = (int)(l / GRID_CELL_WIDTH);
	int right = (ceil)(r / GRID_CELL_WIDTH);

	for (int i = top; i <= bottom; i++)
		for (int j = left; j <= right; j++)
			cells[i][j].push_back(listobj);
}
