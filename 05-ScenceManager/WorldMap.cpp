#include "WorldMap.h"
WorldMap::WorldMap(float X, float Y)
{
	this->x = X;
	this->y = Y;
}
void WorldMap::Render()
{
	ani = WORLD_MAP_ANI;
	animation_set->at(ani)->Render(x, y);
}