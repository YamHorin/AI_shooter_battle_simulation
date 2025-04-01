#include "ShooterReloadingState.h"
#include "ShooterMovingState.h"
#include "definitions.h"
void ShooterReloadingState::OnEnter(Shooter* pn)
{
	pn->SetIsReloading(true);
	pn->SetIsMoving(false);
	pn->SetIsFighting(false);

}

void ShooterReloadingState::Transition(Shooter* pn)
{
	OnExit(pn);

	pn->SetShield(FULL_SHIELD);
	pn->SetAmmo(FULL_AMMO);
	pn->SetCurrentState(new ShooterMovingState());
	pn->GetCurrentState()->OnEnter(pn);

	
}

void ShooterReloadingState::OnExit(Shooter* pn)
{
	pn->SetIsReloading(false);
	pn->SetIsMoving(true);
	pn->SetIsFighting(false);
}
