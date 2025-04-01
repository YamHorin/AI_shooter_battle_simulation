
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include "glut.h"
#include <queue>
#include "Cell.h"
#include <iostream>
#include "Room.h"
#include "CompareCells.h"
#include "Bullet.h"
#include "Grenade.h"
#include "definitions.h"
#include "Shooter.h"
#include "ToolCarrier.h"

using namespace std;



Room* rooms[NUM_ROOMS];

bool bulletFired = false;
bool grenadeThrown = false;
Bullet* pb=nullptr;
Grenade* pg = nullptr;

queue <Bullet*> bullets;
queue <Grenade*> grenades;
queue <int> roomsQueue;
Cell* ShooterArr[NUM_OF_SHOOTERS];
Cell* ToolCarrierArr[2];

Shooter* shooters[NUM_OF_SHOOTERS];
ToolCarrier* toolCarriers[2];
ToolWarehouse* tool_Ware_houses[2] ;


int maze[MSZ][MSZ] = { 0 }; // WALLs
double security_map[MSZ][MSZ] = {0}; 

priority_queue <Cell*, vector <Cell*>, CompareCells> Steps;
priority_queue <Cell*, vector <Cell*>, CompareCells> StepsToolCarrier;

void ShowDungeon();
void GenerateSecurityMap();
int getNumOfEnemiesInTheRoom(int roomIndex, Team teamShooter);
int whitchRoomAmI(int row, int col);
void UpdatePQ_AStar(Cell* pNeighbor, priority_queue<Cell*, vector<Cell*>, CompareCells>& pq);
void playGameToolCarriers();
bool checkForBulletHit(int row, int col, int shooterNum);


void delay(double number_of_seconds)
{
	// Converting time into milli_seconds
	double milli_seconds = 1000 * number_of_seconds;

	// Storing start time
	clock_t start_time = clock();

	// looping till required time is not achieved
	while (clock() < start_time + milli_seconds);
}



double caculateHeuristic(int row, int col, int targetRow, int targetCol)
{
	return abs(row - targetRow) + abs(col - targetCol);
}

double caculateDiatanceBetWeenCenterRoomToPoint(int row, int col)
{
	double min = 1000;
	double distance;
	int roomIndex = whitchRoomAmI(row, col);
	for (int i = 0; i < NUM_ROOMS; i++)
	{

		distance = caculateHeuristic(row, col, rooms[i]->getCenterY(), rooms[i]->getCenterX());
		if (distance < min)
			min = distance;
	}
	return min;

	
}
int getShooterNumberBaseOnRowAndCol(int row, int col)
{
	for (int i = 0; i < NUM_OF_SHOOTERS; i++)
	{
		if (ShooterArr[i]->getRow() == row && ShooterArr[i]->getCol() == col)
			return i;
	}
	return -1;
}


int getTOOLWareHouse(Team team)
{
	for (int i = 0; i < 2; i++)
	{
		if (tool_Ware_houses[i]->getTeamToolWarehouse() == team)
			return i;
	}
	return 2;
}

int getTC(Team team)
{
	for (int i = 0; i < 2; i++)
	{
		if (toolCarriers[i]->GetTeamToolCarrier() == team)
			return i;
	}
	return 2;
}


void MoveOneBlockTC(int row, int col, Cell* pc, int TCNum)
{
	while (pc != nullptr)
	{
		if (pc->getParent() == nullptr)
		{
			maze[row][col] = SPACE;
			while (!StepsToolCarrier.empty())
				StepsToolCarrier.pop();
		}
		else if (pc->getParent()->getParent() == nullptr)
		{
			maze[pc->getRow()][pc->getCol()] = TOOL_CARRIER;
			maze[pc->getParent()->getRow()][pc->getParent()->getCol()] = SPACE;
			ToolCarrierArr[TCNum]->setRow(pc->getRow());
			ToolCarrierArr[TCNum]->setCol(pc->getCol());
			pc->setParent(nullptr);
			while (!StepsToolCarrier.empty())
				StepsToolCarrier.pop();
		}
		pc = pc->getParent();
	}
}


