#include "SceneAssignment2.h"
#include "GL\glew.h"
#include "Application.h"
#include <sstream>

SceneAssignment2::SceneAssignment2()
{
}

SceneAssignment2::~SceneAssignment2()
{
}

void SceneAssignment2::Init()
{
    SceneBase::Init();

    m_worldHeight = 100.f;
    m_worldWidth = m_worldHeight * (float)Application::GetWindowWidth() / Application::GetWindowHeight();

    //Physics code here
    m_speed = 1.f;

    Math::InitRNG();

    m_objectCount = 0;

    m_ghost = new GameObject(GameObject::GO_BALL);

    m_gravity.Set(0, -9.8, 0);

    nudgeDisplacement.Set(0, 0, 0);

    m_score = 0;

    worldInit();
    //PlaySound(TEXT("8bit.wav"), NULL, SND_ASYNC | SND_LOOP);

}

void SceneAssignment2::worldInit()
{
    {
        GameObject* go = FetchGO();
        go->type = GameObject::GO_BOARD;
        go->active = true;
        go->pos.Set(m_worldWidth * 0.4f, m_worldHeight * 0.5f, -1);
        go->scale.Set(80, 90, 1);
        go->normal.Set(0, 0, 1);
    }
    for (int i = 0; i < 3; i++)
    {

        GameObject* go = FetchGO();
        go->type = GameObject::GO_WALL;
        go->active = true;
        go->pos.Set(40, 0, 0);
        go->scale.Set(2, 82, 2);
        go->normal.Set(1, 0, 0);
        if (i == 0 || i == 2)
            go->scale.Set(2, 96, 1);

        Mtx44 rotate;
        rotate.SetToRotation(90 * i, 0, 0, 1);

        go->pos = rotate * go->pos;
        go->normal = rotate * go->normal;
        if (i == 1)
            go->pos += Vector3(m_worldWidth * 0.4f, m_worldHeight * 0.55, 0);
        else
            go->pos += Vector3(m_worldWidth * 0.4f, m_worldHeight * 0.5, 0);

    }

    for (int i = 0; i < 8; i++)
    {
        if (i == 6)
            continue;
        GameObject* go = FetchGO();
        go->type = GameObject::GO_WALL;
        go->active = true;
        go->pos.Set(30, 0, 0);
        go->scale.Set(2, 26, 2);
        go->normal.Set(1, 0, 0);

        Mtx44 rotate;
        rotate.SetToRotation(45 * i, 0, 0, 1);

        go->pos = rotate * go->pos;
        go->normal = rotate * go->normal;

        if (i == 1 || i == 3)
        {
            go->scale.Set(2, 28, 1);
            switch (i)
            {
            case 1:
                go->pos += Vector3(m_worldWidth * 0.46f, m_worldHeight * 0.62, 0);
                break;
            case 3:
                go->pos += Vector3(m_worldWidth * 0.34f, m_worldHeight * 0.62, 0);
                break;
            }
        }
        else
            go->pos += Vector3(m_worldWidth * 0.4f, m_worldHeight * 0.5, 0);
    }
    {
        GameObject* go = FetchGO();
        go->type = GameObject::GO_WALL;
        go->active = true;
        go->pos.Set(m_worldWidth * 0.4f, m_worldHeight * 0.5, 0);
        go->scale.Set(2, 18, 2);
        go->normal.Set(1, 0, 0);
    }
    {
        GameObject* go = FetchGO();
        go->type = GameObject::GO_BLUEBALL;
        go->active = true;
        go->pos.Set(m_worldWidth * 0.4f, m_worldHeight * 0.7f, 0);
        go->scale.Set(4, 4, 4);
        go->normal.Set(1, 0, 0);
    }
    {
        GameObject* go = FetchGO();
        go->type = GameObject::GO_BLUEBALL;
        go->active = true;
        go->pos.Set(m_worldWidth * 0.35f, m_worldHeight * 0.6f, 0);
        go->scale.Set(4, 4, 4);
        go->normal.Set(1, 0, 0);
    }
    {
        GameObject* go = FetchGO();
        go->type = GameObject::GO_BLUEBALL;
        go->active = true;
        go->pos.Set(m_worldWidth * 0.45f, m_worldHeight * 0.6f, 0);
        go->scale.Set(4, 4, 4);
        go->normal.Set(1, 0, 0);
    }
    {
        GameObject* go = FetchGO();
        go->type = GameObject::GO_WALL;
        go->active = false;
        go->pos.Set(m_worldWidth * 0.625f, m_worldHeight * 0.15f, 0);
        go->scale.Set(2, 46, 2);
        go->normal.Set(1, 0, 0);
    }
    {
        GameObject* go = FetchGO();
        go->type = GameObject::GO_LEFTFLIPPER;
        go->active = true;
        go->scale.Set(2, 10, 2);
        go->normal.Set(1, 0, 0);
        go->pos.Set(20, - 64, 0);
        Mtx44 rotation;
        rotation.SetToRotation(90, 0, 0, 1);

        go->pos = rotation * go->pos;
        go->normal = rotation * go->normal;
        leftNormal = go->normal;
        go->pos += (m_worldWidth * 0.74f, m_worldHeight * 0.15f, 0);
        leftdefPos = go->pos;
    }
    {
        GameObject* go = FetchGO();
        go->type = GameObject::GO_RIGHTFLIPPER;
        go->active = true;
        go->scale.Set(2, 10, 2);
        go->normal.Set(1, 0, 0);
        go->pos.Set(20, -78, 0);
        Mtx44 rotation;
        rotation.SetToRotation(90, 0, 0, 1);

        go->pos = rotation * go->pos;
        go->normal = rotation * go->normal;
        rightNormal = go->normal;
        go->pos += (m_worldWidth * 0.74f, m_worldHeight * 0.15f, 0);
        rightdefPos = go->pos;
    }
    {
        GameObject* go = FetchGO();
        go->type = GameObject::GO_WALL;
        go->active = true;
        go->pos.Set(m_worldWidth * 0.57f, m_worldHeight * 0.15f, 0);
        go->scale.Set(2, 46, 2);
        go->normal.Set(1, 0, 0);
    }
}
GameObject* SceneAssignment2::FetchGO()
{
    for (std::vector<GameObject *>::iterator it = m_goList.begin(); it != m_goList.end(); ++it)
    {
        GameObject *go = (GameObject *)*it;
        if (!go->active)
        {
            go->active = true;
            ++m_objectCount;
            return go;
        }
    }
    for (unsigned i = 0; i < 10; ++i)
    {
        GameObject *go = new GameObject(GameObject::GO_BALL);
        m_goList.push_back(go);
    }
    GameObject *go = m_goList.back();
    go->active = true;
    ++m_objectCount;
    return go;
}

