#include "ShooterMovingState.h"
#include "Shooter.h"
#include "ShooterFightingState.h"
#include "ShooterSurvivingState.h"
void ShooterMovingState::OnEnter(Shooter* pn)
{
	pn->SetIsMoving(true);
	pn->SetIsFighting(false);
	pn->SetIsTryingToSurvive(false);
	pn->SetIsReloading(false);
}

void ShooterMovingState::Transition(Shooter* pn)
{
	OnExit(pn);

	if(pn->GetHealth()<40)
	{
		pn->SetCurrentState(new ShooterSurvivingState());
		pn->GetCurrentState()->OnEnter(pn);
	}
	pn->SetCurrentState(new ShooterFightingState());
	pn->GetCurrentState()->OnEnter(pn);

}

void ShooterMovingState::OnExit(Shooter* pn)
{
	pn->SetIsMoving(false);
	
}
