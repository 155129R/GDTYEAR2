#include "SceneText.h"
#include "GL\glew.h"

#include "shader.hpp"
#include "MeshBuilder.h"
#include "Application.h"
#include "Utility.h"
#include "LoadTGA.h"
#include <sstream>
#include "LoadHmap.h"

SceneText::SceneText()
{
}

SceneText::~SceneText()
{
}

void SceneText::Init()
{
	// Black background
	glClearColor(0.0f, 0.0f, 0.4f, 0.0f);
	// Enable depth test
	glEnable(GL_DEPTH_TEST);
	// Accept fragment if it closer to the camera than the former one
	glDepthFunc(GL_LESS); 
	
	glEnable(GL_CULL_FACE);
	
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glGenVertexArrays(1, &m_vertexArrayID);
	glBindVertexArray(m_vertexArrayID);

	m_programID = LoadShaders( "Shader//Fog.vertexshader", "Shader//Fog.fragmentshader" );
	
	// Get a handle for our uniform
	m_parameters[U_MVP] = glGetUniformLocation(m_programID, "MVP");
	m_parameters[U_MODELVIEW] = glGetUniformLocation(m_programID, "MV");
	m_parameters[U_MODELVIEW_INVERSE_TRANSPOSE] = glGetUniformLocation(m_programID, "MV_inverse_transpose");
	m_parameters[U_MATERIAL_AMBIENT] = glGetUniformLocation(m_programID, "material.kAmbient");
	m_parameters[U_MATERIAL_DIFFUSE] = glGetUniformLocation(m_programID, "material.kDiffuse");
	m_parameters[U_MATERIAL_SPECULAR] = glGetUniformLocation(m_programID, "material.kSpecular");
	m_parameters[U_MATERIAL_SHININESS] = glGetUniformLocation(m_programID, "material.kShininess");
	m_parameters[U_LIGHTENABLED] = glGetUniformLocation(m_programID, "lightEnabled");
	m_parameters[U_NUMLIGHTS] = glGetUniformLocation(m_programID, "numLights");
	m_parameters[U_LIGHT0_TYPE] = glGetUniformLocation(m_programID, "lights[0].type");
	m_parameters[U_LIGHT0_POSITION] = glGetUniformLocation(m_programID, "lights[0].position_cameraspace");
	m_parameters[U_LIGHT0_COLOR] = glGetUniformLocation(m_programID, "lights[0].color");
	m_parameters[U_LIGHT0_POWER] = glGetUniformLocation(m_programID, "lights[0].power");
	m_parameters[U_LIGHT0_KC] = glGetUniformLocation(m_programID, "lights[0].kC");
	m_parameters[U_LIGHT0_KL] = glGetUniformLocation(m_programID, "lights[0].kL");
	m_parameters[U_LIGHT0_KQ] = glGetUniformLocation(m_programID, "lights[0].kQ");
	m_parameters[U_LIGHT0_SPOTDIRECTION] = glGetUniformLocation(m_programID, "lights[0].spotDirection");
	m_parameters[U_LIGHT0_COSCUTOFF] = glGetUniformLocation(m_programID, "lights[0].cosCutoff");
	m_parameters[U_LIGHT0_COSINNER] = glGetUniformLocation(m_programID, "lights[0].cosInner");
	m_parameters[U_LIGHT0_EXPONENT] = glGetUniformLocation(m_programID, "lights[0].exponent");
	m_parameters[U_LIGHT1_TYPE] = glGetUniformLocation(m_programID, "lights[1].type");
	m_parameters[U_LIGHT1_POSITION] = glGetUniformLocation(m_programID, "lights[1].position_cameraspace");
	m_parameters[U_LIGHT1_COLOR] = glGetUniformLocation(m_programID, "lights[1].color");
	m_parameters[U_LIGHT1_POWER] = glGetUniformLocation(m_programID, "lights[1].power");
	m_parameters[U_LIGHT1_KC] = glGetUniformLocation(m_programID, "lights[1].kC");
	m_parameters[U_LIGHT1_KL] = glGetUniformLocation(m_programID, "lights[1].kL");
	m_parameters[U_LIGHT1_KQ] = glGetUniformLocation(m_programID, "lights[1].kQ");
	m_parameters[U_LIGHT1_SPOTDIRECTION] = glGetUniformLocation(m_programID, "lights[1].spotDirection");
	m_parameters[U_LIGHT1_COSCUTOFF] = glGetUniformLocation(m_programID, "lights[1].cosCutoff");
	m_parameters[U_LIGHT1_COSINNER] = glGetUniformLocation(m_programID, "lights[1].cosInner");
	m_parameters[U_LIGHT1_EXPONENT] = glGetUniformLocation(m_programID, "lights[1].exponent");
	// Get a handle for our "colorTexture" uniform
	m_parameters[U_COLOR_TEXTURE_ENABLED] = glGetUniformLocation(m_programID, "colorTextureEnabled[0]");
	m_parameters[U_COLOR_TEXTURE] = glGetUniformLocation(m_programID, "colorTexture[0]");

	m_parameters[U_COLOR_TEXTURE_ENABLED1] = glGetUniformLocation(m_programID, "colorTextureEnabled[1]");
	m_parameters[U_COLOR_TEXTURE1] = glGetUniformLocation(m_programID, "colorTexture[1]");
	// Get a handle for our "textColor" uniform
	m_parameters[U_TEXT_ENABLED] = glGetUniformLocation(m_programID, "textEnabled");
	m_parameters[U_TEXT_COLOR] = glGetUniformLocation(m_programID, "textColor");

    m_parameters[U_FOG_COLOR] = glGetUniformLocation(m_programID, "fogParam.color");
    m_parameters[U_FOG_DENSITY] = glGetUniformLocation(m_programID, "fogParam.density");
    m_parameters[U_FOG_ENABLE] = glGetUniformLocation(m_programID, "fogParam.enable");
    m_parameters[U_FOG_ENDPOINT] = glGetUniformLocation(m_programID, "fogParam.endPoint");
    m_parameters[U_FOG_STARTPOINT] = glGetUniformLocation(m_programID, "fogParam.startPoint");
    m_parameters[U_FOG_TYPE] = glGetUniformLocation(m_programID, "fogParam.type");
	
    //fogColor = Color(0.7f, 0.5f, 0.2f);
    fogColor = Color(0.2f, 0.2f, 0.2f);
	// Use our shader
	glUseProgram(m_programID);

	lights[0].type = Light::LIGHT_POINT;
	lights[0].position.Set(0, 100, 0);
    lights[0].color.Set(0.5f, 0.5f, 0.3f);
	lights[0].power = 20.f;
	lights[0].kC = 1.f;
	lights[0].kL = 0.01f;
	lights[0].kQ = 0.001f;
	lights[0].cosCutoff = cos(Math::DegreeToRadian(45));
	lights[0].cosInner = cos(Math::DegreeToRadian(30));
	lights[0].exponent = 3.f;
	lights[0].spotDirection.Set(0.f, 1.f, 0.f);

	lights[1].type = Light::LIGHT_POINT;
	lights[1].position.Set(-15, 150, 1145);
	lights[1].color.Set(1.f, 0.f, 0.f);
	lights[1].power = 100.0f;
	lights[1].kC = 1.f;
	lights[1].kL = 0.01f;
	lights[1].kQ = 0.001f;
	lights[1].cosCutoff = cos(Math::DegreeToRadian(45));
	lights[1].cosInner = cos(Math::DegreeToRadian(30));
	lights[1].exponent = 3.f;
	lights[1].spotDirection.Set(0.f, 1.f, 0.f);
	
	glUniform1i(m_parameters[U_NUMLIGHTS], 1);
	glUniform1i(m_parameters[U_TEXT_ENABLED], 0);

	glUniform1i(m_parameters[U_LIGHT0_TYPE], lights[0].type);
	glUniform3fv(m_parameters[U_LIGHT0_COLOR], 1, &lights[0].color.r);
	glUniform1f(m_parameters[U_LIGHT0_POWER], lights[0].power);
	glUniform1f(m_parameters[U_LIGHT0_KC], lights[0].kC);
	glUniform1f(m_parameters[U_LIGHT0_KL], lights[0].kL);
	glUniform1f(m_parameters[U_LIGHT0_KQ], lights[0].kQ);
	glUniform1f(m_parameters[U_LIGHT0_COSCUTOFF], lights[0].cosCutoff);
	glUniform1f(m_parameters[U_LIGHT0_COSINNER], lights[0].cosInner);
	glUniform1f(m_parameters[U_LIGHT0_EXPONENT], lights[0].exponent);
	
	glUniform1i(m_parameters[U_LIGHT1_TYPE], lights[1].type);
	glUniform3fv(m_parameters[U_LIGHT1_COLOR], 1, &lights[1].color.r);
	glUniform1f(m_parameters[U_LIGHT1_POWER], lights[1].power);
	glUniform1f(m_parameters[U_LIGHT1_KC], lights[1].kC);
	glUniform1f(m_parameters[U_LIGHT1_KL], lights[1].kL);
	glUniform1f(m_parameters[U_LIGHT1_KQ], lights[1].kQ);
	glUniform1f(m_parameters[U_LIGHT1_COSCUTOFF], lights[1].cosCutoff);
	glUniform1f(m_parameters[U_LIGHT1_COSINNER], lights[1].cosInner);
	glUniform1f(m_parameters[U_LIGHT1_EXPONENT], lights[1].exponent);


    glUniform3fv(m_parameters[U_FOG_COLOR], 1, &fogColor.r);
    glUniform1f(m_parameters[U_FOG_STARTPOINT], 10);
    glUniform1f(m_parameters[U_FOG_ENDPOINT], 3000);
    glUniform1f(m_parameters[U_FOG_DENSITY], 0.0005f);
    glUniform1i(m_parameters[U_FOG_TYPE], 0);
    glUniform1f(m_parameters[U_FOG_ENABLE], 0);
  

	camera.Init(Vector3(-15, 230, 530), Vector3(-15, 230, 540), Vector3(0, 1, 0));

	for(int i = 0; i < NUM_GEOMETRY; ++i)
	{
		meshList[i] = NULL;

	}
	meshList[GEO_AXES] = MeshBuilder::GenerateAxes("reference");
	meshList[GEO_CROSSHAIR] = MeshBuilder::GenerateCrossHair("crosshair");
	meshList[GEO_QUAD] = MeshBuilder::GenerateQuad("quad", Color(1, 1, 1), 1.f);
	meshList[GEO_QUAD]->textureID = LoadTGA("Image//calibri.tga");
	meshList[GEO_TEXT] = MeshBuilder::GenerateText("text", 16, 16);
	meshList[GEO_TEXT]->textureID = LoadTGA("Image//calibri.tga");
	meshList[GEO_TEXT]->material.kAmbient.Set(1, 0, 0);
	meshList[GEO_HOUSE] = MeshBuilder::GenerateOBJ("House", "OBJ//House2.obj");
	meshList[GEO_HOUSE]->textureArray[0] = LoadTGA("Image//House2.tga");
    meshList[GEO_ANGEL] = MeshBuilder::GenerateOBJ("Angel", "OBJ//Stacy.obj");
    meshList[GEO_ANGEL]->textureArray[0] = LoadTGA("Image//stacy.tga");

	//meshList[GEO_RING] = MeshBuilder::GenerateRing("ring", Color(1, 0, 1), 36, 1, 0.5f);
	meshList[GEO_LIGHTBALL] = MeshBuilder::GenerateSphere("lightball", Color(1, 1, 1), 18, 36, 1.f);
	//meshList[GEO_SPHERE] = MeshBuilder::GenerateSphere("sphere", Color(1, 0, 0), 18, 36, 10.f);
	//meshList[GEO_CONE] = MeshBuilder::GenerateCone("cone", Color(0.5f, 1, 0.3f), 36, 10.f, 10.f);
	//meshList[GEO_CONE]->material.kDiffuse.Set(0.99f, 0.99f, 0.99f);
	//meshList[GEO_CONE]->material.kSpecular.Set(0.f, 0.f, 0.f);
	//

	// Load the ground mesh and texture
	//meshList[GEO_GRASS_DARKGREEN] = MeshBuilder::GenerateQuad("GRASS_DARKGREEN", Color(1, 1, 1), 1.f);
	//meshList[GEO_GRASS_DARKGREEN]->textureID = LoadTGA("Image//grass_darkgreen.tga");
	//meshList[GEO_GRASS_LIGHTGREEN] = MeshBuilder::GenerateQuad("GEO_GRASS_LIGHTGREEN", Color(1, 1, 1), 1.f);
	//meshList[GEO_GRASS_LIGHTGREEN]->textureID = LoadTGA("Image//grass_lightgreen.tga");

	// Load skybox texture and mesh
	//meshList[GEO_TOP] = MeshBuilder::GenerateQuad("GEO_TOP", Color(1, 1, 1), 1.f);
	//meshList[GEO_TOP]->textureID = LoadTGA("Image//top.tga");

	//meshList[GEO_BOTTOM] = MeshBuilder::GenerateQuad("GEO_BOTTOM", Color(1, 1, 1), 1.f);
	//meshList[GEO_BOTTOM]->textureID = LoadTGA("Image//bottom.tga");

	//meshList[GEO_LEFT] = MeshBuilder::GenerateQuad("GEO_LEFT", Color(1, 1, 1), 1.f);
	//meshList[GEO_LEFT]->textureID = LoadTGA("Image//left.tga");

	//meshList[GEO_RIGHT] = MeshBuilder::GenerateQuad("GEO_RIGHT", Color(1, 1, 1), 1.f);
	//meshList[GEO_RIGHT]->textureID = LoadTGA("Image//right.tga");

	//meshList[GEO_FRONT] = MeshBuilder::GenerateQuad("GEO_FRONT", Color(1, 1, 1), 1.f);
	//meshList[GEO_FRONT]->textureID = LoadTGA("Image//front.tga");

	//meshList[GEO_BACK] = MeshBuilder::GenerateQuad("GEO_BACK", Color(1, 1, 1), 1.f);
	//meshList[GEO_BACK]->textureID = LoadTGA("Image//back.tga");

	meshList[GEO_SKYPLANE] = MeshBuilder::GenerateSkyPlane("GEO_SKYPLANE", Color(1, 1, 1), 128, 200.f, 2000.f, 1.f, 1.f);
	meshList[GEO_SKYPLANE]->textureArray[0] = LoadTGA("Image//NightSky.tga");

	//Read the heightMap data before using the function
	meshList[GEO_TERRAIN] = MeshBuilder::GenerateTerrain("GEO_TERRAIN", "Image//htmap4.raw", m_heightMap);
	meshList[GEO_TERRAIN]->textureArray[0] = LoadTGA("Image//sand1.tga");
	meshList[GEO_TERRAIN]->textureArray[1] = LoadTGA("Image//sand2.tga");
    meshList[GEO_TERRAIN]->material.kSpecular.Set(0.f, 0.f, 0.f);

    meshList[GEO_WATER] = MeshBuilder::GenerateQuad("lava", Color(0,0,0), 1);
	//meshList[GEO_WATER]->textureArray[0] = LoadTGA("Image//lava2.tga");
	meshList[GEO_WATER]->textureArray[1] = LoadTGA("Image//lava1.tga");

    meshList[GEO_SPRITE_ANIMATION] = MeshBuilder::GenerateSpriteAnimation("capguy", 1, 8);
    meshList[GEO_SPRITE_ANIMATION]->textureArray[0] = LoadTGA("Image//capguy.tga");
	meshList[GEO_FIRE] = MeshBuilder::GenerateSpriteAnimation("fire", 5, 5);
	meshList[GEO_FIRE]->textureArray[0] = LoadTGA("Image//fire.tga");
    meshList[GEO_TREE] = MeshBuilder::GenerateQuad("TREE", Color(0,0,0),5.f);
    meshList[GEO_TREE]->textureArray[0] = LoadTGA("Image//sakura.tga");
    {
        SpriteAnimation *sa = dynamic_cast<SpriteAnimation *>(meshList[GEO_SPRITE_ANIMATION]);
        if (sa)
        {
            sa->m_anim = new Animation();
            sa->m_anim->Set(0, 7, 1, 1.f, true);
        }
    }
		{
			SpriteAnimation *sa = dynamic_cast<SpriteAnimation *>(meshList[GEO_FIRE]);
			if (sa)
			{
				sa->m_anim = new Animation();
				sa->m_anim->Set(0, 24, 1, 1.f, true);
			}
		}

    meshList[GEO_PARTICLE_WATER] = MeshBuilder::GenerateQuad("GEO_PARTICLE_WATER", Color(0.1f, 0.1f, 0.1f), 1.f);
    meshList[GEO_PARTICLE_EMBER] = MeshBuilder::GenerateQuad("GEO_PARTICLE_Ember", Color(0.8f, 0.5f, 0.1f), 1.f);
    meshList[GEO_PARTICLE_SAKURA] = MeshBuilder::GenerateQuad("GEO_PARTICLE_SAKURA", Color(1.f, 0.7f, 0.8f), 1.f);

		
	// Projection matrix : 45° Field of View, 4:3 ratio, display range : 0.1 unit <-> 1000 units
	Mtx44 perspective;
	perspective.SetToPerspective(45.0f, 4.0f / 3.0f, 0.1f, 10000000.0f);
	//perspective.SetToOrtho(-80, 80, -60, 60, -1000, 1000);
	projectionStack.LoadMatrix(perspective);
	
	rotateAngle = 0;

	bLightEnabled = true;
	srand(time(0));
	for (int i = 0; i < 50; i++)
	{
		firePos.push_back(Vector3(Math::RandFloatMinMax(-2000, 2000), 80, (Math::RandFloatMinMax(-2000, 2000))));
	}
	//int variables for particle stuff
	m_particleCount = 0;
	MAX_PARTICLE = 1000;
	m_gravity.Set(0, -9.8f, 0);
}