bool SceneAssignment2::CheckCollision(GameObject *go1, GameObject *go2, float dt)
{
    switch (go2->type)
    {
    case GameObject::GO_BALL:
    {
                                float distSquared = ((go1->pos + go1->vel * dt) - (go2->pos + go2->vel * dt)).LengthSquared();
                                float combinedRadius = go1->scale.x + go2->scale.x;

                                Vector3 relativeVelocity = go1->vel - go2->vel;
                                Vector3 relativeposition = go1->pos - go2->pos;

                                //Practical 4, Exercise 13: improve collision detection algorithm
                                return (distSquared <= combinedRadius * combinedRadius && relativeVelocity.Dot(relativeposition) < 0);
                                break;
    }
    case GameObject::GO_BLUEBALL:
    {
                                float distSquared = ((go1->pos + go1->vel * dt) - (go2->pos + go2->vel * dt)).LengthSquared();
                                float combinedRadius = go1->scale.x + go2->scale.x;

                                Vector3 relativeVelocity = go1->vel - go2->vel;
                                Vector3 relativeposition = go1->pos - go2->pos;

                                //Practical 4, Exercise 13: improve collision detection algorithm
                                return (distSquared <= combinedRadius * combinedRadius && relativeVelocity.Dot(relativeposition) < 0);
                                break;
    }
    case GameObject::GO_WALL:
    {
                                Vector3 w0 = go2->pos;
                                Vector3 b1 = go1->pos;
                                Vector3 N = go2->normal;
                                Vector3 dir = w0 - b1;
                                if (dir.Dot(N) < 0)
                                    N = -N;
                                float r = go1->scale.x;
                                float h = go2->scale.x ;
                                float l = go2->scale.y ;
                                Vector3 NP = Vector3(-N.y, N.x); //Vector3(N.y, -N.x)
                                return abs((w0 - b1).Dot(N)) < r + h * 0.5f && abs(dir.Dot(NP)) < l * 0.5f
                                    && go1->vel.Dot(N) > 0;
                                break;
    }
    case GameObject::GO_LEFTFLIPPER:
    {
                                Vector3 w0 = go2->pos;
                                Vector3 b1 = go1->pos;
                                Vector3 N = go2->normal;
                                Vector3 dir = w0 - b1;
                                if (dir.Dot(N) < 0)
                                    N = -N;
                                float r = go1->scale.x;
                                float h = go2->scale.x;
                                float l = go2->scale.y;
                                Vector3 NP = Vector3(-N.y, N.x); //Vector3(N.y, -N.x)
                                return abs((w0 - b1).Dot(N)) < r + h * 0.5f && abs(dir.Dot(NP)) < l * 0.5f
                                    && go1->vel.Dot(N) > 0;
                                break;
    }
    case GameObject::GO_RIGHTFLIPPER:
    {
                                       Vector3 w0 = go2->pos;
                                       Vector3 b1 = go1->pos;
                                       Vector3 N = go2->normal;
                                       Vector3 dir = w0 - b1;
                                       if (dir.Dot(N) < 0)
                                           N = -N;
                                       float r = go1->scale.x;
                                       float h = go2->scale.x;
                                       float l = go2->scale.y;
                                       Vector3 NP = Vector3(-N.y, N.x); //Vector3(N.y, -N.x)
                                       return abs((w0 - b1).Dot(N)) < r + h * 0.5f && abs(dir.Dot(NP)) < l * 0.5f
                                           && go1->vel.Dot(N) > 0;
                                       break;
    }
    case GameObject::GO_PILLAR:
    {
                                  Vector3 p1 = go1->pos;
                                  Vector3 p2 = go2->pos;
                                  float r1 = go1->scale.x;
                                  float r2 = go2->scale.x;
                                  Vector3 u = go1->vel;
                                  float combinedRadius = r1 + r2;
                                  return ((p2 - p1).LengthSquared() < combinedRadius * combinedRadius && (p2 - p1).Dot(u) > 0);
                                  break;
    }
    }

}