void MoveOneBlockShooter(int row, int col, Cell* pc, int ShooterNum , Team teamShooter)
{
	//changeTargetShooter(ShooterNum);
	int shooter_team = (teamShooter == BLUE) ? SHOOTER_BLUE : SHOOTER_RED;

	
	while (pc != nullptr)
	{
		if (pc->getParent() == nullptr)
		{
			maze[row][col] = SPACE; 
			while (!Steps.empty())
				Steps.pop();
		}

		else if (pc->getParent()->getParent() == nullptr)
		{
			
			maze[pc->getRow()][pc->getCol()] = shooter_team;
			maze[pc->getParent()->getRow()][pc->getParent()->getCol()] = SPACE;

			ShooterArr[ShooterNum]->setRow(pc->getRow());
			ShooterArr[ShooterNum]->setCol(pc->getCol());
			cout << "shooter move to row: " << pc->getRow() << " col: " << pc->getCol() << endl;
			pc->setParent(nullptr);
			while (!Steps.empty())
				Steps.pop();
		}
		pc = pc->getParent();
		delay(0.01);


	}
}
queue <Cell*> CheckNeighboursTC(Cell* pCurrent, Team team)
{
	//delay(0.001);
	Cell* pn;
	queue <Cell*> cells;
	int shooter_team = (team == BLUE) ? SHOOTER_BLUE : SHOOTER_RED;
	int gray = (team == BLUE) ? GRAY_BLUE_TEAM : GRAY_RED_TEAM;
	int enemy = (team == BLUE) ? SHOOTER_RED : SHOOTER_BLUE;
	int row = pCurrent->getRow();
	int col = pCurrent->getCol();
	double sm = security_map[pCurrent->getTargetRow()][pCurrent->getTargetCol()];

	// up
	if (maze[row + 1][col] == SPACE || maze[row + 1][col] == TOOL_WAREHOUSE || maze[row + 1][col] == shooter_team)
	{
		pn = new Cell(row + 1, col, pCurrent->getTargetRow(), pCurrent->getTargetCol(), sm, pCurrent);
		pn->setG(pCurrent->getG() + sm);
		pn->updateF();
		StepsToolCarrier.push(pn);
		cells.push(pn);
		maze[row + 1][col] = gray;
	}
	// down
	if (maze[row - 1][col] == SPACE || maze[row - 1][col] == TOOL_WAREHOUSE || maze[row - 1][col] == shooter_team)
	{
		pn = new Cell(row - 1, col, pCurrent->getTargetRow(), pCurrent->getTargetCol(), sm, pCurrent);
		pn->setG(pCurrent->getG() + sm);
		pn->updateF();
		StepsToolCarrier.push(pn);

		cells.push(pn);
		maze[row - 1][col] = gray;
	}
	// left
	if (maze[row][col - 1] == SPACE || maze[row][col - 1] == TOOL_WAREHOUSE || maze[row][col - 1] == shooter_team)
	{
		pn = new Cell(row, col - 1, pCurrent->getTargetRow(), pCurrent->getTargetCol(), sm, pCurrent);
		pn->setG(pCurrent->getG() + sm);
		pn->updateF();
		StepsToolCarrier.push(pn);

		cells.push(pn);
		maze[row][col - 1] = gray;
	}
	// right
	if (maze[row][col + 1] == SPACE || maze[row][col + 1] == TOOL_WAREHOUSE || maze[row][col + 1] ==shooter_team )
	{
		pn = new Cell(row, col + 1, pCurrent->getTargetRow(), pCurrent->getTargetCol(), sm, pCurrent);
		pn->setG(pCurrent->getG() + sm);
		pn->updateF();
		StepsToolCarrier.push(pn);

		cells.push(pn);
		maze[row][col + 1] = gray;
	}
	return cells;
}

queue <Cell*> CheckNeighbours(Cell* pCurrent, int ShooterNum,Team teamShooter)
{

	//delay(0.001);
	Cell* pn;
	queue <Cell*> cells;
	int gray = (teamShooter == BLUE) ? GRAY_BLUE_TEAM : GRAY_RED_TEAM;
	int enemy = (teamShooter == BLUE) ? SHOOTER_RED : SHOOTER_BLUE;
	int row = pCurrent->getRow();
	int col = pCurrent->getCol();
	double sm = security_map[pCurrent->getTargetRow()][pCurrent->getTargetCol()];

	// up
	if (maze[row + 1][col] == SPACE || maze[row + 1][col] == MIDDLE_PART_OF_THE_ROOM )
	{
		pn = new Cell(row + 1, col, pCurrent->getTargetRow(), pCurrent->getTargetCol(), sm, pCurrent);
		pn->setG(pCurrent->getG() + sm);
		pn->updateF();
		Steps.push(pn);
		cells.push(pn);
		maze[row + 1][col] = gray;
	}
	// down
	if (maze[row - 1][col] == SPACE || maze[row - 1][col] == MIDDLE_PART_OF_THE_ROOM)
	{
		pn = new Cell(row - 1, col, pCurrent->getTargetRow(), pCurrent->getTargetCol(), sm, pCurrent);
		pn->setG(pCurrent->getG() + sm);
		pn->updateF();
		Steps.push(pn);

		cells.push(pn);
		maze[row - 1][col] = gray;
	}
	// left
	if (maze[row][col - 1] == SPACE || maze[row][col - 1] == MIDDLE_PART_OF_THE_ROOM)
	{
		pn = new Cell(row, col - 1, pCurrent->getTargetRow(), pCurrent->getTargetCol(), sm, pCurrent);
		pn->setG(pCurrent->getG() + sm);
		pn->updateF();
		Steps.push(pn);

		cells.push(pn);
		maze[row][col - 1] = gray;
	}
	// right
	if (maze[row][col + 1] == SPACE || maze[row][col + 1] == MIDDLE_PART_OF_THE_ROOM)
	{
		pn = new Cell(row, col + 1, pCurrent->getTargetRow(), pCurrent->getTargetCol(), sm, pCurrent);
		pn->setG(pCurrent->getG() + sm);
		pn->updateF();
		Steps.push(pn);

		cells.push(pn);
		maze[row][col + 1] = gray;
	}
	return cells;
}

