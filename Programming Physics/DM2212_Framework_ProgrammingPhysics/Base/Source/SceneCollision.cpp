#include "SceneCollision.h"
#include "GL\glew.h"
#include "Application.h"
#include <sstream>

SceneCollision::SceneCollision()
{
}

SceneCollision::~SceneCollision()
{
}

void SceneCollision::Init()
{
    SceneBase::Init();

    m_worldHeight = 100.f;
    m_worldWidth = m_worldHeight * (float)Application::GetWindowWidth() / Application::GetWindowHeight();

    //Physics code here
    m_speed = 1.f;

    Math::InitRNG();

    m_objectCount = 0;

    m_ghost = new GameObject(GameObject::GO_BALL);

    for (int i = 0; i < 4; i++)
    {

        GameObject* go = FetchGO();
        go->type = GameObject::GO_WALL;
        go->active = true;
        go->pos.Set(40, 0, 0);
        go->scale.Set(2, 82, 1);
        go->normal.Set(1, 0, 0);

        Mtx44 rotate;
        rotate.SetToRotation(90 * i, 0, 0, 1);

        go->pos = rotate * go->pos;
        go->normal = rotate * go->normal;

        go->pos += Vector3(m_worldWidth * 0.5f, m_worldHeight * 0.5, 0);
    }

    for (int i = 0; i < 8; i++)
    {
        if (i == 1 || i == 3 || i == 6)
            ++i;
        GameObject* go = FetchGO();
        go->type = GameObject::GO_WALL;
        go->active = true;
        go->pos.Set(30, 0, 0);
        go->scale.Set(2, 26, 1);
        go->normal.Set(1, 0, 0);

        Mtx44 rotate;
        rotate.SetToRotation(45 * i, 0, 0, 1);

        go->pos = rotate * go->pos;
        go->normal = rotate * go->normal;

        go->pos += Vector3(m_worldWidth * 0.5f, m_worldHeight * 0.5, 0);
    }
    {
        GameObject* go = FetchGO();
        go->type = GameObject::GO_WALL;
        go->active = true;
        go->pos.Set(m_worldWidth * 0.5f, m_worldHeight * 0.5, 0);
        go->scale.Set(2, 18, 1);
        go->normal.Set(1, 0, 0);
    }
    {
        GameObject* go = FetchGO();
        go->type = GameObject::GO_WALL;
        go->active = true;
        go->pos.Set(m_worldWidth * 0.725f, m_worldHeight * 0.256f, 0);
        go->scale.Set(2, 30, 1);
        go->normal.Set(1, 0, 0);
    }
    //go = FetchGO();
    //go->type = GameObject::GO_PILLAR;
    //go->active = true;
    //go->scale.Set(3, 3, 1);
    //go->normal.Set(1, 0, 0);
    //go->pos.Set(m_worldWidth * 0.5f + 10, m_worldHeight * 0.5, 0);

    //go = FetchGO();
    //go->type = GameObject::GO_PILLAR;
    //go->active = true;
    //go->scale.Set(3, 3, 1);
    //go->normal.Set(1, 0, 0);
    //go->pos.Set(m_worldWidth * 0.5f - 10, m_worldHeight * 0.5, 0);
}

GameObject* SceneCollision::FetchGO()
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

bool SceneCollision::CheckCollision(GameObject *go1, GameObject *go2, float dt)
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
    case GameObject::GO_WALL:
    {
                                Vector3 w0 = go2->pos;
                                Vector3 b1 = go1->pos + go1->vel * dt;
                                Vector3 N = go2->normal;
                                Vector3 dir = w0 - b1;
                                if (dir.Dot(N) < 0)
                                    N = -N;
                                float r = go1->scale.x;
                                float h = go2->scale.x;
                                float l = go2->scale.y;
                                Vector3 NP = Vector3(-N.y, N.x); //Vector3(N.y, -N.x)
                                return abs((w0 - b1).Dot(N)) < r + h * 0.5 && abs((w0 - b1).Dot(NP)) < r + l * 0.5
                                    && go1->vel.Dot(N) > 0;
                                /*break;*/
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
    /*else if (go2->type == GameObject::GO_PILLAR)
    {

    }*/
}

float SceneCollision::CheckCollision2(GameObject *go1, GameObject *go2)
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
    }
    return -1;
}

void SceneCollision::CollisionResponse(GameObject *go1, GameObject *go2, float dt)
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

                                break;
    }
    case GameObject::GO_WALL:
    {
                                u1 = go1->vel;
                                Vector3 N = go2->normal;
                                Vector3 uN = u1.Dot(N) * N;
                                go1->vel = u1 - (2 * uN);
                                break;
    }
    case GameObject::GO_PILLAR:
    {
                                  u1 = go1->vel;

                                  Vector3 N = (go2->pos - go1->pos).Normalized();
                                  Vector3 u1n = (u1.Dot(N) * N);

                                  go1->vel = u1 - 2 * u1n;

                                  break;
    }
    }

}

void SceneCollision::Update(double dt)
{
    SceneBase::Update(dt);

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
    static bool bSpacebutton = false;
    if (!bSpacebutton && Application::IsKeyPressed(VK_SPACE))
    {
        bSpacebutton = true;
        GameObject *go = FetchGO();
        go->type = GameObject::GO_BALL;
        go->pos.Set(m_worldWidth * 0.76f, m_worldHeight * 0.2f, 0);
        go->vel.Set(0, 30, 0);
        go->scale.Set(2, 2, 2);
        go->mass = 125;

    }
    else if (bSpacebutton && !Application::IsKeyPressed(VK_SPACE))
    {
        bSpacebutton = false;
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
        float sc = 2;
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
            go->pos += go->vel * static_cast<float>(dt);
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
                if (CheckCollision(goA, goB, dt)/*checkcollision2 > 0 && checkcollision2 <= dt*/)
                {
                    CollisionResponse(goA, goB, dt);
                    //Exercise 3: audit kinetic energy
                }
            }
        }
    }
}


void SceneCollision::RenderGO(GameObject *go)
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
                                RenderMesh(meshList[GEO_CUBE], false);
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
    }
}

void SceneCollision::Render()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    //Calculating aspect ratio
    m_worldHeight = 100.f;
    m_worldWidth = m_worldHeight * (float)Application::GetWindowWidth() / Application::GetWindowHeight();

    // Projection matrix : Orthographic Projection
    Mtx44 projection;
    projection.SetToOrtho(0, m_worldWidth, 0, m_worldHeight, -10, 10);
    projectionStack.LoadMatrix(projection);

    // Camera matrix
    viewStack.LoadIdentity();
    viewStack.LookAt(
        camera.position.x, camera.position.y, camera.position.z,
        camera.target.x, camera.target.y, camera.target.z,
        camera.up.x, camera.up.y, camera.up.z
        );
    // Model matrix : an identity matrix (model will be at the origin)
    modelStack.LoadIdentity();

    RenderMesh(meshList[GEO_AXES], false);

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

void SceneCollision::Exit()
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