float SceneAssignment2::CheckCollision2(GameObject *go1, GameObject *go2)
{
    switch (go2->type)
    {
    case GameObject::GO_BALL:
    {
                                Vector3 rel = go1->vel - go2->vel;
                                Vector3 Dir = go1->pos - go2->pos;
                                float r = go2->scale.x + go1->scale.x;

                                bool directionCheck = (rel.Dot(Dir));
                                if (directionCheck > 0)
                                    return -1;
                                float a = rel.Dot(rel);
                                float b = 2 * rel.Dot(Dir);
                                float c = Dir.Dot(Dir) - (r * r);
                                float d = (b * b) - (4 * (a * c));
                                if (d < 0)
                                    return -1;

                                float t = (-b - sqrtf(d)) / (2 * a);
                                if (t < 0)
                                    t = (-b + sqrtf(d)) / (2 * a);


                                return t;
    }
    case GameObject::GO_BLUEBALL:
    {
                                Vector3 rel = go1->vel - go2->vel;
                                Vector3 Dir = go1->pos - go2->pos;
                                float r = go2->scale.x + go1->scale.x;

                                bool directionCheck = (rel.Dot(Dir));
                                if (directionCheck > 0)
                                    return -1;
                                float a = rel.Dot(rel);
                                float b = 2 * rel.Dot(Dir);
                                float c = Dir.Dot(Dir) - (r * r);
                                float d = (b * b) - (4 * (a * c));
                                if (d < 0)
                                    return -1;

                                float t = (-b - sqrtf(d)) / (2 * a);
                                if (t < 0)
                                    t = (-b + sqrtf(d)) / (2 * a);


                                return t;
    }
    case GameObject::GO_WALL:
    {
                                Vector3 w0 = go2->pos;
                                Vector3 b1 = go1->pos;
                                Vector3 N = go2->normal;
                                Vector3 dir = w0 - b1;
                                if (dir.Dot(N) < 0)
                                    N = -N;
                                Vector3 U = go1->vel;
                                if (U.Dot(N) < 0)
                                    return -1;
                                float r = go1->scale.x;
                                float h = go2->scale.x;
                                w0 -= (r + h * 0.5) * N;

                                float dist = (w0 - b1).Dot(N);
                                float speed = U.Dot(N);

                                float th = dist / speed;

                                Vector3 NP = Vector3(-N.y, N.x);

                                float l = go2->scale.y;
                                Vector3 w1 = w0 + (l * 0.5) * NP;
                                Vector3 w2 = w0 - (l * 0.5) * NP;

                                Vector3 bh = go1->pos + go1->vel * th;

                                if ((u1 - bh).Dot(w2 - bh) > 0)
                                    return -1;

                                return th;
    }
    case GameObject::GO_PILLAR:
    {
                                  break;
    }
    case GameObject::GO_LEFTFLIPPER:
    {
                                Vector3 w0 = go2->pos;
                                Vector3 b1 = go1->pos;
                                Vector3 N = go2->normal;
                                Vector3 dir = w0 - b1;
                                if (dir.Dot(N) < 0)
                                    N = -N;
                                Vector3 U = go1->vel;
                                if (U.Dot(N) < 0)
                                    return -1;
                                float r = go1->scale.x;
                                float h = go2->scale.x;
                                w0 -= (r + h * 0.5) * N;

                                float dist = (w0 - b1).Dot(N);
                                float speed = U.Dot(N);

                                float th = dist / speed;

                                Vector3 NP = Vector3(-N.y, N.x);

                                float l = go2->scale.y;
                                Vector3 w1 = w0 + (l * 0.5) * NP;
                                Vector3 w2 = w0 - (l * 0.5) * NP;

                                Vector3 bh = go1->pos + go1->vel * th;

                                if ((u1 - bh).Dot(w2 - bh) > 0)
                                    return -1;

                                return th;
    }
    case GameObject::GO_RIGHTFLIPPER:
    {
                                       Vector3 w0 = go2->pos;
                                       Vector3 b1 = go1->pos;
                                       Vector3 N = go2->normal;
                                       Vector3 dir = w0 - b1;
                                       if (dir.Dot(N) < 0)
                                           N = -N;
                                       Vector3 U = go1->vel;
                                       if (U.Dot(N) < 0)
                                           return -1;
                                       float r = go1->scale.x;
                                       float h = go2->scale.x;
                                       w0 -= (r + h * 0.5) * N;

                                       float dist = (w0 - b1).Dot(N);
                                       float speed = U.Dot(N);

                                       float th = dist / speed;

                                       Vector3 NP = Vector3(-N.y, N.x);

                                       float l = go2->scale.y;
                                       Vector3 w1 = w0 + (l * 0.5) * NP;
                                       Vector3 w2 = w0 - (l * 0.5) * NP;

                                       Vector3 bh = go1->pos + go1->vel * th;

                                       if ((u1 - bh).Dot(w2 - bh) > 0)
                                           return -1;

                                       return th;
    }
    }
    return -1;
}

