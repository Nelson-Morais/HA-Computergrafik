/*
Author: Nelson Morais
This file is part of SpaceTrials ComputerGrafik


Quellen:
	//Particles: https://www.youtube.com/watch?v=GK0jHlv3e3w&ab_channel=TheCherno

*/
#pragma once
#include <vector>
#include <random>
#include "vector.h"
#include "BaseModel.h"
#include "VertexBuffer.h"
#include "ParticleShader.h"
#include "IndexBuffer.h"




#define PARTICLE_ANZAHL 200

struct ParticlePieces
{
	Vector position;
	Vector rotation;
	Vector velocity;
	Vector velocityRotation;
	float sizeBegin;
	float sizeEnd;
	float SizeVariation;
	float LifeTime;
};

class ParticleHelper : public BaseModel
{
public:
	ParticleHelper();
	virtual ~ParticleHelper();
	virtual void update(float dtime);
	virtual void draw(BaseCamera& Cam);
	void Emit(const ParticlePieces& particlePieces);
	void reactorParticles(const Vector& Position, const Vector& velocity);


protected:

	virtual void MakeBox();
	virtual void makePane(const Vector& v0, const Vector& v1, const Vector& v2, const Vector& v3, const Vector& n);
	void init();
	struct Particle {
		Vector position = Vector(0, 0, 0);
		Vector rotation = Vector(0, 0, 0);
		Vector velocity = Vector(0, 0, 0);
		Vector velocityRot = Vector(0, 0, 0);
		float SizeBegin = 0, SizeEnd = 0;
		float LifeTime = 0.0f;
		float LifeRemaining = 0.0f;

		bool isActiv = false;
	};

	std::vector<Particle> ParticlePool;
	unsigned int poolIndex;
	VertexBuffer VB;
	IndexBuffer IB;
};


//Random number generator
class Random
{
public:
	static void Init()
	{
		s_RandomEngine.seed(std::random_device()());
	}

	static float Float()
	{
		return (float)s_Distribution(s_RandomEngine) / (float)std::numeric_limits<uint32_t>::max();
	}

private:
	static std::mt19937 s_RandomEngine;
	static std::uniform_int_distribution<std::mt19937::result_type> s_Distribution;
};
