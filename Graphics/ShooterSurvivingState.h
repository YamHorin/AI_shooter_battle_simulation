#pragma once
#include "ShooterState.h"
class ShooterSurvivingState:
	public ShooterState
{
	void OnEnter(Shooter* pn);
	void Transition(Shooter* pn); // to another state
	void OnExit(Shooter* pn);
};