void SceneAssignment2::CollisionResponse(GameObject *go1, GameObject *go2, float dt)
{
    switch (go2->type)
    {
    case GameObject::GO_BALL:
    {
                                m1 = go1->mass;
                                m2 = go2->mass;
                                u1 = go1->vel;
                                u2 = go2->vel;
                                initialKE = 0.5f * m1 * v1.LengthSquared() + 0.5f * m2 * v2.LengthSquared();

                                initialMomentum = m1 * u1 + m2 * u2;

                                Vector3 momentum1 = m1 * u1;
                                Vector3 momentum2 = m2 * u2;

                                Vector3 u1n, u2n;
                                Vector3 N = (go2->pos - go1->pos).Normalized();
                                u1n = (u1.Dot(N) * N);
                                u2n = (u2.Dot(N) * N);

                                go1->vel = u1 + ((2 * m2) / (m1 + m2)) * (u2n - u1n);
                                go2->vel = u2 + ((2 * m1) / (m1 + m2)) * (u1n - u2n);

                                v1 = go1->vel;
                                v2 = go2->vel;
                                finalKE = 0.5f * m1 * v1.LengthSquared() + 0.5f * m2 * v2.LengthSquared();
                                finalMomentum = m1 * v1 + m2 * v2;
                                changeColor = true;

                                break;
    }
    case GameObject::GO_BLUEBALL:
    {
                                m1 = go1->mass;
                                m2 = go2->mass;
                                u1 = go1->vel;
                                u2 = go2->normal * 2;
                                initialKE = 0.5f * m1 * v1.LengthSquared() + 0.5f * m2 * v2.LengthSquared();

                                initialMomentum = m1 * u1 + m2 * u2;

                                Vector3 momentum1 = m1 * u1;
                                Vector3 momentum2 = m2 * u2;

                                Vector3 u1n, u2n;
                                Vector3 N = (go2->pos - go1->pos).Normalized();
                                u1n = (u1.Dot(N) * N);
                                u2n = (u2.Dot(N) * N);

                                go1->vel = (u1 + ((2 * m2) / (m1 + m2)) * (u2n - u1n)) * 1.2f;
                                //go2->vel = u2 + ((2 * m1) / (m1 + m2)) * (u1n - u2n);

                                v1 = go1->vel;
                                v2 = go2->vel;
                                finalKE = 0.5f * m1 * v1.LengthSquared() + 0.5f * m2 * v2.LengthSquared();
                                finalMomentum = m1 * v1 + m2 * v2;
                                m_score += 1;
                                break;
    }
    case GameObject::GO_WALL:
    {
                                u1 = go1->vel;
                                Vector3 N = go2->normal;
                                Vector3 uN = u1.Dot(N) * N;
                                Vector3 updatedVel = ((u1 - (2 * uN)) * 0.5f);
                                go1->vel = updatedVel;


                                break;
    }
    case GameObject::GO_PILLAR:
    {
                                  u1 = go1->vel;
                                  Vector3 N = (go2->pos - go1->pos).Normalized();
                                  Vector3 u1n = (u1.Dot(N) * N);
                                  go1->vel = (u1 - 2 * u1n) * 0.2;
                                  break;
    }
    case GameObject::GO_LEFTFLIPPER:
    {

                                           u1 = go1->vel;
                                           Vector3 N = go2->normal;
                                           Vector3 uN = u1.Dot(N) * N;
                                           if (go2->swingAngle == 0)
                                               go1->vel = u1 - (2 * uN);
                                           else
                                           {
                                               Vector3 newVel = (u1 - (uN)* go2->swingAngle) * 0.0001f;

                                               if (newVel.LengthSquared() <= 60000)
                                                   go1->vel = newVel;

                                           }
                                           //go1->vel += 2 * Math::PI * dt * (go1->normal.Dot(leftNormal));

                                   break;
    }
    case GameObject::GO_RIGHTFLIPPER:
    {


                                            u1 = go1->vel;
                                            Vector3 N = go2->normal;
                                            Vector3 uN = u1.Dot(N) * N;
                                            if (go2->swingAngle == 0)
                                                go1->vel = u1 - (2 * uN);
                                            else
                                            {
                                                Vector3 newVel = (u1 - (uN)* go2->swingAngle * 0.5) * 0.0001f;
                                                if (newVel.LengthSquared() <= 60000)
                                                    go1->vel = newVel;
                                            }
                                            //go1->vel += 2 * Math::PI * dt * (go1->normal.Dot(leftNormal));

                                       break;
    }
    }

}