void MoveFuncToolCarrier(int TCNum, Team team, Target target)
{
	int grey = (team == BLUE) ? GRAY_BLUE_TEAM : GRAY_RED_TEAM;
	int grey_enemy = (team == BLUE) ? GRAY_RED_TEAM : GRAY_BLUE_TEAM;
	int black = (team == BLUE) ? BLACK_BLUE_TEAM : BLACK_RED_TEAM;
	int black_enemy = (team == BLUE) ? BLACK_RED_TEAM : BLACK_BLUE_TEAM;

	queue <Cell*> cellsToCheck;
	Cell* pc;
	Cell* pNeighbor;
	int row, col, targetRow, targetCol;
	bool go_on = true;
	double newG;
	delay(0.01);
	while (go_on)
	{
		if (StepsToolCarrier.empty())
		{
			go_on = false;
			break;
		}

		pc = StepsToolCarrier.top();
		StepsToolCarrier.pop();

		row = pc->getRow();
		col = pc->getCol();
		targetRow = pc->getTargetRow();
		targetCol = pc->getTargetCol();
		if (row == targetRow && col == targetCol)
		{
			//TODO: check if the shooter is in the middle of the room
			MoveOneBlockTC(row, col, pc, TCNum);
			go_on = false;
			break;
		}
		// paint it black
		if (maze[row][col] != TOOL_CARRIER) maze[row][col] = black;


		cellsToCheck = CheckNeighboursTC(pc, team);

		while (!cellsToCheck.empty() && go_on)
		{
			pNeighbor = cellsToCheck.front();
			cellsToCheck.pop();
			newG = pNeighbor->getG() + security_map[pNeighbor->getTargetRow()][pNeighbor->getTargetCol()];
			if (newG < pNeighbor->getG())
			{
				pNeighbor->setG(newG);
				pNeighbor->updateF();
				pNeighbor->setParent(pc);
				UpdatePQ_AStar(pNeighbor, StepsToolCarrier);
			}
		}

	}

	for (int i = 0; i < MSZ; i++)
	{
		for (int j = 0; j < MSZ; j++)
		{
			if (maze[i][j] == grey || maze[i][j] == black)
				maze[i][j] = SPACE;
		}
	}
}

void MoveFuncShooter(int ShooterNum ,Team teamShooter)
{

	int shooter_team = (teamShooter == BLUE) ? SHOOTER_BLUE : SHOOTER_RED;
	int enemy = (teamShooter == BLUE) ? SHOOTER_RED : SHOOTER_BLUE;
	int grey = (teamShooter == BLUE) ? GRAY_BLUE_TEAM : GRAY_RED_TEAM;
	int grey_enemy = (teamShooter == BLUE) ? GRAY_RED_TEAM : GRAY_BLUE_TEAM;
	int black = (teamShooter == BLUE) ? BLACK_BLUE_TEAM : BLACK_RED_TEAM;
	int black_enemy = (teamShooter == BLUE) ? BLACK_RED_TEAM : BLACK_BLUE_TEAM;
	
	queue <Cell*> cellsToCheck;
	Cell* pc;
	Cell* pNeighbor;
	int row, col , targetRow, targetCol;
	bool go_on = true;
	double newG;
	delay(0.01);
	while (go_on)
	{
		if (Steps.empty())
		{
			go_on = false;
			break;
		}

		pc = Steps.top();
		Steps.pop();

		row = pc->getRow();
		col = pc->getCol();
		targetRow = pc->getTargetRow();
		targetCol = pc->getTargetCol();
		if (row== targetRow && col==targetCol)
		{
			//TODO: check if the shooter is in the middle of the room
			cout << "Shooter " << ShooterNum << " is in the middle of the room" << endl;
			MoveOneBlockShooter(row, col, pc, ShooterNum, teamShooter);
			go_on = false;
			break;
		}
		// paint it black
		if (maze[row][col] != shooter_team) maze[row][col] = black;
		

		cellsToCheck = CheckNeighbours(pc, ShooterNum, teamShooter);

		while (!cellsToCheck.empty() && go_on)
		{
			pNeighbor = cellsToCheck.front();
			cellsToCheck.pop();
			newG = pNeighbor->getG() + security_map[pNeighbor->getTargetRow()][pNeighbor->getTargetCol()];
			if (newG < pNeighbor->getG())
			{
				pNeighbor->setG(newG);
				pNeighbor->updateF();
				pNeighbor->setParent(pc);
				UpdatePQ_AStar(pNeighbor, Steps);
			}
		}
		
	}
	for (int i = 0; i < MSZ; i++)
	{
		for (int j = 0; j < MSZ; j++)
		{
			if (maze[i][j] == grey || maze[i][j] == black)
				maze[i][j] = SPACE;
		}
	}


}

