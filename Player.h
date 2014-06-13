#ifndef PLAYER_
#define PLAYER_

#include "Utils.h"

class Camera;
#include "camera.h"


#include "Entity.h"



class Player : public Entity
{
public:
	Player(XYZ &p);
	~Player();
	void render();
	void update(bool *keys, float deltaTime);

	Camera *camera;

	void renderMesh();
	void renderCube();

	float speed_bonus;
	float attack_bonus;
	float jump_bonus;

	void receiveDamage(float d);

	const float BASE_MOVE_SPEED = 200.0f;
	const float BASE_HEALTH = 100.0f;
	const float BASE_ATTACK_DAMAGE = 20.0f;
	const float BASE_JUMP_V = 450.0f; // 500

	void attack(Entity *e, float dmg);

	MissionBonus oneTimeUseBonus[10];

private:
	void updateBoxes();
	void move(bool *keys, float deltaTime);
	float jumpVel;
	float gravity = -1.5;
	bool jumped;

	const int JUMP_DELAY = 500; // MS

	long lastJumpLand;

	
};



#endif