//
//  Application.hpp
//  ogl4
//
//  Created by Philipp Lensing on 16.09.16.
//  Copyright © 2016 Philipp Lensing. All rights reserved.
//

#ifndef Application_hpp
#define Application_hpp

#include <stdio.h>
#include <list>
#include "camera.h"
#include "phongshader.h"
#include "constantshader.h"
#include "vertexbuffer.h"
#include "indexbuffer.h"
#include "basemodel.h"
#include "ShadowMapGenerator.h"
#include "SpaceShip.h"
#include "PostFXHelper.h"
#include "ParticleHelper.h"
#include "AsteroidHelper.h"

#define ASTROID_ANZAHL 10

class Application
{
public:
    typedef std::list<BaseModel*> ModelList;
    Application(GLFWwindow* pWin);
    void start();
    void update(float dtime);
    void draw();
    void end();
    void keyPress(float&, float&, float&);
    void Application::sunPos(Vector sunPosition, Vector& vector2D);

protected:
	void createScene();
	//void createNormalTestScene();
	//void createShadowTestScene();
    Camera Cam;
    ModelList Models;
    GLFWwindow* pWindow;
	BaseModel* pModel;
    BaseModel* modelErde;
	ShadowMapGenerator ShadowGenerator;
    SpaceShip* spaceship;
    float updown, leftright,acceleration;
    PostFXHelper* PostFX;
    BaseModel* reactors;
    Matrix reactorM;
    Matrix pos;
    Model* astroid;
    ParticleHelper* particles;
    ParticleShaderInstance* particleShader;
    std::vector<AstroidHelper*> astroidPool;
    Vector ursprung;
   // LineBoxModel lbm;
    AstroidHelper* astroids;

};

#endif /* Application_hpp */
