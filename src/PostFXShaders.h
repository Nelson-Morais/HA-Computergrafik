#pragma once
/*
Author: Nelson Morais
This file is part of SpaceTrials ComputerGrafik

*/

#include "BaseShader.h"

#ifdef WIN32
#define ASSET_DIRECTORY "../../assets/"
#else
#define ASSET_DIRECTORY "../assets/"
#endif

class MainShader :
	public BaseShader
{
public:
	MainShader();
	virtual ~MainShader() {};
	MainShader(std::string vertexShader, std::string fragmentShader);
	virtual void activate(const BaseCamera& Cam) const;
	virtual void deactivate() const;
protected:
	GLint ColorTexLoc;
	GLint BloomTexLoc;
	GLint GodRaysTexLoc;

	mutable unsigned int UpdateState;
	enum UPDATESTATES
	{
		FIRST_CREATION = 1 << 0
	};
};

class GaussFilter : public MainShader {

public:
	GaussFilter();
	virtual ~GaussFilter() {};
	virtual void activate(const BaseCamera& Cam) const;
protected:
	
	const float Weights[5] = { 0.2270270270f, 0.1945945946f, 0.1216216216f, 0.0540540541f, 0.0162162162f };
	GLint WeightsLoc[5];

	mutable unsigned int GaussUpdateState;
	enum GAUSUPDATESTATES {
		WEIGHTS_CHANGED = 1 << 0
	};
};

class GodRaysFilter : public MainShader {
public:
	GodRaysFilter();
	virtual void activate(const BaseCamera& Cam) const;
	void lpos(Vector LPos);
	
protected:
	GLint LPosLoc;
	Vector LPos;
	

	mutable unsigned int GodRaysUpdateState;
	enum LIGHTUPDATESTATE {
		LIGHTPOS_CHANGED = 1 << 0
	};
};

