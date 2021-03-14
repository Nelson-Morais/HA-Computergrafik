/*
Author: Nelson Morais
This file is part of SpaceTrials ComputerGrafik


*/


#include "SpaceShip.h"

SpaceShip::SpaceShip():posAccel(0), posPitch(0) {};

SpaceShip::~SpaceShip() {}

bool SpaceShip::loadModels(const char* model) {

	if (model != nullptr) {
		Matrix m,r;
		shipModel = new Model(model);
		shipModel->shader(pShader);
		Vector pos(3000,0,0);
		m.translation(pos);
		//r.rotationX(-1);
		shipModel->transform(m);
		return true;
	}
	return false;

}

void SpaceShip::steer(float updown, float leftright){
	shipMovement.X = updown;
	shipMovement.Y = leftright;
}
void SpaceShip::accelerate(float acceleration){

	shipacceleration += acceleration;
	if (shipacceleration > 0) shipacceleration = 0;
	if (shipacceleration < -10) shipacceleration = -10;
	
	shipMovement.Z = shipacceleration;

}

void SpaceShip::cameraFX(float& camShake) {
	//Camera follows ship
	if (shipMovement.X < 0) {

		if (posPitch < 5) posPitch += 0.01;

	}
	else if (shipMovement.X > 0) {

		if (posPitch > -5) posPitch += -0.01;

	}
	else if (shipMovement.X == 0) {
		if (posPitch > 0) {
			posPitch += -0.01;
		}
		if (posPitch < 0) {
			posPitch += 0.01;
		}
	}
	if (shipMovement.Z == -10) {

		if (posAccel < 5) posAccel += 0.01;
		else camShake = (rand() % 7 + static_cast<float>((-3)))/50;
	}
	else {
		if (posAccel > 0) posAccel += -0.005;
		camShake = 0;
	}

}


void SpaceShip::update(float dtime, Camera& cam){
	Matrix shipUrsprung;
	shipUrsprung = shipModel->transform();

	//Pitch and roll
	Matrix pitchMatrix, rollMatrix;
	pitchMatrix.rotationZ(shipMovement.X * dtime);
	rollMatrix.rotationX(shipMovement.Y * dtime);

	//Acceleration
	Matrix accelMatrix;
	accelMatrix.translation(shipMovement.Z * dtime*5,0,0);
	shipMatrix = shipUrsprung * pitchMatrix * rollMatrix * accelMatrix;
	this->shipModel->transform(shipMatrix);

	//shipModel->boundingBox().transform(shipMatrix);
	
	float camShake;
	cameraFX(camShake);
	
	Matrix view, yRotation , zRotation , t;
	
	yRotation.rotationY(-3.14159/2);
	zRotation.rotationZ(-3.14159/2.4);
	t.translation(0 , 0 , 0 + posPitch);
	Matrix shipM = shipModel->transform() * zRotation * yRotation * t;
	Vector target = shipM.translation();
	target.X += camShake/10;
	target.Y += camShake/10;
	target.Z += camShake/10;
	view.lookAt(target, shipM.up(), (shipM.translation()) - shipM.forward() + shipM.up() * (25+posAccel));

	cam.setViewMatrix(view);
	
}
void SpaceShip::draw(const BaseCamera& cam){
	shipModel->draw(cam);

}


