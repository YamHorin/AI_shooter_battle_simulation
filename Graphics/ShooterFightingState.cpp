#include "ShooterFightingState.h"
#include "ShooterLosingState.h"
#include "ShooterReloadingState.h"
#include "ShooterSurvivingState.h"
#include "Shooter.h"
#include "ShooterMovingState.h"
void ShooterFightingState::OnEnter(Shooter* pn)
{
	pn->SetIsFighting(true);
	pn->SetIsMoving(false);
	pn->SetIsReloading(false);

}

void ShooterFightingState::Transition(Shooter* pn)
{
	OnExit(pn);
	
	int health = pn->GetHealth();
	int ammo = pn->GetAmmo();
	int shield = pn->GetShield();
	int fightersNear = pn->GetFightersNear();
	bool isShooterScared = pn->GetIsShooterScared();
	if (fightersNear > 0 && health >0)
	{
		if (health < 40 )
		{
			pn->SetCurrentState(new ShooterSurvivingState());
			pn->GetCurrentState()->OnEnter(pn);
		}
		else if (shield <= FULL_SHIELD / 2 && isShooterScared) // if shooter is scared and has low shield
		{
			pn->SetCurrentState(new ShooterSurvivingState());
			pn->GetCurrentState()->OnEnter(pn);
		}
		else if (ammo < FULL_AMMO/2 && isShooterScared) // if shooter is scared and has low ammo
		{
			pn->SetCurrentState(new ShooterSurvivingState());
			pn->GetCurrentState()->OnEnter(pn);
		}
		else if (ammo ==0 )
		{
			pn->SetCurrentState(new ShooterReloadingState());
			pn->GetCurrentState()->OnEnter(pn);
		}

		else
		{
			pn->SetCurrentState(new ShooterFightingState());
			pn->GetCurrentState()->OnEnter(pn);
		}

		
	}
	else if (fightersNear == 0 && health > 0)
	{
		pn->SetCurrentState(new ShooterMovingState());
		pn->GetCurrentState()->OnEnter(pn);
	}
	else if (health <= 0)
	{
		pn->SetCurrentState(new ShooterLosingState());
		pn->GetCurrentState()->OnEnter(pn);
	}
	


}

void ShooterFightingState::OnExit(Shooter* pn)
{
	pn->SetIsFighting(false);
}
