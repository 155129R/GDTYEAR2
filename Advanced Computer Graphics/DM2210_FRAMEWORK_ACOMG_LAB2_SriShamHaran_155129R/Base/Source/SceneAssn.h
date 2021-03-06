#ifndef SCENE_ASSN_H
#define SCENE_ASSN_H

#include "Scene.h"
#include "Mtx44.h"
#include "Camera3.h"
#include "Mesh.h"
#include "MatrixStack.h"
#include "Light.h"
#include "Particle.h"
#include "DepthFBO.h"
#include <vector>
class SceneAssn : public Scene
{
    enum UNIFORM_TYPE
    {
        U_MVP = 0,
        U_MODELVIEW,
        U_MODELVIEW_INVERSE_TRANSPOSE,
        U_MATERIAL_AMBIENT,
        U_MATERIAL_DIFFUSE,
        U_MATERIAL_SPECULAR,
        U_MATERIAL_SHININESS,
        U_LIGHTENABLED,
        U_NUMLIGHTS,
        U_LIGHT0_TYPE,
        U_LIGHT0_POSITION,
        U_LIGHT0_COLOR,
        U_LIGHT0_POWER,
        U_LIGHT0_KC,
        U_LIGHT0_KL,
        U_LIGHT0_KQ,
        U_LIGHT0_SPOTDIRECTION,
        U_LIGHT0_COSCUTOFF,
        U_LIGHT0_COSINNER,
        U_LIGHT0_EXPONENT,
        U_LIGHT1_TYPE,
        U_LIGHT1_POSITION,
        U_LIGHT1_COLOR,
        U_LIGHT1_POWER,
        U_LIGHT1_KC,
        U_LIGHT1_KL,
        U_LIGHT1_KQ,
        U_LIGHT1_SPOTDIRECTION,
        U_LIGHT1_COSCUTOFF,
        U_LIGHT1_COSINNER,
        U_LIGHT1_EXPONENT,

        //U_COLOR_TEXTURE_ENABLED + i
        U_COLOR_TEXTURE_ENABLED,
        U_COLOR_TEXTURE_ENABLED1,

        U_COLOR_TEXTURE,
        U_COLOR_TEXTURE1,

        U_TEXT_ENABLED,
        U_TEXT_COLOR,

        U_FOG_COLOR,
        U_FOG_STARTPOINT,
        U_FOG_ENDPOINT,
        U_FOG_DENSITY,
        U_FOG_TYPE,
        U_FOG_ENABLE,

        U_LIGHT_DEPTH_MVP_GPASS,
        U_LIGHT_DEPTH_MVP,
        U_SHADOW_MAP,

        U_TOTAL,
    };
    enum GEOMETRY_TYPE
    {
        GEO_AXES,
        GEO_CROSSHAIR,
        GEO_LIGHTBALL,
        GEO_SPHERE,
        GEO_SPHERE2,
        GEO_SPHERE3,
        GEO_SPHERE4,
        GEO_SPHERE5,
        GEO_SPHERE6,
        GEO_SPHERE7,
        GEO_SPHERE8,
        GEO_SPHERE9,
        GEO_QUAD,
        GEO_CUBE,
        GEO_RING,
        GEO_CONE,
        GEO_LEFT,
        GEO_RIGHT,
        GEO_TOP,
        GEO_BOTTOM,
        GEO_FRONT,
        GEO_BACK,
        GEO_GRASS_DARKGREEN,
        GEO_GRASS_LIGHTGREEN,
        GEO_HOUSE,
        GEO_TEXT,
        GEO_ANGEL,
        //TSL
        GEO_SKYPLANE,
        GEO_TERRAIN,
        GEO_WATER,
        GEO_TREE,
        GEO_FIRE,
        GEO_SPRITE_ANIMATION,

        GEO_PARTICLE_WATER,
        GEO_PARTICLE_EMBER,
        GEO_PARTICLE_SAKURA,

        GEO_LIGHT_DEPTH_QUAD,

        NUM_GEOMETRY,
    };

    enum RENDER_PASS
    {
        RENDER_PASS_PRE,
        RENDER_PASS_MAIN,
    };

public:
    SceneAssn();
    ~SceneAssn();

    virtual void Init();
    virtual void Update(double dt);
    virtual void Render();
    virtual void Exit();

    void RenderText(Mesh* mesh, std::string text, Color color);
    void RenderTextOnScreen(Mesh* mesh, std::string text, Color color, float size, float x, float y);
    void RenderMeshIn2D(Mesh *mesh, bool enableLight, float size = 1.0f, float x = 0.0f, float y = 0.0f);
    void RenderMesh(Mesh *mesh, bool enableLight);
    void RenderGround();
    void RenderSkybox();
    void RenderSkyPlane(); //TSL
    void RenderTerrain();

    //Particle stuff
    void updateParticles(double dt);
    ParticleObject* GetParticle(void);
    void RenderParticles(ParticleObject* particles);

    void RenderPassGPass();
    void RenderPassMain();
    void RenderWorld();

private:
    //Terrain
    std::vector<unsigned char>m_heightMap;

    unsigned m_vertexArrayID;
    Mesh* meshList[NUM_GEOMETRY];
    unsigned m_programID;
    unsigned m_parameters[U_TOTAL];

    Camera3 camera;

    float rotateAngle;

    MS modelStack;
    MS viewStack;
    MS projectionStack;

    Light lights[2];

    Color fogColor;

    bool bLightEnabled;

    float angleAni;
    float moveDist = 0.f;
    float fps;

    std::vector<Vector3>firePos;

    std::vector<ParticleObject*> particleList;
    Vector3 m_gravity;
    int m_particleCount;
    unsigned MAX_PARTICLE;

    unsigned m_gPassShaderID;
    DepthFBO m_lightDepthFBO;
    Mtx44 m_lightDepthProj;
    Mtx44 m_lightDepthView;

    RENDER_PASS m_renderPass;
};

#endif