void UpdatePQ_AStar(Cell* pNeighbor, priority_queue<Cell*, vector<Cell*>, CompareCells>&pq)
{
	vector <Cell*> tmp;

	while (pq.top() != pNeighbor)
	{
		tmp.push_back(pq.top());
		pq.pop();
	}
	// now pq.top() is pNeighbor
	pq.pop();
	pq.push(pNeighbor);
	while (!tmp.empty())
	{
		pq.push(tmp.back());
		tmp.pop_back();
	}
}
int whitchRoomAmI(int row, int col)
{
	for (int roomIndex = 0; roomIndex < NUM_ROOMS; roomIndex++)
	{
		int left, right, top, bottom;
		left = rooms[roomIndex]->getCenterX() - rooms[roomIndex]->getWidth() / 2;
		right = rooms[roomIndex]->getCenterX() + rooms[roomIndex]->getWidth() / 2;
		top = rooms[roomIndex]->getCenterY() + rooms[roomIndex]->getHeight() / 2;
		bottom = rooms[roomIndex]->getCenterY() - rooms[roomIndex]->getHeight() / 2;
		if (row >= bottom && row <= top && col >= left && col <= right)
			return roomIndex;
	}
	return -1;
}


int getNumOfEnemiesInTheRoom(int roomIndex , Team teamShooter)
{
	int oppestTeam = (teamShooter == BLUE) ? SHOOTER_RED : SHOOTER_BLUE;
	int r, c;
	int count = 0;
	int left, right, top, bottom;
	left = rooms[roomIndex]->getCenterX() - rooms[roomIndex]->getWidth() / 2;
	right = rooms[roomIndex]->getCenterX() + rooms[roomIndex]->getWidth() / 2;
	top = rooms[roomIndex]->getCenterY() + rooms[roomIndex]->getHeight() / 2;
	bottom = rooms[roomIndex]->getCenterY() - rooms[roomIndex]->getHeight() / 2;
	for (r = bottom; r <= top; r++)
		for (c = left; c <= right; c++)
			if (maze[r][c] == oppestTeam)
				count++;
	return count;
}

double calculateAngle(double x1, double y1, double x2, double y2) {
	double deltaY = y2 - y1;
	double deltaX = x2 - x1;
	double angleRad = atan2(deltaY, deltaX);  
	double angleDeg = angleRad * (180.0 / PI);  
	return angleDeg;
}

