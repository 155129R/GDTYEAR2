#include "SceneAssignment1.h"
#include "GL\glew.h"
#include "Application.h"
#include <sstream>

SceneAssignment1::SceneAssignment1()
{
}

SceneAssignment1::~SceneAssignment1()
{
}
bool SceneAssignment1::CheckCollision(GameObject *go1, GameObject *go2, float dt)
{
    //Exercise 1: move collision code to CheckCollision()
    float distSquared = (go1->pos - go2->pos).LengthSquared();
    float combinedRadius = go1->scale.x + go2->scale.x;
    Vector3 relativeVelocity = go1->vel - go2->vel;
    Vector3 relativeDisplacement = go1->pos - go2->pos;
    //Practical 4, Exercise 13: improve collision detection algorithm
    return(distSquared <= combinedRadius * combinedRadius && relativeVelocity.Dot(relativeDisplacement) < 0);
}
void SceneAssignment1::Init()
{
    SceneBase::Init();
    currState = SS_MENU;
    //Physics code here
    m_speed = 1.f;

    Math::InitRNG();

    //Exercise 2a: Construct 100 GameObject with type GO_ASTEROID and add into m_goList
    for (unsigned i = 0; i < 100; i++)
    {
        m_goList.push_back(new GameObject(GameObject::GO_ASTEROID));
    }

    //Exercise 2b: Initialize m_lives and m_score

    //Calculating aspect ratio
    m_worldHeight = 100.f;
    m_worldWidth = m_worldHeight * (float)Application::GetWindowWidth() / Application::GetWindowHeight();

    //Exercise 2c: Construct m_ship, set active, type, scale and pos
    m_ship = new GameObject(GameObject::GO_SHIP);
    m_ship->active = true;
    m_ship->type = GameObject::GO_SHIP;
    m_ship->scale.Set(4.5f, 4.5f, 4.5f);
    m_ship->pos.Set(m_worldWidth * 0.5f, m_worldHeight * 0.5f, 1);
    m_ship->mass = 8.f;
    elecCharge = 10.f;
    for (size_t i = 0; i < 40; ++i)
    {
        GameObject *newgo = new GameObject(GameObject::GO_MINION);
    }

    toggleBoost = false;

}

GameObject* SceneAssignment1::FetchGO()
{
    //Exercise 3a: Fetch a game object from m_goList and return it
    for (std::vector<GameObject *>::iterator it = m_goList.begin(); it != m_goList.end(); ++it)
    {
        GameObject *go = (GameObject*)*it;
        if (!go->active)
        {
            go->active = true;
            return go;
        }
    }
    for (int i = 0; i < 10; ++i)
    {
        GameObject *newgo = new GameObject(GameObject::GO_ASTEROID);
        m_goList.push_back(newgo);
    }

    FetchGO();


}

Enemy* SceneAssignment1::FetchEnemy()
{
    //Exercise 3a: Fetch a game object from m_goList and return it
    for (std::vector<Enemy *>::iterator it = m_EnemyList.begin(); it != m_EnemyList.end(); ++it)
    {
        Enemy *enRet = (Enemy*)*it;
        if (!enRet->active)
        {
            enRet->active = true;
            return enRet;
        }
    }
    for (int i = 0; i < 10; ++i)
    {
        Enemy *newEn = new Enemy(GameObject::GO_MINION);
        m_EnemyList.push_back(newEn);
    }
    Enemy *newEn = new Enemy(GameObject::GO_BOSS);

    FetchEnemy();

}

