#pragma once
#include "ggl.h"
struct UniformTexture {
	GLuint mLocation;
	GLuint mTexture;
	UniformTexture(){
		mLocation = -1;
		mTexture = 0;
	}
};
class Shader {
public:
	GLuint mProgram;
	std::map<std::string, UniformTexture*> mUniformTextures;
	GLint mModelMatrixLocation, mViewMatrixLocation, mProjectionMatrixLocation;
	GLint mPositionLocation, mColorLocation, mTexcoordLocation,mNormalLocation;
	void Init(const char*vs, const char *fs);
	void Bind(float *M, float *V, float *P);
	void SetTexture(const char *name, const char* ImagePath);
};