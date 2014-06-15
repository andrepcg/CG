#ifndef ENEMY_H
#define ENEMY_H

#include "Player.h"
#include "Enemy.h"

enum Estado { WANDERING, REACHED_DESTINATION, WAITING_AFTER_ARRIVE };

class Enemy : public Entity
{
public: 
	Enemy(float x, float y, float z, QuadTree *q, CollisionGrid *g);
	void newRandomWalkToPos();
	void walkTo(XY pos, float deltaTime);
	void wander(float deltaTime);
	void update(Player *p, float deltaTime);

	vec2f externalForce;
	Circle playerLOS;
	Rect walkToPos;

 	void renderMesh();
 	void renderCube();
	void attack(Entity *e, float dmg);

	float anguloDir;
	

private:
	Estado estado;
	void updateBoxes();
	int waitBeforeNewDestination;
	long destinationReachedMS;
	const int PLAYER_LOS_RADIUS = 800;
	float MAX_SPEED = 80;

	const int BASE_HEALTH = 100;
	
	long lastAttackTime;
	int ATTACK_DELAY; // ms

	void mediaVelocidade();
	static const int AVG_SIZE = 20;
	float velAvg[AVG_SIZE][2];
	int curAvg = 0;

	bool m_x, m_y;
};
#endif