#include "PostFXShaders.h"
#include <string>
/*
Author: Nelson Morais
This file is part of SpaceTrials ComputerGrafik

*/

//MAINSHADER für zusammen blenden
MainShader::MainShader() :
	UpdateState(0xFFFFFFFF)
{
	std::string vs = ASSET_DIRECTORY + std::string("vspostfx.glsl");
	std::string fs = ASSET_DIRECTORY + std::string("fspostfx.glsl");

	if (!load(vs.c_str(), fs.c_str())) {
		throw std::exception();
	}
	ColorTexLoc = getParameterID("MainFrame");
	BloomTexLoc = getParameterID("BloomFrame");
	GodRaysTexLoc = getParameterID("GodRaysFrame");

}

MainShader::MainShader(std::string vertexShader, std::string fragmentShader)
	:UpdateState(0xFFFFFFFF) {
	std::string vs = ASSET_DIRECTORY + vertexShader;
	std::string fs = ASSET_DIRECTORY + fragmentShader;

	if (!load(vs.c_str(), fs.c_str())) {
		throw std::exception();
	}
	ColorTexLoc = getParameterID("MainFrame");
	BloomTexLoc = getParameterID("BloomFrame");
	GodRaysTexLoc = getParameterID("GodRaysFrame");
}


void MainShader::activate(const BaseCamera& Cam) const {
	BaseShader::activate(Cam);

	if (UpdateState & FIRST_CREATION) {
		glUniform1i(ColorTexLoc, 0);
		glUniform1i(BloomTexLoc, 1);
		glUniform1i(GodRaysTexLoc,2);
	
	}

	UpdateState = 0x0;
}

void MainShader::deactivate() const {
	BaseShader::deactivate();
}


//Gaussian Blur effect
GaussFilter::GaussFilter() :
	MainShader("vspostfx.glsl", "fsgauss.glsl"),
	GaussUpdateState(0xffffffff)
{
	for (unsigned int i = 0; i < 5; i++) {
		std::string locParameterName = "Weights[" + std::to_string(i) + "]";
		WeightsLoc[i] = getParameterID(locParameterName.c_str());
	}
}

void GaussFilter::activate(const BaseCamera& Cam) const
{
	MainShader::activate(Cam);
	if (GaussUpdateState & WEIGHTS_CHANGED)
		for (unsigned int i = 0; i < 5; i++)
			setParameter(WeightsLoc[i], Weights[i]);
	UpdateState = 0x0;
}



GodRaysFilter::GodRaysFilter() :
	MainShader("vspostfx.glsl", "fsgodrays.glsl"),
	LPos(-500, 500, 0),
	GodRaysUpdateState(0xffffffff)
{
	LPosLoc = getParameterID("LPos");
}

void GodRaysFilter::activate(const BaseCamera& Cam) const
{
	MainShader::activate(Cam);
		if (GodRaysUpdateState & LIGHTPOS_CHANGED) {
	glUniform2f(LPosLoc, LPos.X, LPos.Y);
		}
	GodRaysUpdateState = 0x0;
}

void GodRaysFilter::lpos(Vector LPos) {
	this->LPos = LPos;
	GodRaysUpdateState |= LIGHTUPDATESTATE::LIGHTPOS_CHANGED;

}
