#pragma once
#include "shooter.h"
class Shooter;
class ShooterState
{
public:
	virtual void OnEnter(Shooter* pn) = 0;
	virtual void Transition(Shooter* pn) = 0; // to another state
	virtual void OnExit(Shooter* pn) = 0;
};

