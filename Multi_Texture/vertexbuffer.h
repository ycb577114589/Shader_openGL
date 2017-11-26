#pragma	once
#include "ggl.h"
#include "utils.h"
struct Vertex {
	float Position[4];
	float Color[4];
	float Texcoord[4];
	float Normal[4];
};

//����ʵ�� vbo��cpu�ϵ����ݿ�
class VertexBuffer {
public:
	Vertex *mVertexes;
	int mVertexCount;
	GLuint mVBO;
	void SetSize(int vertexCount);
	void SetPosition(int index, float x, float y, float z, float w = 1.0f);
	void SetColor(int index, float r, float g, float b, float a = 1.0f);
	void SetTexcoord(int index, float x, float y);
	void SetNormal(int index, float x, float y, float z);
	void Bind();
	void Unbind();
	Vertex& Get(int index); 
};