void SceneAssignment1::Update(double dt)
{
    SceneBase::Update(dt);
    dt *= m_speed;

    if (currState == SS_MENU)
    {
        if (Application::IsKeyPressed('1'))
            currState = SS_WAVE1;
    }
    if (currState != SS_MENU){
        if (Application::IsKeyPressed(VK_OEM_MINUS))
        {
            m_speed = Math::Max(0.f, m_speed - 0.1f);
        }
        if (Application::IsKeyPressed(VK_OEM_PLUS))
        {
            m_speed += 0.1f;
        }
        m_force.SetZero();
        //Exercise 6: set m_force values based on WASD
        if (toggleBoost == true)
            m_shipSpeed = 1000.f;
        else if (toggleBoost == false)
            m_shipSpeed = 100.f;
        if (Application::IsKeyPressed('W'))
        {
            if (!toggleBoost)
                m_force.Set(0, m_shipSpeed, 0);
            else
                m_force.Set(0, m_shipSpeed + 200, 0);

            //m_force.y = 80.f;
        }
        if (Application::IsKeyPressed('A'))
        {
            if (!toggleBoost)
                m_force.Set(-m_shipSpeed, 0, 0);
            else
                m_force.Set(-m_shipSpeed - 200, 0, 0);

            //m_force.x = -80.f;
        }
        if (Application::IsKeyPressed('S'))
        {
            if (!toggleBoost)
                m_force.Set(0, -m_shipSpeed, 0);
            else
                m_force.Set(0, -m_shipSpeed - 200, 0);

            //m_force.y = -80.f;
        }
        if (Application::IsKeyPressed('D'))
        {
            if (!toggleBoost)
                m_force.Set(m_shipSpeed, 0, 0);
            else
                m_force.Set(m_shipSpeed + 200, 0, 0);

            //m_force.x = 80.f;
        }
        //Exercise 8: use 2 keys to increase and decrease mass of ship
        if (Application::IsKeyPressed('9'))
        {
            m_ship->mass = Math::Max(0.1f, m_ship->mass - 0.1f);
        }
        if (Application::IsKeyPressed('0'))
        {
            m_ship->mass += 0.1f;
        }

        //Exercise 11: use a key to spawn some asteroids
        if (Application::IsKeyPressed('V'))
        {


        }
        if (Application::IsKeyPressed('M'))
        {


        }
        //Exercise 14: use a key to spawn a bullet

        if (Application::IsKeyPressed(VK_SPACE))
        {
            if (elecCharge > 1.5f){
                toggleBoost = true;
                elecCharge -= 3 * (float)dt;
            }
        }
        if (toggleBoost == true && !(Application::IsKeyPressed(VK_SPACE)))
        {
            toggleBoost = false;
        }
        if (Application::IsKeyPressed(VK_F1))
        {
            toggleData = !toggleData;
        }
        if (Application::IsKeyPressed('1'))
        {
            for (std::vector<GameObject *>::iterator it = m_goList.begin(); it != m_goList.end(); ++it)
            {
                GameObject *go = (GameObject *)*it;
                go->active = false;
            }
            m_goList.clear();
            m_EnemyList.clear();

            //SceneAssignment1::Init();
            currState = SS_WAVE1;
            astSpawned = false;
            m_health = 100;
            waveCount = 20;

        }
        if (Application::IsKeyPressed('2'))
        {
            m_goList.clear();
            m_EnemyList.clear();
            SceneAssignment1::Init();
            currState = SS_WAVE2;
            wave2Spawned = false;
            m_health = 100;
            waveCount = 20;
        }
        if (Application::IsKeyPressed('3'))
        {

            m_goList.clear();
            m_EnemyList.clear();
            SceneAssignment1::Init();
            currState = SS_WAVE3;
            wave3Spawned = false;
            m_health = 100;
            waveCount = 40;
        }
        //Exercise 15: limit the spawn rate of bullets

        if (currState == SS_WAVE1)
        {

            if (astSpawned == false)
            {
                for (int i = 0; i < waveCount; ++i)
                {

                    GameObject* go = FetchGO();
                    go->type = GameObject::GO_ASTEROID;
                    go->scale.Set(2.f, 2.f, 2.f);
                    go->pos.Set(Math::RandFloatMinMax(0, m_worldWidth), Math::RandFloatMinMax(0, m_worldHeight), 1);
                    go->vel.Set(Math::RandFloatMinMax(-5, 5), Math::RandFloatMinMax(-5, 5), 0);
                    go->mass = 4.f;

                }
                astSpawned = true;
            }
        }
        if (currState == SS_WAVE2)
        {

            if (wave2Spawned == false)
            {

                for (int i = 0; i < waveCount / 2; ++i)
                {

                    Enemy* newMinion = FetchEnemy();
                    newMinion->type = GameObject::GO_MINION;
                    newMinion->pos.Set(m_worldWidth - (Math::RandIntMinMax(1, 8)), Math::RandFloatMinMax(0, m_worldHeight), Math::RandFloatMinMax(1, 3));
                    newMinion->scale.Set(5, 5, 5);
                    newMinion->vel = (newMinion->pos - m_ship->pos).Normalized();
                    newMinion->m_health = 100;
                    newMinion->mass = 8;
                    newMinion->speed = Math::RandFloatMinMax(10, 60);
                    newMinion->m_eT = Enemy::ET_MINION;
                }
                for (int i = 0; i < waveCount / 2; ++i)
                {

                    GameObject* go = FetchGO();
                    go->type = GameObject::GO_ASTEROID;
                    go->scale.Set(2.f, 2.f, 2.f);
                    go->pos.Set(Math::RandFloatMinMax(0, m_worldWidth), Math::RandFloatMinMax(0, m_worldHeight), 1);
                    go->vel.Set(Math::RandFloatMinMax(-5, 5), Math::RandFloatMinMax(-5, 5), 0);
                    go->mass = 4.f;
                }
                wave2Spawned = true;
            }
        }
        if (currState == SS_WAVE3)
        {

            if (wave3Spawned == false)
            {

                //for (int i = 0; i < waveCount / 2; ++i)
                //{

                //    Enemy* newMinion = FetchEnemy();
                //    newMinion->type = GameObject::GO_MINION;
                //    newMinion->pos.Set(m_worldWidth - (Math::RandIntMinMax(1, 8)), Math::RandFloatMinMax(0, m_worldHeight), Math::RandFloatMinMax(1, 3));
                //    newMinion->scale.Set(5, 5, 5);
                //    newMinion->vel = (newMinion->pos - m_ship->pos).Normalized();
                //    newMinion->m_health = 100;
                //    newMinion->mass = 8;
                //    newMinion->speed = Math::RandFloatMinMax(10, 60);
                //    newMinion->m_eT = Enemy::ET_MINION;
                //}
                for (int i = 0; i < waveCount; ++i)
                {

                    GameObject* go = FetchGO();
                    go->type = GameObject::GO_ASTEROID;
                    go->scale.Set(2.f, 2.f, 2.f);
                    go->pos.Set(Math::RandFloatMinMax(0, m_worldWidth), Math::RandFloatMinMax(0, m_worldHeight), 1);
                    go->vel.Set(Math::RandFloatMinMax(-5, 5), Math::RandFloatMinMax(-5, 5), 0);
                    go->mass = 4.f;
                }
                Enemy* newBoss = FetchEnemy();
                newBoss->type = GameObject::GO_BOSS;
                newBoss->pos.Set(m_worldWidth - 10, m_worldHeight * 0.5f, 1);
                newBoss->scale.Set(15, 15, 15);
                newBoss->vel = (newBoss->pos - m_ship->pos).Normalized();
                newBoss->m_health = 200;
                newBoss->mass = 8;
                newBoss->speed = 40;
                newBoss->m_eT = Enemy::ET_BOSS;
                wave3Spawned = true;
            }
        }
        //Mouse Section
        static bool bLButtonState = false;
        if (elecCharge < 4.6f)
            elecCharge += 0.3f * (float)dt;
        if (elecCharge <= 0)
            elecCharge = 0.01f;
        if (!bLButtonState && Application::IsMousePressed(0))
        {
            bLButtonState = true;
            std::cout << "LBUTTON DOWN" << std::endl;
            if (elecCharge > 0.1)
            {
                GameObject* go = FetchGO();
                go->type = GameObject::GO_BULLET;
                go->scale.Set(1.2f, 1.2f, 1.2f);
                go->pos = m_ship->pos + Vector3(m_ship->vel.x * 0.3f, m_ship->vel.y * 0.3f, m_ship->vel.z * 0.3f);
                double x, y;
                Application::GetCursorPos(&x, &y);
                int w = Application::GetWindowWidth();
                int h = Application::GetWindowHeight();
                float worldX = x / w * m_worldWidth;
                float worldY = (h - y) / h * m_worldHeight;
                Vector3 dir = ((Vector3(worldX, worldY, m_ship->pos.z)) - m_ship->pos).Normalized();
                if (!dir.IsZero())
                    dir.Normalize();
                else
                    dir.Set(1, 0, 0);
                go->vel = dir * BULLET_SPEED + dt;
                elecCharge -= 0.6;
                PlaySound(TEXT("laser.wav"), NULL, SND_ASYNC);

            }
        }
        else if (bLButtonState && !Application::IsMousePressed(0))
        {
            bLButtonState = false;
            std::cout << "LBUTTON UP" << std::endl;
        }
        static bool bRButtonState = false;
        if (!bRButtonState && Application::IsMousePressed(1))
        {
            bRButtonState = true;
            std::cout << "RBUTTON DOWN" << std::endl;
            if (elecCharge > 2.f)
            {
                GameObject* go = FetchGO();
                go->type = GameObject::GO_MISSILE;
                go->scale.Set(12.f, 12.f, 12.f);
                go->pos = m_ship->pos + Vector3(m_ship->vel.x * 0.3f, m_ship->vel.y * 0.3f, m_ship->vel.z * 0.3f);
                double x, y;
                Application::GetCursorPos(&x, &y);
                int w = Application::GetWindowWidth();
                int h = Application::GetWindowHeight();
                float worldX = x / w * m_worldWidth;
                float worldY = (h - y) / h * m_worldHeight;
                Vector3 dir = ((Vector3(worldX, worldY, m_ship->pos.z)) - m_ship->pos).Normalized();
                if (!dir.IsZero())
                    dir.Normalize();
                else
                    dir.Set(1, 0, 0);
                go->vel = dir * BULLET_SPEED + dt;
                elecCharge -= 2.f;
                PlaySound(TEXT("missile.wav"), NULL, SND_ASYNC);

            }
        }
        else if (bRButtonState && !Application::IsMousePressed(1))
        {
            bRButtonState = false;
            std::cout << "RBUTTON UP" << std::endl;
        }

        //Physics Simulation Section
        if (currState == SS_WAVE1 &&  waveCount <= 3)
        {
            currState = SS_WAVE2;
            waveCount = 20;
        }
        if (currState == SS_WAVE2 &&  waveCount <= 4)
        {
            currState = SS_WAVE3;
            waveCount = 40;
        }

        //Exercise 7: Update ship's velocity based on m_force
        //Vector3 acceleration(m_force.x / m_ship->mass, m_force.y / m_ship->mass, m_force.z / m_ship->mass);
        Vector3 acceleration;
        if (m_ship->mass > Math::EPSILON)
            acceleration = m_force * (1.f / m_ship->mass);

        m_ship->vel += acceleration * dt;

        if (m_ship->vel.LengthSquared() > MAX_SPEED * MAX_SPEED)
        {
            m_ship->vel.Normalize();
            m_ship->vel *= MAX_SPEED;
        }

        m_ship->pos += m_ship->vel * dt;
        m_force.SetZero();
        if (m_health <= 0)
        {
            m_ship->pos.Set(m_worldWidth * 0.5f, m_worldHeight * 0.5f, 1);
            m_ship->vel.SetZero();
            --m_lives;

            m_health = 100;
        }
        //Exercise 9: wrap ship position if it leaves screen
        if (m_ship->pos.y > m_worldHeight)
        {
            m_ship->pos.y -= m_worldHeight;
        }
        else if (m_ship->pos.y < 0)
        {
            m_ship->pos.y += m_worldHeight;
        }

        if (m_ship->pos.x > m_worldWidth)
        {
            m_ship->pos.x -= m_worldWidth;
        }
        else if (m_ship->pos.x < 0)
        {
            m_ship->pos.x += m_worldWidth;
        }
        if (m_lives <= 0)
        {
            currState = SS_LOSE;
        }
        std::cout << waveCount << std::endl;
        for (std::vector<Enemy *>::iterator it = m_EnemyList.begin(); it != m_EnemyList.end(); ++it)
        {
            Enemy *en = (Enemy *)*it;

            if (en->active == true)
            {

                en->vel = ((m_ship->pos - en->pos).Normalized()) * en->speed * dt;
                //en->pos.x += en->vel.x * 10 * dt;
                //en->pos.y += en->vel.y * 10 * dt;
                en->pos += en->vel * 10 * (float)dt;
                if (en->pos.y > m_worldHeight)
                    en->pos.y -= m_worldHeight;
                else if (en->pos.y < 0)
                    en->pos.y += m_worldHeight;

                if (en->pos.x > m_worldWidth)
                    en->pos.x -= m_worldWidth;
                else if (en->pos.x < 0)
                    en->pos.y += m_worldWidth;

                if (en->m_eT == Enemy::ET_MINION)
                {
                    for (std::vector<GameObject *>::iterator it = m_goList.begin(); it != m_goList.end(); ++it)
                    {
                        GameObject *go = (GameObject *)*it;
                        if (go->active)
                        {
                            if (go->type == GameObject::GO_MISSILE)
                            {
                                Vector3 seperation = go->pos - en->pos;
                                float distSquare = seperation.LengthSquared();
                                float combinedRadiusSquared = go->scale.x + en->scale.x;
                                if (distSquare <= combinedRadiusSquared)
                                {
                                    go->active = false;
                                    en->m_health -= 50;
                                    if (en->m_health <= 0){
                                        en->active = false;
                                        m_score += 2;
                                        waveCount--;
                                    }

                                }
                            }
                            if (go->type == GameObject::GO_ASTEROID)
                            {
                                Vector3 seperation = go->pos - en->pos;
                                float distSquare = seperation.LengthSquared();
                                float combinedRadiusSquared = go->scale.x + en->scale.x;
                                if (distSquare <= combinedRadiusSquared)
                                {
                                    go->active = false;
                                    waveCount--;
                                    en->m_health -= 10;
                                    if (en->m_health <= 0){
                                        en->active = false;
                                        m_score += 2;
                                        waveCount--;
                                    }

                                }
                            }
                            Vector3 seperation = m_ship->pos - en->pos;
                            float distSquare = seperation.LengthSquared();
                            float combinedRadiusSquared = m_ship->scale.x + en->scale.x;
                            if (distSquare <= combinedRadiusSquared)
                            {
                                m_health -= 1 * dt;
                                break;
                            }

                            if (go->type == GameObject::GO_BULLET)
                            {
                                Vector3 seperation = go->pos - en->pos;
                                float distSquare = seperation.LengthSquared();
                                float combinedRadiusSquared = go->scale.x + en->scale.x;
                                if (distSquare <= combinedRadiusSquared)
                                {
                                    go->active = false;
                                    en->m_health -= 10;
                                    if (en->m_health <= 0){
                                        en->active = false;
                                        m_score += 2;
                                        waveCount--;
                                    }

                                }
                            }


                        }
                    }
                }
                if (en->m_eT == Enemy::ET_BOSS)
                {
                    for (std::vector<GameObject *>::iterator it = m_goList.begin(); it != m_goList.end(); ++it)
                    {
                        GameObject *go = (GameObject *)*it;
                        if (go->active)
                        {
                            if (go->type == GameObject::GO_MISSILE)
                            {
                                Vector3 seperation = go->pos - en->pos;
                                float distSquare = seperation.LengthSquared();
                                float combinedRadiusSquared = go->scale.x + en->scale.x;
                                if (distSquare <= combinedRadiusSquared)
                                {
                                    go->active = false;
                                    en->m_health -= 50;
                                    if (en->m_health <= 0){
                                        en->active = false;
                                        m_score += 100;
                                        waveCount--;
                                        currState = SS_WIN;
                                    }

                                }
                            }
                        }
                    }
                }
            }
        }

        for (std::vector<GameObject *>::iterator it = m_goList.begin(); it != m_goList.end(); ++it)
        {

            GameObject *go = (GameObject *)*it;
            if (go->active)
            {
                //Exercise 12: handle collision between GO_SHIP and GO_ASTEROID using simple distance-based check
                if (go->type == GameObject::GO_MISSILE)
                {
                    go->pos += go->vel * dt;
                    go->decayTimer += 10 * (float)dt;
                    if (go->decayTimer >= 10){
                        go->active = false;
                        go->decayTimer = 0.f;
                    }

                    if (go->type == GameObject::GO_MISSILE){

                        if (go->pos.y > m_worldHeight)
                            go->pos.y -= m_worldHeight;
                        else if (go->pos.y < 0)
                            go->pos.y += m_worldHeight;

                        if (go->pos.x > m_worldWidth)
                            go->pos.x -= m_worldWidth;
                        else if (go->pos.x < 0)
                            go->pos.y += m_worldWidth;
                    }
                    for (std::vector<GameObject *>::iterator it2 = m_goList.begin(); it2 != m_goList.end(); ++it2)
                    {
                        GameObject* go2 = (GameObject *)*it2;
                        if (go->active)
                        {
                            if (go2->type == GameObject::GO_ASTEROID){
                                if (CheckCollision(go, go2, dt) == true)
                                {
                                    m1 = go->mass;
                                    m2 = go2->mass;
                                    u1 = go->vel;
                                    u2 = go2->vel;

                                    Vector3 momentum1 = go->mass * go->vel;
                                    Vector3 momentum2 = go2->mass * go2->vel;


                                    // go->vel = (((u1 * (m1 - m2))) + (2 * (m2*u2))) * (1.f / (m1 + m2));
                                    go2->vel = (((u1 * (m2 - m1))) + (2 * (m1*u1))) * (1.f / (m1 + m2));

                                    // v1 = go->vel;
                                }

                            }
                        }

                    }
                }
                if (go->type == GameObject::GO_ASTEROID)
                {

                    go->pos += go->vel * dt;


                    if (go->type == GameObject::GO_ASTEROID){

                        if (go->pos.x > m_worldWidth - 1 || go->pos.x < 1 || go->pos.y > m_worldHeight - 1 || go->pos.y < 1)
                        {
                            go->vel = -go->vel;
                        }
                        for (std::vector<Enemy *>::iterator it2 = m_EnemyList.begin(); it2 != m_EnemyList.end(); ++it2)
                        {
                            Enemy *en = (Enemy *)*it2;
                            if (en->active && en->m_eT == Enemy::ET_BOSS)
                            {

                                Vector3 seperation = go->pos - en->pos;
                                float distSquare = seperation.LengthSquared();
                                float combinedRadiusSquared = go->scale.x + en->scale.x;
                                if (distSquare <= combinedRadiusSquared + 80)
                                {
                                    go->vel = (m_ship->pos - go->pos).Normalized() * 20;
                                }
                            }
                        }
                        //if (go->pos.y > m_worldHeight)
                        //    go->pos.y -= m_worldHeight;
                        //else if (go->pos.y < 0)
                        //    go->pos.y += m_worldHeight;

                        //if (go->pos.x > m_worldWidth)
                        //    go->pos.x -= m_worldWidth;
                        //else if (go->pos.x < 0)
                        //    go->pos.y += m_worldWidth;
                    }
                    float distSquared = (go->pos - m_ship->pos).LengthSquared();
                    float combinedRadiusSquared = (go->scale.x + m_ship->scale.x) * (go->scale.x + m_ship->scale.x);
                    if (distSquared <= combinedRadiusSquared)
                    {

                        --waveCount;
                        m_health -= 20;

                        go->active = false;
                        break;
                    }




                    for (std::vector<GameObject *>::iterator it2 = m_goList.begin(); it2 != m_goList.end(); ++it2)
                    {
                        GameObject* go2 = (GameObject *)*it2;
                        if (go->active)
                        {
                            if (go2->type == GameObject::GO_ASTEROID){
                                if (CheckCollision(go, go2, dt) == true)
                                {
                                    m1 = go->mass;
                                    m2 = go2->mass;
                                    u1 = go->vel;
                                    u2 = go2->vel;

                                    Vector3 momentum1 = go->mass * go->vel;
                                    Vector3 momentum2 = go2->mass * go2->vel;


                                    go->vel = (((u1 * (m1 - m2))) + (2 * (m2*u2))) * (1.f / (m1 + m2));
                                    go2->vel = (((u1 * (m2 - m1))) + (2 * (m1*u1))) * (1.f / (m1 + m2));

                                    // v1 = go->vel;
                                }

                            }
                        }

                    }

                }

                //Exercise 16: unspawn bullets when they leave screen
                if (go->type == GameObject::GO_BULLET)
                {
                    go->pos += go->vel * dt;


                    if (go->type == GameObject::GO_BULLET){
                        if (go->pos.y > m_worldHeight)
                        {
                            go->active = false;
                        }
                        else if (go->pos.y < 0)
                        {
                            go->active = false;

                        }

                        if (go->pos.x > m_worldWidth)
                        {
                            go->active = false;

                        }
                        else if (go->pos.x < 0)
                        {
                            go->active = false;

                        }
                    }
                    for (std::vector<GameObject *>::iterator it2 = m_goList.begin(); it2 != m_goList.end(); ++it2)
                    {
                        GameObject *go2 = (GameObject *)*it2;
                        if (go2->type == GameObject::GO_ASTEROID)
                        {
                            if (go2->active)
                            {
                                Vector3 seperation = go->pos - go2->pos;
                                float distSquare = seperation.LengthSquared();
                                float combinedRadiusSquared = go->scale.x + go2->scale.x;
                                if (distSquare <= combinedRadiusSquared)
                                {
                                    go->active = false;
                                    go2->active = false;

                                    m_score += 1;
                                    waveCount--;




                                }
                            }
                        }
                    }

                }
                //Exercise 18: collision check between GO_BULLET and GO_ASTEROID
            }
        }

    }
}