void playGameShooters()
{
	int i, j;

	GenerateSecurityMap();

	int distance;
	Cell* pc;
	int roomIndex , numOfEnemy;
	Team teamShooter;
	Target target;
	for (i = 0; i < NUM_OF_SHOOTERS; i++)
	{

		if (checkForBulletHit(ShooterArr[0]->getRow(), ShooterArr[0]->getCol(), i))
		{
			cout << "Shooter "<< i <<"is hit\n";
		}

		roomIndex = whitchRoomAmI(ShooterArr[i]->getRow(), ShooterArr[i]->getCol());
		teamShooter = (maze[ShooterArr[i]->getRow()][ShooterArr[i]->getCol()] == SHOOTER_BLUE) ? BLUE : RED;
		if (roomIndex != -1)
		{
			numOfEnemy = getNumOfEnemiesInTheRoom(roomIndex, teamShooter);
			
		}
		else
			numOfEnemy = 0;
		if (numOfEnemy ==0)
			shooters[i]->SetStopFlag(false);
		shooters[i]->SetFightersNear(numOfEnemy);

		shooters[i]->DoSomeWork();
		
		//cout << "Shooter " << i << " has " << numOfEnemy << "enemies"<< endl;
		//cout << "shooters[i]->CanShoot() "<< shooters[i]->CanShoot() << endl;
		//cout << "shooters[i]->GetIsFighting()" << shooters[i]->GetIsFighting() << endl;
		if (shooters[i]->GetIsFighting())
		{
			//get a shooter from the opposite team
			int oppestTeam = (teamShooter == BLUE) ? SHOOTER_RED : SHOOTER_BLUE;
			double angle =0;
			int r, c;
			int targetRow=0, targetCol =0;
			int count = 0;
			int left, right, top, bottom;
			left = rooms[roomIndex]->getCenterX() - rooms[roomIndex]->getWidth() / 2;
			right = rooms[roomIndex]->getCenterX() + rooms[roomIndex]->getWidth() / 2;
			top = rooms[roomIndex]->getCenterY() + rooms[roomIndex]->getHeight() / 2;
			bottom = rooms[roomIndex]->getCenterY() - rooms[roomIndex]->getHeight() / 2;
			for (r = bottom; r <= top; r++)
			{
				for (c = left; c <= right; c++)
				{
					if (maze[r][c] == oppestTeam)
					{
						targetRow = r;
						targetCol = c;
					}


				}

			}
			//calculate the angle between the shooter and the target
			if (targetRow!=0 && targetCol!=0)
				angle = calculateAngle(ShooterArr[i]->getCol(), ShooterArr[i]->getRow(), targetCol, targetRow);

			//shoot a bullet or a granade
			int random = 1;
			if (random == 0)
			{
				pb = shooters[i]->shootABullet(ShooterArr[i]->getCol(), ShooterArr[i]->getRow(), angle);
				bullets.push(pb);
				bulletFired = true;
				pb->setIsMoving(true);
				delay(0.001);

			}
			else
			{
				pg = shooters[i]->shootAGrenade(ShooterArr[i]->getCol(), ShooterArr[i]->getRow());
				grenades.push(pg);
				grenadeThrown = true;
				pg->explode();
				delay(0.001);
			}
			cout << "Shooter " << i << " is shooting\n";
		}
		if (shooters[i]->GetIsMoving() || shooters[i]->GetIsTryingToSurvive())
		{

			int targetRoomIndex = shooters[i]->GetRoomIndexTarget();
			int centerRoomX = rooms[targetRoomIndex]->getCenterX();
			int centerRoomY = rooms[targetRoomIndex]->getCenterY();
			Cell* start = new Cell(ShooterArr[i]->getRow(), ShooterArr[i]->getCol(), nullptr , centerRoomY , centerRoomX);
			Steps.push(start);
			MoveFuncShooter(i, teamShooter);
			if (ShooterArr[i]->getRow() == centerRoomY && ShooterArr[i]->getCol() == centerRoomX)
			{
				//check is the room is already in the queue
				targetRoomIndex += 1;
				if (targetRoomIndex > NUM_ROOMS)
					targetRoomIndex = 0;
				shooters[i]->SetRoomIndexTarget(targetRoomIndex);

			}
			int shooter_team = (teamShooter == BLUE) ? SHOOTER_BLUE : SHOOTER_RED;
			maze[ShooterArr[i]->getRow()][ShooterArr[i]->getCol()] = shooter_team;
			shooters[i]->SetX(ShooterArr[i]->getCol());
			shooters[i]->SetY(ShooterArr[i]->getRow());

		}

		if (shooters[i]->GetIsReloading())
		{
			ToolCarrier* tc = toolCarriers[getTC(teamShooter)];
			//made a request to the tool carrier
			tc->getNewRequest(shooters[i]);
			playGameToolCarriers();
		}

	}





	
	



	
}
void playGameToolCarriers()
{
	GenerateSecurityMap();
	int i, j;
	int distance;
	Cell* pc;
	Team team;
	Target target;
	for (i = 0; i < 2; i++)
	{
		team = toolCarriers[i]->GetTeamToolCarrier();
		toolCarriers[i]->DoSomeWork();
		if (toolCarriers[i]->GetIsReloading())
		{
			//find the path to the closest tool house
			int TH = getTOOLWareHouse(team);
			int targetRow = tool_Ware_houses[TH]->getRow();
			int targetCol = tool_Ware_houses[TH]->getCol();

			Cell* start = new Cell(ToolCarrierArr[i]->getRow(), ToolCarrierArr[i]->getCol(), nullptr ,targetRow ,targetCol );
			StepsToolCarrier.push(start);
			MoveFuncToolCarrier(i, team, WAREHOUSE);
		}
		else // tool carrier is giving tools
		{
			int targetRow = toolCarriers[i]->GetTargetRow();
			int targetCol = toolCarriers[i]->GetTargetCol();
			Cell* start = new Cell(ToolCarrierArr[i]->getRow(), ToolCarrierArr[i]->getCol(), nullptr, targetRow, targetCol);
			StepsToolCarrier.push(start);
			MoveFuncToolCarrier(i, team, SHOOTER);

		}

		maze[ToolCarrierArr[i]->getRow()][ToolCarrierArr[i]->getCol()] = TOOL_CARRIER;
		toolCarriers[i]->SetX(ToolCarrierArr[i]->getCol());
		toolCarriers[i]->SetY(ToolCarrierArr[i]->getRow());
	}

}


