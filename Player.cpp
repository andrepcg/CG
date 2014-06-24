#include "Player.h"
#include "Enemy.h"
#include "Point.h"
#include "Model.h"

Player::Player(XYZ &p)
{
	pos = XYZ(p.x, p.y, p.z);

	v = vec2f(0, 0);
	REPEL_SIZE = 60;
	BOUNDING_BOX_SIZE = 30;

	ATTACK_RADIUS = REPEL_SIZE;
	ORIGINAL_REPEL_RADIUS = REPEL_SIZE;
	ORIGINAL_ATTACK_RADIUS = ATTACK_RADIUS;
	ORIGINAL_REPEL_FORCE = REPEL_FORCE;

	isP = true;

	currentlyUsing = NONE;

	health = BASE_HEALTH;

	itemInventory[FORCE_FIELD] = 10;
	itemInventory[JUMP_DROP_EXPLOSION] = 10;

	boundingBox = Rect(p.x - BOUNDING_BOX_SIZE / 2.0f, p.z - BOUNDING_BOX_SIZE / 2.0f, BOUNDING_BOX_SIZE, BOUNDING_BOX_SIZE);
	repelCircle = Circle(pos.x, pos.z, REPEL_SIZE / 2);
	attackCircle = Circle(pos.x, pos.z, ATTACK_RADIUS / 2);

	mesh = new mar::Model();
	mesh->load("Content/models/", "android.obj", "android.mtl");
}

void Player::update(bool *keys, float deltaTime){
	updateBullets(deltaTime);

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

	if (currentlyUsing == FORCE_FIELD){
	}

	updateBoxes();

}

void Player::processEventKeys(){
	if (eventKeys[GLFW_KEY_X]){
		if (itemInventory[FORCE_FIELD] > 0){
			forceFieldStartTime = currentTimeMillis();
			currentlyUsing = FORCE_FIELD;
			REPEL_SIZE = ATTACK_RADIUS = 300;
			REPEL_FORCE = 1000;
			itemInventory[FORCE_FIELD]--;
		}
		eventKeys[GLFW_KEY_X] = false;
	}

	// jump explosion
	else if (eventKeys[GLFW_KEY_Z]){
		if (itemInventory[JUMP_DROP_EXPLOSION] > 0){
			currentlyUsing = JUMP_DROP_EXPLOSION;
			extraGravity = -2000;
			itemInventory[JUMP_DROP_EXPLOSION]--;
			jumped = true;
			jumpVel = BASE_JUMP_V + 2000;
		}
		eventKeys[GLFW_KEY_Z] = false;
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
	if (keys['D'] || keys['A'] || keys['S'] || keys['W'])
		headbob(deltaTime);


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

		if (currentlyUsing == JUMP_DROP_EXPLOSION)
			jumpVel += gravity * deltaTime + extraGravity * deltaTime;
		else
			jumpVel += gravity * deltaTime;
	}

	if (jumped && pos.y <= 0.1){
		jumpVel = 0;
		pos.y = 0;
		jumped = false;
		lastJumpLand = currentTimeMillis();
		
		if (currentlyUsing == JUMP_DROP_EXPLOSION){
			currentlyUsing = NONE;
			jumpRepel();
		}
	}

	v.x = v.y = 0;
}

void Player::headbob(float d){
	r += d;
	pos.y += 60 * sin(r * ((BASE_MOVE_SPEED + speed_bonus) / 20.0f)) * d;
}

void Player::attack(Entity *e, float dmg){
	long t = currentTimeMillis();
	e->receiveDamage(dmg);
// 	if (t - lastAttackTime > ATTACK_DELAY){
// 		e->receiveDamage(dmg);
// 		lastAttackTime = t;
// 	}
}

void Player::render(){
	//camera->render();

	if (currentlyUsing == FORCE_FIELD){

	}

	// TODO player model
	if (!camera->firstPerson){
		glPushMatrix();
		glTranslatef(0, -50, 0);
		glRotatef(180, 0, 1, 0);
		mesh->render(false);
		glPopMatrix();
	}

}

void Player::renderForceField(){
	DrawCircle(pos.x, pos.z, 2, REPEL_SIZE / 2.0, 20, RGBf(0, 0, 1));

	glPushMatrix();
	glColor4f(1, 0, 1, 0.5);
	glTranslatef(pos.x, -10, pos.z);
	glutSolidSphere(REPEL_SIZE, 50, 50);
	glPopMatrix();
}

Player::~Player()
{
}

void Player::jumpRepel(){

	Circle c = Circle(pos.x, pos.z, attackCircle.radius * 4);
	std::vector<Entity*> closeBy = quad->queryRange(c);

	for (unsigned int i = 0; i < closeBy.size(); i++){

		if (!closeBy[i]->isPlayer()){
			this->attack(closeBy[i], 50);
			repelF(closeBy[i]);
		}

	}

}

void Player::repelF(Entity *e){

	Enemy *ee = (Enemy*)e;

	float dx = pos.x - e->getPos().x;
	float dy = pos.z - e->getPos().z;
	float distance = sqrt(dx*dx + dy*dy);

	float force = floor(randInt(3000, 5000) / distance);

	float angle = atan2(dy, dx);
	float x_speed = force * cos(angle);
	float y_speed = force * sin(angle);

	ee->externalForce.x -= x_speed;
	ee->externalForce.y -= y_speed;
	ee->setYVel(randInt(20, 40));
	ee->setAir(true);
}

void Player::shoot(){
	long curTime = currentTimeMillis();

	if (curTime - lastShootTime > SHOOT_DELAY){
		
		Bullet b;
		b.pos.x = pos.x;
		b.pos.y = pos.y + camera->HEAD_HEIGHT;
		b.pos.z = pos.z;
		b.xRot = camera->xRotation;
		b.yRot = camera->yRotation;

		bullets.push_back(b);
	}
}

void Player::updateBullets(float deltaTime){

	std::vector<Bullet>::iterator it = bullets.begin();
	while (it != bullets.end())	{
		Circle c = Circle(it->pos.x, it->pos.z, 30);

		float YRotationRadius = (it->yRot / 180 * PI);
		float XRotationRadius = (it->xRot / 180 * PI);

		it->pos.x += float(sin(YRotationRadius)) * BULLET_SPEED * deltaTime;
		it->pos.z -= float(cos(YRotationRadius)) * BULLET_SPEED * deltaTime;
		it->pos.y -= float(sin(XRotationRadius)) * BULLET_SPEED * deltaTime;

		bool a = false;
		if (it->pos.y < 100){
			std::vector<Entity*> closeBy = quad->queryRange(c);
			for (unsigned int i = 0; a == false && i < closeBy.size(); i++){

				if (!closeBy[i]->isPlayer()){
					this->attack(closeBy[i], 50);
					a = true;
				}
			}
		}

		if (it->pos.y < -10 || a)
			it = bullets.erase(it);
		else
			++it;
	}
}

void Player::drawBullets(){
	for (unsigned int i = 0; i < bullets.size(); i++){
		glPushMatrix();
		glColor3f(1, 0, 0);
		glTranslated(bullets[i].pos.x, bullets[i].pos.y, bullets[i].pos.z);
		glutSolidSphere(1, 10, 10);
		glPopMatrix();
	}
}