void SceneText::Update(double dt)
{
	if(Application::IsKeyPressed('1'))
		glEnable(GL_CULL_FACE);
	if(Application::IsKeyPressed('2'))
		glDisable(GL_CULL_FACE);
	if(Application::IsKeyPressed('3'))
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	if(Application::IsKeyPressed('4'))
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	
	if(Application::IsKeyPressed('5'))
	{
		lights[0].type = Light::LIGHT_POINT;
		glUniform1i(m_parameters[U_LIGHT0_TYPE], lights[0].type);
	}
	else if(Application::IsKeyPressed('6'))
	{
		lights[0].type = Light::LIGHT_DIRECTIONAL;
		glUniform1i(m_parameters[U_LIGHT0_TYPE], lights[0].type);
	}
	else if(Application::IsKeyPressed('7'))
	{
		lights[0].type = Light::LIGHT_SPOT;
		glUniform1i(m_parameters[U_LIGHT0_TYPE], lights[0].type);
	}
	else if(Application::IsKeyPressed('8'))
	{
		bLightEnabled = true;
	}
	else if(Application::IsKeyPressed('9'))
	{
		bLightEnabled = false;
	}

	if(Application::IsKeyPressed('I'))
		lights[0].position.z -= (float)(100.f * dt);
	if(Application::IsKeyPressed('K'))  
		lights[0].position.z += (float)(100.f * dt);
	if(Application::IsKeyPressed('J'))  
		lights[0].position.x -= (float)(100.f * dt);
	if(Application::IsKeyPressed('L'))  
		lights[0].position.x += (float)(100.f * dt);
	if(Application::IsKeyPressed('O'))  
		lights[0].position.y -= (float)(100.f * dt);
	if(Application::IsKeyPressed('P'))  
		lights[0].position.y += (float)(100.f * dt);
    

    if (rotateAngle < 670.f)
	rotateAngle += (float)(10 * dt);
    if (moveDist <1000.f)
    moveDist += (float)(20 * dt);

	camera.Update(dt);


	fps = (float)(1.f / dt);

    lights[0].position.Set(camera.position.x, camera.position.y + 10, camera.position.z);
    {
        SpriteAnimation *sa = dynamic_cast<SpriteAnimation *>(meshList[GEO_SPRITE_ANIMATION]);
        if (sa)
        {
            if (rotateAngle >= 670.f)
                sa->m_anim->ended = true;
            else{
                sa->Update(dt);
                sa->m_anim->animActive = true;
            }

        }
    }
	{
		SpriteAnimation *sa = dynamic_cast<SpriteAnimation *>(meshList[GEO_FIRE]);
		if (sa)
		{

				sa->Update(dt);
				sa->m_anim->animActive = true;
		}
	}

	updateParticles(dt); //Update dem particles here
}

