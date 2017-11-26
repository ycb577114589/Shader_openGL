#pragma once
#include"shader.h"
#include "ggl.h"
#include "vertexbuffer.h"
class Ground {
	VertexBuffer *mVertexBuffer; 
	Shader *mShader;
	glm::mat4 mModelMatrix;
public:
	void init();
	void Draw(glm::mat4 &viewMatrix, glm::mat4 &projectionMatrix);
};