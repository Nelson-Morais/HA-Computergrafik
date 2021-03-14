#pragma once

#include "PostFXShaders.h"
/*
Author: Nelson Morais
This file is part of SpaceTrials ComputerGrafik

*/

class PostFXHelper {

public:
	PostFXHelper(int, int);
	virtual ~PostFXHelper();
	void InitVB();
	void drawFX(Vector&);
	void beforeDraw();
	void afterDraw();
protected:
	int width, height;
	unsigned int colorBuffers[3];
	unsigned int pingpongFBO[3];
	unsigned int pingpongBuffer[3];
	

	SimpleCamera Cam;
	GodRaysFilter* godraysfilter;
	GLuint VAO;
	GLuint FBO;
	GLuint RBO;
	GaussFilter* gaussfilter;
	MainShader* mainshader;

};

