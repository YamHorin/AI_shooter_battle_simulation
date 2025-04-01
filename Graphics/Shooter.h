#pragma once
#include "definitions.h"
#include "ToolCarrier.h"
#include "Bullet.h"
#include "Grenade.h"
#include <string.h>
#include <chrono>
#include "ShooterState.h"
#include <queue>
class ShooterState;
class ToolCarrier;
using namespace std;
class Shooter
{
private:
	int x, y; // position
	Team teamShooter;
	double dx, dy; // direction
	bool isMoving, isFighting, isReloading, isAlive , isTryingToSurvive;
	bool isShooterScared ,ToolCarrierFlag;
	bool flagCanShoot , flagStop;
	std::chrono::steady_clock::time_point lastAttackTime; // time of last attack
	double attackCooldown;
	int fightersNear;
	int health;
	int shield;
	int ammo;
	queue <int> roomsQueue;
	int roomIndexTarger;

	ShooterState* pCurrentState;
	public:
		Shooter(int x, int y  , Team teamShooter);
		bool CanShoot();
		void SetCanShoot(bool value) { flagCanShoot = value; }
		void makeRequestToolCarrier(ToolCarrier* TC);
		void GotHitFromBullet();
		void GotHitFromGranade();
		void DoSomeWork();
		void SetRoomIndexTarget(int value) { this->roomIndexTarger = value; }
		int GetRoomIndexTarget() { return this->roomIndexTarger; }
		void enterANewRoom(int roomIndex);
		bool findRoomInQueue(int roomIndex);
		bool canAttack();
		void SetStopFlag(bool value) { flagStop = value; }
		bool GetStopFlag() { return flagStop; }
		double GetAttackCooldown() { return this->attackCooldown; };
		void SetAttackCooldown(double value) { this->attackCooldown = value; }
		Bullet* shootABullet(double xx, double yy, double angle);
		Grenade* shootAGrenade(double xx, double yy);
		Grenade* shootAGrenade(int x, int y);

		void SetToolCarruerFlag(bool value) { ToolCarrierFlag = value; }	
		bool GetToolCarrierFlag() { return ToolCarrierFlag; }
		void SetTeamShooter(Team teamShooter) { this->teamShooter = teamShooter; }
		Team GetTeamShooter() { return this->teamShooter; }
		void SetIsShooterScared(bool value) { this->isShooterScared = value; }
		bool GetIsShooterScared() { return this->isShooterScared; }
		void SetFightersNear(int value) { fightersNear = value; }
		void SetIsAlive(bool value) { this->isAlive = value; }
		bool GetIsAlive() { return this->isAlive; }
		void SetIsMoving(bool value) { isMoving = value; }
		void SetIsFighting(bool value) { isFighting = value; }
		void SetIsReloading(bool value) { isReloading = value; }
		void SetCurrentState(ShooterState* ps) { pCurrentState = ps; }
		void SetIsTryingToSurvive(bool value) { this->isTryingToSurvive = value; }
		bool GetIsTryingToSurvive() { return this->isTryingToSurvive; }
		ShooterState* GetCurrentState() { return pCurrentState; }
		bool GetIsMoving() { return this->isMoving; }
		bool GetIsFighting() { return this->isFighting; }
		bool GetIsReloading() { return this->isReloading; }
		int GetFightersNear() { return this->fightersNear; }
		int GetX() { return this->x; }
		int GetY() { return this->y; }
		void SetX(int value) { this->x = value; }
		void SetY(int value) { this->y = value; }
		void SetAmmo(int value) { this->ammo = value; }
		int GetAmmo() { return this->ammo; }
		void SetHealth(int value) { this->health = value; }
		int GetHealth() { return this->health; }
		void SetShield(int value) { this->shield = value; }
		int GetShield() { return this->shield; }
		void SetDx(double value) { this->dx = value; }
		void SetDy(double value) { this->dy = value; }
		double GetDx() { return this->dx; }
		double GetDy() { return this->dy; }










};