void RestorePath(Cell* pc)
{
	int r, c;
	while (pc != nullptr)
	{
		r = pc->getRow();
		c = pc->getCol();
		if (maze[r][c] == WALL)
			maze[r][c] = SPACE;
		pc = pc->getParent();
	}
}

// row, col are the indices of neighbor cell
void AddNeighbor(int r, int c, Cell* pCurrent,
	priority_queue<Cell*, vector<Cell*>, CompareCells>& pq,
	vector <Cell>& grays,  vector <Cell> &black) // blacks shouldn't be changed
{
	double newg,cost;
	vector<Cell>::iterator itGray;
	vector<Cell>::iterator itBlack;


	if (maze[r][c] == WALL) cost = WALL_COST;
	else cost = SPACE_COST;
	newg = pCurrent->getG() + cost;
	Cell* pNeighbor = new Cell(r, c, pCurrent->getTargetRow(), pCurrent->getTargetCol(),
		newg, pCurrent);
	// check what to do with the neighbor Cell
	// 1. if the neighbor is black: do nothing
	// 2. if the neighbor is white: add it to PQ and to grays
	// 3. if it is gray:
	// 3.1 if F of neighbor is not below the neighbor copy that is stored in PQ then do nothing
	// 3.2 otherwise then we must update the PQ and grays
	itGray = find(grays.begin(), grays.end(), *pNeighbor);
	itBlack = find(black.begin(), black.end(), *pNeighbor);

	if (itBlack == black.end()) // then it is not black
	{
		if (itGray == grays.end()) // then it is not gray => it is white
		{
			// paint it gray
			pq.push(pNeighbor);
			grays.push_back(*pNeighbor);
		}
		else // it is gray
		{
			if (pNeighbor->getF() < itGray->getF()) // then we found a better path and we have to exchange it
			{
				grays.erase(itGray);
				grays.push_back(*pNeighbor);

				// and do the same with PQ
				vector<Cell*> tmp;
				while (!pq.empty() &&  !((*pq.top()) == (*pNeighbor)))
				{
					tmp.push_back(pq.top()); // save the top element in tmp
					pq.pop(); // remove top element from pq
				}
				if (pq.empty()) // ERROR!
				{
					cout << "PQ is empty\n";
					exit(2);
				}
				else // we have found the neighbor cell in PQ
				{
					pq.pop(); // remove old neighbor from pq
					pq.push(pNeighbor);
					// now restore pq
					while (!tmp.empty())
					{
						pq.push(tmp.back());
						tmp.pop_back();
					}
				}
			}
		}
	}


}



// run A* from room at index1 to room at index2
void BuildPath(int index1, int index2)
{
	int r, c, tr, tc;

	r = rooms[index1]->getCenterY();
	c = rooms[index1]->getCenterX();
	tr = rooms[index2]->getCenterY();
	tc = rooms[index2]->getCenterX();
	Cell* pCurrent;
	Cell* start = new Cell(r,c ,tr ,tc , 0, nullptr);
	priority_queue<Cell*, vector<Cell*>, CompareCells> pq;
	vector <Cell> grays;
	vector <Cell> black;
	vector<Cell>::iterator itGray;

	pq.push(start);
	grays.push_back(*start);
	// pq shouldn't be empty because we are going to reach the target beforehand
	while (!pq.empty()) 	
	{
		pCurrent = pq.top();
		if (pCurrent->getH() < 0.001) // this is a targt cell
		{
			RestorePath(pCurrent);
			return;
		}
		else // target hasn't been reached
		{
			// 1. remove pCurrent from pq 
			pq.pop();
			// 2. find and remove pCurrent from grays
			itGray = find(grays.begin(), grays.end(), *pCurrent);
			if (itGray == grays.end()) // pCurrent wasn't found
			{
				cout << "Error: pCurrent wasn't found in grays\n";
				exit(1);
			}
			grays.erase(itGray);
			// 3. paint pCurrent black
			black.push_back(*pCurrent);
			// 4. take care of neighbors
			r = pCurrent->getRow();
			c = pCurrent->getCol();
			// up
			if (r + 1 < MSZ)
				AddNeighbor(r + 1, c, pCurrent, pq, grays, black);
			// down
			if (r - 1 >=0)
				AddNeighbor(r - 1, c, pCurrent, pq, grays, black);
			// left
			if (c - 1 >= 0)
				AddNeighbor(r , c-1, pCurrent, pq, grays, black);
			// right
			if (c + 1 < MSZ)
				AddNeighbor(r, c + 1, pCurrent, pq, grays, black);
		}

	}
	cout << "Error pq is empty, target hasn't been found\n";
}

