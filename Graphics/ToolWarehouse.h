#pragma once 
#include "definitions.h"
class ToolWarehouse
{
private:
	Team teamToolWarehouse;
	int x, y;
	int shieldLeft;
	int ammoLeft;
public:
	ToolWarehouse(int x, int y , Team team);
	Team getTeamToolWarehouse() { return this->teamToolWarehouse; }
	
	int GetX() { return this->x; }
	int GetY() { return this->y; }
	int getRow() { return this->x; }
	int getCol() { return this->y; }
	void SetX(int value) { this->x = value; }
	void SetY(int value) { this->y = value; }
	void GetAmmoFromWarehouse(int amount) { this->ammoLeft -= amount; }
	int GetAmmoLeft() { return this->ammoLeft; }
	void GetShieldFromWarehouse(int amount) { this->shieldLeft -= amount; }
	int GetShieldLeft() { return this->shieldLeft; }
};