void SceneAssignment2::Update(double dt)
{
    
    SceneBase::Update(dt);
    shouldUpdate = true;
    if (Application::IsKeyPressed('9'))
    {
        m_speed = Math::Max(0.f, m_speed - 0.1f);
    }
    if (Application::IsKeyPressed('0'))
    {
        m_speed += 0.1f;
    }


    for (int i = 0; i <= 8; i++)
    {
        float angle = 360 / 8;
    }
   // std::cout << springEnergy << std::endl;
    if (springOn == true)
    {
        if (springEnergy < 80)
            springEnergy += 40 * (float)dt;
    }
    else
    {
            springEnergy = 30;
    }
    static bool bSpacebutton = false;
    if (!bSpacebutton && Application::IsKeyPressed(VK_SPACE))
    {
        bSpacebutton = true;
        springOn = true;
    }
    else if (bSpacebutton && !Application::IsKeyPressed(VK_SPACE))
    {
        bSpacebutton = false;
        if (m_ballCount < MAX_BALL)
        {
            GameObject *go = FetchGO();
            go->type = GameObject::GO_BALL;
            go->pos.Set(m_worldWidth * 0.6f, m_worldHeight * 0.2f, 0);
            go->vel.Set(0, springEnergy, 0);
            go->scale.Set(1.5f, 1.5f, 1.5f);
            go->mass = 125;
            springOn = false;
            m_ballCount++;
        }
    }
    //Mouse Section
    static bool bLButtonState = false;
    if (!bLButtonState && Application::IsMousePressed(0))
    {
        bLButtonState = true;
        std::cout << "LBUTTON DOWN" << std::endl;

        double x, y;
        Application::GetCursorPos(&x, &y);
        int w = Application::GetWindowWidth();
        int h = Application::GetWindowHeight();
        float posX = static_cast<float>(x) / w * m_worldWidth;
        float posY = (h - static_cast<float>(y)) / h * m_worldHeight;
        /*float posX = m_worldWidth * 0.5f;
        float posY = 5.f/100.f * m_worldHeight;*/

        m_ghost->pos.Set(posX, posY, 0); //IMPT
        m_ghost->active = true;
        float sc = 2;
        m_ghost->scale.Set(sc, sc, sc);
    }
    else if (bLButtonState && !Application::IsMousePressed(0))
    {
        bLButtonState = false;
        std::cout << "LBUTTON UP" << std::endl;

        //spawn small GO_BALL
        GameObject *go = FetchGO();
        go->active = true;
        go->type = GameObject::GO_BALL;
        double x, y;
        Application::GetCursorPos(&x, &y);
        int w = Application::GetWindowWidth();
        int h = Application::GetWindowHeight();
        float posX = static_cast<float>(x) / w * m_worldWidth;
        float posY = (h - static_cast<float>(y)) / h * m_worldHeight;

        go->pos = m_ghost->pos;
        go->vel.Set(m_ghost->pos.x - posX, m_ghost->pos.y - posY, 0);
        m_ghost->active = false;
        float sc = 1.5f;
        go->scale.Set(sc, sc, sc);
        go->mass = sc * sc * sc;


        countTime = 0;
        m_collisionTime = -1;
        for (std::vector<GameObject *>::iterator it = m_goList.begin(); it != m_goList.end(); ++it)
        {
            GameObject *go2 = (GameObject *)*it;
            if (go2->active && go != go2)
            {
                float t = CheckCollision2(go, go2);
                if (t > 0 && t <= dt)
                {
                    m_collisionTime = t;
                }
            }
        }
    }
    static bool bRButtonState = false;
    if (!bRButtonState && Application::IsMousePressed(1))
    {
        bRButtonState = true;
        std::cout << "RBUTTON DOWN" << std::endl;

        double x, y;
        Application::GetCursorPos(&x, &y);
        int w = Application::GetWindowWidth();
        int h = Application::GetWindowHeight();
        float posX = static_cast<float>(x) / w * m_worldWidth;
        float posY = (h - static_cast<float>(y)) / h * m_worldHeight;

        m_ghost->pos.Set(posX, posY, 0); //IMPT
        m_ghost->active = true;
        float sc = 3;
        m_ghost->scale.Set(sc, sc, sc);
    }
    else if (bRButtonState && !Application::IsMousePressed(1))
    {
        bRButtonState = false;
        std::cout << "RBUTTON UP" << std::endl;

        //spawn large GO_BALL
        GameObject *go = FetchGO();
        go->active = true;
        go->type = GameObject::GO_BALL;
        double x, y;
        Application::GetCursorPos(&x, &y);
        int w = Application::GetWindowWidth();
        int h = Application::GetWindowHeight();
        float posX = static_cast<float>(x) / w * m_worldWidth;
        float posY = (h - static_cast<float>(y)) / h * m_worldHeight;

        go->pos = m_ghost->pos;
        go->vel.Set(m_ghost->pos.x - posX, m_ghost->pos.y - posY, 0);
        //float sc = 3;
        go->scale = m_ghost->scale;
        go->mass = m_ghost->scale.x * m_ghost->scale.x * m_ghost->scale.x;
        m_ghost->active = false;
    }

    if (m_ghost->active && bRButtonState)
    {
        double x, y;
        Application::GetCursorPos(&x, &y);
        int w = Application::GetWindowWidth();
        int h = Application::GetWindowHeight();
        float posX = static_cast<float>(x) / w * m_worldWidth;
        float posY = (h - static_cast<float>(y)) / h * m_worldHeight;

        float size = (m_ghost->pos - Vector3(x, y, 0)).Length();
        size = Math::Clamp(size, 2.0f, 10.0f);
        m_ghost->scale.Set(size, size, size);
    }

    if (Application::IsKeyPressed('R'))
    {
        for (auto go : m_goList)
        {
            if (go->type == GameObject::GO_BALL && go->active)
            {
                go->active = false;
                m_objectCount--;
                m_ballCount--;
            }
        }
    }
    static bool bNButtonState = false;
    if (!bNButtonState && Application::IsKeyPressed('N'))
    {
        bNButtonState = true;
        for (auto go : m_goList)
        {
            if (go->type != GameObject::GO_BALL && go->active)
            {
                nudgeDisplacement = 40 * (float)dt;
                go->pos = go->pos + nudgeDisplacement;
            }
            if (go->type == GameObject::GO_BALL && go->active)
            {
                go->vel += (Math::RandFloatMinMax(-10, 10), Math::RandFloatMinMax(-10, 10), Math::RandFloatMinMax(-10, 10));
            }
        }
    }
    if (bNButtonState && !(Application::IsKeyPressed('N')))
    {
        bNButtonState = false;
        for (auto go : m_goList)
        {
            if (go->type != GameObject::GO_BALL && go->active)
            {
                go->pos -= nudgeDisplacement;
            }
        }
        nudgeDisplacement = (0,0,0);
    }
    static bool aButtonState = false;
    if (!aButtonState && Application::IsKeyPressed('A'))
    {
        leftFlipper = true;
        aButtonState = true;
    }
    if (aButtonState && !Application::IsKeyPressed('A'))
    {
        leftFlipper = false;
        aButtonState = false;
    }
    if (leftFlipper)
    {
        Mtx44 flippy;
        flippy.SetToRotation(145, 0, 0, 1);
        for (auto go : m_goList)
        {
            if (go->type == GameObject::GO_LEFTFLIPPER && go->active)
            {
                //std::cout << go->normal << std::endl;
                if (go->normal.y > 0.5)
                {

                    go->normal += flippy * go->normal * (20 * (float)dt);
                    go->pos.y += 20 * (float)dt;
                }
                go->swingAngle = Math::RadianToDegree(leftdefPos.Dot(go->pos - (go->scale * 0.1f)));

            }
        }
    }
    else
    {
        Mtx44 flippy;
        flippy.SetToRotation(145, 0, 0, 1);
        for (auto go : m_goList)
        {
            if (go->type == GameObject::GO_LEFTFLIPPER && go->active)
            {
                if (go->normal.y < 1)
                {
                    go->normal = leftNormal;
                    go->pos = leftdefPos;
                }
                go->swingAngle = 0;
            }
        }
    }

    static bool dButtonState = false;
    if (!dButtonState && Application::IsKeyPressed('D'))
    {
        rightFlipper = true;
        dButtonState = true;
    }
    if (dButtonState && !Application::IsKeyPressed('D'))
    {
        rightFlipper = false;
        dButtonState = false;

    }
    if (rightFlipper)
    {
        Mtx44 flippy;
        flippy.SetToRotation(-145, 0, 0, 1);
        for (auto go : m_goList)
        {
            if (go->type == GameObject::GO_RIGHTFLIPPER && go->active)
            {

                if (go->normal.y > 0.5)
                {
                    go->normal += flippy * go->normal * (10 * (float)dt);
                    go->pos.y += 5 * (float)dt;
                }
                go->swingAngle = Math::RadianToDegree(rightdefPos.Dot(go->pos + (go->scale * 0.1f)));
            }
        }
    }
    else
    {
        Mtx44 flippy;
        flippy.SetToRotation(145, 0, 0, 1);
        for (auto go : m_goList)
        {
            if (go->type == GameObject::GO_RIGHTFLIPPER && go->active)
            {

                if (go->normal.y < 1)
                {
                    go->normal = rightNormal;
                    go->pos = rightdefPos;
                }
                go->swingAngle = 0;

            }
        }
    }

    //Physics Simulation Section
    dt *= m_speed;

    /*if (counter)
    countTime += dt;*/
    for (std::vector<GameObject *>::iterator it = m_goList.begin(); it != m_goList.end(); ++it)
    {
        GameObject *go = (GameObject *)*it;
        if (go->active)
        {
            if (go->type == GameObject::GO_BALL)
            {
                if (go->pos.y < m_worldHeight * 0.1f)
                {
                    m_ballCount--;
           
                    go->active = false;
                    if (m_ballCount < 1)
                    {
                        ballDead = true;
                    }
                }
            }

            //Exercise 2a: Rebound game object at screen edges
            //Exercise 2b: Unspawn if it really leave the screen
            /*if (go->pos.x > m_worldWidth || go->pos.x < 0 || go->pos.y > m_worldHeight || go->pos.y < 0)
            {
            go->active = false;
            --m_objectCount;
            }*/
            if ((go->pos.x > m_worldWidth - go->scale.x) && go->vel.x > 0 || go->pos.x < go->scale.x && go->vel.x < 0)
            {
                go->vel.x = -go->vel.x;
            }
            if (go->pos.y > m_worldHeight - go->scale.y || go->pos.y < go->scale.y)
            {
                go->vel.y = -go->vel.y;
            }

            if (go->pos.x > m_worldWidth + 20 || go->pos.x < -20 || go->pos.y > m_worldWidth + 20 || go->pos.y < -20)
            {
                go->active = false;
                --m_objectCount;
            }

            for (std::vector<GameObject *>::iterator it2 = it + 1; it2 != m_goList.end(); ++it2)
            {
                GameObject *go2 = static_cast<GameObject *>(*it2);
                if (!go2->active)
                    continue;
                GameObject *goA = go, *goB = go2;
                if (go->type != GameObject::GO_BALL)
                {
                    if (go2->type != GameObject::GO_BALL)
                        continue;
                    goA = go2;
                    goB = go;
                }
                if (CheckCollision(goA, goB, dt))
                {
                    if (go->type == GameObject::GO_BALL && (go2->type == GameObject::GO_WALL || go2->type == GameObject::GO_LEFTFLIPPER || go2->type == GameObject::GO_RIGHTFLIPPER))
                    {
                        go2->swingAngle = 0.0;
                        shouldUpdate = false;
                    }
                    CollisionResponse(goA, goB, dt);
                    //Exercise 3: audit kinetic energy
                }
            }
        }
        if (shouldUpdate)
        {
            if (go->active)
            {
                if (go->type == GameObject::GO_BALL)
                {
                    go->vel += (m_gravity * (float)dt) * 0.8f;

                    //std::cout << go->vel << std::endl;
                }
            }
        }
        go->pos += go->vel * static_cast<float>(dt);

    }
}


