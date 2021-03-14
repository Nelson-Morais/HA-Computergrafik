/*
Author: Nelson Morais
This file is part of SpaceTrials ComputerGrafik
Quellen:
	https://learnopengl.com/Advanced-Lighting/Bloom
*/
#include "PostFXHelper.h"
#include <GL/glew.h>
#include <cstddef>


PostFXHelper::PostFXHelper(int width, int height) :width(width), height(height) {

	gaussfilter = new GaussFilter();
	mainshader = new MainShader();
	godraysfilter = new GodRaysFilter();

	//render and frame buffer objects
	glGenFramebuffers(1, &FBO);
	glBindFramebuffer(GL_FRAMEBUFFER, FBO);
	glGenRenderbuffers(1, &RBO);
	glBindRenderbuffer(GL_RENDERBUFFER, RBO);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, width, height);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, RBO);

	glGenTextures(3, colorBuffers);

	for (unsigned int i = 0; i < 3; i++)
	{
		glBindTexture(GL_TEXTURE_2D, colorBuffers[i]);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, this->width, this->height, 0, GL_RGBA, GL_FLOAT, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		// attatch the tex to frame buffer
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, GL_TEXTURE_2D, colorBuffers[i], 0);

	}

	//render multi color buffers
	unsigned int attachments[3] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2 };
	glDrawBuffers(3, attachments);

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		std::cout << "Framebuffer not complete!" << std::endl;

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	//2 buffers ( ping pong verfahren ) für die 2 color buffer texture

	glGenFramebuffers(3, pingpongFBO);
	glGenTextures(3, pingpongBuffer);
	for (unsigned int i = 0; i < 3; i++)
	{
		glBindFramebuffer(GL_FRAMEBUFFER, pingpongFBO[i]);
		glBindTexture(GL_TEXTURE_2D, pingpongBuffer[i]);
		glTexImage2D(
			GL_TEXTURE_2D, 0, GL_RGBA16F, this->width, this->height, 0, GL_RGBA, GL_FLOAT, NULL
		);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glFramebufferTexture2D(
			GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, pingpongBuffer[i], 0
		);
	}

	//init the VB for the frame
	this->InitVB();
}


PostFXHelper::~PostFXHelper() {
	delete gaussfilter;
}

void PostFXHelper::beforeDraw()
{
	glBindFramebuffer(GL_FRAMEBUFFER, FBO);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void PostFXHelper::afterDraw()
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}


void PostFXHelper::drawFX(Vector& sunPos) {
	bool horizontal = false, first_iteration = true;
	int amount = 10;
	//bloom
	//GAUSS Filter, amount = 10 sind die iterationen für blur vertical + horizontal
	gaussfilter->activate(Cam);
	for (unsigned int i = 0; i < 10; i++) {
		glBindFramebuffer(GL_FRAMEBUFFER, pingpongFBO[horizontal]);
		glClear(GL_COLOR_BUFFER_BIT);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, first_iteration ? colorBuffers[1] : pingpongBuffer[!horizontal]);
		//render a quad for the frame
		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
		glBindVertexArray(0);
		horizontal = !horizontal;
		if (first_iteration)
			first_iteration = false;

	}

	gaussfilter->deactivate();

	//god rays
	godraysfilter->lpos(sunPos);
	godraysfilter->activate(Cam);
	glBindFramebuffer(GL_FRAMEBUFFER, pingpongFBO[2]);
	glClear(GL_COLOR_BUFFER_BIT);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, colorBuffers[2]);
	//render a quad for the frame
	glBindVertexArray(VAO);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	glBindVertexArray(0);
	godraysfilter->deactivate();


	//mainshader blendet die bilder zusammen. 
	mainshader->activate(Cam);

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glClear(GL_COLOR_BUFFER_BIT);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, colorBuffers[0]);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, pingpongBuffer[!horizontal]);
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, pingpongBuffer[2]);


	//render a quad for the frame
	glBindVertexArray(VAO);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	glBindVertexArray(0);

	mainshader->deactivate();
}

void PostFXHelper::InitVB()
{
	unsigned int VBO;
	float quadVertices[] = {
		-1.0f,  1.0f, 0.0f, 0.0f, 1.0f,
		-1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
		 1.0f,  1.0f, 0.0f, 1.0f, 1.0f,
		 1.0f, -1.0f, 0.0f, 1.0f, 0.0f,
	};

	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));

	//render quad
	glBindVertexArray(VAO);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	glBindVertexArray(0);
}
