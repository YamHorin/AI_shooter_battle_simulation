#pragma once 
#include "ToolCarrier.h"
class ToolCarrier;

class ToolCarrierState
{
public:
	virtual void OnEnter(ToolCarrier* pn) = 0;
	virtual void Transition(ToolCarrier* pn) = 0; // to another state
	virtual void OnExit(ToolCarrier* pn) = 0;
};