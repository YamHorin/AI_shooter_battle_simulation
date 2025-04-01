#pragma once

const int WIDTH = 700;
const int HEIGHT = 700;

const int NUM_ROOMS = 12;

const double WALL_COST = 5;
const double SPACE_COST = 1;

const int MSZ = 100;

const int SPACE = 1;
const int WALL = 0;
const double PI = 3.14;
const double SECURITY_FACTOR = 0.001;

const int NUM_OF_SHOOTERS = 4;

const int bulletDamege = 20;
const int grenadeDamege = 5;
const int FULL_HEALTH = 100;	
const int FULL_SHIELD = 100;

const int FULL_AMMO_TC = 150;
const int FULL_SHIELD_TC = 500;
const int FULL_AMMO = 30;
const int COST_OF_SHOOT = 10;
const int COST_OF_GRENADE = 50;

const int SHOOTER_HEALTH = 100;
const int TOOL_CARRIER_HEALT = 100;

enum Team { BLUE, RED };
enum Target { WAREHOUSE, SHOOTER };

const int SHOOTER_BLUE = 2;

const int SHOOTER_RED = 5;

const int TOOL_CARRIER = 3;
const int TOOL_WAREHOUSE = 4;


const int GRAY_BLUE_TEAM = 6;
const int GRAY_RED_TEAM = 7;

const int BLACK_BLUE_TEAM = 8;
const int BLACK_RED_TEAM = 9;

const int PATH = 10;

const int MIDDLE_PART_OF_THE_ROOM = 11;
const int BULLET = 12;