void SceneText::RenderTerrain()
{
	modelStack.PushMatrix();
	modelStack.Scale(4000.f, 250.f, 4000.f); //values varies
	RenderMesh(meshList[GEO_TERRAIN], true);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(0, 50, 0);
	modelStack.Rotate(-90.f, 1, 0, 0);
	modelStack.Scale(4000.f, 4000.f,1.f); //values varies
	RenderMesh(meshList[GEO_WATER], false);
	modelStack.PopMatrix();
}

void SceneText::RenderText(Mesh* mesh, std::string text, Color color)
{
	if(!mesh || mesh->textureID <= 0)
		return;
	
	glDisable(GL_DEPTH_TEST);
	glUniform1i(m_parameters[U_TEXT_ENABLED], 1);
	glUniform3fv(m_parameters[U_TEXT_COLOR], 1, &color.r);
	glUniform1i(m_parameters[U_LIGHTENABLED], 0);
	glUniform1i(m_parameters[U_COLOR_TEXTURE_ENABLED], 1);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, mesh->textureID);
	glUniform1i(m_parameters[U_COLOR_TEXTURE], 0);
	for(unsigned i = 0; i < text.length(); ++i)
	{
		Mtx44 characterSpacing;
		characterSpacing.SetToTranslation(i * 1.0f, 0, 0); //1.0f is the spacing of each character, you may change this value
		Mtx44 MVP = projectionStack.Top() * viewStack.Top() * modelStack.Top() * characterSpacing;
		glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, &MVP.a[0]);
	
		mesh->Render((unsigned)text[i] * 6, 6);
	}
	glBindTexture(GL_TEXTURE_2D, 0);
	glUniform1i(m_parameters[U_TEXT_ENABLED], 0);
	glEnable(GL_DEPTH_TEST);
}

