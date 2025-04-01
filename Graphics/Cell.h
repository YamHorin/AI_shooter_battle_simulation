#pragma once
class Cell
{
private:
	int row, col;
	int target_row, target_col;
	Cell* parent;
	double f, g, h;
//	double 
public:
	Cell();
	Cell(int r, int c, Cell*p);
	Cell(int r, int c, Cell* p, int tr, int tc);
	Cell(int r, int c,int tr,int tc, double newg,Cell* p);
	int getRow() { return row; }
	int getCol() { return col; }
	Cell* getParent() { return parent; }
	void ComputeH();
	void setG(double newg) { g = newg; }
	void updateF() { f = g + h; }
	void setParent(Cell* p) { parent = p; }
	void setH(double newh) { h = newh; }
	void setRow(int r) { row = r; }
	void setCol(int c) { col = c; }
	void setTargetRow(int tr) { target_row = tr; }
	void setTargetCol(int tc) { target_col = tc; }
	double getF() { return f; }
	double getH() { return h; }
	double getG() { return g; }
	int getTargetRow() { return target_row; }
	int getTargetCol() { return target_col; }

	bool operator == (const Cell& other) { return row == other.row && col == other.col; }

};

