#pragma once
#include"ggl.h"
//��������
unsigned char* LoadFileContent(const char*path, int &filesize );
//��ν�shader�������
GLuint CompileShader(GLenum shaderType, const char * shaderCode);
GLuint CreateProgram(GLuint vsShader, GLuint fsShader);
float GetFrameTime();
unsigned char* DecodeBMP(unsigned char* bmpFileData, int &width, int &height); 
//����bmp�ļ����ݵ��ڴ�飬ͼƬ��,��.  ��Ҫ����bmp�ļ���ſ��Ե��ã�����һ��unsigned char*ָ�롣���ص����������ݵ���ʼ��ַ
GLuint CreateTexture2D(unsigned char *pixelData, int width, int height, GLenum type); 
//����һ����ǣ�openGL����������Ψһ��ʶ��,֮��ͨ�������ʶ������ʹ�ó�ʼ�����˵�������Դ
GLuint CreateTexture2DFromBMP(const char *bmpPath); 
//����һ��bmp·��������һ��openGL����������
GLuint CreateBufferObject(GLenum bufferType, GLsizeiptr size, GLenum usage, void *data = nullptr);