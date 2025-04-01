#include "Grenade.h"

Grenade::Grenade(double r, double c)
{
	int i;
	row = r;
	col = c;
	double alpha = 2*PI/NUM_BULLETS; // bullet separation angle
	for (i = 0;i < NUM_BULLETS;i++)
	{
		bullets[i] = new Bullet(c, r, i * alpha);
	}
}

void Grenade::explode()
{
	int i;

	for (i = 0;i < NUM_BULLETS;i++)
	{
		bullets[i]->setIsMoving(true);
	}


}

void Grenade::expand(int maze[MSZ][MSZ])
{
	int i;

	for (i = 0;i < NUM_BULLETS;i++)
	{
		bullets[i]->move(maze);
	}

}

void Grenade::show()
{
	int i;

	for (i = 0;i < NUM_BULLETS;i++)
	{
		bullets[i]->show();
	}

}

void Grenade::SimulateExplosion(int maze[MSZ][MSZ], double sm[MSZ][MSZ])
{
	int i;

	for (i = 0;i < NUM_BULLETS;i++)
	{
		bullets[i]->SimulateExplosion(maze,sm);
	}

}
