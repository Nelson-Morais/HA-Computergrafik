#pragma once
/*
Author: Nelson Morais
This file is part of SpaceTrials ComputerGrafik

*/
#include <stdio.h>
#include "model.h"
#include "AstroidShader.h"

#define ASTROID_ANZAHL 255

class AstroidHelper : public Model {

public:
	AstroidHelper(const char* model, Vector center, float);
	virtual ~AstroidHelper() {};
	void update(float dtime);
	virtual void draw(const BaseCamera& Cam);
	bool collision(Model* model);
	
protected:
	void createAstroids(Vector, float);
	struct Astroid {
		Vector center;
		float rotationSpeed;
		Vector pitchRoll;

		Vector position;
		float rotation;
		float rotationVelocity;

		float scale;
		Matrix AstroidMat;
	};
	std::vector<Astroid> AstroidPool;
	AstroidShader* aShader;
	float activeTime;

};