#ifndef GAMEMANAGER_
#define GAMEMANAGER_

#include "Utils.h"
#include "Player.h"
#include <vector>
#include "Enemy.h"
#include "Model.h"



class GameManager
{
public:
	GameManager(Rect gameBounds, Player *p);
	~GameManager();
	void update(float deltaTime);
	
	Rect bounds;
	MissionEstado estado;

	Circle missionObjective;
	std::vector<Bonus> bonus;
	int missionTime;

	void drawMission();
	float getMissionCompletedTime();

	int MISSION_DELAY = 5; // segundos
	int MISSION_RADIUS_MIN = 200;
	int MISSION_RADIUS_MAX = 800;
	
	void DrawEntities();
	std::vector<Enemy*> entities;
	
	QuadTree *quad;
	CollisionGrid *collisionGrid;
	void cleanDeadEntities();
	Mesh *mesh;

	bool performance = true;

	int completedMissions;

	bool drawCollisionCircle, drawLOS;

private:
	long lastMissionStartTime;
	long lastMissionEndTime;
	long playerInsideRadiusTime;

	long lastInsideRadiusTime;
	bool playerWasInsideRadius;
	
	Player *player;
	void newRandomMission();
	void applyBonusPlayer();
	Enemy* createEntity();
	void reviveEntity();

};


#endif