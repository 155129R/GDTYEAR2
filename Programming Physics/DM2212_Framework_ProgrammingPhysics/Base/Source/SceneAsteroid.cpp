#include "SceneAsteroid.h"
#include "GL\glew.h"
#include "Application.h"
#include <sstream>

SceneAsteroid::SceneAsteroid()
{
}

SceneAsteroid::~SceneAsteroid()
{
}

void SceneAsteroid::Init()
{
    SceneBase::Init();

    //Physics code here
    m_speed = 1.f;

    Math::InitRNG();

    //Exercise 2a: Construct 100 GameObject with type GO_ASTEROID and add into m_goList
    for (unsigned i = 0; i < 100; i++)
    {
        m_goList.push_back(new GameObject(GameObject::GO_ASTEROID));
    }

    //Exercise 2b: Initialize m_lives and m_score
    m_lives = 3;
    m_score = 0;

    //Calculating aspect ratio
    m_worldHeight = 100.f;
    m_worldWidth = m_worldHeight * (float)Application::GetWindowWidth() / Application::GetWindowHeight();

    //Exercise 2c: Construct m_ship, set active, type, scale and pos
    m_ship = new GameObject(GameObject::GO_SHIP);
    m_ship->active = true;
    m_ship->type = GameObject::GO_SHIP;
    m_ship->scale.Set(4.5f, 4.5f, 4.5f);
    m_ship->pos.Set(m_worldWidth * 0.5f, m_worldHeight * 0.5f, 1);
    m_ship->mass = 5.f;

    for (size_t i = 0; i < 40; ++i)
    {
        GameObject *newgo = new GameObject(GameObject::GO_MINION);
    }
}

GameObject* SceneAsteroid::FetchGO()
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

Enemy* SceneAsteroid::FetchEnemy()
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

	FetchEnemy();


}

void SceneAsteroid::Update(double dt)
{
    SceneBase::Update(dt);
    dt *= m_speed;
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
    if (Application::IsKeyPressed('W'))
    {
        //m_force.Set(0, 100.f, 0);
        m_force.y = 80.f;
    }
    if (Application::IsKeyPressed('A'))
    {
        //m_force.Set(-100.f, 0, 0);
        m_force.x = -80.f;
    }
    if (Application::IsKeyPressed('S'))
    {
        //m_force.Set(0, -100.f, 0);
        m_force.y = -80.f;
    }
    if (Application::IsKeyPressed('D'))
    {
        //m_force.Set(100.f, 0, 0);
        m_force.x = 80.f;
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
        GameObject* go = FetchGO();
        go->type = GameObject::GO_ASTEROID;
        go->scale.Set(2.f, 2.f, 2.f);
        go->pos.Set(Math::RandFloatMinMax(0, m_worldWidth), Math::RandFloatMinMax(0, m_worldHeight), 1);
        go->vel.Set(Math::RandFloatMinMax(-5, 5), Math::RandFloatMinMax(-5, 5), 0);

    }
    if (Application::IsKeyPressed('M'))
    {
		Enemy* newMinion = FetchEnemy();
		newMinion->type = GameObject::GO_MINION;
		newMinion->pos.Set(m_worldWidth - (Math::RandIntMinMax(1, 8)), Math::RandFloatMinMax(0, m_worldHeight), Math::RandFloatMinMax(1,3));
		newMinion->scale.Set(5, 5, 5);
		newMinion->vel = (newMinion->pos - m_ship->pos).Normalized();
				
    }
    //Exercise 14: use a key to spawn a bullet
	if (spawnBulletRate < 4.6f)
    spawnBulletRate += dt;
    if (Application::IsKeyPressed(VK_SPACE))
    {

    }

    //Exercise 15: limit the spawn rate of bullets


    //Mouse Section
    static bool bLButtonState = false;
    if (/*!bLButtonState &&*/ Application::IsMousePressed(0))
    {
        bLButtonState = true;
        std::cout << "LBUTTON DOWN" << std::endl;
		if (spawnBulletRate > 0.1)
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
			spawnBulletRate -= 0.6;
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
    }
    else if (bRButtonState && !Application::IsMousePressed(1))
    {
        bRButtonState = false;
        std::cout << "RBUTTON UP" << std::endl;
    }

    //Physics Simulation Section

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

	for (std::vector<Enemy *>::iterator it = m_EnemyList.begin(); it != m_EnemyList.end(); ++it)
	{
		Enemy *en = (Enemy *)*it;
		if (en->type == GameObject::GO_MINION){
			en->vel = (m_ship->pos - en->pos).Normalized();
			en->pos.x += en->vel.x * 10 * dt;
			en->pos.y += en->vel.y * 10 * dt;
			if (en->pos.y > m_worldHeight)
				en->pos.y -= m_worldHeight;
			else if (en->pos.y < 0)
				en->pos.y += m_worldHeight;

			if (en->pos.x > m_worldWidth)
				en->pos.x -= m_worldWidth;
			else if (en->pos.x < 0)
				en->pos.y += m_worldWidth;
		}
	}
    for (std::vector<GameObject *>::iterator it = m_goList.begin(); it != m_goList.end(); ++it)
    {
        GameObject *go = (GameObject *)*it;
        if (go->active)
        {
            //Exercise 12: handle collision between GO_SHIP and GO_ASTEROID using simple distance-based check

            if (go->type == GameObject::GO_ASTEROID)
            {
				go->pos += go->vel * dt;

                float distSquared = (go->pos - m_ship->pos).LengthSquared();
                float combinedRadiusSquared = (go->scale.x + m_ship->scale.x) * (go->scale.x + m_ship->scale.x);
                if (distSquared <= combinedRadiusSquared)
                {
                    m_ship->pos.Set(m_worldWidth * 0.5f, m_worldHeight * 0.5f, 1);
                    m_ship->vel.SetZero();
                    --m_lives;
                    if (m_lives <= 0)
                    {
                        m_lives = 3;
                        m_score = 0;
                    }
                    go->active = false;
                    break;
                }

            }
            //Exercise 13: asteroids should wrap around the screen like the ship
            if (go->type == GameObject::GO_ASTEROID){
                go->pos += go->vel * dt;

                if (go->pos.y > m_worldHeight)
                    go->pos.y -= m_worldHeight;
                else if (go->pos.y < 0)
                    go->pos.y += m_worldHeight;

                if (go->pos.x > m_worldWidth)
                    go->pos.x -= m_worldWidth;
                else if (go->pos.x < 0)
                    go->pos.y += m_worldWidth;
            }
            //Exercise 16: unspawn bullets when they leave screen
            if (go->type == GameObject::GO_BULLET)
            {
				go->pos += go->vel * dt;

                for (std::vector<GameObject *>::iterator it = m_goList.begin(); it != m_goList.end(); ++it)
                {
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
                    GameObject *other = (GameObject *)*it;
                    if (other->type == GameObject::GO_ASTEROID)
                    {
                        if (other->active)
                        {
                            Vector3 seperation = go->pos - other->pos;
                            float distSquare = seperation.LengthSquared();
                            float combinedRadiusSquared = go->scale.x + other->scale.x;
                            if (distSquare <= combinedRadiusSquared)
                            {
                                go->active = false;
                                other->active = false;
                                if (other->type == GameObject::GO_ASTEROID)
                                m_score += 2;

                            }
                        }
                    }
                }

            }
            //Exercise 18: collision check between GO_BULLET and GO_ASTEROID
        }
    }
}


