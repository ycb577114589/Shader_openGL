#pragma once
#include"ggl.h"
//加载数据
unsigned char* LoadFileContent(const char*path, int &filesize );
//如何将shader编译出来
GLuint CompileShader(GLenum shaderType, const char * shaderCode);
GLuint CreateProgram(GLuint vsShader, GLuint fsShader);
float GetFrameTime();
unsigned char* DecodeBMP(unsigned char* bmpFileData, int &width, int &height); 
//传入bmp文件内容的内存块，图片宽,高.  需要解码bmp文件后才可以调用，返回一个unsigned char*指针。返回的是像素数据的起始地址
GLuint CreateTexture2D(unsigned char *pixelData, int width, int height, GLenum type); 
//返回一个标记，openGL中纹理对象的唯一标识符,之后将通过这个标识符，来使用初始化好了的纹理资源
GLuint CreateTexture2DFromBMP(const char *bmpPath); 
//接受一个bmp路径，返回一个openGL里的纹理对象
GLuint CreateBufferObject(GLenum bufferType, GLsizeiptr size, GLenum usage, void *data = nullptr);