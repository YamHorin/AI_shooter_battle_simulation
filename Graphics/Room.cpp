#include "Room.h"
#include "glut.h"

Room::Room()
{
	centerX = 0;
	centerY = 0;
	width = 0;
	height = 0;
}

Room::Room(int x, int y, int w, int h, int board[MSZ][MSZ])
{
	int i, j;
	centerX = x;
	centerY = y;
	width = w;
	height = h;

	for (i = centerY - height / 2;i <= centerY + height / 2;i++)
		for (j = centerX - width / 2;j <= centerX + width / 2;j++)
			board[i][j] = SPACE;

}
bool Room::Overlap(int cx, int cy, int w, int h)
{
	int xOffset = abs(cx - centerX);
	int yOffset = abs(cy - centerY);
	return xOffset<width/2+w/2+5 && yOffset < height / 2 + h / 2 + 5;
}
/*
void Room::show(int board[MSZ][MSZ])
{
	int left, right, bottom, top;

	left = centerX - width / 2;
	right = centerX + width / 2;
	bottom = centerY - height / 2;
	top = centerY + height / 2;

	glColor3d(1, 1, 1); // white
	glBegin(GL_POLYGON);
	glVertex2d(left, bottom);
	glVertex2d(left, top);
	glVertex2d(right, top);
	glVertex2d(right, bottom);
	glEnd();
}
*/