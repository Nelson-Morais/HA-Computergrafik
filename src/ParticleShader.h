/*
Author: Nelson Morais
This file is part of SpaceTrials ComputerGrafik


Quellen:
	//Particles: https://www.youtube.com/watch?v=GK0jHlv3e3w&ab_channel=TheCherno

*/
#pragma once

#include "BaseShader.h"

#define PARTICLE_ANZAHL 200

//instanziierung 
class ParticleShaderInstance :public BaseShader
{
public:
	ParticleShaderInstance();
	void color(const Color& c);
	const Color& color() const { return Col; }
	virtual ~ParticleShaderInstance() {} ;
	virtual void activate(const BaseCamera& Cam) const;
	void ParticleShaderInstance::modelMatrixHinzufügen(const Matrix& mat, int index);


protected:
	Color Col;
	GLint ColorLoc;
	GLint ViewProjLoc;
	GLint ModelMatLoc;

	float ModelMat[16 * PARTICLE_ANZAHL];

	mutable unsigned int UpdateState;

	enum UPDATESTATES
	{
		COLOR_CHANGED = 1 << 0
	};
};

//instanziierung 
class ParticleShader : public ParticleShaderInstance {
public:

	ParticleShader();
	virtual ~ParticleShader();
protected:
};