#include "ShooterLosingState.h"

void ShooterLosingState::OnEnter(Shooter* pn)
{
	pn->SetIsAlive(false);
}

void ShooterLosingState::Transition(Shooter* pn)
{
	pn->SetCurrentState(new ShooterLosingState());
	pn->GetCurrentState()->OnEnter(pn);
}

void ShooterLosingState::OnExit(Shooter* pn)
{
}
