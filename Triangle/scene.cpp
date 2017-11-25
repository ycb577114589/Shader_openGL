#include "scene.h"
#include "ggl.h"
#include "utils.h"
GLuint vbo,ebo ;
GLuint program; 
GLint positionLocation, modelMatrixLocation, viewMatrixLocation, projectionMatrixLocation,colorLocation;
GLint texcoordLocation, textureLocation;
GLuint texture;
glm::mat4 modelMatrix, viewMatrix, projectionMatrix;

void Init() {  
	//一.创建拿到shader代码部分

	int fileSize = 0;
	unsigned char *vsShaderCode = LoadFileContent("Res/test.vs", fileSize); 
	unsigned char *fsShaderCode = LoadFileContent("Res/test.fs",fileSize);

	GLuint vsShader = CompileShader(GL_VERTEX_SHADER, (char*)vsShaderCode);
	GLuint fsShader = CompileShader(GL_FRAGMENT_SHADER, (char*)fsShaderCode);
	
	//二. 创建程序，并绑定shader部分
	program = CreateProgram(vsShader, fsShader);
	 
	//三. shader已经绑定并且与程序连接，解绑定部分
	delete vsShaderCode;
	delete fsShaderCode;

	glDetachShader(program, vsShader);
	glDetachShader(program, fsShader);

	glDeleteShader(vsShader);
	glDeleteShader(fsShader); 
	float data[] = {
		-0.2f,-0.2f,0.0f,1.0f,  1.0f,0.0f,0.0f,1.0f,
		0.2f,-0.2f,0.0f,1.0f,   0.0f,1.0f,0.0f,1.0f,
		0.0f,0.2f,0.0f,1.0f,    0.0f,0.0f,1.0f,1.0f,
	};
	//vbo状态设置
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 24, data, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	//ebo状态设置
	unsigned short indexes[] = { 0,1,2 };//表示绘制点的时候，从0,1,2顺序绘制
	glGenBuffers(1, &ebo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned short) * 3, indexes, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	positionLocation = glGetAttribLocation(program, "position");
	colorLocation = glGetAttribLocation(program, "color");
	/*
	texcoordLocation = glGetAttribLocation(program, "texcoord");
	textureLocation = glGetUniformLocation(program, "U_Texture");
	texture = CreateTexture2DFromBMP("Res/test.bmp");			 */
	 
}
void SetViewPortSize(float width, float height) {
	projectionMatrix = glm::perspective(60.0f, width / height, 0.1f, 1000.0f);
}
void Draw() {				
	//五。绘图 
	float frameTime = GetFrameTime();
	glClearColor(0.1f, 0.4f, 0.6f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); 
	//1.绑定数据部分
	modelMatrixLocation = glGetUniformLocation(program, "ModelMatrix");
	viewMatrixLocation = glGetUniformLocation(program, "ViewMatrix");
	projectionMatrixLocation = glGetUniformLocation(program, "ProjectionMatrix");
	modelMatrix = glm::translate(0.0f, 0.0f, -0.6f);
	glUseProgram(program);
	
	//位置 ，个数,是否转置,
	glUniformMatrix4fv(modelMatrixLocation,1, GL_FALSE, glm::value_ptr(modelMatrix));
	glUniformMatrix4fv(viewMatrixLocation, 1, GL_FALSE, glm::value_ptr(viewMatrix));
	glUniformMatrix4fv(projectionMatrixLocation, 1, GL_FALSE, glm::value_ptr(projectionMatrix)); 
	//启用position插槽
	glBindBuffer(GL_ARRAY_BUFFER, vbo);		
	glEnableVertexAttribArray(positionLocation);
	glVertexAttribPointer(positionLocation, 4, GL_FLOAT, GL_FALSE, sizeof(float)*8,0);
	//插槽位置，数据分量个数，类型，是否归一化（颜色rgba会对应到0.0~1.0),  两个点的字节距离
	//启用color插槽
	glEnableVertexAttribArray(colorLocation);
	glVertexAttribPointer(colorLocation, 4, GL_FLOAT, GL_FALSE, sizeof(float)*8, (void*)(sizeof (float)*4) );//第一个color与第二个colo数据间隔 

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	 
	  
	//若无ebo可以依此绘制	glDrawArrays(GL_TRIANGLES, 0, 3);
	//从第0个点开始 绘制三个点
	//有了ebo可以指定绘制顺序
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glDrawElements(	GL_TRIANGLES,3,GL_UNSIGNED_SHORT,0);
	//第一个参数 三角形， 第二个参数 多少个索引数据绘图  第三个参数类型， 第四个参数，起始数据
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	//六.初始化为下次做准备
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glUseProgram(0);
	// void glUseProgram(GLuint program);
	// 如果参数为0，则表示所有当前使用的着色器都会被清除 
}	