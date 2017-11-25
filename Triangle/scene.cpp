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
	//һ.�����õ�shader���벿��

	int fileSize = 0;
	unsigned char *vsShaderCode = LoadFileContent("Res/test.vs", fileSize); 
	unsigned char *fsShaderCode = LoadFileContent("Res/test.fs",fileSize);

	GLuint vsShader = CompileShader(GL_VERTEX_SHADER, (char*)vsShaderCode);
	GLuint fsShader = CompileShader(GL_FRAGMENT_SHADER, (char*)fsShaderCode);
	
	//��. �������򣬲���shader����
	program = CreateProgram(vsShader, fsShader);
	 
	//��. shader�Ѿ��󶨲�����������ӣ���󶨲���
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
	//vbo״̬����
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 24, data, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	//ebo״̬����
	unsigned short indexes[] = { 0,1,2 };//��ʾ���Ƶ��ʱ�򣬴�0,1,2˳�����
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
	//�塣��ͼ 
	float frameTime = GetFrameTime();
	glClearColor(0.1f, 0.4f, 0.6f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); 
	//1.�����ݲ���
	modelMatrixLocation = glGetUniformLocation(program, "ModelMatrix");
	viewMatrixLocation = glGetUniformLocation(program, "ViewMatrix");
	projectionMatrixLocation = glGetUniformLocation(program, "ProjectionMatrix");
	modelMatrix = glm::translate(0.0f, 0.0f, -0.6f);
	glUseProgram(program);
	
	//λ�� ������,�Ƿ�ת��,
	glUniformMatrix4fv(modelMatrixLocation,1, GL_FALSE, glm::value_ptr(modelMatrix));
	glUniformMatrix4fv(viewMatrixLocation, 1, GL_FALSE, glm::value_ptr(viewMatrix));
	glUniformMatrix4fv(projectionMatrixLocation, 1, GL_FALSE, glm::value_ptr(projectionMatrix)); 
	//����position���
	glBindBuffer(GL_ARRAY_BUFFER, vbo);		
	glEnableVertexAttribArray(positionLocation);
	glVertexAttribPointer(positionLocation, 4, GL_FLOAT, GL_FALSE, sizeof(float)*8,0);
	//���λ�ã����ݷ������������ͣ��Ƿ��һ������ɫrgba���Ӧ��0.0~1.0),  ��������ֽھ���
	//����color���
	glEnableVertexAttribArray(colorLocation);
	glVertexAttribPointer(colorLocation, 4, GL_FLOAT, GL_FALSE, sizeof(float)*8, (void*)(sizeof (float)*4) );//��һ��color��ڶ���colo���ݼ�� 

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	 
	  
	//����ebo�������˻���	glDrawArrays(GL_TRIANGLES, 0, 3);
	//�ӵ�0���㿪ʼ ����������
	//����ebo����ָ������˳��
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glDrawElements(	GL_TRIANGLES,3,GL_UNSIGNED_SHORT,0);
	//��һ������ �����Σ� �ڶ������� ���ٸ��������ݻ�ͼ  �������������ͣ� ���ĸ���������ʼ����
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	//��.��ʼ��Ϊ�´���׼��
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glUseProgram(0);
	// void glUseProgram(GLuint program);
	// �������Ϊ0�����ʾ���е�ǰʹ�õ���ɫ�����ᱻ��� 
}	