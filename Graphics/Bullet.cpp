#include "Bullet.h"
#include <math.h>
#include "glut.h"

// angle is in radians
Bullet::Bullet(double xx, double yy, double angle)
{
	x = xx;
	y = yy;
	dir_angle = angle;
	dirX = cos(angle);
	dirY = sin(angle);
	speed = 0.3;
	isMoving = false;
}

void Bullet::move(int maze[MSZ][MSZ] ,bool isSimulate)
{
	if (isMoving)
	{
		if (!isSimulate)
			maze[(int)x][(int)y] = BULLET;
			
		x += speed * dirX;
		y += speed * dirY;
		if (maze[(int)y][(int)x] == WALL || maze[(int)y][(int)x] == SHOOTER_BLUE || maze[(int)y][(int)x] == SHOOTER_RED)
			isMoving = false;
	}
}

void Bullet::move(int maze[MSZ][MSZ])
{
	if (isMoving)
	{
			maze[(int)x][(int)y] = BULLET;

		x += speed * dirX;
		y += speed * dirY;
		if (maze[(int)y][(int)x] == WALL || maze[(int)y][(int)x] == SHOOTER_BLUE || maze[(int)y][(int)x] == SHOOTER_RED)
			isMoving = false;
	}
}

void Bullet::show()
{
	glColor3d(1, 1, 0);
	glBegin(GL_POLYGON);
	glVertex2d(x - 0.5, y);
	glVertex2d(x , y+ 0.5);
	glVertex2d(x + 0.5, y);
	glVertex2d(x , y- 0.5);
	glEnd();
}

void Bullet::SimulateExplosion(int maze[MSZ][MSZ], double sm[MSZ][MSZ])
{
	isMoving = true;
	while (isMoving)
	{
		sm[(int)y][(int)x] += SECURITY_FACTOR;
		move(maze , true);
	}
}
