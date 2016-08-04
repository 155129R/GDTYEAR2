#ifndef SCENE_ASSIGNMENT_2_H
#define SCENE_ASSIGNMENT_2_H

#include "GameObject.h"
#include <vector>
#include "SceneBase.h"

class SceneAssignment2 : public SceneBase
{
    static const int MAX_SPEED = 10;
    static const int BULLET_SPEED = 50;
    static const int MISSILE_SPEED = 20;
    static const int MISSILE_POWER = 1;
    static const int MAX_BALL = 1;

public:
    SceneAssignment2();
    ~SceneAssignment2();

    virtual void Init();
    virtual void worldInit();
    virtual void Update(double dt);
    virtual void Render();
    virtual void Exit();

    void RenderGO(GameObject *go);

    GameObject* FetchGO();

    bool CheckCollision(GameObject *go1, GameObject *go2, float dt);
    float CheckCollision2(GameObject *go1, GameObject *go2);
    void CollisionResponse(GameObject *go1, GameObject *go2, float dt);
protected:

    //Physics
    std::vector<GameObject *> m_goList;
    float m_speed;
    float m_worldWidth;
    float m_worldHeight;
    GameObject *m_ghost;
    int m_objectCount;
    int m_ballCount = 0;
    //Auditing
    float m1, m2;
    Vector3 u1, u2, v1, v2;
    Vector3 initialMomentum, finalMomentum;
    Vector3 m_gravity;
    float initialKE, finalKE;
    float m_collisionTime;
    float countTime;
    float springEnergy = 10;
    bool springOn = false;
    bool leftFlipper = false, rightFlipper = false;
    Vector3 leftNormal, rightNormal;
    Vector3 leftdefPos, rightdefPos;
    Vector3 nudgeDisplacement;
    int m_score;
    bool shouldUpdate = true;
    int m_lives;
    bool ballDead;
};

#endif