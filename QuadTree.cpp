#include "QuadTree.h"
#include <iostream>

QuadTree::QuadTree(Rect Bounds)
{
	BoundingBox = Bounds;

	northWest = NULL;
	northEast = NULL;
	southWest = NULL;
	southEast = NULL;
}

bool QuadTree::insert(Entity *e)
{

	if (!BoundingBox.Contains(e->getPos().getXZ()))
		return false;

	if (northWest == NULL && points.size() < QT_NODE_CAPACITY){
		points.push_back(e);
		return true;
	}

	if (northWest == NULL)
		subdivide();

	if (northWest->insert(e) || northEast->insert(e) || southWest->insert(e) || southEast->insert(e))
		return true;

	return false;
}

void QuadTree::subdivide()
{

	// TODO oube lá rapaz cáqui problemas

	Rect BBNW = Rect(BoundingBox.x, BoundingBox.y, BoundingBox.w / 2, BoundingBox.h / 2);
	Rect BBNE = Rect(BoundingBox.x + BoundingBox.w / 2, BoundingBox.y, BoundingBox.w / 2, BoundingBox.h / 2);
	Rect BBSW = Rect(BoundingBox.x, BoundingBox.y + BoundingBox.h / 2, BoundingBox.w / 2, BoundingBox.h / 2);
	Rect BBSE = Rect(BoundingBox.x + BoundingBox.w / 2, BoundingBox.y + BoundingBox.h / 2, BoundingBox.w / 2, BoundingBox.h / 2);

	// create the new quad trees
	northWest = new QuadTree(BBNW);
	northEast = new QuadTree(BBNE);
	southWest = new QuadTree(BBSW);
	southEast = new QuadTree(BBSE);

	std::vector<Entity*> temp1 = queryRange(BBNW);
	std::vector<Entity*> temp2 = queryRange(BBNE);
	std::vector<Entity*> temp3 = queryRange(BBSW);
	std::vector<Entity*> temp4 = queryRange(BBSE);

	for (int i = 0; i < temp1.size(); i++)
		northWest->insert(temp1[i]);

	for (int i = 0; i < temp2.size(); i++)
		northEast->insert(temp2[i]);

	for (int i = 0; i < temp3.size(); i++)
		southWest->insert(temp3[i]);

	for (int i = 0; i < temp4.size(); i++)
		southEast->insert(temp4[i]);


	points.clear();

}

void QuadTree::clear()
{
	points.clear();

	if (northWest != NULL){
		northWest->clear();
		delete northWest;
		northWest = NULL;
	}

	if (northEast != NULL){
		northEast->clear();
		delete northEast;
		northEast = NULL;
	}

	if (southWest != NULL){
		southWest->clear();
		delete southWest;
		southWest = NULL;
	}

	if (southEast != NULL){
		southEast->clear();
		delete southEast;
		southEast = NULL;
	}
}

void AddGroup(std::vector<Entity*>& vtr, std::vector<Entity*>& toAdd)
{
	for (unsigned int i = 0; i < toAdd.size(); i++)
		vtr.push_back(toAdd[i]);
}

std::vector<Entity*> QuadTree::queryPoint(XY p){

	std::vector<Entity*> pointsInRange;

	if (!BoundingBox.Contains(p))
		return pointsInRange;

	if (northWest == NULL){
		for (int p = 0; p < points.size(); p++)
			pointsInRange.push_back(points[p]);
		return pointsInRange;
	}

	AddGroup(pointsInRange, northWest->queryPoint(p));
	AddGroup(pointsInRange, northEast->queryPoint(p));
	AddGroup(pointsInRange, southWest->queryPoint(p));
	AddGroup(pointsInRange, southEast->queryPoint(p));

	return pointsInRange;

}


std::vector<Entity*> QuadTree::queryRange(Rect& range)
{

	std::vector<Entity*> pointsInRange;


	if (!BoundingBox.Intersects(range))
		return pointsInRange; // empty list

	for (int p = 0; p < points.size(); p++){
		if (range.Contains(points[p]->getPos().getXZ()))
			pointsInRange.push_back(points[p]);
	}

	// Terminate here, if there are no children
	if (northWest == 0)
		return pointsInRange;

	// Otherwise, add the points from the children
	AddGroup(pointsInRange, northWest->queryRange(range));
	AddGroup(pointsInRange, northEast->queryRange(range));
	AddGroup(pointsInRange, southWest->queryRange(range));
	AddGroup(pointsInRange, southEast->queryRange(range));

	return pointsInRange;
}


std::vector<Entity*> QuadTree::queryRange(Circle& range)
{

	std::vector<Entity*> pointsInRange;


	if (!BoundingBox.Intersects(range))
		return pointsInRange; // empty list

	for (int p = 0; p < points.size(); p++){
		if (range.Contains(points[p]->getPos().getXZ()))
			pointsInRange.push_back(points[p]);
	}

	// Terminate here, if there are no children
	if (northWest == 0)
		return pointsInRange;

	// Otherwise, add the points from the children
	AddGroup(pointsInRange, northWest->queryRange(range));
	AddGroup(pointsInRange, northEast->queryRange(range));
	AddGroup(pointsInRange, southWest->queryRange(range));
	AddGroup(pointsInRange, southEast->queryRange(range));

	return pointsInRange;
}