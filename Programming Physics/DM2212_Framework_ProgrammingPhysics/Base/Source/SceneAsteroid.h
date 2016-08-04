#ifndef SCENE_ASTEROID_H
#define SCENE_ASTEROID_H

#include "GameObject.h"
#include <vector>
#include "SceneBase.h"
#include "Enemy.h"

class SceneAsteroid : public SceneBase
{
	static const int MAX_SPEED = 10;
	static const int BULLET_SPEED = 80;
	static const int MISSILE_SPEED = 20;
	static const int MISSILE_POWER = 1;

public:
	SceneAsteroid();
	~SceneAsteroid();

	virtual void Init();
	virtual void Update(double dt);
	virtual void Render();
	virtual void Exit();

	void RenderGO(GameObject *go);

	GameObject* FetchGO();
	Enemy* FetchEnemy();
protected:

	//Physics
	std::vector<GameObject *> m_goList;
	std::vector<Enemy *> m_EnemyList;
	float m_speed;
	float m_worldWidth;
	float m_worldHeight;
	GameObject *m_ship;
	Vector3 m_force;
	int m_objectCount;
	int m_lives;
	int m_score;
    float spawnBulletRate = 0.f;
};

#endif