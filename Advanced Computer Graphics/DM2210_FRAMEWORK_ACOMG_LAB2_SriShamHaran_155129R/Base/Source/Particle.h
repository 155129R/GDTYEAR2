#ifndef PARTICLE_H
#define PARTICLE_H

#include "Vector3.h"

enum ParticleObject_TYPE
{
	P_WATER = 0,
	P_EMBER,
    P_SAKURA,
	P_TOTAL
};

class ParticleObject
{
public:
	ParticleObject(ParticleObject_TYPE typeValue = P_WATER);
	~ParticleObject(void);

	Vector3 pos;
	Vector3 vel;
	Vector3 scale;
	
	float rotation;
	float rotationSpeed;
	bool active;

	ParticleObject_TYPE type;
};

#endif /*ParticleObject*/