void BuildPathBetweenTheRooms()
{
	int i, j;

	for (i = 0;i < NUM_ROOMS;i++)
		for (j = i + 1;j < NUM_ROOMS;j++)
		{
			BuildPath(i, j); // A*
			cout << "The path from " << i << " to " << j << " has been paved\n";
		}
}

void SetupDungeon()
{
	int i,j;
	int cx, cy, w, h;
	bool hasOverlap;

	for (i = 0;i < NUM_ROOMS;i++)
	{
		do
		{
			hasOverlap = false;
			w = 6 + rand() % (MSZ / 5);
			h = 6 + rand() % (MSZ / 5);
			cx = 2 + w / 2 + rand() % (MSZ - w - 4);
			cy = 2 + h / 2 + rand() % (MSZ - h - 4);
			for(j=0;j<i && !hasOverlap;j++)
				hasOverlap = rooms[j]->Overlap(cx, cy, w, h);
		} while (hasOverlap); // check the validity of the room
			
		rooms[i] = new Room(cx, cy, w, h,maze);
	}

	for (i = 0;i < 100;i++)
		maze[rand() % MSZ][rand() % MSZ] = WALL;
	BuildPathBetweenTheRooms();

	for (i = 0; i < NUM_ROOMS; i++)
		maze[rooms[i]->getCenterY()][rooms[i]->getCenterX()] = MIDDLE_PART_OF_THE_ROOM;
}
void SetupShooterAndToolCarriers()
{
	int i;
	int cx, cy;
	int roomIndex , counterEnemy =1;
	bool hasOverlap;
	Team teamShooter;
	for (i = 0; i < NUM_OF_SHOOTERS; i++)
	{
		teamShooter = (i % 2 == 0) ? BLUE : RED;
		do
		{
			hasOverlap = false;
			counterEnemy = 0;
			cx = 2 + rand() % (MSZ - 5);
			cy = 2 + rand() % (MSZ - 5);
			roomIndex = whitchRoomAmI(cy, cx);
			if (roomIndex < 0 || roomIndex >= NUM_ROOMS )
				hasOverlap = true;
			else
				counterEnemy = getNumOfEnemiesInTheRoom(roomIndex, teamShooter);
		} while (hasOverlap || counterEnemy <0); // check the validity of the room
		cout << "Shooter " << i << " is in room " << roomIndex << endl;
		ShooterArr[i] = new Cell(cy, cx, 0, 0, 0, nullptr);
		shooters[i] = new Shooter(cx, cy, teamShooter);
		shooters[i]->SetRoomIndexTarget(roomIndex);
		maze[cy][cx] = (teamShooter == BLUE) ? SHOOTER_BLUE : SHOOTER_RED;

	}
	for (i = 0; i < 2; i++)
	{
		teamShooter = (i % 2 == 0) ? BLUE : RED;
		do
		{
			hasOverlap = false;
			cx = 2 + rand() % (MSZ - 4);
			cy = 2 + rand() % (MSZ - 4);
			roomIndex = whitchRoomAmI(cy, cx);
			if (roomIndex == -1)
				hasOverlap = true;
		} while (hasOverlap); // check the validity of the room
		ToolCarrierArr[i] = new Cell(cy, cx, 0, 0, 0, nullptr);
		toolCarriers[i] = new ToolCarrier(cx, cy , teamShooter);
		maze[cy][cx] = TOOL_CARRIER;
	}
}
void SetUPToolHouses()
{
	int i;
	int cx=0, cy=0;
	int roomIndex;
	bool hasOverlap;
	for (i = 0; i < 2; i++)
	{
		do
		{
			hasOverlap = false;
			cx = 2 + rand() % (MSZ - 4);
			cy = 2 + rand() % (MSZ - 4);
			roomIndex = whitchRoomAmI(cy, cx);
			if (roomIndex == -1)
				hasOverlap = true;
		} while (hasOverlap); // check the validity of the room
		maze[cy][cx] = TOOL_WAREHOUSE;
		Team team = (i % 2 == 0) ? BLUE : RED;
		tool_Ware_houses[i] = new ToolWarehouse(cx, cy , team);
		toolCarriers[getTC(team)]->SetToolWarehouse(tool_Ware_houses[i]);
		

	}
}
void init()
{
	glClearColor(0.5, 0.5, 0.5, 0);// color of window background
	glOrtho(0, MSZ, 0, MSZ, -1, 1); // set the coordinates system

	srand(time(0));
	SetupDungeon();
	SetupShooterAndToolCarriers();
	SetUPToolHouses();
	ShowDungeon();
}