void SceneText::RenderTextOnScreen(Mesh* mesh, std::string text, Color color, float size, float x, float y)
{
	if(!mesh || mesh->textureID <= 0)
		return;
	
	glDisable(GL_DEPTH_TEST);
	Mtx44 ortho;
	ortho.SetToOrtho(0, 80, 0, 60, -10, 10);
	projectionStack.PushMatrix();
		projectionStack.LoadMatrix(ortho);
		viewStack.PushMatrix();
			viewStack.LoadIdentity();
			modelStack.PushMatrix();
				modelStack.LoadIdentity();
				modelStack.Translate(x, y, 0);
				modelStack.Scale(size, size, size);
				glUniform1i(m_parameters[U_TEXT_ENABLED], 1);
				glUniform3fv(m_parameters[U_TEXT_COLOR], 1, &color.r);
				glUniform1i(m_parameters[U_LIGHTENABLED], 0);
				glUniform1i(m_parameters[U_COLOR_TEXTURE_ENABLED], 1);
				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, mesh->textureID);
				glUniform1i(m_parameters[U_COLOR_TEXTURE], 0);
				for(unsigned i = 0; i < text.length(); ++i)
				{
					Mtx44 characterSpacing;
					characterSpacing.SetToTranslation(i * 1.0f + 0.5f, 0.5f, 0); //1.0f is the spacing of each character, you may change this value
					Mtx44 MVP = projectionStack.Top() * viewStack.Top() * modelStack.Top() * characterSpacing;
					glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, &MVP.a[0]);
	
					mesh->Render((unsigned)text[i] * 6, 6);
				}
				glBindTexture(GL_TEXTURE_2D, 0);
				glUniform1i(m_parameters[U_TEXT_ENABLED], 0);
			modelStack.PopMatrix();
		viewStack.PopMatrix();
	projectionStack.PopMatrix();
	glEnable(GL_DEPTH_TEST);
}

