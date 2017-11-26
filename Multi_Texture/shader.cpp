#include "shader.h"
#include"utils.h"
#include "vertexbuffer.h"

void Shader::Init(const char *vs, const char *fs) {
	int nFileSize = 0;
	const char* vsCode = (char*)LoadFileContent(vs, nFileSize);
	const char* fsCode = (char*)LoadFileContent(fs, nFileSize);
	GLuint vsShader = CompileShader(GL_VERTEX_SHADER, vsCode);
	if (vsShader == 0) return;
	GLuint fsShader = CompileShader(GL_FRAGMENT_SHADER, fsCode);
	if (fsShader == 0) return;
	mProgram = CreateProgram(vsShader, fsShader);
	glDeleteShader(vsShader);
	glDeleteShader(fsShader);
	if (mProgram != 0) {
		mModelMatrixLocation = glGetUniformLocation(mProgram, "ModelMatrix");
		mViewMatrixLocation = glGetUniformLocation(mProgram, "ViewMatrix");
		mProjectionMatrixLocation = glGetUniformLocation(mProgram, "ProjectionMatrix");
		mColorLocation = glGetAttribLocation(mProgram, "color");
		mPositionLocation = glGetAttribLocation(mProgram, "position");
		mTexcoordLocation = glGetAttribLocation(mProgram, "texcoord");
		mNormalLocation = glGetAttribLocation(mProgram, "normal");
	}
}
void Shader::Bind(float *M, float *V, float *P) {
	glUseProgram(mProgram);
	glUniformMatrix4fv(mModelMatrixLocation, 1, GL_FALSE, M);
	glUniformMatrix4fv(mViewMatrixLocation, 1, GL_FALSE, V);
	glUniformMatrix4fv(mProjectionMatrixLocation, 1, GL_FALSE, P);
	
	int iIndex = 0;
	for (auto iter = mUniformTextures.begin(); iter != mUniformTextures.end(); ++iter) {
		glActiveTexture(GL_TEXTURE0 + iIndex);
		glBindTexture(GL_TEXTURE_2D, iter->second->mTexture);
		glUniform1i(iter->second->mLocation, iIndex++);
	}
	//对于多重纹理， 每一个插槽开启纹理单元，并为之绑定
	glEnableVertexAttribArray(mPositionLocation);
	glVertexAttribPointer(mPositionLocation, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);

	glEnableVertexAttribArray(mColorLocation);
	glVertexAttribPointer(mColorLocation, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(sizeof(float) * 4));

	glEnableVertexAttribArray(mTexcoordLocation);
	glVertexAttribPointer(mTexcoordLocation, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(sizeof(float) * 8));
	 
	glEnableVertexAttribArray(mNormalLocation);  
	glVertexAttribPointer(mNormalLocation, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(sizeof(float) * 12));
}	
	
void Shader::SetTexture(const char*name, const char *imagePath) {
	auto iter = mUniformTextures.find(name);
	if (iter==mUniformTextures.end() ){
		GLint location = glGetUniformLocation(mProgram, name);
		if (location != -1) {
			UniformTexture* t = new UniformTexture;
			t->mLocation = location;
			t->mTexture = CreateTexture2DFromBMP(imagePath);
			mUniformTextures[name]=t;
		}
	}
	else {
		glDeleteTextures(1, &iter->second->mTexture);
		iter->second->mTexture= CreateTexture2DFromBMP(imagePath);
	}
} 