void SceneAssignment2::RenderGO(GameObject *go)
{
    switch (go->type)
    {
    case GameObject::GO_BALL:
        modelStack.PushMatrix();
        modelStack.Translate(go->pos.x, go->pos.y, go->pos.z);
        modelStack.Scale(go->scale.x, go->scale.y, go->scale.z);
        RenderMesh(meshList[GEO_BALL], false);
        modelStack.PopMatrix();
        break;
    case GameObject::GO_BLUEBALL:
        modelStack.PushMatrix();
        modelStack.Translate(go->pos.x, go->pos.y, go->pos.z);
        modelStack.Scale(go->scale.x, go->scale.y, go->scale.z);
        RenderMesh(meshList[GEO_BLUEBALL], false);
        modelStack.PopMatrix();
        break;
    case GameObject::GO_WALL:
    {
                                float degree = Math::RadianToDegree(atan2(go->normal.y, go->normal.x));
                                modelStack.PushMatrix();
                                modelStack.Translate(go->pos.x, go->pos.y, go->pos.z);
                                modelStack.Rotate(degree, 0, 0, 1);
                                modelStack.Scale(go->scale.x, go->scale.y, go->scale.z);
                                RenderMesh(meshList[GEO_CUBE], true);
                                modelStack.PopMatrix();
                                break;
    }
    case GameObject::GO_PILLAR:
    {
                                  modelStack.PushMatrix();
                                  modelStack.Translate(go->pos.x, go->pos.y, go->pos.z);
                                  modelStack.Scale(go->scale.x, go->scale.y, go->scale.z);
                                  RenderMesh(meshList[GEO_PILLAR], false);
                                  modelStack.PopMatrix();
                                  break;
    }
    case GameObject::GO_LEFTFLIPPER:
    {
                                float degree = Math::RadianToDegree(atan2(go->normal.y, go->normal.x));
                                modelStack.PushMatrix();
                                modelStack.Translate(go->pos.x, go->pos.y, go->pos.z);
                                modelStack.Rotate(degree, 0, 0, 1);
                                modelStack.Scale(go->scale.x, go->scale.y, go->scale.z-0.5);
                                RenderMesh(meshList[GEO_FLIPPER], true);
                                modelStack.PopMatrix();
                                break;
    }
    case GameObject::GO_RIGHTFLIPPER:
    {
                                       float degree = Math::RadianToDegree(atan2(go->normal.y, go->normal.x));
                                       modelStack.PushMatrix();
                                       modelStack.Translate(go->pos.x, go->pos.y, go->pos.z);
                                       modelStack.Rotate(degree, 0, 0, 1);
                                       modelStack.Scale(go->scale.x, go->scale.y, go->scale.z - 0.5);
                                       RenderMesh(meshList[GEO_FLIPPER], true);
                                       modelStack.PopMatrix();
                                       break;
    }
    case GameObject::GO_BOARD:
    {
                                        modelStack.PushMatrix();
                                        modelStack.Translate(go->pos.x, go->pos.y, go->pos.z);
                                        modelStack.Scale(go->scale.x, go->scale.y, go->scale.z);
                                        RenderMesh(meshList[GEO_BOARD], true);
                                        modelStack.PopMatrix();
                                        break;
    }
    }
}

