#include "ShooterSurvivingState.h"
#include "ShooterReloadingState.h"
#include "ShooterLosingState.h"
void ShooterSurvivingState::OnEnter(Shooter* pn)
{
	pn->SetIsTryingToSurvive(true);
	pn->SetIsFighting(false);
	pn->SetIsMoving(false);
	pn->SetIsReloading(false);
}

void ShooterSurvivingState::Transition(Shooter* pn)
{
	int health = pn->GetHealth();
	int fightersNear = pn->GetFightersNear();
	if (fightersNear > 0 && health > 0)
	{

			pn->SetCurrentState(new ShooterSurvivingState());
			pn->GetCurrentState()->OnEnter(pn);
	}
	else if (fightersNear == 0 && health > 0)
	{
		pn->SetCurrentState(new ShooterReloadingState());
		pn->GetCurrentState()->OnEnter(pn);
	}
	else
	{
		pn->SetCurrentState(new ShooterLosingState());
		pn->GetCurrentState()->OnEnter(pn);
	}
}

void ShooterSurvivingState::OnExit(Shooter* pn)
{
	pn->SetIsTryingToSurvive(false);
}