void ShowDungeon()
{
	int i, j;
	double s;
	bool isShooter = false;
	for(i=0;i<MSZ;i++)
		for (j = 0;j < MSZ;j++)
		{
			s = security_map[i][j];
			//1. set color of cell
			switch (maze[i][j])
			{
			case SHOOTER_BLUE:
				isShooter = true;
				glColor3d(0, 0, 1); // blue
				break;
			case SHOOTER_RED:
				isShooter = true;
				glColor3d(1, 0, 0); // red
				break;

			case TOOL_CARRIER:
				glColor3d(1.5, 1, 0);  // yellow
				break;
			case TOOL_WAREHOUSE:
				glColor3d(0, 1, 0); // green
				break;

			case MIDDLE_PART_OF_THE_ROOM:
				glColor3d(0.9098, 0.2824, 0.5765); //pink

				break;


			case SPACE:
				glColor3d(1, 1, 1); // gray
				break;
			case WALL:
				glColor3d(0.3, 0.3, 0.4); // dark gray
				break;
			}
			// show cell

			glBegin(GL_POLYGON);
			glVertex2d(j, i);
			glVertex2d(j, i + 1);
			glVertex2d(j+1, i + 1);
			glVertex2d(j + 1, i );
			glEnd();

			if (isShooter)
			{
				glColor3d(1, 1, 0); // Yellow diamond

				glBegin(GL_POLYGON);
				glVertex2f(j + 0.5, i + 0.8); // Top
				glVertex2f(j + 0.8, i + 0.5); // Right
				glVertex2f(j + 0.5, i + 0.2); // Bottom
				glVertex2f(j + 0.2, i + 0.5); // Left
				glEnd();
			}
			isShooter = false;
		}
}

void GenerateSecurityMap()
{
	int numSimulations = 1000;
	int i;

	for (i = 0;i < numSimulations;i++)
	{
		Grenade* g = new Grenade(rand() % MSZ, rand() % MSZ);

		g->SimulateExplosion(maze, security_map);
	}

}


void display()
{
	glClear(GL_COLOR_BUFFER_BIT); // clean frame buffer

	ShowDungeon();
	if (pb != nullptr)
		pb->show();
	if (pg != nullptr)
		pg->show();

	glutSwapBuffers(); // show all
}

void idle() 
{
	playGameShooters();
	
	if (bulletFired)
		pb->move(maze);
	if (grenadeThrown)
		pg->expand(maze);
	glutPostRedisplay(); // indirect call to display
}

bool checkForBulletHit(int row, int col , int shooterNum)
{
	int i, j;
	for (i = row - 1; i < row + 1; i++)
	{
		for (j = col - 1; j < col + 1; j++)
		{
			if (maze[i][j] == BULLET)
			{
				shooters[shooterNum]->GotHitFromBullet();
				return true;
			}
		}
	}
	return false;
}

void menu(int choice)
{
	switch (choice)
	{
	case 1: // fire bullet
		bulletFired = true;
		pb->setIsMoving(true);
		break;
	case 2: // throw grenade
		grenadeThrown = true;
		pg->explode();
		break;
	case 3: // security map
		GenerateSecurityMap();
		break;
	}
}
void mouse(int button, int state, int x, int y)
{
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
	{
		pb = new Bullet(MSZ*x/(double)WIDTH,
			MSZ* (HEIGHT - y) / (double)HEIGHT, (rand() % 360)* PI / 180);

		pg = new Grenade(MSZ * (HEIGHT - y) / (double)HEIGHT, MSZ * x / (double)WIDTH);
	}
}
void main(int argc, char* argv[]) 
{
	glutInit(&argc, argv);
	// definitions for visual memory (Frame buffer) and double buffer
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);
	glutInitWindowSize(WIDTH, HEIGHT);
	glutInitWindowPosition(600, 20);
	glutCreateWindow("BFS");

	// display is a refresh function
	glutDisplayFunc(display);
	// idle is a update function
	glutIdleFunc(idle);
	
	glutMouseFunc(mouse);

	// menu
	glutCreateMenu(menu);
	glutAddMenuEntry("Fire bullet", 1);
	glutAddMenuEntry("Throw Grenade", 2);
	glutAddMenuEntry("Generate Security Map", 3);
	glutAttachMenu(GLUT_RIGHT_BUTTON);


	init();

	glutMainLoop();
}