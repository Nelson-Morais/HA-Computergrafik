#include "AstroidShader.h"
#ifdef WIN32
#define ASSET_DIRECTORY "../../assets/"
#else
#define ASSET_DIRECTORY "../assets/"
#endif


AstroidShader::AstroidShader()
{
	bool loaded = load(ASSET_DIRECTORY"vsastroid.glsl", ASSET_DIRECTORY"fsphong.glsl");
	ViewProjLoc = glGetUniformLocation(ShaderProgram, "ViewProjMat");
	ModelMatLoc = glGetUniformLocation(ShaderProgram, "ModelMats");
	this->assignLocations();
	this->firstColorSet(Color(1, 1, 1));

	Matrix m, m2, m3, m4;
	m2.translation(Vector(0, 0, 0));
	m3.rotationYawPitchRoll(Vector(0, 0, 0));
	m4.scale(1);
	m = m2 * m3 * m4;
	for (int i = 0; i < 255; i++) {
		addModelMatrix(m,i);
	}

}

AstroidShader::~AstroidShader()
{
}

void AstroidShader::activate(const BaseCamera& cam) const
{
	PhongShader::activate(cam);

	Matrix ViewProjMat = cam.getProjectionMatrix() * cam.getViewMatrix();
	glUniformMatrix4fv(ViewProjLoc, 1, GL_FALSE, ViewProjMat.m);

	glUniformMatrix4fv(ModelMatLoc, 255, GL_FALSE, ModelMat);
}

void AstroidShader::addModelMatrix( Matrix& mat, int index)
{
	for (int i = 0; i < 16; i++) {
		ModelMat[index * 16 + i] = mat.m[i];
	}
}