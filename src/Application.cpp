//
//  Application.cpp
//  ogl4
//
//  Created by Philipp Lensing on 16.09.16.
//  Copyright © 2016 Philipp Lensing. All rights reserved.
//

#include "Application.h"
#ifdef WIN32
#include <GL/glew.h>
#include <glfw/glfw3.h>
#define _USE_MATH_DEFINES
#include <math.h>
#else
#define GLFW_INCLUDE_GLCOREARB
#define GLFW_INCLUDE_GLEXT
#include <glfw/glfw3.h>
#endif
#include "lineplanemodel.h"
#include "triangleplanemodel.h"
#include "trianglespheremodel.h"
#include "lineboxmodel.h"
#include "triangleboxmodel.h"
#include "model.h"
#include "ShaderLightmapper.h"


#ifdef WIN32
#define ASSET_DIRECTORY "../../assets/"
#else
#define ASSET_DIRECTORY "../assets/"
#endif


Application::Application(GLFWwindow* pWin) : pWindow(pWin), Cam(pWin), pModel(NULL), ShadowGenerator(2048, 2048), ursprung(3000, 0, 0)
{

	int windowWidth, windowHeight;
	glfwGetWindowSize(pWindow, &windowWidth, &windowHeight);
	createScene();
	PostFX = new PostFXHelper(windowWidth,windowHeight);
	//createNormalTestScene();
	//createShadowTestScene();

}
void Application::start()
{
    glEnable (GL_DEPTH_TEST); // enable depth-testing
    glDepthFunc (GL_LESS); // depth-testing interprets a smaller value as "closer"
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glEnable(GL_BLEND);
    glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void Application::update(float dtime)
{
	

	keyPress(updown, leftright,acceleration);
	spaceship->accelerate(acceleration);
	spaceship->steer(updown, leftright);
	updown = 0;
	leftright = 0;
	spaceship->update(dtime,Cam);
	
	
	for (AstroidHelper* e : astroidPool) {
		e->update(dtime);
		if (e->collision(spaceship->getModel())) {
			Matrix m;
			m.translation(ursprung);
			Model* tmp = spaceship->getModel();
			tmp->transform(m);
			std::cout << "You got hit, going back to the begining!" << std::endl;
		}
	}

	//OLD COLLISION !!!!!!
	/*	for ( Model* e : astroidPool){

		if (e->collision(spaceship->getModel())) {
			Matrix m;
			m.translation(ursprung);
			Model* tmp = spaceship->getModel();
			tmp->transform(m);
			            
		}
	}*/

	//Reactors 
	Vector reactor1Pos(13.7, 1.5, 0);
	Vector reactor2Pos(13.8, 0, -1.9);
	Vector reactor3Pos(13.8, 0, 1.9);
	Matrix shipPos = spaceship->getMatrix();
	Vector turbines(0, 0, 0);

	particles->reactorParticles(shipPos*reactor1Pos, turbines);
	particles->reactorParticles(shipPos * reactor2Pos, turbines);
	particles->reactorParticles(shipPos * reactor3Pos, turbines);
	particles->update(dtime);

	Cam.update();
}




void Application::keyPress(float& updown, float& leftright,float& acceleration) {


	if (glfwGetKey(pWindow, GLFW_KEY_S) == GLFW_PRESS) {
		updown = -1;
	}
	if (glfwGetKey(pWindow, GLFW_KEY_W) == GLFW_PRESS) {
		updown = 1;
	}


	if (glfwGetKey(pWindow, GLFW_KEY_D) == GLFW_PRESS) {
		leftright = -1;
	}

	if (glfwGetKey(pWindow, GLFW_KEY_A) == GLFW_PRESS) {
		leftright = 1;
	}

	if (glfwGetKey(pWindow, GLFW_KEY_SPACE) == GLFW_PRESS) {
		acceleration = -0.1;
	}
	else {
		acceleration = 0.01;
	}


}

void Application::draw()
{
	ShadowGenerator.generate(Models);
	
    // 1. clear screen
    glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	ShaderLightMapper::instance().activate();

	PostFX->beforeDraw();
    // 2. setup shaders and draw models
    for( ModelList::iterator it = Models.begin(); it != Models.end(); ++it )
    {
        (*it)->draw(Cam);
    }
	particles->draw(Cam);
	PostFX->afterDraw();
	Vector sunPosition;
	this->sunPos(Vector(-4300, 2000, -1000),sunPosition);
	PostFX->drawFX(sunPosition);
	ShaderLightMapper::instance().deactivate();
	
    // 3. check once per frame for opengl errors
    GLenum Error = glGetError();
    assert(Error==0);
}
void Application::end()
{
    for( ModelList::iterator it = Models.begin(); it != Models.end(); ++it )
        delete *it;
    
    Models.clear();
}

void Application::createScene()
{
	Matrix sunM,erdeM;
	
	//Space
	Matrix space;
	pModel = new Model(ASSET_DIRECTORY "skybox.obj", true);
	pModel->shader(new PhongShader(), true);
	pModel->shadowCaster(false);
	space.scale(50);
	pModel->transform(space);
	Models.push_back(pModel);
	

	//Astroid OLD !!!!
	/*Matrix astroidM, astroidS, astroidT;
	
	PhongShader* shader = new PhongShader();
	for (int i = 0; i < 300; i++ ){
	astroid = new Model(ASSET_DIRECTORY "Asteroid.obj");
	astroid->shader(shader, true);
	astroidS.scale((rand() % 5 + static_cast<float>((0))));
	astroidM.translation((rand() % 5000 + static_cast<float>((-2000))), (rand() % 5000 + static_cast<float>((-2000))), (rand() % 5000 + static_cast<float>((-2000))));
	astroidT.rotationX((rand() % 20 + static_cast<float>((-10))));
	astroidT.rotationY((rand() % 20 + static_cast<float>((-10))));
	astroidT.rotationZ((rand() % 20 + static_cast<float>((-10))));
	astroid->transform(astroidM* astroidT* astroidS);
	astroidPool.push_back(astroid);
	Models.push_back(astroid);
	}*/

	//Astroid new
	astroids = new AstroidHelper(ASSET_DIRECTORY "rock.obj", Vector(0,0,0), 2);
	Models.push_back(astroids);
	astroidPool.push_back(astroids);
	astroids = new AstroidHelper(ASSET_DIRECTORY "rock.obj", Vector(0, 0, 0), 3);
	Models.push_back(astroids);
	astroidPool.push_back(astroids);
	//spaceship
	spaceship = new SpaceShip();
	spaceship->shader(new PhongShader(), true);
	spaceship->loadModels(ASSET_DIRECTORY "Viper_MK4/Viper-mk-IV-fighter.dae");
	Models.push_back(spaceship);
	
	//sun
	pModel = new TriangleSphereModel(200.0f);
	pModel->shader(new ConstantShader());
	Vector sun(-4300, 2000, -1000);
	sunM.translation(sun);
	pModel->transform(sunM);
	Models.push_back(pModel);

	//Erde
	/**/pModel = new TriangleSphereModel(150.0f);
	PhongShader* ps = new PhongShader();
	ps->diffuseTexture(Texture::LoadShared("../../assets/erde.png"));
	pModel->shader(ps);
	Vector erde(0, 0, 0);
	erdeM.translation(erde);
	pModel->transform(erdeM);
	Models.push_back(pModel);
	
	//Particles für reactors 
	particles = new ParticleHelper();
	this->particleShader = new ParticleShaderInstance();
	this->particleShader->color(Color(255, 0, 0));
	particles->shader(this->particleShader, true);

	/*//reactors
	reactors = new TriangleSphereModel(0.1);
	ConstantShader* cs = new ConstantShader();
	cs->color(Color(255,0,0));
	reactors->shader(cs);
	Models.push_back(reactors);
	*/
	
	Color c = Color(1.0f,1.0f, 1.0f);
	Vector a = Vector(1, 0, 0.1f);
	float innerradius = 45;
	float outerradius = 60;
	
	for (int i = 0; i < 100; i++){
	// point lights
	PointLight* pl = new PointLight();
	pl->position(Vector(sun));
	pl->color(c);
	pl->attenuation(a);
	ShaderLightMapper::instance().addLight(pl);

	/*
	SpotLight* sl = new SpotLight();
	sl->position(Vector(sun));
	sl->color(Color(c));
	sl->direction(Vector(-1, -1, 0));
	sl->innerRadius(45);
	sl->outerRadius(60);
	ShaderLightMapper::instance().addLight(sl);
	
	SpotLight* dl = new SpotLight();
	Vector pos = spaceship->transform().translation();
	dl->direction(Vector(-1, -1, 0));
	dl->position(pos);
	dl->color(c);
	ShaderLightMapper::instance().addLight(dl);
	*/

	}
}

void Application::sunPos(Vector sunPosition, Vector& v)
{
	//modell
	Matrix modelProjectionM, modelViewM;
	modelViewM = Cam.getViewMatrix();
	modelProjectionM = Cam.getProjectionMatrix() * modelViewM;
	v = modelProjectionM * sunPosition;
	//cam
	Vector camPosition, dir;
	Matrix projectionCam, viewMatrix;
	camPosition = Cam.getPosition();
	projectionCam = Cam.getProjectionMatrix();
	dir = projectionCam.invert() * Vector(0, 0, 0);
	viewMatrix = Cam.getViewMatrix();


	Vector worldDir, camToSun;
	worldDir = viewMatrix.invert().transformVec3x3(dir);
	camToSun = sunPosition - camPosition;

	if (worldDir.dot(camToSun) < 0) {
		v.X = 1 - (1 + v.X) / 2;
		v.Y = 1 - (1 + v.Y) / 2;
	}
	else {
		v.X = (1 + v.X) / 2;
		v.Y = (1 + v.Y) / 2;
	}
}

/*void Application::createNormalTestScene()
{
	pModel = new LinePlaneModel(10, 10, 10, 10);
	ConstantShader* pConstShader = new ConstantShader();
	pConstShader->color(Color(0, 0, 0));
	pModel->shader(pConstShader, true);
	// add to render list
	Models.push_back(pModel);


	pModel = new Model(ASSET_DIRECTORY "cube.obj", false);
	pModel->shader(new PhongShader(), true);
	Models.push_back(pModel);


}

void Application::createShadowTestScene()
{
	pModel = new Model(ASSET_DIRECTORY "shadowcube.obj", false);
	pModel->shader(new PhongShader(), true);
	Models.push_back(pModel);

	pModel = new Model(ASSET_DIRECTORY "bunny.dae", false);
	pModel->shader(new PhongShader(), true);
	Models.push_back(pModel);
	
	// directional lights
	DirectionalLight* dl = new DirectionalLight();
	dl->direction(Vector(0, -1, -1));
	dl->color(Color(0.5, 0.5, 0.5));
	dl->castShadows(true);
	ShaderLightMapper::instance().addLight(dl);
	
	SpotLight* sl = new SpotLight();
	sl->position(Vector(2, 2, 0));
	sl->color(Color(0.5, 0.5, 0.5));
	sl->direction(Vector(-1, -1, 0));
	sl->innerRadius(10);
	sl->outerRadius(13);
	sl->castShadows(true);
	ShaderLightMapper::instance().addLight(sl);
}
*/