void SceneText::RenderMeshIn2D(Mesh *mesh, bool enableLight, float size, float x, float y)
{
	Mtx44 ortho;
	ortho.SetToOrtho(-80, 80, -60, 60, -10, 10);
	projectionStack.PushMatrix();
		projectionStack.LoadMatrix(ortho);
		viewStack.PushMatrix();
			viewStack.LoadIdentity();
			modelStack.PushMatrix();
				modelStack.LoadIdentity();
				modelStack.Scale(size, size, size);
				modelStack.Translate(x, y, 0);
       
				Mtx44 MVP, modelView, modelView_inverse_transpose;
	
				MVP = projectionStack.Top() * viewStack.Top() * modelStack.Top();
				glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, &MVP.a[0]);
				if(mesh->textureID > 0)
				{
					glUniform1i(m_parameters[U_COLOR_TEXTURE_ENABLED], 1);
					glActiveTexture(GL_TEXTURE0);
					glBindTexture(GL_TEXTURE_2D, mesh->textureID);
					glUniform1i(m_parameters[U_COLOR_TEXTURE], 0);
				}
				else
				{
					glUniform1i(m_parameters[U_COLOR_TEXTURE_ENABLED], 0);
				}
				mesh->Render();
				if(mesh->textureID > 0)
				{
					glBindTexture(GL_TEXTURE_2D, 0);
				}
       
			modelStack.PopMatrix();
		viewStack.PopMatrix();
	projectionStack.PopMatrix();

}

void SceneText::RenderMesh(Mesh *mesh, bool enableLight)
{
	Mtx44 MVP, modelView, modelView_inverse_transpose;
	
	MVP = projectionStack.Top() * viewStack.Top() * modelStack.Top();
	glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, &MVP.a[0]);

    modelView = viewStack.Top() * modelStack.Top(); 
    glUniformMatrix4fv(m_parameters[U_MODELVIEW], 1, GL_FALSE, &modelView.a[0]);
	if(enableLight && bLightEnabled)
	{
		glUniform1i(m_parameters[U_LIGHTENABLED], 1);
		modelView = viewStack.Top() * modelStack.Top();
        glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, &MVP.a[0]);
		glUniformMatrix4fv(m_parameters[U_MODELVIEW], 1, GL_FALSE, &modelView.a[0]);
		modelView_inverse_transpose = modelView.GetInverse().GetTranspose();
		glUniformMatrix4fv(m_parameters[U_MODELVIEW_INVERSE_TRANSPOSE], 1, GL_FALSE, &modelView_inverse_transpose.a[0]);
		
		//load material
		glUniform3fv(m_parameters[U_MATERIAL_AMBIENT], 1, &mesh->material.kAmbient.r);
		glUniform3fv(m_parameters[U_MATERIAL_DIFFUSE], 1, &mesh->material.kDiffuse.r);
		glUniform3fv(m_parameters[U_MATERIAL_SPECULAR], 1, &mesh->material.kSpecular.r);
		glUniform1f(m_parameters[U_MATERIAL_SHININESS], mesh->material.kShininess);
	}
	else
	{	
		glUniform1i(m_parameters[U_LIGHTENABLED], 0);
	}
	for (int i = 0; i < Mesh::MAX_TEXTURES; ++i)
	{
		if (mesh->textureArray[i] > 0)
		{
			glUniform1i(m_parameters[U_COLOR_TEXTURE_ENABLED + i], 1);
			glActiveTexture(GL_TEXTURE0 + i);
			glBindTexture(GL_TEXTURE_2D, mesh->textureArray[i]);
			glUniform1i(m_parameters[U_COLOR_TEXTURE + i], i);

		}
		else
			glUniform1i(m_parameters[U_COLOR_TEXTURE_ENABLED + i], 0);

	}
		
	mesh->Render();

	//if(mesh->textureID > 0)
	//{
		//glBindTexture(GL_TEXTURE_2D, 0);
	//}

}
static const float SKYBOXSIZE = 3000.f;
void SceneText::RenderSkybox(){

	modelStack.PushMatrix();
	modelStack.Rotate(90, 0, 1, 0);
	modelStack.Translate(0, 0, -SKYBOXSIZE / 2 + 2.f);
	modelStack.Scale(SKYBOXSIZE, SKYBOXSIZE, SKYBOXSIZE);
	RenderMesh(meshList[GEO_LEFT], false);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Rotate(-90, 0, 1, 0);
	modelStack.Translate(0, 0, -SKYBOXSIZE / 2 + 2.f);
	modelStack.Scale(SKYBOXSIZE, SKYBOXSIZE, SKYBOXSIZE);
	RenderMesh(meshList[GEO_RIGHT], false);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(0, 0, -SKYBOXSIZE / 2 + 2.f);
	modelStack.Scale(SKYBOXSIZE, SKYBOXSIZE, SKYBOXSIZE);
	RenderMesh(meshList[GEO_FRONT], false);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Rotate(180, 0, 1, 0);
	modelStack.Translate(0, 0, -SKYBOXSIZE / 2 + 2.f);
	modelStack.Scale(SKYBOXSIZE, SKYBOXSIZE, SKYBOXSIZE);
	RenderMesh(meshList[GEO_BACK], false);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Rotate(90, 1, 0, 0);
	modelStack.Translate(0, 0, -SKYBOXSIZE / 2 + 2.f);
	modelStack.Rotate(90, 0, 0, 1);
	modelStack.Scale(SKYBOXSIZE, SKYBOXSIZE, SKYBOXSIZE);
	RenderMesh(meshList[GEO_TOP], false);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Rotate(-90, 1, 0, 0);
	modelStack.Translate(0, 0, -SKYBOXSIZE / 2 + 2.f);
	modelStack.Rotate(-90, 0, 0, 1);
	modelStack.Scale(SKYBOXSIZE, SKYBOXSIZE, SKYBOXSIZE);
	RenderMesh(meshList[GEO_BOTTOM], false);
	modelStack.PopMatrix();
}