void SceneAssignment1::RenderGO(GameObject *go)
{

    float rotation = Math::RadianToDegree(atan2(m_ship->vel.y, m_ship->vel.x));
    float goRotation = Math::RadianToDegree(atan2(go->vel.y, go->vel.x));
    switch (go->type)
    {
    case GameObject::GO_SHIP:

        modelStack.PushMatrix();
        modelStack.Translate(go->pos.x, go->pos.y, go->pos.z);
        modelStack.Rotate(rotation, 0, 0, 1);
        modelStack.Scale(go->scale.x, go->scale.y, go->scale.z);
        RenderMesh(meshList[GEO_SHIP], false);
        modelStack.PopMatrix();

        break;
    case GameObject::GO_ASTEROID:

        modelStack.PushMatrix();
        modelStack.Translate(go->pos.x, go->pos.y, go->pos.z);
        modelStack.Scale(go->scale.x, go->scale.y, go->scale.z);
        RenderMesh(meshList[GEO_ASTEROID], false);
        modelStack.PopMatrix();
        break;
    case GameObject::GO_BULLET:

        modelStack.PushMatrix();
        modelStack.Translate(go->pos.x, go->pos.y, go->pos.z);
        modelStack.Scale(go->scale.x, go->scale.y, go->scale.z);
        RenderMesh(meshList[GEO_BULLET], false);
        modelStack.PopMatrix();
        break;
    case GameObject::GO_ENEMYBULLET:

        modelStack.PushMatrix();
        modelStack.Translate(go->pos.x, go->pos.y, go->pos.z);
        modelStack.Scale(go->scale.x, go->scale.y, go->scale.z);
        RenderMesh(meshList[GEO_ENEMYBULLET], false);
        modelStack.PopMatrix();
        break;
    case GameObject::GO_MINION:

        modelStack.PushMatrix();
        modelStack.Translate(go->pos.x, go->pos.y, go->pos.z);
        modelStack.Scale(go->scale.x, go->scale.y, go->scale.z);
        modelStack.Rotate(goRotation, 0, 0, 1);

        RenderMesh(meshList[GEO_MINION], false);
        modelStack.PopMatrix();
        break;
    case GameObject::GO_MISSILE:

        modelStack.PushMatrix();
        modelStack.Translate(go->pos.x, go->pos.y, go->pos.z);
        modelStack.Scale(go->scale.x, go->scale.y, go->scale.z);
        modelStack.Rotate(goRotation, 0, 0, 1);

        RenderMesh(meshList[GEO_MISSILE], false);
        modelStack.PopMatrix();
        break;
    case GameObject::GO_BOSS:
    {
                                modelStack.PushMatrix();
                                modelStack.Translate(go->pos.x, go->pos.y, go->pos.z);
                                modelStack.Scale(go->scale.x, go->scale.y, go->scale.z);
                                modelStack.Rotate(goRotation, 0, 0, 1);
                                RenderMesh(meshList[GEO_BOSS], false);
                                modelStack.PopMatrix();

                                modelStack.PushMatrix();
                                modelStack.Translate(go->pos.x, go->pos.y, go->pos.z + 1);
                                modelStack.Scale(go->scale.x + 20, go->scale.y + 20, go->scale.z + 20);
                                modelStack.Rotate(goRotation, 0, 0, 1);
                                RenderMesh(meshList[GEO_FIELD], false);
                                modelStack.PopMatrix(); 
    }
        break;
    }
}

