#pragma once 
#include "ToolCarrierState.h"
class ToolCarrierMovingState :
	public ToolCarrierState
{
	void OnEnter(ToolCarrier* pn);
	void Transition(ToolCarrier* pn); // to another state
	void OnExit(ToolCarrier* pn);
};