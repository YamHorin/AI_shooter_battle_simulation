#include "Shooter.h"
#include <time.h>
#include <iostream>
#include "ShooterState.h"
#include "ToolCarrier.h"
#include "Bullet.h"
#include "Grenade.h"
#include "definitions.h"
#include "random"
#include "ShooterMovingState.h"
#include "ShooterFightingState.h"
#include "ShooterSurvivingState.h"
#include "ShooterReloadingState.h"
using namespace std;

Shooter::Shooter(int x, int y, Team teamShooter)
{
	this->x = x;
	this->y = y;
	this->teamShooter = teamShooter;
	this->dx = 0;
	this->dy = 0;
	this->isMoving = true;
	this->isFighting = false;
	this->isReloading = false;
	this->isAlive = true;
	this->isTryingToSurvive = false;
	int randomNum = rand() % 2;
	if (randomNum == 1)
	{
		this->isShooterScared = true;
	}
	else
	{
		this->isShooterScared = false;
	}
	this->ToolCarrierFlag = false;
	this->ammo = FULL_AMMO;
	this->health = FULL_HEALTH;
	this->shield = FULL_SHIELD;
	this->SetCurrentState(new ShooterMovingState());
	this->GetCurrentState()->OnEnter(this);
	this->fightersNear = 0;
	this->flagCanShoot = false;
	this->flagStop = false;
	this->attackCooldown = 5;
	this->lastAttackTime = std::chrono::steady_clock::now();

}

bool Shooter::CanShoot()
{
	auto now = std::chrono::steady_clock::now();
	double elapsedSeconds = std::chrono::duration<double>(now - lastAttackTime).count();
	//cout << "attackCooldown" << attackCooldown << endl;
	//cout << "elapsedSeconds" << elapsedSeconds << endl;
	if (elapsedSeconds > attackCooldown) {
		lastAttackTime = now; // Reset the attack timer
		return true;
	}
	return false;
}



void Shooter::makeRequestToolCarrier(ToolCarrier* TC)
{
	//TODO
}

void Shooter::GotHitFromBullet()
{
	cout << "player team" << this->teamShooter << "got heat be a bullet\n";
	if (shield > 0)
	{
		shield -= bulletDamege;
	}
	else
	{
		this->health -= bulletDamege;
	}
}

void Shooter::GotHitFromGranade()
{
	cout << "player team" << this->teamShooter << "got heat be a granade\n";
	if (shield > 0)
	{
		shield -= grenadeDamege;
	}
	else
	{
		this->health -= grenadeDamege;

	}
}

void Shooter::DoSomeWork()
{
	
	if (isMoving)
	{
		
		if (fightersNear > 0)
		{
			this->GetCurrentState()->Transition(this);
		}
	}
	if (isFighting)
	{
		flagCanShoot = canAttack();
		this->GetCurrentState()->Transition(this);
	}
	if (isReloading && ToolCarrierFlag)
	{		
		this->GetCurrentState()->Transition(this);

	}
	if (isTryingToSurvive)
	{
		this->GetCurrentState()->Transition(this);
	}



}
void Shooter::enterANewRoom(int roomIndex)
{
	if (roomsQueue.size() <= NUM_ROOMS)
		roomsQueue.push(roomIndex);
	else
	{
		while (roomsQueue.size() > 0)
		{
			roomsQueue.pop();
		}
		roomsQueue.push(roomIndex);
	}
}
bool Shooter::findRoomInQueue(int roomIndex)
{
	queue <int> temp;
	bool isFound = false;
	while (!roomsQueue.empty())
	{
		temp.push(roomsQueue.front());
		if (roomsQueue.front() == roomIndex)
			isFound = true;
		roomsQueue.pop();
	}
	while (!temp.empty())
	{
		roomsQueue.push(temp.front());
		temp.pop();
	}
	return isFound;
}
bool Shooter::canAttack() {

	auto now = std::chrono::steady_clock::now();
	double elapsedSeconds = std::chrono::duration<double>(now - lastAttackTime).count();

	if (elapsedSeconds > attackCooldown) {
		lastAttackTime = now; // Reset the attack timer
		return true;
	}
	return false;
}

Bullet* Shooter::shootABullet(double xx, double yy, double angle)
{
	this->ammo -= COST_OF_SHOOT;
	Bullet* pb = new Bullet(xx,yy, angle);
	pb->setIsMoving(true);
	return pb;
}




Grenade* Shooter::shootAGrenade(double xx, double yy)
{
	this->ammo -= COST_OF_GRENADE;
	Grenade* pb = new Grenade(MSZ * (HEIGHT - y) / (double)HEIGHT, MSZ * x / (double)WIDTH);
	return pb;
}

Grenade* Shooter::shootAGrenade(int x, int y)
{
	this->ammo -= COST_OF_GRENADE;
	Grenade* pb = new Grenade((double) y, (double)x);
	return pb;
}
