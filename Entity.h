#ifndef ENTITY
#define ENTITY


class QuadTree;
class CollisionGrid;
#include "QuadTree.h"
#include "CollisionGrid.h"
#include "Mesh.h"


class Entity
{
public:
	
	void repelEntities();
	void repel(Entity *e);
	bool collide();
	void moveBy(float vX, float vY);

	void setMesh(Mesh *m);
	XYZ getPos();

	void renderMesh();
	void renderCube();
	void updateBoxes();
	
	bool isPlayer();
	
	void receiveDamage(float d);
	virtual void attack(Entity *e, float dmg) = 0;

	float getHealth();
	float setHealth(float h);

	void setQuadTree(QuadTree *q);
	void setGrid(CollisionGrid *g);


protected:
	bool isP = false;
	float health;
	float moveSpeed;
	QuadTree *quad;
	CollisionGrid *grid;
	vec2f v;

	float _moveX, _moveY;

	int REPEL_FORCE = 35;
	int REPEL_SIZE = 50;
	int BOUNDING_BOX_SIZE;
	int ATTACK_RADIUS;
	
	XYZ pos;
	Circle repelCircle;
	Circle attackCircle;
	Rect boundingBox;
	Mesh *mesh;

	float ATTACK_DAMAGE;

	

	

};

#endif