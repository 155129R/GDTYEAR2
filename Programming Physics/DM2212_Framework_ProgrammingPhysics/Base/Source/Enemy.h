#pragma once
#include "GameObject.h"
class Enemy : public GameObject
{
public:

	enum enemyType
	{
		ET_MINION,
		ET_BOSS,
	};

	Enemy(int health = 100, enemyType eT = ET_MINION);
	virtual ~Enemy();
    int m_health;
    enemyType m_eT;
    float mass;
    float speed;
private:

};

