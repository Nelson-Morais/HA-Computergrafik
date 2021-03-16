#include "AsteroidHelper.h"
#include <list>
#include "ParticleHelper.h"
#include <math.h>
/*
Author: Nelson Morais
This file is part of SpaceTrials ComputerGrafik

*/
AstroidHelper::AstroidHelper(const char* model, Vector center, float intensity):AstroidPool(ASTROID_ANZAHL),Model(model)
{
	AstroidPool.resize(ASTROID_ANZAHL);
	aShader = new AstroidShader();
	aShader->diffuseTexture(Texture::LoadShared("../../assets/rock.jpg"));
	BaseModel::shader(aShader);

	createAstroids(center, intensity);
}

bool AstroidHelper::collision(Model* model) {
	
		Vector modelPos = model->transform().translation();

		Vector bbMin = this->boundingBox().Min;
		float rAstroidMin = bbMin.length();

		Vector bbMax = this->boundingBox().Max;
		float rAstroidMax = bbMax.length();

		float rAstroid;

		if (rAstroidMax > rAstroidMin) {
			rAstroid = rAstroidMax;
		}
		else {
			rAstroid = rAstroidMin;
		}

		bbMin = model->boundingBox().Min;
		float rModelMin = bbMin.length();
		bbMax = model->boundingBox().Max;
		float rModelMax = bbMax.length();

		float rModel;

		if (rModelMax > rModelMin) {
			rModel = rModelMax;
		}
		else {
			rModel = rModelMin;
		}

		for (Astroid& e : AstroidPool) {

			Vector AstroidPos(e.AstroidMat.m03, e.AstroidMat.m13, e.AstroidMat.m23);

			Vector tmp(modelPos.X - AstroidPos.X, modelPos.Y - AstroidPos.Y, modelPos.Z - AstroidPos.Z);
			float r = tmp.length();

			if (r < rModel + rAstroid * e.scale) {
				return true;
			}
		}
		return false;
}

void AstroidHelper::createAstroids(Vector center, float intensity ) {
	//int i = 0;
	//int roundTrip = 2 * 3.14159265359 * i/ 180;
	//float radius = 20;
	
		for (Astroid& a : AstroidPool) {
				a.position = Vector((0.5f - Random::Float())*3000*intensity, (0.5f - Random::Float()) * 3000*intensity, (0.5f - Random::Float())  *3000*intensity);
				a.rotation = 0;
				a.rotationSpeed = 0.1f+ Random::Float() / 2;
				a.pitchRoll = Vector((0.5f - Random::Float()), (0.5f - Random::Float()), (0.5f - Random::Float()));
				a.rotationVelocity = Random::Float() / 2;
				a.scale = Random::Float()/2;
				a.center = center;
				//i++;
		}
}

 void AstroidHelper::update(float dtime){
	
	 activeTime += dtime;
	 for (Astroid& a : AstroidPool) {
		Matrix TransMat, RotMat, ScaleMat, RotateCenter, RotateRing, MoveRing;
		a.rotation += a.rotationVelocity * dtime;

		TransMat.translation(a.position);
		RotMat.rotationY(a.rotation);
		ScaleMat.scale(Vector(a.scale, a.scale, a.scale));

		RotateCenter.rotationY(activeTime*a.rotationSpeed);
		RotateRing.rotationYawPitchRoll(a.pitchRoll);
		MoveRing.translation(a.center);

		a.AstroidMat = MoveRing * RotateRing * RotateCenter * TransMat * RotMat * ScaleMat ;
	}
 
 }

 //analog wie Model.cpp
 void AstroidHelper::draw(const BaseCamera& Cam) {
	 
	 if (!pShader || !aShader) {
		 std::cout << "AsteroidSystem::draw() no shader found" << std::endl;
		 return;
	 }

	 std::list<Node*> DrawNodes;
	 DrawNodes.push_back(&RootNode);

	 while (!DrawNodes.empty())
	 {
		 Node* pNode = DrawNodes.front();
		 Matrix GlobalTransform;

		 if (pNode->Parent != NULL)
			 pNode->GlobalTrans = pNode->Parent->GlobalTrans * pNode->Trans;
		 else
			 pNode->GlobalTrans = transform() * pNode->Trans;

		 pShader->modelTransform(pNode->GlobalTrans);

		 int index = 0;
		 for (Astroid& p : AstroidPool) {
			 //std::cout << "draw() AstroidHelper" << std::endl;
			 aShader->addModelMatrix(p.AstroidMat * transform(), index);
			 index++;
		 }

		 for (unsigned int i = 0; i < pNode->MeshCount; ++i)
		 {
			 pShader->modelTransform(transform());
			 Mesh& mesh = pMeshes[pNode->Meshes[i]];

			 pShader->activate(Cam);

			 mesh.VB.activate();
			 mesh.IB.activate();
			 glDrawElementsInstanced(GL_TRIANGLES, mesh.IB.indexCount(), mesh.IB.indexFormat(), 0, AstroidPool.size());
			 mesh.IB.deactivate();
			 mesh.VB.deactivate();
		 }
		 for (unsigned int i = 0; i < pNode->ChildCount; ++i)
			 DrawNodes.push_back(&(pNode->Children[i]));

		 DrawNodes.pop_front();
	 }
 }