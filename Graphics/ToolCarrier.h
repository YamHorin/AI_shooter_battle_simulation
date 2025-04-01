#pragma once
#include "definitions.h"
#include "Shooter.h"
#include "ToolWarehouse.h"
#include <string.h>
#include <queue>
using namespace std;
class Shooter;
class ToolCarrierState;

class ToolCarrier
{
private:
	int x, y , targetCol, targetRow; // position
	Team teamToolCarrier;
	bool isMoving, isGivingTools, isReloading, isAlive;
	int health;
	int shieldLeft;
	int ammoLeft;
	ToolWarehouse* toolWarehouse;
	ToolCarrierState* pCurrentState;
	queue<Shooter*> requests;
	int enemyNear;
public:
	ToolCarrier(int x, int y , Team teamToolCarrier);
	ToolCarrier(int x, int y, Team teamToolCarrier , ToolWarehouse* toolWarehouse);
	void GotHitFromBullet();
	void GotHitFromGranade();
	int getNumberOfRequests() { return this->requests.size(); }
	void DoSomeWork();
	Shooter* GetTopRequest() { return this->requests.front(); }
	void getNewRequest(Shooter* newRequest);
	ToolWarehouse* GetToolWarehouse() { return this->toolWarehouse; }
	void SetToolWarehouse(ToolWarehouse* value) { this->toolWarehouse = value; }
	Team GetTeamToolCarrier() { return this->teamToolCarrier; }
	void SetIsMoving(bool value) { isMoving = value; }
	void SetIsGivingTools(bool value) { isGivingTools = value; }
	void SetIsReloading(bool value) { isReloading = value; }
	void SetCurrentState(ToolCarrierState* ps) { pCurrentState = ps; }
	void SetIsAlive(bool value) { this->isAlive = value; }
	bool GetIsAlive() { return this->isAlive; }
	bool GetIsMoving() { return this->isMoving; }
	bool GetIsGivingTools() { return this->isGivingTools; }
	bool GetIsReloading() { return this->isReloading; }
	int GetX() { return this->x; }
	int GetY() { return this->y; }
	void SetX(int value) { this->x = value; }
	void SetY(int value) { this->y = value; }
	void SetAmmoLeft(int value) { this->ammoLeft = value; }
	int GetAmmoLeft() { return this->ammoLeft; }
	void SetHealth(int value) { this->health = value; }
	int GetHealth() { return this->health; }
	void SetShieldLeft(int value) { this->shieldLeft = value; }
	int GetShieldLeft() { return this->shieldLeft; }
	int GetEnemyNear() { return this->enemyNear; }
	int GetTargetRow() { return this->targetRow; }
	int GetTargetCol() { return this->targetCol; }
	void SetTargetRow(int value) { this->targetRow = value; }
	void SetTargetCol(int value) { this->targetCol = value; }

	void SetEnemyNear(int value) { this->enemyNear = value; }
	ToolCarrierState* GetCurrentState() { return this->pCurrentState; };
};