void SceneText::RenderSkyPlane(){
	modelStack.PushMatrix();
	modelStack.Translate(500, 1800, -500);
	RenderMesh(meshList[GEO_SKYPLANE], false);
	modelStack.PopMatrix();
}

void SceneText::RenderGround()
{
	modelStack.PushMatrix();
	modelStack.Rotate(-90, 1, 0, 0);
	modelStack.Translate(0, 0, -10);
	modelStack.Rotate(-90, 0, 0, 1);
	modelStack.Scale(400.0f, 400.0f, 400.0f);

	for (int x=0; x<10; x++)
	{
		for (int z=0; z<10; z++)
		{
			modelStack.PushMatrix();
			modelStack.Translate(x-5.0f, z-5.0f, 0.0f);
			if ( ((x*9+z) % 2) == 0)
				RenderMesh(meshList[GEO_GRASS_DARKGREEN], false);
			else
				RenderMesh(meshList[GEO_GRASS_LIGHTGREEN], false);
			modelStack.PopMatrix();
		}
	}
	modelStack.PopMatrix();
}


void SceneText::Render()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	Mtx44 perspective;
	perspective.SetToPerspective(45.0f, 4.0f / 3.0f, 0.1f, 10000.0f);
	//perspective.SetToOrtho(-80, 80, -60, 60, -1000, 1000);
	projectionStack.LoadMatrix(perspective);
	
	// Camera matrix
	viewStack.LoadIdentity();
	viewStack.LookAt(
						camera.position.x, camera.position.y, camera.position.z,
						camera.target.x, camera.target.y, camera.target.z,
						camera.up.x, camera.up.y, camera.up.z
					);
	// Model matrix : an identity matrix (model will be at the origin)
	modelStack.LoadIdentity();

	if(lights[0].type == Light::LIGHT_DIRECTIONAL)
	{
		Vector3 lightDir(lights[0].position.x, lights[0].position.y, lights[0].position.z);
		Vector3 lightDirection_cameraspace = viewStack.Top() * lightDir;
		glUniform3fv(m_parameters[U_LIGHT0_POSITION], 1, &lightDirection_cameraspace.x);
	}
	else if(lights[0].type == Light::LIGHT_SPOT)
	{
		Position lightPosition_cameraspace = viewStack.Top() * lights[0].position;
		glUniform3fv(m_parameters[U_LIGHT0_POSITION], 1, &lightPosition_cameraspace.x);
		Vector3 spotDirection_cameraspace = viewStack.Top() * lights[0].spotDirection;
		glUniform3fv(m_parameters[U_LIGHT0_SPOTDIRECTION], 1, &spotDirection_cameraspace.x);
	}
	else
	{
		Position lightPosition_cameraspace = viewStack.Top() * lights[0].position;
		glUniform3fv(m_parameters[U_LIGHT0_POSITION], 1, &lightPosition_cameraspace.x);
	}
	if(lights[1].type == Light::LIGHT_DIRECTIONAL)
	{
		Vector3 lightDir(lights[1].position.x, lights[1].position.y, lights[1].position.z);
		Vector3 lightDirection_cameraspace = viewStack.Top() * lightDir;
		glUniform3fv(m_parameters[U_LIGHT1_POSITION], 1, &lightDirection_cameraspace.x);
	}
	else if(lights[1].type == Light::LIGHT_SPOT)
	{
		Position lightPosition_cameraspace = viewStack.Top() * lights[1].position;
		glUniform3fv(m_parameters[U_LIGHT1_POSITION], 1, &lightPosition_cameraspace.x);
		Vector3 spotDirection_cameraspace = viewStack.Top() * lights[1].spotDirection;
		glUniform3fv(m_parameters[U_LIGHT1_SPOTDIRECTION], 1, &spotDirection_cameraspace.x);
	}
	else
	{
		Position lightPosition_cameraspace = viewStack.Top() * lights[1].position;
		glUniform3fv(m_parameters[U_LIGHT1_POSITION], 1, &lightPosition_cameraspace.x);
	}
	
	//RenderMesh(meshList[GEO_AXES], false);
    glUniform1i(m_parameters[U_FOG_ENABLE], 1);
	// Render LightBall
	modelStack.PushMatrix();
	modelStack.Translate(lights[0].position.x, lights[0].position.y, lights[0].position.z);
	//RenderMesh(meshList[GEO_LIGHTBALL], false);
	modelStack.PopMatrix();

	//RenderGround();