void SceneAssignment1::Render()
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
    if (currState == SS_MENU){
        modelStack.PushMatrix();
        modelStack.Translate(m_worldWidth * 0.5f, m_worldHeight * 0.5f, 2);
        modelStack.Scale(m_worldWidth * 1.5f, m_worldHeight * 1.5f, 1);
        //modelStack.Scale(2.f, 2.f, 2.f);
        RenderMesh(meshList[GEO_MENU], false);
        modelStack.PopMatrix();
    }
    else if (currState == SS_LOSE)
    {
        modelStack.PushMatrix();
        modelStack.Scale(m_worldWidth * 3, m_worldHeight * 3, 1);
        RenderMesh(meshList[GEO_PLANE], false);
        modelStack.PopMatrix();

        std::ostringstream ss;
        
        ss << "YOU LOSE";
        RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(0, 1, 0), 8, 10, 25);
        ss.str("");
        ss << "Your score: " << m_score;
        RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(0, 1, 0), 3, 20, 15);
    }
    else if (currState == SS_WIN)
    {
        modelStack.PushMatrix();
        modelStack.Scale(m_worldWidth * 3, m_worldHeight * 3, 1);
        RenderMesh(meshList[GEO_PLANE], false);
        modelStack.PopMatrix();

        std::ostringstream ss;

        ss << "YOU WIN";
        RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(0, 1, 0), 8, 10, 25);

        ss.str("");
        ss << "Your score: " << m_score;
        RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(0, 1, 0), 3, 20, 15);

    }
   // RenderMesh(meshList[GEO_AXES], false);
    else if (currState != SS_MENU)
    {
        modelStack.PushMatrix();
        modelStack.Scale(m_worldWidth * 3, m_worldHeight * 3, 1);
        RenderMesh(meshList[GEO_PLANE], false);
        modelStack.PopMatrix();
        if (toggleBoost)
        {
            modelStack.PushMatrix();
            modelStack.Translate(50, 3, 2);
            modelStack.Scale(5, 5, 5);
            RenderMesh(meshList[GEO_BOOST], false);
            modelStack.PopMatrix();
        }
        if (elecCharge > 2.f)
        {
            modelStack.PushMatrix();
            modelStack.Translate(50, 3, 2);
            modelStack.Scale(5, 5, 5);
            RenderMesh(meshList[GEO_MISSILE], true);
            modelStack.PopMatrix();
        }
        if (m_ship->active)
        {
            RenderGO(m_ship);
        }
        for (std::vector<GameObject *>::iterator it = m_goList.begin(); it != m_goList.end(); ++it)
        {
            GameObject *go = (GameObject *)*it;
            if (go->active)
            {
                RenderGO(go);
            }
        }
        for (auto q : m_EnemyList)
        {
            if (q->active)
            {
                RenderGO(q);
                {
                    modelStack.PushMatrix();
                    std::ostringstream ss;
                    ss.precision(3);
                    ss << q->m_health;
                    modelStack.Translate(q->pos.x, q->pos.y + 5, q->pos.z);
                    modelStack.Scale(2.f, 2.f, 2.f);
                    RenderText(meshList[GEO_TEXT], ss.str(), Color(0, 1, 0));
                    modelStack.PopMatrix();
                }
            }
        }

        //On screen text
        if (toggleData){
            std::ostringstream ss;
            ss.precision(3);
            ss << "Speed: " << m_speed;
            RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(0, 1, 0), 3, 0, 35);



            //Exercise 5b: Render position, velocity & mass of ship
            ss.str("");
            ss.precision(3);
            ss << "position:" << m_ship->pos;
            RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(0, 1, 0), 3, 0, 15);

            ss.str("");
            ss.precision(3);
            ss << "velocity:" << m_ship->vel;
            RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(0, 1, 0), 3, 0, 10);



            RenderTextOnScreen(meshList[GEO_TEXT], "Asteroid", Color(0, 1, 0), 3, 0, 0);

            ss.str("");
            ss.precision(5);
            ss << "FPS: " << fps;
            RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(0, 1, 0), 3, 0, 30);
        }
        {
            std::ostringstream ss;


            //Exercise 5a: Render m_lives, m_score
            modelStack.PushMatrix();
            ss.str("");
            ss.precision(3);
            ss << "Health:" << m_health;
            modelStack.Translate(m_ship->pos.x - 6, m_ship->pos.y + 8, m_ship->pos.z);
            modelStack.Scale(3.f, 3.f, 3.f);
            RenderText(meshList[GEO_TEXT], ss.str(), Color(1, 1, 0));
            modelStack.PopMatrix();

            ss.str("");
            ss.precision(3);
            ss << "Lives:" << m_lives;
            RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(0, 1, 0), 2, 37.5f, 55);
            ss.str("");
            ss.precision(3);
            ss << "Score:" << m_score;
            RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(0, 1, 0), 2, 55, 55);




            ss.str("");
            ss.precision(3);
            ss << "Wave " << currState  << " : " << waveCount;
            RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(1, 0, 0), 2, 0, 55);
        }
        modelStack.PushMatrix();

        modelStack.Translate(80, 10, 2);
        modelStack.Scale(80, 3, 1);
        RenderMesh(meshList[GEO_CUBE], false);

        modelStack.PopMatrix();
        modelStack.PushMatrix();

        modelStack.Translate(80, 10, 3);
        modelStack.Scale(elecCharge * 7, 2, 1);
        RenderMesh(meshList[GEO_CHARGE], false);
        modelStack.PopMatrix();

        RenderTextOnScreen(meshList[GEO_TEXT], "Charge", Color(0, 0.6f, 1), 3, 35, 7);
    }
}

void SceneAssignment1::Exit()
{


    SceneBase::Exit();
    //Cleanup GameObjects
    while (m_goList.size() > 0)
    {
        GameObject *go = m_goList.back();
        delete go;
        m_goList.pop_back();
    }
    if (m_ship)
    {
        delete m_ship;
        m_ship = NULL;
    }
}
