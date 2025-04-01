#include "ToolCarrierReloadingState.h"
#include "ToolCarrierMovingState.h"
void ToolCarrierReloadingState::OnEnter(ToolCarrier* pn)
{
	pn->SetIsReloading(true);
}

void ToolCarrierReloadingState::Transition(ToolCarrier* pn)
{
	OnExit(pn);
	pn->SetCurrentState(new ToolCarrierMovingState());
	pn->GetCurrentState()->OnEnter(pn);
}

void ToolCarrierReloadingState::OnExit(ToolCarrier* pn)
{
	pn->SetAmmoLeft(FULL_AMMO_TC);
	pn->SetShieldLeft(FULL_SHIELD_TC);
	pn->SetIsReloading(false);


}