//  RenderSkybox();
    modelStack.PushMatrix();
    modelStack.Scale(2.5f, 3.f, 2.5f);
	
	RenderSkyPlane();
    modelStack.PopMatrix();

	RenderTerrain();
	


	Vector3 pos;
	pos.Set(-50, 0, -50);
	pos.y = 100.f + 250.f * ReadHeightMap(m_heightMap, pos.x / 4000.f, pos.z / 4000.f);

	modelStack.PushMatrix();
	//modelStack.Translate(-20, 0, -20);

	modelStack.Translate(pos.x,pos.y,pos.z);
    modelStack.Scale(0.1f, 0.1f, 0.1f);
	RenderMesh(meshList[GEO_HOUSE], true);
	modelStack.PopMatrix();
	
	//modelStack.PushMatrix();
	////modelStack.Translate(20, 0, -20);
	//modelStack.Translate(pos.x + 20, pos.y, pos.z);
	//RenderMesh(meshList[GEO_OBJECT], true);
	//modelStack.PopMatrix();


	// Render the crosshair
	//RenderMeshIn2D(meshList[GEO_CROSSHAIR], false, 5.0f);

    pos.Set(-50, 0, 1900 - moveDist);
    pos.y = 10.f + 250.f * ReadHeightMap(m_heightMap, pos.x / 4000.f, pos.z / 4000.f);
    angleAni = Math::RadianToDegree(atan2(-(pos - camera.position).Normalized().x, -(pos - camera.position).Normalized().z));

    modelStack.PushMatrix();

    modelStack.Translate(pos.x, pos.y, pos.z);
    modelStack.Scale(20, 20, 20);
    modelStack.Rotate(angleAni, 0, 1, 0);

    RenderMesh(meshList[GEO_ANGEL], true);
    modelStack.PopMatrix();
	glUniform1i(m_parameters[U_FOG_ENABLE], 0);
	//On screen text
	std::ostringstream ss;
	ss.precision(5);
	ss << "FPS: " << fps;
	RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(0, 1, 0), 3, 0, 6);
	RenderTextOnScreen(meshList[GEO_TEXT], "Volcano residence", Color(0, 1, 0), 3, 0, 0);
	glUniform1i(m_parameters[U_FOG_ENABLE], 1);

    modelStack.PushMatrix();
    pos.Set(-90, 0, -600);
    pos.y = 195.f + 250.f * ReadHeightMap(m_heightMap, pos.x / 4000.f, pos.z / 4000.f);
    angleAni = Math::RadianToDegree(atan2(-(pos - camera.position).Normalized().x, -(pos - camera.position).Normalized().z));
    
    modelStack.Translate(pos.x, pos.y, pos.z);
    modelStack.Scale(80, 80, 80);
    modelStack.Rotate(angleAni, 0, 1, 0);

    RenderMesh(meshList[GEO_TREE], false);
    modelStack.PopMatrix();

    pos.Set(20 + rotateAngle, 0, 20 + rotateAngle);
    pos.y = 20.f + 250.f * ReadHeightMap(m_heightMap, pos.x / 4000.f, pos.z / 4000.f);

    glDisable(GL_CULL_FACE);

    modelStack.PushMatrix();
    modelStack.Translate(pos.x, pos.y, pos.z);
    modelStack.Scale(30, 30, 30);
    RenderMesh(meshList[GEO_SPRITE_ANIMATION], false);
    modelStack.PopMatrix();
    glEnable(GL_CULL_FACE);

	for (auto q : firePos)
	{
		angleAni = Math::RadianToDegree(atan2(-(q - camera.position).Normalized().x, -(q - camera.position).Normalized().z));


		modelStack.PushMatrix();
		modelStack.Translate(q.x, q.y, q.z);
		modelStack.Scale(30, 30, 30);
		modelStack.Rotate(angleAni, 0, 1, 0);

		RenderMesh(meshList[GEO_FIRE], false);
		modelStack.PopMatrix();

	}

	for (auto particle : particleList)
	{
		RenderParticles(particle);
	}
    modelStack.PushMatrix();
    pos.Set(-90, 0, -600);
    pos.y = 195.f + 250.f * ReadHeightMap(m_heightMap, pos.x / 4000.f, pos.z / 4000.f);
    angleAni = Math::RadianToDegree(atan2(-(pos - camera.position).Normalized().x, -(pos - camera.position).Normalized().z));

    modelStack.Translate(pos.x, pos.y, pos.z);
    modelStack.Scale(80, 80, 80);
    modelStack.Rotate(angleAni, 0, 1, 0);

    RenderMesh(meshList[GEO_TREE], true);
    modelStack.PopMatrix();

}

ParticleObject* SceneText::GetParticle(void)
{
	for (std::vector<ParticleObject*>::iterator it = particleList.begin(); it != particleList.end(); ++it)
	{
		ParticleObject* particle = (ParticleObject*) *it;
		if (!particle->active)
		{
			particle->active = true;
			m_particleCount++;
			return particle;
		}
	}
	for (unsigned i = 0; i < 10; ++i)
	{
        ParticleObject* particle = new ParticleObject(ParticleObject_TYPE::P_WATER);
		particleList.push_back(particle);
	}
    for (unsigned i = 0; i < 10; ++i)
    {
        ParticleObject* particle = new ParticleObject(ParticleObject_TYPE::P_EMBER);
        particleList.push_back(particle);
    }
    for (unsigned i = 0; i < 10; ++i)
    {
        ParticleObject* particle = new ParticleObject(ParticleObject_TYPE::P_SAKURA);
        particleList.push_back(particle);
    }
	ParticleObject* particle = particleList.back();
	particle->active = true;
	m_particleCount++;
	return particle;
}

