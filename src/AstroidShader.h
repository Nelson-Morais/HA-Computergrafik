#pragma once

#include "PhongShader.h"
class AstroidShader: public PhongShader
{
public:
	AstroidShader();
	virtual ~AstroidShader();

	virtual void activate(const BaseCamera& cam) const;
	void addModelMatrix(Matrix& mat, int index);

protected:
	GLint ViewProjLoc;
	GLint ModelMatLoc;

	float ModelMat[16 * 255];
	
};

