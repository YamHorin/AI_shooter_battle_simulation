#include "ToolCarrierMovingState.h"
#include "ToolCarrierGivingToolsState.h"

void ToolCarrierMovingState::OnEnter(ToolCarrier* pn)
{
	pn->SetIsMoving(true);
	pn->SetIsGivingTools(false);
	pn->SetIsReloading(false);

}

void ToolCarrierMovingState::Transition(ToolCarrier* pn)
{
	int numRequests = pn->getNumberOfRequests();
	if (numRequests > 0)
	{
		pn->SetCurrentState(new ToolCarrierGivingToolsState());
		pn->GetCurrentState()->OnEnter(pn);
	}
	else
	{
		pn->SetCurrentState(new ToolCarrierMovingState());
		pn->GetCurrentState()->OnEnter(pn);
	}
}

void ToolCarrierMovingState::OnExit(ToolCarrier* pn)
{
}
