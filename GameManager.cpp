#define _CRT_SECURE_NO_WARNINGS

#include "GameManager.h"
#include "Model.h"
#include "Mesh.h"

mar::Model *tesouro;

GameManager::GameManager(Rect gameBounds, Player *p){
	bounds = gameBounds;
	player = p;
	lastMissionEndTime = currentTimeMillis();
	estado = DELAY_BEFORE_MISSION;
	playerWasInsideRadius = false;

	quad = new QuadTree(gameBounds);
	quad->clear();

	collisionGrid = new CollisionGrid(gameBounds, 64);
	collisionGrid->setCell(20, 20, BLOCKED);

	mesh = new Mesh();
	mesh->LoadMesh("./Content/models/puddi.obj");

	tesouro = new mar::Model();
	tesouro->load("Content/models/", "treasure_chest.obj", "treasure_chest.mtl");

	for (int i = 0; i < 200; i++){
		entities.push_back(createEntity());
	}

	for (unsigned int i = 0; i < entities.size(); i++)
		quad->insert(entities[i]);
}

Enemy* GameManager::createEntity(){
	Enemy *e = new Enemy(rand() % quad->BoundingBox.w, 0, rand() % quad->BoundingBox.h, quad, collisionGrid);
	e->setMesh(mesh);
	// TODO fazer spawn ao numero maximo de entidades mas algumas estao mortas
	//e->setDead();
	return e;
}

void GameManager::reviveEntity(){
	for (unsigned int i = 0; i < entities.size(); i++){
		Enemy *e = (Enemy*)entities[i];
		if (e->isDead() == true){
			e->initialize(true);
		}
	}
}

void GameManager::update(float deltaTime){

	quad->clear();

	quad->insert(player);

	for (unsigned int i = 0; i < entities.size(); i++)
		quad->insert(entities[i]);

	for (unsigned int i = 0; i < entities.size(); i++)
		entities[i]->update(player, deltaTime);


	if (estado == WAITING_FOR_MISSION_COMPLETE){

		if (playerInsideRadiusTime > missionTime * 1000)
			estado = MISSION_COMPLETED;

		if (missionObjective.Contains(player->getPos().getXZ())){
			if (playerWasInsideRadius){
				long t = currentTimeMillis();
				playerInsideRadiusTime += t - lastInsideRadiusTime;
				lastInsideRadiusTime = t;
			}
			else{
				playerWasInsideRadius = true;
				lastInsideRadiusTime = currentTimeMillis();
			}
			
		}
		else
			playerWasInsideRadius = false;
	}
	else if (estado == MISSION_COMPLETED){
		lastMissionEndTime = currentTimeMillis();
		estado = DELAY_BEFORE_MISSION;

		applyBonusPlayer();
		completedMissions++;
	}
	else if (estado == DELAY_BEFORE_MISSION){
		long t = currentTimeMillis();

		if (t - lastMissionEndTime > MISSION_DELAY * 1000)
			estado = CREATING_MISSION;
	}
	else if (estado == CREATING_MISSION){
		newRandomMission();
		estado = WAITING_FOR_MISSION_COMPLETE;
		lastMissionStartTime = currentTimeMillis();

	}
}

void GameManager::applyBonusPlayer(){
	for (unsigned int i = 0; i < bonus.size(); i++){

		std::cout << "Bonus: " << bonus[i].b << std::endl;

		if (bonus[i].b == DAMAGE_INCREASE)
			player->attack_bonus += player->BASE_ATTACK_DAMAGE * bonus[i].amount;

		else if (bonus[i].b == SPEED_INCREASE)
			player->speed_bonus += player->BASE_MOVE_SPEED * bonus[i].amount;

		else if (bonus[i].b == JUMP_INCREASE)
			player->jump_bonus += player->BASE_JUMP_V * bonus[i].amount * 0.5;

		else if (bonus[i].b == HEALTH_INCREASE)
			player->setHealth(player->getHealth() + player->BASE_HEALTH * bonus[i].amount);

		// TODO one time bonus
		else if (bonus[i].b == JUMP_DROP_EXPLOSION)
			player->itemInventory[JUMP_DROP_EXPLOSION]++;

		else if (bonus[i].b == FORCE_FIELD)
			player->itemInventory[FORCE_FIELD]++;

		else if (bonus[i].b == BOX_CRATE)
			player->itemInventory[BOX_CRATE]++;
	}
}

float GameManager::getMissionCompletedTime(){

	return playerInsideRadiusTime / 1000.0;

}


void GameManager::newRandomMission(){
	missionObjective.x = randInt(bounds.x, bounds.x + bounds.w);
	missionObjective.y = randInt(bounds.y, bounds.y + bounds.h);
	missionObjective.radius = randInt(MISSION_RADIUS_MIN, MISSION_RADIUS_MAX);
	bonus.clear();

	int numBonus = randInt(1, 2);
	for (int i = 0; i < numBonus; i++){
		Bonus b;
		b.b = (MissionBonus) randInt(1, 8);
		b.amount = randInt(1, 15)/100.0f;
		bonus.push_back(b);
	}

	missionTime = randInt(5, 20);
	playerInsideRadiusTime = lastInsideRadiusTime = 0.0;
	playerWasInsideRadius = false;

}

GLfloat mat_specular[] = { 0.3, 1.0, 0.3, 1.0 };
GLfloat mat_shininess[] = { 50.0 };
GLfloat spotDir[] = { 0, -1, 0 };

void GameManager::drawMission(){
	if (estado == WAITING_FOR_MISSION_COMPLETE){

		GLfloat light_position[] = { missionObjective.x, 1000, missionObjective.y, 1.0 };

		glEnable(GL_LIGHT1);
		glLightfv(GL_LIGHT1, GL_SPECULAR, mat_specular);
		glLightfv(GL_LIGHT1, GL_POSITION, light_position);
		glLightf(GL_LIGHT1, GL_SPOT_CUTOFF, 30.0);
		glLightf(GL_LIGHT1, GL_SPOT_EXPONENT, 90.0);
		glLightfv(GL_LIGHT1, GL_SPOT_DIRECTION, spotDir);

		DrawCircle(missionObjective.x, missionObjective.y, 1, missionObjective.radius, 32, RGBf(1, 0, 0));
		glPushMatrix();

		glTranslatef(missionObjective.x, -10, missionObjective.y);

		glScalef(3.0, 3.0, 3.0);
		tesouro->render(false);
		glPopMatrix();

	}
}

void GameManager::DrawEntities() {
	glClearColor(0.0, 0.0, 0.0, 1.0);
	glColor3f(1.0, 1.0, 1.0);


		for (unsigned int i = 0; i < entities.size(); i++){
			if (entities[i]->isDead() == false)
				entities[i]->renderCube();
		}
	


	if (drawCollisionCircle)
		for (unsigned int i = 0; i < entities.size(); i++)
			DrawCircle(entities[i]->getRepelCircle().x, entities[i]->getRepelCircle().y, 0, entities[i]->getRepelCircle().radius, 10, RGBf(1.0, 0.0, 0.0));

	if (drawLOS)
		for (unsigned int i = 0; i < entities.size(); i++)
			DrawCircle(entities[i]->playerLOS.x, entities[i]->playerLOS.y, 0, entities[i]->playerLOS.radius, 20, RGBf(1.0, 1.0, 0.0));

	player->drawBullets();

	if (player->currentlyUsing == FORCE_FIELD){
		player->renderForceField();
	}
}
