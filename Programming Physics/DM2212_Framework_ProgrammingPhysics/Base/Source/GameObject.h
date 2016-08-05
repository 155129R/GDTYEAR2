#ifndef GAME_OBJECT_H
#define GAME_OBJECT_H

#include "Vector3.h"

struct GameObject
{
	enum GAMEOBJECT_TYPE
	{
		GO_NONE = 0,
		GO_BALL,
		GO_CUBE,
        GO_ASTEROID,
        GO_SHIP,
        GO_BULLET,
        GO_MISSILE,
        GO_MINION,
        GO_BOSS,
        GO_ENEMYBULLET,
		GO_FIELD,
        GO_MINIONBULLET,
        GO_BLUEBALL,
        GO_WALL,
        GO_PILLAR,
        GO_LEFTFLIPPER,
        GO_RIGHTFLIPPER,
        GO_BOARD,
        GO_GATE,
		GO_TOTAL, //must be last
	};
	GAMEOBJECT_TYPE type;
	Vector3 pos;
	Vector3 vel;
	Vector3 scale;
    Vector3 normal;
    float swingAngle;
	bool active;
	float mass;
    float decayTimer = 0.f;
	GameObject(GAMEOBJECT_TYPE typeValue = GO_BALL);
	~GameObject();
};

#endif