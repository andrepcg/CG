#include "Player.h"

Player::Player(XYZ &p)
{
	pos = XYZ(p.x, p.y, p.z);

	v = vec2f(0, 0);
	REPEL_SIZE = 80;
	BOUNDING_BOX_SIZE = 30;

	ATTACK_RADIUS = REPEL_SIZE;
	ORIGINAL_REPEL_RADIUS = REPEL_SIZE;
	ORIGINAL_ATTACK_RADIUS = ATTACK_RADIUS;
	ORIGINAL_REPEL_FORCE = REPEL_FORCE;

	isP = true;

	currentlyUsing = NONE;

	health = BASE_HEALTH;

	itemInventory[FORCE_FIELD] = 2;

	boundingBox = Rect(p.x - BOUNDING_BOX_SIZE / 2.0f, p.z - BOUNDING_BOX_SIZE / 2.0f, BOUNDING_BOX_SIZE, BOUNDING_BOX_SIZE);
	repelCircle = Circle(pos.x, pos.z, REPEL_SIZE / 2);
	attackCircle = Circle(pos.x, pos.z, ATTACK_RADIUS / 2);
}

void Player::update(bool *keys, float deltaTime){
	repelEntities();

	processEventKeys();
	move(keys, deltaTime);

	// TODO usar o force field temporariamente -> contagem de tempo em que o REPEL_SIZE está maior
	if (currentlyUsing == FORCE_FIELD){
		long t = currentTimeMillis();
		if (t - forceFieldStartTime > (FORCE_FIELD_DURATION * 1000)){
			REPEL_SIZE = ORIGINAL_REPEL_RADIUS;
			ATTACK_RADIUS = ORIGINAL_ATTACK_RADIUS;
			REPEL_FORCE = ORIGINAL_REPEL_FORCE;
			currentlyUsing = NONE;
		}
	}

	updateBoxes();
}

void Player::processEventKeys(){
	if (eventKeys[GLFW_KEY_X]){
		if (itemInventory[FORCE_FIELD] > 0){
			forceFieldStartTime = currentTimeMillis();
			currentlyUsing = FORCE_FIELD;
			REPEL_SIZE = ATTACK_RADIUS = 250;
			REPEL_FORCE = 1000;
			itemInventory[FORCE_FIELD]--;
		}
		eventKeys[GLFW_KEY_X] = false;
	}
}

void Player::updateBoxes(){

	boundingBox.x = pos.x - BOUNDING_BOX_SIZE / 2.0f;
	boundingBox.y = pos.z - BOUNDING_BOX_SIZE / 2.0f;

	repelCircle.x = attackCircle.x = pos.x;
	repelCircle.y = attackCircle.y = pos.z;

	repelCircle.radius = REPEL_SIZE / 2.0f;
	attackCircle.radius = ATTACK_RADIUS / 2.0f;

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
		jumpVel += gravity * deltaTime;
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

	if (currentlyUsing == FORCE_FIELD)
		DrawCircle(pos.x, pos.z, 2, REPEL_SIZE / 2.0, 20, RGBf(0, 0, 1));

	// TODO player model
}

Player::~Player()
{
}