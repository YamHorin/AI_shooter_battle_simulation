#include "ToolCarrierMovingState.h"
#include "ToolCarrierGivingToolsState.h"
#include "ToolCarrier.h"
#include "ToolCarrierReloadingState.h"
void ToolCarrierGivingToolsState::OnEnter(ToolCarrier* pn)
{
	pn->SetIsGivingTools(true);

}

void ToolCarrierGivingToolsState::Transition(ToolCarrier* pn)
{
	OnExit(pn);
	if (pn->getNumberOfRequests() > 0)
	{
		pn->SetCurrentState(new ToolCarrierGivingToolsState());
		pn->GetCurrentState()->OnEnter(pn);
	}
	else
	{
		int ammo = pn->GetAmmoLeft();
		int shield = pn->GetShieldLeft();
		if (ammo < FULL_AMMO/2 || shield < FULL_SHIELD/2)
		{
			pn->SetCurrentState(new ToolCarrierReloadingState());
			pn->GetCurrentState()->OnEnter(pn);
		}
		else
		{
			pn->SetCurrentState(new ToolCarrierMovingState());
			pn->GetCurrentState()->OnEnter(pn);
		}
	}
}

void ToolCarrierGivingToolsState::OnExit(ToolCarrier* pn)
{
	int currentAmmon = pn->GetAmmoLeft();
	pn->SetAmmoLeft(currentAmmon - FULL_AMMO);
	int currentShield = pn->GetShieldLeft();
	pn->SetShieldLeft(currentShield - FULL_SHIELD);
	pn->SetIsGivingTools(false);
}
