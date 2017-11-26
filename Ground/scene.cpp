#include "scene.h"
#include "ggl.h"
#include "utils.h"
#include "Ground.h"  
#include"shader.h"
#include "vertexbuffer.h"
GLint  textureLocation;
GLuint texture;
glm::mat4 modelMatrix, viewMatrix, projectionMatrix;
Ground ground;
Shader *shader;
VertexBuffer *vertexbuffer;

void Init() { 
	vertexbuffer = new VertexBuffer;
	vertexbuffer->SetSize(3);
	vertexbuffer->SetPosition(0, -0.2f, -0.2f, 0.0f);
	vertexbuffer->SetPosition(1, 0.2f, -0.2f, 0.0f);
	vertexbuffer->SetPosition(2, 0, -0.2f, -1.0f);
	vertexbuffer->SetColor(0, 1.0f, 0.0f, 1.0f);
	vertexbuffer->SetColor(1, 0.5f, 0.5f, 0.0f);
	vertexbuffer->SetColor(2, 0.0f, 0.8f, 0.8f);
	vertexbuffer->SetTexcoord(0, 0.0f, 0.0f);
	vertexbuffer->SetTexcoord(1, 1.0f, 0.0f);
	vertexbuffer->SetTexcoord(2, 0.5f, 1.0f);
	shader = new Shader;								
	shader->Init("Res/test.vs", "Res/test.fs");					


	modelMatrix = glm::translate(0.0f, 0.0f, -0.6f);						
	textureLocation = glGetUniformLocation(shader->mProgram, "U_Texture");	
	texture = CreateTexture2DFromBMP("Res/test.bmp");
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	ground.init();
}
void SetViewPortSize(float width, float height) {
	projectionMatrix = glm::perspective(60.0f, width / height, 0.1f, 1000.0f);
}	
void Draw() {
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//五.绘图
	//启用插槽
	float frameTime = GetFrameTime();
	glClearColor(0.1f, 0.4f, 0.6f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	ground.Draw(viewMatrix, projectionMatrix);

	vertexbuffer->Bind();
	shader->Bind(glm::value_ptr(modelMatrix), glm::value_ptr(viewMatrix), glm::value_ptr(projectionMatrix));
	glBindTexture(GL_TEXTURE_2D, texture);
	glUniform1i(textureLocation, 0);
	glDrawArrays(GL_TRIANGLES,0, 3);
	vertexbuffer->Unbind();
	// void glUseProgram(GLuint program);
	// 如果参数为0，则表示所有当前使用的着色器都会被清除 
}	