#ifndef QUADTREE
#define QUADTREE

#include <vector>
#include "Estruturas.h"

class Entity;

#include "Entity.h"

class QuadTree
{

public:
	static const int QT_NODE_CAPACITY = 4;

	Rect BoundingBox;

	std::vector<Entity*> points;

	QuadTree *northWest;
	QuadTree *northEast;
	QuadTree *southWest;
	QuadTree *southEast;

	QuadTree(Rect _boundingBox);
	bool insert(Entity *e);
	void subdivide();
	void clear();
	std::vector<Entity*> queryRange(Rect& range);
	std::vector<Entity*> queryRange(Circle& range);
	std::vector<Entity*> queryPoint(XY p);
}; 

#endif