#include "Entity.h"
#include <typeinfo>
#include "Utils.h"

class Enemy;
#include "Enemy.h"
#include "Mesh.h"

void Entity::setMesh(Mesh *m){
	mesh = m;
}

XYZ Entity::getPos(){
	return pos;
}

void Entity::setPos(XYZ p){
	pos = p;
}

bool Entity::isPlayer(){
	return isP;
}

XY vertical = XY(0, 1);
XY horizontal = XY(1, 0);
XY diagonal = XY(1, 1);

void Entity::moveBy(float x, float y){
	vec2f direccao = vec2f((x > 0) ? 1 : -1, (y > 0) ? 1 : -1);

	Rect newBoundingBox = Rect(pos.x + x - BOUNDING_BOX_SIZE / 2.0f, pos.z + y - BOUNDING_BOX_SIZE / 2.0f, BOUNDING_BOX_SIZE, BOUNDING_BOX_SIZE);

	XY playerCell = XY((int)(pos.x / grid->cellSize), (int)(pos.z / grid->cellSize)); 

	XY vert = direccao * vertical;
	XY hori = direccao * horizontal;
	XY diag = direccao * diagonal;

	vert = vert + playerCell;
	hori = hori + playerCell;
	diag = diag + playerCell;

	// if bloqueado na vertical - y
	if (grid->boundingBox.Contains(vert) && grid->getCell(vert.x, vert.y) == BLOCKED){
		Rect col = grid->getCellRectXY(vert.x, vert.y);
		if (!col.Intersects(newBoundingBox))
			pos.z += y;

	}
	else
		pos.z += y;

	// if bloqueado na horizontal - x
	if (grid->boundingBox.Contains(hori) && grid->getCell(hori.x, hori.y) == BLOCKED){
		Rect col = grid->getCellRectXY(hori.x, hori.y);
		if (!col.Intersects(newBoundingBox))
			pos.x += x;
	}
	else
		pos.x += x;

}


void Entity::repelEntities(){

	std::vector<Entity*> closeBy = quad->queryRange(attackCircle);

	for (unsigned int i = 0; i < closeBy.size(); i++){

		if (!this->isPlayer() && closeBy[i]->isPlayer()){

			if (closeBy[i]->getPos().y <= 50)
				this->attack(closeBy[i], ATTACK_DAMAGE);
		}


		if (repelCircle.radius == attackCircle.radius  || repelCircle.Contains(closeBy[i]->pos.getXZ()))
			repel(closeBy[i]);


	}

}

void Entity::repel(Entity *e){
	if (e == this || e->isPlayer())
		return;

	Enemy *ee = (Enemy*)e;

 	if (ee->isDead())
 		return;

	float dx = pos.x - e->pos.x;
	float dy = pos.z - e->pos.z;
	float distance = sqrt(dx*dx + dy*dy);
	//float distance = dx*dx + dy*dy;

	float force = floor(REPEL_FORCE / distance);

	float angle = atan2(dy, dx);
	float x_speed = force * cos(angle);
	float y_speed = force * sin(angle);

	if (!this->isPlayer()){
		v.x += x_speed;
		v.y += y_speed;
	}
	ee->externalForce.x -= x_speed;
	ee->externalForce.y -= y_speed;

}

void Entity::receiveDamage(float d){
	health -= d;
	std::cout << "Dano " << d << std::endl;
}

float Entity::getHealth(){
	return health;
}

float Entity::setHealth(float h){
	return health = h;
}

void Entity::setQuadTree(QuadTree *q){
	quad = q;
}
void Entity::setGrid(CollisionGrid *g){
	grid = g;
}
Circle Entity::getRepelCircle(){
	return repelCircle;
}

void Entity::setYVel(float v){
	yVel = v;
}

void Entity::setAir(bool t){
	air = t;
}