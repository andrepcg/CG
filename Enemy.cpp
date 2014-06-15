#include <math.h>
#include <cstdlib>
#include <ctime>

#include "Utils.h"
#include "Player.h"
#include "Enemy.h"



Enemy::Enemy(float x, float y, float z, QuadTree *q, CollisionGrid *g){
	pos = XYZ(x, y, z);
	v = vec2f(0, 0);
	moveSpeed = 50.0f;
	quad = q;
	grid = g;

	estado = WANDERING;
	waitBeforeNewDestination = randInt(2, 6) * 1000; // milliseconds

	newRandomWalkToPos();

	float bX = x - PLAYER_LOS_RADIUS / 2.0f;
	float bY = y - PLAYER_LOS_RADIUS / 2.0f;

	BOUNDING_BOX_SIZE = 20;
	health = BASE_HEALTH;
	ATTACK_DAMAGE = 0.2f;
	ATTACK_DELAY = 200;
	ATTACK_RADIUS = 70;

	playerLOS = Circle(pos.x, pos.z, PLAYER_LOS_RADIUS / 2.0f);

	boundingBox = Rect(x - BOUNDING_BOX_SIZE / 2.0f, z - BOUNDING_BOX_SIZE / 2.0f, BOUNDING_BOX_SIZE, BOUNDING_BOX_SIZE);

	repelCircle = Circle(pos.x, pos.z, REPEL_SIZE / 2);
	attackCircle = Circle(pos.x, pos.z, ATTACK_RADIUS / 2);
}

void Enemy::update(Player *p, float deltaTime){

	if (playerLOS.Contains(p->getPos().getXZ())){
		walkTo(p->getPos().getXZ(), deltaTime);
	}
	else {
		wander(deltaTime);
	}

	repelEntities();

	v.x += externalForce.x;
	v.y += externalForce.y;

	moveBy(clamp(v.x, -MAX_SPEED, MAX_SPEED), clamp(v.y, -MAX_SPEED, MAX_SPEED));
	
	externalForce.x = externalForce.y = 0;

	updateBoxes();
}

void Enemy::updateBoxes(){
	playerLOS.x = pos.x;
	playerLOS.y = pos.z;

	boundingBox.x = pos.x - BOUNDING_BOX_SIZE / 2.0f;
	boundingBox.y = pos.z - BOUNDING_BOX_SIZE / 2.0f;

	repelCircle.x = attackCircle.x = pos.x;
	repelCircle.y = attackCircle.y = pos.z;

}





void Enemy::wander(float deltaTime){

	if (walkToPos.Contains(pos.getXZ())){
		estado = REACHED_DESTINATION;
	}

	if (estado == WANDERING)
		walkTo(XY(walkToPos.x + 25, walkToPos.y + 25), deltaTime);
	else if (estado == REACHED_DESTINATION){
		estado = WAITING_AFTER_ARRIVE;
		v = vec2f(0, 0);
		destinationReachedMS = currentTimeMillis();
		waitBeforeNewDestination = randInt(2, 6) * 1000;
		newRandomWalkToPos();
	}
	else if (estado == WAITING_AFTER_ARRIVE){
		if ((currentTimeMillis() - destinationReachedMS) > waitBeforeNewDestination)
			estado = WANDERING;
	}
}


void Enemy::walkTo(XY p, float deltaTime){

	float vX = p.x - pos.x;
	float vY = p.y - pos.z;

	XY vec = normalizeVec(vX, vY);

	v.x = vec.x * moveSpeed * deltaTime;
	v.y = vec.y * moveSpeed * deltaTime;
}

void Enemy::newRandomWalkToPos(){
	walkToPos = Rect(randInt(0, 8192) - 25, randInt(0, 8192) - 25, 50, 50);
}

void Enemy::renderMesh(){
	glPushMatrix();
	glTranslated(pos.x, -15, pos.z);
	mesh->Render();

	glPopMatrix();
	
}

void Enemy::renderCube(){
	glPushMatrix();
	glColor3f(0.0f, 1.0f, 0.0f);
	glTranslated(pos.x, 0, pos.z);
	glutSolidCube(20);
	glPopMatrix();

	
}

void Enemy::attack(Entity *e, float dmg){
	long t = currentTimeMillis();
	if (t - lastAttackTime > ATTACK_DELAY){
		e->receiveDamage(dmg);
		lastAttackTime = t;
	}
}