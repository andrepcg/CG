#include "Utils.h"

enum MissionStatus
{
	WAITING_FOR_MISSION_COMPLETE,
	MISSION_COMPLETED,
	DELAY_BEFORE_MISSION,
	CREATING_MISSION
};

class Mission
{
public:
	Mission();
	~Mission();
	MissionStatus estado;
	Circle missionObjective;

private:

};

