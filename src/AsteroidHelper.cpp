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

	Vector b = model->transform().translation();

	Vector test = this->boundingBox().Min;
	float rMetroidMin = sqrt(test.X*test.X + test.Y * test.Y + test.Z + test.Z );
	test = this->boundingBox().Max;
	float rMetroidMax = sqrt(test.X * test.X + test.Y * test.Y + test.Z + test.Z);

	float rAstroid;

	if (rMetroidMax > rMetroidMin) {
		rAstroid = rMetroidMax;
	}
	else {
		rAstroid = rMetroidMin;
	}

	test = model->boundingBox().Min;
	float rModelMin = sqrt(test.X * test.X + test.Y * test.Y + test.Z + test.Z);
	test = model->boundingBox().Max;
	float rModelMax = sqrt(test.X * test.X + test.Y * test.Y + test.Z + test.Z);

	float rModel;

	if (rModelMax > rModelMin) {
		rModel = rModelMax;
	}
	else {
		rModel = rModelMin;
	}


	for (Astroid& e : AstroidPool) {

		//AABB test = boundingBox().transform(e.AstroidMat);
		
		//Größe der AABB berechnen.
		/*Vector sizeBBa =  * e.scale;
		Vector sizeBBb = model->boundingBox().size() * model->Transform.scaleR().length();



		bool collisionX = e.AstroidMat.m03 + sizeBBa.X >= b.X && b.X + sizeBBb.X >= e.AstroidMat.m03;
		bool collisionY = e.AstroidMat.m13 + sizeBBa.Y >= b.Y && b.Y + sizeBBb.Y >= e.AstroidMat.m13;
		bool collisionZ = e.AstroidMat.m23 + sizeBBa.Z >= b.Z && b.Z + sizeBBb.Z >= e.AstroidMat.m23;



		if (collisionX && collisionY && collisionZ) {
			return true;
		}*/

		Vector t(e.AstroidMat.m03, e.AstroidMat.m13, e.AstroidMat.m23);

		Vector tmp(b.X - t.X, b.Y - t.Y, b.Z - t.Z);

		float r = sqrt(tmp.X * tmp.X + tmp.Y * tmp.Y + tmp.Z * tmp.Z);

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
				// std::cout << "draw() AstroidHelper" << std::endl;
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