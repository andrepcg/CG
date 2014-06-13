#define _CRT_SECURE_NO_WARNINGS

#include "GameManager.h"


GameManager::GameManager(Rect gameBounds, Player *p){
	bounds = gameBounds;
	player = p;
	lastMissionEndTime = currentTimeMillis();
	estado = DELAY_BEFORE_MISSION;
	playerWasInsideRadius = false;

	quad = new QuadTree(Rect(0, 0, 8192, 8192));
	quad->clear();

	collisionGrid = new CollisionGrid(Rect(0, 0, 8192, 8192), 64);
	collisionGrid->setCell(20, 20, BLOCKED);

	mesh = new Mesh();
	mesh->LoadMesh("./Content/models/puddi.obj");

	for (int i = 0; i < 100; i++){
		entities.push_back(new Enemy(rand() % quad->BoundingBox.w, 0, rand() % quad->BoundingBox.h, quad, collisionGrid));
		entities[i]->setMesh(mesh);
	}

	for (int i = 0; i < entities.size(); i++)
		quad->insert(entities[i]);
}

void GameManager::update(float deltaTime){

	quad->clear();

	quad->insert(player);

	for (int i = 0; i < entities.size(); i++)
		quad->insert(entities[i]);

	for (int i = 0; i < entities.size(); i++)
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
	for (int i = 0; i < bonus.size(); i++){
		if (bonus[i].b == DAMAGE_INCREASE)
			player->attack_bonus += player->BASE_ATTACK_DAMAGE * bonus[i].amount;

		else if (bonus[i].b == SPEED_INCREASE)
			player->speed_bonus += player->BASE_MOVE_SPEED * bonus[i].amount;

		else if (bonus[i].b == JUMP_INCREASE)
			player->jump_bonus += player->BASE_JUMP_V * bonus[i].amount * 0.5;

		else if (bonus[i].b == HEALTH_INCREASE)
			player->setHealth(player->getHealth() + player->BASE_HEALTH * bonus[i].amount);

		// TODO one time bonus
		else if (bonus[i].b == JUMP_LAND_EXPLOSION)
			player->oneTimeUseBonus[JUMP_LAND_EXPLOSION] += 1;
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
		b.b = (MissionBonus)randInt(1, sizeof(MissionBonus));
		b.amount = randInt(1, 15)/100.0f;
		bonus.push_back(b);
	}

	missionTime = randInt(5, 20);
	playerInsideRadiusTime = lastInsideRadiusTime = 0.0;
	playerWasInsideRadius = false;

}

void GameManager::drawMission(){
	if (estado == WAITING_FOR_MISSION_COMPLETE)
		DrawCircle(missionObjective.x, missionObjective.y, 1, missionObjective.radius, 32, RGBf(1, 0, 0));
}

void GameManager::DrawEntities() {
	glColor3f(1.0f, 1.0f, 1.0f);


	if (performance)
		for (int i = 0; i < entities.size(); i++)
			entities[i]->renderCube();
	else
		for (int i = 0; i < entities.size(); i++)
			entities[i]->renderMesh();


	// bounding box
	//for (int i = 0; i < entities.size(); i++)
	//drawRectFloor(entities[i]->repelBox.x, entities[i]->repelBox.y, entities[i]->repelBox.w, entities[i]->repelBox.h, RGBf(1.0, 0.0, 0.0));



}