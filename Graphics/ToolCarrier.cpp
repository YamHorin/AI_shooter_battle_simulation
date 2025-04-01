#include "ToolCarrier.h"
#include "definitions.h"
#include "ToolCarrierGivingToolsState.h"
#include "ToolCarrierMovingState.h"
#include "ToolCarrierReloadingState.h"
#include "ToolWarehouse.h"
#include <iostream>
using namespace std;

ToolCarrier::ToolCarrier(int x, int y, Team teamToolCarrier)
{
	this->x = x;
	this->y = y;
	this->targetCol = 0;
	this->requests = queue<Shooter*>();
	this->targetRow = 0;
	this->teamToolCarrier = teamToolCarrier;
	this->isMoving = true;
	this->isGivingTools = false;
	this->isReloading = false;
	this->isAlive = true;
	this->health = FULL_HEALTH;
	this->shieldLeft = FULL_SHIELD_TC;
	this->ammoLeft = FULL_AMMO_TC;
	this->pCurrentState = new ToolCarrierMovingState();
	this->pCurrentState->OnEnter(this);
	this->enemyNear = 0;
	this->toolWarehouse = nullptr;

}
ToolCarrier::ToolCarrier(int x, int y, Team teamToolCarrier, ToolWarehouse* toolWarehouse)
{
	this->x = x;
	this->y = y;
	this->targetCol = 0;
	this->targetRow = 0;
	this->teamToolCarrier = teamToolCarrier;
	this->isMoving = true;
	this->isGivingTools = false;
	this->isReloading = false;
	this->isAlive = true;
	this->health = FULL_HEALTH;
	this->shieldLeft = FULL_SHIELD_TC;
	this->ammoLeft = FULL_AMMO_TC;
	this->pCurrentState = new ToolCarrierMovingState();
	this->pCurrentState->OnEnter(this);
	this->enemyNear = 0;
	this->toolWarehouse = toolWarehouse;
}
void ToolCarrier::GotHitFromBullet()
{
	if (shieldLeft > 0)
	{
		shieldLeft -= bulletDamege;
	}
	else
	{
		health -= bulletDamege;
	}
}

void ToolCarrier::GotHitFromGranade()
{
	if (shieldLeft > 0)
	{
		shieldLeft -= grenadeDamege;
	}
	else
	{
		health -= grenadeDamege;
	}
}

void ToolCarrier::DoSomeWork()
{
	if (isMoving)
	{
		if (requests.size() > 0)
		{
			cout << "player team" << this->teamToolCarrier << "is moving\n";
			this->pCurrentState->Transition(this);
		}
	}
	if (isGivingTools)
	{
		this->targetCol = requests.front()->GetX();
		this->targetRow = requests.front()->GetY();
		Shooter* shooter = requests.front();
		if (this->x == targetCol && this->y == targetRow)
		{
			cout << "player team" << this->teamToolCarrier << "is giving tools\n";
			shooter->SetAmmo(FULL_AMMO);
			shooter->SetShield(FULL_SHIELD);
			this->ammoLeft -= FULL_AMMO;
			this->shieldLeft -= FULL_SHIELD;
			requests.pop();
			this->pCurrentState->Transition(this);
		}

	}
	if (isReloading)
	{
		if (x == toolWarehouse->GetX() && this->y == toolWarehouse->GetY())
		{
			int diffrenceAmmo = FULL_AMMO_TC - this->ammoLeft;
			int diffrenceShield = FULL_SHIELD_TC - this->shieldLeft;
			if (toolWarehouse->GetAmmoLeft() >= diffrenceAmmo && toolWarehouse->GetShieldLeft() >= diffrenceShield)
			{
				toolWarehouse->GetAmmoFromWarehouse(diffrenceAmmo);
				toolWarehouse->GetShieldFromWarehouse(diffrenceShield);
				cout << "player team" << this->teamToolCarrier << "is reloading\n";
				this->ammoLeft = FULL_AMMO_TC;
				this->shieldLeft = FULL_SHIELD_TC;
				this->pCurrentState->Transition(this);
			}


		}
	}

}

void ToolCarrier::getNewRequest(Shooter* newRequest)
{
	bool isExist = false;
	queue<Shooter*> temp;
	while (!requests.empty())
	{
		if (requests.front() == newRequest)
		{
			isExist = true;
		}
		temp.push(requests.front());
		requests.pop();
	}
	while (!temp.empty())
	{
		requests.push(temp.front());
		temp.pop();
	}
	if (!isExist)
	{
		requests.push(newRequest);
	}
}






