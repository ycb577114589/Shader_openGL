#include "utils.h"
GLuint CompileShader(GLenum shaderType, const char *shaderCode) {
	GLuint shader = glCreateShader(shaderType);
	//����shader���ഴ��shader ���� 
	glShaderSource(shader, 1, &shaderCode, nullptr);
	//����shader
	glCompileShader(shader);
	GLint compileResult = GL_TRUE;
	//�鿴shader״̬
	glGetShaderiv(shader, GL_COMPILE_STATUS, &compileResult);
	/*
	����ԭ�ͣ�
		void glGetShaderiv(int shader, int pname, int[] params, int offset)
		�������壺
		shader��һ��shader��id��
		pnameʹ��GL_COMPILE_STATUS��
		params�Ƿ���ֵ�����һ����������GL_TRUE�������򷵻�GL_FALSE��
	*/
	//������־
	if (compileResult == GL_FALSE) {
		char szLog[1024] = { 0 };
		GLsizei logLen = 0;//ʵ�ʴ�����־����
		glGetShaderInfoLog(shader, 1024, &logLen, szLog);
		/*
		����׶�ʹ��glGetShaderInfoLog��ȡ�������
		����ԭ�ͣ�
		String glGetShaderInfoLog (int shader)
		�������壺
		shader��һ������shader����ƬԪshader��id��
		*/
		printf("Compile shader fail error log is : %s \n shader code :\n %s \n ", szLog, shaderCode);
		glDeleteShader(shader);
		shader = 0;
	}
	return shader;
}

GLuint CreateProgram(GLuint vsShader, GLuint fsShader) {
	GLuint program = glCreateProgram();
	//��vs,fs�󶨵�������				
	glAttachShader(program, vsShader);	
	glAttachShader(program, fsShader);	
	//Link
	glLinkProgram(program); 
	GLint nResult;
	glGetProgramiv(program, GL_LINK_STATUS, &nResult);
	if (nResult == GL_FALSE) {
		char log[1024] = { 0 };
		GLsizei writed = 0;
		glGetProgramInfoLog(program, 1024, &writed, log);
		printf("Create CPU program fail error %s\n", log);
		glDeleteProgram(program);
		program = 0;}
	return program;
}

unsigned char* DecodeBMP(unsigned char* bmpFileData, int &width, int &height) {
	if (0x4D42 == *((unsigned short *)bmpFileData)) {
		int pixelDataOffset = *((int*)(bmpFileData + 10));
		//�˴�ƫ�Ƶ�ַ�󣬾���λͼ���������� �ĵ�ַ
		width = *((int *)(bmpFileData + 18));
		height = *((int*)(bmpFileData+  22));
		unsigned char*pixelData = bmpFileData + pixelDataOffset;
		for (int i = 0; i < width*height * 3; i += 3) {
			unsigned	char temp = pixelData[i];
			pixelData[i] = pixelData[i + 2];
			pixelData[i + 2] = temp;
		}
		return pixelData;
	}
	return nullptr;
}
GLuint CreateTexture2D(unsigned char* pixelData, int width, int height, GLenum type) {
	GLuint texture;
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	//����2d���� ������Ŵ�ʱ��ʲô�㷨�ɼ����أ� ʹ�����Թ���
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
	
	
	//���úú󣬽����������ϴ����Կ��� , �ڴ�->�Դ�
	glTexImage2D(GL_TEXTURE_2D, 0, type, width,height,0,type,GL_UNSIGNED_BYTE,pixelData);

	//�ڶ������� ,bitmapLevel���ָ���  0����ͬ�������������Ϊ�������ɫ,����:����0������128*128  ��ô1��������Ϊ64*64 ,2 ��������Ϊ32*32 ����ͼ�δ�С��64*64�Ϳ��Դ�1ȡ
	//��Ȼ��ռ�ݹ����ڴ档
	//�����������������������Կ����Ǻ������ظ�ʽ
	//���ģ�����������������ݿ�ߣ�
	//����������д0
	//���߸��������������ڴ�����ʲô��ʽ
	//�ڰ˸���ÿһ�����������У�ÿһ��������ʲô��������
	//���һ���� pixelData�� ������������������λ�ÿ������Կ��ϣ���Ȼ��������Ҳ����

	glBindTexture(GL_TEXTURE_2D, 0);
	//��ʼ������ֹ֮����в���Ӱ�쵱ǰ������
	return texture;
}	
/*
GLuint CreateTexture2DFromBMP(const char* Path) {
	int nFileSize = 0;
	unsigned char* bmpFileContent = LoadFileContent(Path,nFileSize);
	if (bmpFileContent == nullptr)	return 0;
	int bmpWidth = 0, bmpHeight = 0;
	unsigned char *pixelData=DecodeBMP(bmpFileContent, bmpWidth, bmpHeight);
	GLuint texture = 0;
	if (bmpWidth != 0)
		texture=CreateTexture2D(pixelData, bmpWidth, bmpHeight, GL_RGB);
	delete bmpFileContent; 
	//delete pixelData;
	return texture;
}*/
GLuint CreateTexture2DFromBMP(const char* bmpPath) {
	int nFileSize = 0;
	unsigned char* bmpFileContent = LoadFileContent(bmpPath, nFileSize);
	if (bmpFileContent == nullptr){
		return 0;
	}
	int bmpWidth = 0, bmpHeight = 0;
	unsigned char* pixelData = DecodeBMP(bmpFileContent, bmpWidth, bmpHeight);
	if (bmpWidth == 0) {
		delete bmpFileContent;
		return 0;
	}
	GLuint	texture = CreateTexture2D(pixelData, bmpWidth, bmpHeight, GL_RGB);
	delete	bmpFileContent;
	return texture;
}