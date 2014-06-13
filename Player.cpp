#include "Player.h"

Player::Player(XYZ &p)
{
	pos = XYZ(p.x, p.y, p.z);

	REPEL_SIZE = 60;
	v = vec2f(0, 0);
	REPEL_SIZE = 80;
	BOUNDING_BOX_SIZE = 30;

	isP = true;

	ATTACK_RADIUS = REPEL_SIZE;

	health = BASE_HEALTH;

	boundingBox = Rect(p.x - BOUNDING_BOX_SIZE / 2.0f, p.z - BOUNDING_BOX_SIZE / 2.0f, BOUNDING_BOX_SIZE, BOUNDING_BOX_SIZE);

	repelCircle = Circle(pos.x, pos.z, REPEL_SIZE / 2);
	attackCircle = Circle(pos.x, pos.z, ATTACK_RADIUS / 2);
}

void Player::update(bool *keys, float deltaTime){
	repelEntities();

	move(keys, deltaTime);

	

	updateBoxes();
}

void Player::updateBoxes(){

	boundingBox.x = pos.x - BOUNDING_BOX_SIZE / 2.0f;
	boundingBox.y = pos.z - BOUNDING_BOX_SIZE / 2.0f;

	repelCircle.x = attackCircle.x = pos.x;
	repelCircle.y = attackCircle.y = pos.z;

}

void Player::move(bool *keys, float deltaTime){
	float YRotationRadius = (camera->yRotation / 180 * PI);
	 
	if (keys['W']){
		//XRotationRadius = (Xrotation / 180 * Pi);
		v.x += float(sin(YRotationRadius)) * (BASE_MOVE_SPEED + speed_bonus) * deltaTime;
		v.y -= float(cos(YRotationRadius)) * (BASE_MOVE_SPEED + speed_bonus) * deltaTime;
	}

	if (keys['S']){
		//XRotationRadius = (Xrotation / 180 * Pi);
		v.x -= float(sin(YRotationRadius)) * (BASE_MOVE_SPEED + speed_bonus) * deltaTime;
		v.y += float(cos(YRotationRadius)) * (BASE_MOVE_SPEED + speed_bonus) * deltaTime;
	}

	if (keys['A']){
		v.x -= float(cos(YRotationRadius)) * (BASE_MOVE_SPEED + speed_bonus) * deltaTime;
		v.y -= float(sin(YRotationRadius)) * (BASE_MOVE_SPEED + speed_bonus) * deltaTime;
	}

	if (keys['D']){
		v.x += float(cos(YRotationRadius)) * (BASE_MOVE_SPEED + speed_bonus) * deltaTime;
		v.y += float(sin(YRotationRadius)) * (BASE_MOVE_SPEED + speed_bonus) * deltaTime;
	}

	if (keys['E'])
		pos.y += BASE_MOVE_SPEED * deltaTime;

	if (keys['Q'])
		pos.y -= BASE_MOVE_SPEED * deltaTime;

	if (keys[GLFW_KEY_SPACE]){
		if (!jumped && currentTimeMillis() - lastJumpLand > JUMP_DELAY)
		{
			jumpVel = BASE_JUMP_V + jump_bonus;
			jumped = true;
		}
	}

	this->moveBy(v.x, v.y);

	if (jumped){

		pos.y += jumpVel * deltaTime;
		jumpVel += gravity;
	}

	if (jumped && pos.y <= 0.1){
		jumpVel = 0;
		pos.y = 0;
		jumped = false;
		lastJumpLand = currentTimeMillis();
	}

	v.x = v.y = 0;
}

void Player::attack(Entity *e, float dmg){
	long t = currentTimeMillis();
// 	if (t - lastAttackTime > ATTACK_DELAY){
// 		e->receiveDamage(dmg);
// 		lastAttackTime = t;
// 	}
}

void Player::render(){
	camera->render();

	// TODO player model
}

Player::~Player()
{
}