void SceneText::updateParticles(double dt)
{
	if (m_particleCount < MAX_PARTICLE)
	{
		ParticleObject* particle = GetParticle();
        if (particle->type == ParticleObject_TYPE::P_WATER)
        {

            particle->scale.Set(5, 5, 5);
            particle->vel.Set(1, 1, 1);
            particle->rotationSpeed = (Math::RandFloatMinMax(20.f, 40.f));
            particle->pos.Set(Math::RandFloatMinMax(-1700, 1700), 1200, Math::RandFloatMinMax(-1700, 1700));
        }
        else if (particle->type == ParticleObject_TYPE::P_EMBER)
        {
            float scale = Math::RandFloatMinMax(25, 30);
            particle->scale.Set(scale, scale, scale);
            particle->vel.Set(Math::RandFloatMinMax(1, 10), 1, Math::RandFloatMinMax(1, 10));
            particle->rotationSpeed = (Math::RandFloatMinMax(20.f, 40.f));
            float posY = ReadHeightMap(m_heightMap, particle->pos.x / 4000.f, particle->pos.z / 4000.f);
            particle->pos.Set(Math::RandFloatMinMax(-1700, 1700), 0, Math::RandFloatMinMax(-1700, 1700));
        }
        else if (particle->type == ParticleObject_TYPE::P_SAKURA)
        {
            float scale = Math::RandFloatMinMax(5, 8);
            particle->scale.Set(scale, scale, scale);
            particle->vel.Set(Math::RandFloatMinMax(-100, -5), Math::RandFloatMinMax(-100, 15), Math::RandFloatMinMax(-100, -5));
            particle->rotationSpeed = (Math::RandFloatMinMax(10.f, 80.f));
            float posY = 195.f + 250.f * ReadHeightMap(m_heightMap, (-90) / 4000.f, (-600)/ 4000.f);;
            particle->pos.Set(Math::RandFloatMinMax(-90, -80), Math::RandFloatMinMax(posY - 5, posY + 5), Math::RandFloatMinMax(-600, -595));
        }
	}

	for (std::vector<ParticleObject*>::iterator it = particleList.begin(); it != particleList.end(); ++it)
	{
		ParticleObject* particle = (ParticleObject*)*it;
		if (particle->active)
		{
			if (particle->type == ParticleObject_TYPE::P_WATER)
			{
				particle->vel += m_gravity * (float)dt;
				particle->pos += particle->vel * (float) dt * 10.f;
                particle->scale -= Vector3(0.6f, 0.6f, 0.6f) * (float)dt;
				particle->rotation += particle->rotationSpeed * (float)dt;
                if (particle->pos.y < (ReadHeightMap(m_heightMap, particle->pos.x / 4000.f, particle->pos.z / 4000.f) * 350.f))
                {
                    particle->active = false;
                    m_particleCount--;
                }
			}
            if (particle->type == ParticleObject_TYPE::P_EMBER)
            {
                particle->vel -= m_gravity * (float)dt;
                particle->pos += particle->vel * (float)dt * 6.f;
                particle->rotation += particle->rotationSpeed * 10  * (float)dt;
                if (particle->pos.y > 100)
                {
                    particle->active = false;
                    m_particleCount--;
                }
            }
            if (particle->type == ParticleObject_TYPE::P_SAKURA)
            {
                particle->vel += m_gravity * (float)dt;
                particle->pos += particle->vel * (float)dt * 0.5f;
                particle->rotation += particle->rotationSpeed * (float)dt;
                if (particle->pos.y < (ReadHeightMap(m_heightMap, particle->pos.x / 4000.f, particle->pos.z / 4000.f) * 350.f))
                {
                    particle->active = false;
                    m_particleCount--;
                }
            }

		}
	}
}

void SceneText::RenderParticles(ParticleObject* particles)
{
	switch (particles->type)
	{
	case ParticleObject_TYPE::P_WATER:
		angleAni = Math::RadianToDegree(atan2(-(particles->pos - camera.position).Normalized().x, -(particles->pos - camera.position).Normalized().z));
		modelStack.PushMatrix();
		modelStack.Translate(particles->pos.x, particles->pos.y, particles->pos.z);
		modelStack.Scale(particles->scale.x, particles->scale.y, particles->scale.z);
		modelStack.Rotate(angleAni, 0, 1, 0);
		RenderMesh(meshList[GEO_PARTICLE_WATER], false);
		modelStack.PopMatrix();
		break;
    case ParticleObject_TYPE::P_EMBER:
        angleAni = Math::RadianToDegree(atan2(-(particles->pos - camera.position).Normalized().x, -(particles->pos - camera.position).Normalized().z));
        modelStack.PushMatrix();
        modelStack.Translate(particles->pos.x, particles->pos.y, particles->pos.z);
        modelStack.Scale(particles->scale.x, particles->scale.y, particles->scale.z);
        modelStack.Rotate(angleAni, 0, 1, 0);
        RenderMesh(meshList[GEO_FIRE], false);
        modelStack.PopMatrix();
        break;
    case ParticleObject_TYPE::P_SAKURA:
        angleAni = Math::RadianToDegree(atan2(-(particles->pos - camera.position).Normalized().x, -(particles->pos - camera.position).Normalized().z));
        modelStack.PushMatrix();
        modelStack.Translate(particles->pos.x, particles->pos.y, particles->pos.z);
        modelStack.Scale(particles->scale.x, particles->scale.y, particles->scale.z);
        modelStack.Rotate(angleAni, 0, 1, 0);
        RenderMesh(meshList[GEO_PARTICLE_SAKURA], false);
        modelStack.PopMatrix();
        break;
	default:
		break;
	};
}

void SceneText::Exit()
{
	while (particleList.size() > 0)
	{
		ParticleObject* particle = particleList.back();
		delete particle;
		particleList.pop_back();
	}
	// Cleanup VBO
	for(int i = 0; i < NUM_GEOMETRY; ++i)
	{
		if(meshList[i])
			delete meshList[i];
	}
	glDeleteProgram(m_programID);
	glDeleteVertexArrays(1, &m_vertexArrayID);
}
