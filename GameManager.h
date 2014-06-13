#ifndef GAMEMANAGER_
#define GAMEMANAGER_

#include "Utils.h"
#include "Player.h"
#include <vector>
#include "Enemy.h"



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
	int MISSION_RADIUS_MIN = 400;
	int MISSION_RADIUS_MAX = 1000;
	
	void DrawEntities();
	std::vector<Enemy*> entities;
	QuadTree *quad;
	CollisionGrid *collisionGrid;

	Mesh *mesh;

	bool performance = true;

	int completedMissions;

private:
	long lastMissionStartTime;
	long lastMissionEndTime;
	long playerInsideRadiusTime;

	long lastInsideRadiusTime;
	bool playerWasInsideRadius;
	
	Player *player;
	void newRandomMission();
	void applyBonusPlayer();


};


#endif