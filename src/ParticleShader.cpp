/*
Author: Nelson Morais
This file is part of SpaceTrials ComputerGrafik


Quellen:
	//Particles: https://www.youtube.com/watch?v=GK0jHlv3e3w&ab_channel=TheCherno

*/
#include "ParticleShader.h"

#ifdef WIN32
#define ASSET_DIRECTORY "../../assets/"
#else
#define ASSET_DIRECTORY "../assets/"
#endif

ParticleShaderInstance::ParticleShaderInstance() :
	Col(1, 1, 1),
	UpdateState(0xfffffff)
{
	if (!load(ASSET_DIRECTORY "vsparticle.glsl", ASSET_DIRECTORY"fsparticle.glsl")) {
		throw std::exception();
	}
	ViewProjLoc = glGetUniformLocation(ShaderProgram, "ViewProjMat");
	ModelMatLoc = glGetUniformLocation(ShaderProgram, "ModelMats");
	ColorLoc = glGetUniformLocation(ShaderProgram, "Color");


	//matrix hinzufügen 
	Matrix m, m2, m3, m4;
	m2.translation(Vector(0, 0, 0));
	m3.rotationYawPitchRoll(Vector(0, 0, 0));
	m4.scale(1);
	m = m2 * m3 * m4;
	for (int i = 0; i < PARTICLE_ANZAHL; i++) {
		for (int j = 0; j < 16; j++) {
			ModelMat[i * 16 + j] = m.m[j];
		}
	}
}

void ParticleShaderInstance::color(const Color& c)
{
	Col = c;
	UpdateState |= COLOR_CHANGED;
}

void ParticleShaderInstance::activate(const BaseCamera& Cam) const
{
	BaseShader::activate(Cam);
	if (UpdateState & COLOR_CHANGED) {
		glUniform3f(ColorLoc, Col.R, Col.G, Col.B);
	}

	Matrix ViewProjMat = Cam.getProjectionMatrix() * Cam.getViewMatrix();
	glUniformMatrix4fv(ViewProjLoc, 1, GL_FALSE, ViewProjMat.m);
	glUniformMatrix4fv(ModelMatLoc, PARTICLE_ANZAHL, GL_FALSE, ModelMat);

	UpdateState = 0x0;
}
void ParticleShaderInstance::modelMatrixHinzufügen(const Matrix& mat, int index)
{
	for (int i = 0; i < 16; i++) {
		ModelMat[index * 16 + i] = mat.m[i];
	}
}

ParticleShader::ParticleShader()
{
	if (!load(ASSET_DIRECTORY"vsparticel.glsl", ASSET_DIRECTORY"fsparticel.glsl", ASSET_DIRECTORY"gsparticel.glsl")) {
		throw std::exception();
	}
	ViewProjLoc = glGetUniformLocation(ShaderProgram, "ViewProjMat");
	ModelMatLoc = glGetUniformLocation(ShaderProgram, "ModelMats");
	ColorLoc = glGetUniformLocation(ShaderProgram, "Color");
}

ParticleShader::~ParticleShader()
{
}