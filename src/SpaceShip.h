#pragma once
/*
Author: Nelson Morais
This file is part of SpaceTrials ComputerGrafik


*/
#ifndef Tank_hpp
#define Tank_hpp

#include <stdio.h>
#include "model.h"
#include "PhongShader.h"

class SpaceShip : public Model
{

public:
	SpaceShip();
	virtual ~SpaceShip();
	bool loadModels(const char* model);
	void steer(float updown, float leftright);
	void accelerate(float acceleration);
	void update(float dtime, Camera& cam);
	virtual void draw(const BaseCamera& cam);
	void cameraFX(float&);
	const Matrix& getMatrix() { return shipMatrix; };
	Model* getModel() { return shipModel; };
	float getAccel() { return posAccel; };
	

protected:
	float posPitch, posAccel;
	Model* shipModel;
	Vector shipMovement;
	float shipacceleration;
	Matrix shipMatrix;



};

#endif /* SpaceShip_hpp*/