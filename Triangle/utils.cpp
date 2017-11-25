#include "utils.h"
GLuint CompileShader(GLenum shaderType, const char *shaderCode) {
	GLuint shader = glCreateShader(shaderType);
	//传入shader种类创建shader 对象 
	glShaderSource(shader, 1, &shaderCode, nullptr);
	//编译shader
	glCompileShader(shader);
	GLint compileResult = GL_TRUE;
	//查看shader状态
	glGetShaderiv(shader, GL_COMPILE_STATUS, &compileResult);
	/*
	函数原型：
		void glGetShaderiv(int shader, int pname, int[] params, int offset)
		参数含义：
		shader是一个shader的id；
		pname使用GL_COMPILE_STATUS；
		params是返回值，如果一切正常返回GL_TRUE代，否则返回GL_FALSE。
	*/
	//错误日志
	if (compileResult == GL_FALSE) {
		char szLog[1024] = { 0 };
		GLsizei logLen = 0;//实际错误日志长度
		glGetShaderInfoLog(shader, 1024, &logLen, szLog);
		/*
		编译阶段使用glGetShaderInfoLog获取编译错误
		函数原型：
		String glGetShaderInfoLog (int shader)
		参数含义：
		shader是一个顶点shader或者片元shader的id。
		*/
		printf("Compile shader fail error log is : %s \n shader code :\n %s \n ", szLog, shaderCode);
		glDeleteShader(shader);
		shader = 0;
	}
	return shader;
}

GLuint CreateProgram(GLuint vsShader, GLuint fsShader) {
	GLuint program = glCreateProgram();
	//将vs,fs绑定到程序上				
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
		//此处偏移地址后，就是位图中像素数据 的地址
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
	//设置2d纹理， 当纹理放大时用什么算法采集像素， 使用线性过滤
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
	
	
	//设置好后，将像素数据上传至显卡上 , 内存->显存
	glTexImage2D(GL_TEXTURE_2D, 0, type, width,height,0,type,GL_UNSIGNED_BYTE,pixelData);

	//第二个参数 ,bitmapLevel这种概念  0，不同级别的像素数据为多边形着色,级别:比如0代表了128*128  那么1可以设置为64*64 ,2 可以设置为32*32 。当图形大小是64*64就可以从1取
	//当然会占据过大内存。
	//第三个参数，纹理数据在显卡上是何种像素格式
	//第四，五个参数，像素数据宽高，
	//第六个必须写0
	//第七个，纹理数据在内存上是什么格式
	//第八个，每一个像素数据中，每一个分量是什么样的类型
	//最后一个， pixelData， 像素数据在哪里。从这个位置拷贝到显卡上，当然换个名字也可以

	glBindTexture(GL_TEXTURE_2D, 0);
	//初始化，防止之后会有操作影响当前的纹理
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