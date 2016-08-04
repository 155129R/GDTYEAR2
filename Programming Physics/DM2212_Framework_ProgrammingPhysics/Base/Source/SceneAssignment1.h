#ifndef SCENE_ASSIGNMENT1_H
#define SCENE_ASSIGNMENT1_H

#include "GameObject.h"
#include <vector>
#include "SceneBase.h"
#include "Enemy.h"

class SceneAssignment1 : public SceneBase
{
    static const int MAX_SPEED = 10;
    static const int BULLET_SPEED = 80;
    static const int MISSILE_SPEED = 20;
    static const int MISSILE_POWER = 1;

public:
    SceneAssignment1();
    ~SceneAssignment1();

    virtual void Init();
    virtual void Update(double dt);
    virtual void Render();
    virtual void Exit();

    void RenderGO(GameObject *go);

    GameObject* FetchGO();
    Enemy* FetchEnemy();
    bool CheckCollision(GameObject *go1, GameObject *go2, float dt);

    enum sceneState
    {
        SS_MENU,
        SS_WAVE1,
        SS_WAVE2,
        SS_WAVE3,
        SS_LOSE,
        SS_WIN
    };

    sceneState currState = SS_MENU;
protected:

    //Physics
    std::vector<GameObject *> m_goList;
    std::vector<Enemy *> m_EnemyList;
    float m_speed;
    float m_worldWidth;
    float m_worldHeight;
    GameObject *m_ship;
    Vector3 m_force;
    float m_shipSpeed = 80.f;
    int m_objectCount;
    int m_lives = 3;
    int m_score = 0;
    float elecCharge = 0.f;
    bool astSpawned = false;
    bool wave2Spawned = false;
    bool wave3Spawned = false;
    float waveCount = 20;
    bool toggleData = false;
    bool toggleBoost;
    float m_health = 100;

    //Auditing
    float m1, m2;
    Vector3 u1, u2, v1, v2;
    Vector3 initialMomentum, finalMomentum;
    float initialKE, finalKE;
};

#endif