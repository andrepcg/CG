#include "CollisionGrid.h"
#include <string.h>



CollisionGrid::CollisionGrid(Rect b, int c){
	cellSize = c;
	boundingBox = b;

	int co, ro;

	co = b.w / c;
	ro = b.h / c;

	gridCols = co;
	gridRows = ro;

	grid = (Tipo*) malloc(ro * co * sizeof(int));
	memset(grid, 0, gridCols * gridRows);

}

Tipo CollisionGrid::getCellFromWorldPos(float x, float y){

	int gridX = x / cellSize;
	int gridY = y / cellSize;

	return GRID(gridX, gridY);

}

void CollisionGrid::setCellFromWorldPos(float x, float y, Tipo t){

	int gridX = x / cellSize;
	int gridY = y / cellSize;

	GRID(gridX, gridY) = t;
}

void CollisionGrid::setCell(int x, int y, Tipo t){
	GRID(x, y) = t;
}

Tipo CollisionGrid::getCell(int x, int y){
	return GRID(x, y);
}

Rect CollisionGrid::getCellRect(float x, float y){
	int gridX = (int) x;
	int gridY = (int) y;

	return Rect(x, y, cellSize, cellSize);
}

Rect CollisionGrid::getCellRectXY(int x, int y){
	return Rect(x * cellSize, y * cellSize, cellSize, cellSize);
}