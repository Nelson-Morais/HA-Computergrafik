/*
Author: Nelson Morais
This file is part of SpaceTrials ComputerGrafik

Quellen: 
	//Particles: https://www.youtube.com/watch?v=GK0jHlv3e3w&ab_channel=TheCherno

*/

#include "ParticleHelper.h"
#include <chrono>

std::mt19937 Random::s_RandomEngine;
std::uniform_int_distribution<std::mt19937::result_type> Random::s_Distribution;

ParticleHelper::ParticleHelper() :
	poolIndex(PARTICLE_ANZAHL - 1)
{
	ParticlePool.resize(PARTICLE_ANZAHL);
	MakeBox();
}

ParticleHelper::~ParticleHelper()
{
}

void ParticleHelper::update(float dtime)
{
	for (auto& p : ParticlePool) {
		if (!p.isActiv)
			continue;
		p.LifeRemaining -= dtime;
		if (p.LifeRemaining <= 0.0f) {
			p.isActiv = false;
			continue;
		}

		p.position += p.velocity * dtime;
		p.rotation += p.velocityRot * dtime;
	}
}

void ParticleHelper::draw(BaseCamera& cam) {
	
	BaseModel::draw(cam);
	int particleCount = 0;

	for (auto& p : ParticlePool) {
		if (!p.isActiv) {
			continue;
		}
		Matrix ModelMat, TransMat, RotMat, ScaleMat;
		TransMat.translation(p.position);
		RotMat.rotationYawPitchRoll(p.rotation);
		ScaleMat.scale(p.SizeEnd + ((p.LifeRemaining / p.LifeTime) * (p.SizeBegin - p.SizeEnd)));
		ModelMat = TransMat * RotMat * ScaleMat;
		dynamic_cast<ParticleShaderInstance*>(pShader)->modelMatrixHinzufügen(ModelMat, particleCount);
		particleCount++;
	}
	if (particleCount != 0) {
			pShader->activate(cam);
			VB.activate();
			IB.activate();
			glDrawElementsInstanced(GL_TRIANGLES, IB.indexCount(), IB.indexFormat(), 0, particleCount - 1);
			IB.deactivate();
			VB.deactivate();
	}
}

void ParticleHelper::Emit(const ParticlePieces& particlePieces)
{
	auto& particle = ParticlePool[poolIndex];
	particle.isActiv = true;
	particle.position = particlePieces.position;
	particle.rotation = particlePieces.rotation;
	particle.velocityRot = particlePieces.velocityRotation + (Vector(Random::Float() - 0.5f, Random::Float() - 0.5f, Random::Float() - 0.5f) * 2);
	particle.velocity = particlePieces.velocity + (Vector(Random::Float() - 0.5f, Random::Float() - 0.5f, Random::Float() - 0.5f) * 2);
	particle.LifeTime = particlePieces.LifeTime - (2.0f + (float)Random::Float());
	particle.LifeRemaining = particle.LifeTime;
	particle.SizeBegin = particlePieces.sizeBegin;// +(Random::Float() * 0.2f);
	particle.SizeEnd = particlePieces.sizeEnd;


	if (poolIndex - 1 > PARTICLE_ANZAHL)
		poolIndex = PARTICLE_ANZAHL - 1;
	else {
		poolIndex--;
	}
}

void ParticleHelper::reactorParticles(const Vector& Position, const Vector& velocity)
{
	ParticlePieces tmp;
	tmp.position = Position;
	tmp.velocity = velocity;
	tmp.velocityRotation = (Vector(0, 0, 4));
	tmp.sizeBegin = 0.05;
	tmp.sizeEnd = 0;
	tmp.LifeTime = 3;
	Emit(tmp);
}

void ParticleHelper::MakeBox()
{

	float x = 1;
	float y = 1;
	float z =1;

	/*
	  f---g		//abcd n(0,0,1) //bfgc n(0,1,0)
	 /|  /|
	b---c |		//aehd n(0,-1,0) //efgh n(0,0,-1)
	| e-|-h
	|/  |/		//dcgh n(1,0,0) //aefb (-1,0,0)
	a---d

	  5---6
	 /|  /|
	1---2 |
	| 4-|-7
	|/  |/
	0---3

	1---2                2         1---2
	|   |  wird			/|   und   |  /
	|   |             /  |         |/
	0---3            0---3         0
	*/

	Vector a(-x, -y, z);
	Vector b(-x, y, z);
	Vector c(x, y, z);
	Vector d(x, -y, z);

	Vector e(-x, -y, -z);
	Vector f(-x, y, -z);
	Vector g(x, y, -z);
	Vector h(x, -y, -z);


	VB.begin();


	makePane(a, b, c, d, Vector(0, 0, 1));

	makePane(h, g, f, e, Vector(0, 0, -1));

	makePane(b, f, g, c, Vector(0, 1, 0));

	makePane(e, a, d, h, Vector(0, -1, 0));

	makePane(d, c, g, h, Vector(0, 0, 1));

	makePane(e, f, b, a, Vector(-1, 0, 0));

	VB.end();

	IB.begin();
	for (int i = 0; i < 6; i++) {

		//counter clockwise.
		//triangle1
		int index = i * 4;
		IB.addIndex(index);
		IB.addIndex(index + 3);
		IB.addIndex(index + 2);
		//triangle2
		IB.addIndex(index);
		IB.addIndex(index + 2);
		IB.addIndex(index + 1);
	}

	IB.end();
}


void ParticleHelper::makePane(const Vector& v0, const Vector& v1, const Vector& v2, const Vector& v3, const Vector& n) {

	VB.addNormal(n);
	VB.addTexcoord0(0, 1);
	VB.addVertex(v0);
	VB.addTexcoord0(0, 0);
	VB.addVertex(v1);
	VB.addTexcoord0(1, 0);
	VB.addVertex(v2);
	VB.addTexcoord0(1, 1);
	VB.addVertex(v3);
}

void ParticleHelper::init()
{
	VB.begin();
	VB.addVertex(Vector(0.0, 0.0, 0.0));
	VB.end();
}
