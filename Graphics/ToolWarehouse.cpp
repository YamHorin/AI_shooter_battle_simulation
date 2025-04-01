#include "ToolWarehouse.h"
#include "definitions.h"

ToolWarehouse::ToolWarehouse(int x, int y, Team team)
{
	this->x = x;
	this->y = y;
	this->shieldLeft = FULL_SHIELD * 6;
	this->ammoLeft = FULL_AMMO * 6;
	this->teamToolWarehouse = team;
}