void SceneAssignment2::Render()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    //Calculating aspect ratio
    m_worldHeight = 100.f;
    m_worldWidth = m_worldHeight * (float)Application::GetWindowWidth() / Application::GetWindowHeight();

    // Projection matrix : Orthographic Projection
    //Mtx44 projection;
    Mtx44 perspective;
    perspective.SetToPerspective(45.0f, 4.0f / 3.0f, 0.1f, 10000.0f);
    //perspective.SetToOrtho(-80, 80, -60, 60, -1000, 1000);
    projectionStack.LoadMatrix(perspective);
    //projectionStack.LoadMatrix(projection);

    // Camera matrix
    viewStack.LoadIdentity();
    viewStack.LookAt(
        camera.position.x, camera.position.y, camera.position.z,
        camera.target.x, camera.target.y, camera.target.z,
        camera.up.x, camera.up.y, camera.up.z
        );
    // Model matrix : an identity matrix (model will be at the origin)
    modelStack.LoadIdentity();

   // RenderMesh(meshList[GEO_AXES], false);
    modelStack.PushMatrix();
    modelStack.Translate(m_worldWidth * 0.5f, m_worldHeight + m_worldHeight * 1.f, -2);
    modelStack.Scale(Application::GetWindowWidth() * 0.4f, Application::GetWindowHeight() * 0.8f, 1);
    modelStack.Rotate(50, 1, 0, 0);
    RenderMesh(meshList[GEO_BACKGROUND], false);
    modelStack.PopMatrix();
    if (m_ghost->active)
        RenderGO(m_ghost);
    for (std::vector<GameObject *>::iterator it = m_goList.begin(); it != m_goList.end(); ++it)
    {
        GameObject *go = (GameObject *)*it;
        if (go->active)
        {
            RenderGO(go);
        }
    }
    {
        std::ostringstream ss;
        ss << "Score " << m_score;
        RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(1, 0, 0), 3, 0, 9);
    }

    for (int iter = 0; iter < m_lives; iter++)
    {
        glDisable(GL_DEPTH_TEST);

        Mtx44 ortho;
        ortho.SetToOrtho(0, 80, 0, 60, -10, 10);
        projectionStack.PushMatrix();
        projectionStack.LoadMatrix(ortho);
        viewStack.PushMatrix();
        viewStack.LoadIdentity();
        modelStack.PushMatrix();
        modelStack.LoadIdentity();
        modelStack.Translate(0, 0, 1);
        RenderMesh(meshList[GEO_BALL], true);

        modelStack.PopMatrix();
        viewStack.PopMatrix();
        projectionStack.PopMatrix();
        glEnable(GL_DEPTH_TEST);
    }
    //On screen text
    //std::ostringstream ss;
    //ss << "Object count: " << m_objectCount;
    //RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(0, 1, 0), 3, 0, 9);

    //ss.str(std::string());
    //ss.precision(5);
    //ss << "Initial momentum: " << initialMomentum;
    //RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(0, 1, 0), 3, 0, 15);

    //ss.str(std::string());
    //ss.precision(5);
    //ss << "Final   momentum: " << finalMomentum;
    //RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(0, 1, 0), 3, 0, 12);

    ////Exercise 3: render initial and final kinetic energy

    //ss.str(std::string());
    //ss.precision(3);
    //ss << "Speed: " << m_speed;
    //RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(0, 1, 0), 3, 0, 6);

    //ss.str(std::string());
    //ss.precision(5);
    //ss << "FPS: " << fps;
    //RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(0, 1, 0), 3, 0, 3);

    //RenderTextOnScreen(meshList[GEO_TEXT], "Collision", Color(0, 1, 0), 3, 0, 0);
}

void SceneAssignment2::Exit()
{
    SceneBase::Exit();
    //Cleanup GameObjects
    while (m_goList.size() > 0)
    {
        GameObject *go = m_goList.back();
        delete go;
        m_goList.pop_back();
    }
    if (m_ghost)
    {
        delete m_ghost;
        m_ghost = NULL;
    }
}
