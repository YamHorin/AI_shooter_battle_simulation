#pragma once
#include "ShooterState.h"
class ShooterLosingState:
	public ShooterState
{
	void OnEnter(Shooter* pn);
	void Transition(Shooter* pn); // to another state
	void OnExit(Shooter* pn);
};

