#ifndef GRIDC
#define GRIDC

#include <vector>
#include "Estruturas.h"

#define GRID(x, y) (grid[gridCols * y + x])

enum Tipo {EMPTY, BLOCKED};

class CollisionGrid
{

public:
	Rect boundingBox;
	int cellSize;
	int gridRows;
	int gridCols;

	Tipo *grid;

	CollisionGrid(Rect b, int c);
	Tipo getCellFromWorldPos(float x, float y);
	void setCellFromWorldPos(float x, float y, Tipo t);
	void setCell(int x, int y, Tipo t);
	Tipo getCell(int x, int y);
	Rect getCellRect(float x, float y);
	Rect getCellRectXY(int x, int y);
};

#endif