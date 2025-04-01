#pragma once
class Player
{
private:
	double x, y;
	double dir_angle;
	double dirX, dirY;
	double speed;
	bool isMoving;
	int health;
	int ammo;

public:
	Player(double xx, double yy, double angle);
	void doSomeWork();
	void move(int maze[MSZ][MSZ]);
	void show();
	void setIsMoving(bool value) { isMoving = value; }
	void setHealth(int h) { health = h; }
	int getHealth() { return health; }
	void setAmmo(int a) { ammo = a; }
	int getAmmo() { return ammo; }
};