void SceneAsteroid::RenderGO(GameObject *go)
{

    float rotation = Math::RadianToDegree(atan2(m_ship->vel.y, m_ship->vel.x));
    float goRotation = Math::RadianToDegree(atan2(go->vel.y, go->vel.x));
    switch (go->type)
    {
    case GameObject::GO_SHIP:
        //Exercise 4a: render a sphere with radius 1
        modelStack.PushMatrix();
        modelStack.Translate(go->pos.x, go->pos.y, go->pos.z);
        modelStack.Rotate(rotation , 0, 0, 1);
        modelStack.Scale(go->scale.x, go->scale.y, go->scale.z);
        RenderMesh(meshList[GEO_SHIP], false);
        modelStack.PopMatrix();
        //Exercise 17a: render a ship texture or 3D ship model
        //Exercise 17b:	re-orientate the ship with velocity
        break;
    case GameObject::GO_ASTEROID:
        //Exercise 4b: render a cube with length 2
        modelStack.PushMatrix();
        modelStack.Translate(go->pos.x, go->pos.y, go->pos.z);
        modelStack.Scale(go->scale.x, go->scale.y, go->scale.z);
        RenderMesh(meshList[GEO_ASTEROID], false);
        modelStack.PopMatrix();
        break;

    case GameObject::GO_BULLET:
        //Exercise 4c: render a sphere with radius 0.2
        modelStack.PushMatrix();
        modelStack.Translate(go->pos.x, go->pos.y, go->pos.z);
        modelStack.Scale(go->scale.x, go->scale.y, go->scale.z);
        RenderMesh(meshList[GEO_BULLET], false);
        modelStack.PopMatrix();
        break;

    case GameObject::GO_MINION:
        //Exercise 4c: render a sphere with radius 0.2
        modelStack.PushMatrix();
        modelStack.Translate(go->pos.x, go->pos.y, go->pos.z);
        modelStack.Scale(go->scale.x, go->scale.y, go->scale.z);
        modelStack.Rotate(goRotation, 0, 0, 1);

        RenderMesh(meshList[GEO_MINION], false);
        modelStack.PopMatrix();
        break;
    }
}

void SceneAsteroid::Render()
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
    
    modelStack.PushMatrix();
    modelStack.Scale(m_worldWidth * 3, m_worldHeight * 3, 1);
    RenderMesh(meshList[GEO_PLANE], false);
    modelStack.PopMatrix();

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

    //On screen text

    std::ostringstream ss;
    ss.precision(3);
    ss << "Speed: " << m_speed;
    RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(0, 1, 0), 3, 0, 35);

    ss.str("");
    ss.precision(5);
    ss << "FPS: " << fps;
    RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(0, 1, 0), 3, 0, 30);

    //Exercise 5a: Render m_lives, m_score
    ss.str("");
    ss.precision(3);
    ss << "Lives:" << m_lives;
    RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(0, 1, 0), 3, 0, 25);

    ss.str("");
    ss.precision(3);
    ss << "Score:" << m_score;
    RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(0, 1, 0), 3, 0, 20);

    //Exercise 5b: Render position, velocity & mass of ship
    ss.str("");
    ss.precision(3);
    ss << "position:" << m_ship->pos;
    RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(0, 1, 0), 3, 0, 15);

    ss.str("");
    ss.precision(3);
    ss << "velocity:" << m_ship->vel;
    RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(0, 1, 0), 3, 0, 10);

    ss.str("");
    ss.precision(3);
    ss << "Charge:" << spawnBulletRate;
    RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(0, 1, 0), 3, 0, 5);

    RenderTextOnScreen(meshList[GEO_TEXT], "Asteroid", Color(0, 1, 0), 3, 0, 0);
}

void SceneAsteroid::Exit()
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
