#include "ggl.h"
#include "scene.h"
#include "utils.h"

#pragma  comment(lib,"opengl32.lib")
#pragma comment(lib,"glew32.lib")
#pragma comment(lib,"winmm.lib")
unsigned char *LoadFileContent(const char *path, int &filesize) {
	unsigned char *fileContent = nullptr;
	filesize = 0;
	FILE *pFile = fopen(path, "rb");
	if (pFile) {
		fseek(pFile, 0, SEEK_END);
		int nLen = ftell(pFile);
		if (nLen > 0) {
			rewind(pFile);
			fileContent = new unsigned char[nLen+1];
			fread(fileContent,nLen,sizeof (unsigned char),pFile );
			fileContent[nLen] = '\0';
			filesize = nLen;
		}
		fclose(pFile);
	}	
	return fileContent;
}	
float GetFrameTime() {
	static unsigned long lastTime = 0, timeSinceComputerStart = 0;
	timeSinceComputerStart = timeGetTime();
	unsigned long frameTime = lastTime == 0 ? 0 : timeSinceComputerStart - lastTime;
	lastTime = timeSinceComputerStart;
	return float(frameTime) / 1000.0f;
}
LRESULT CALLBACK GLWindowProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	case WM_CLOSE://�رմ���
		PostQuitMessage(0);
		break;
	}
	return DefWindowProc(hwnd, msg, wParam, lParam);
}

INT WINAPI WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nShowCmd)
{

	//register class		
	WNDCLASSEX wndclass;
	wndclass.cbClsExtra = 0;
	wndclass.cbSize = sizeof(WNDCLASSEX);

	wndclass.cbWndExtra = 0; //����Ϊ����׼������洢�ռ�
	wndclass.hbrBackground = NULL;//���ڱ���������ⶫ������������openGL
	wndclass.hCursor = NULL;//�����
	wndclass.hIcon = LoadCursor(NULL, IDC_ARROW);	//���ɳ������ļ����е�ͼ��
	wndclass.hIconSm = NULL;//���������Ͻ���ʾͼ��
	wndclass.hInstance = hInstance;//windows���ڴ�������ʵ��
	wndclass.lpfnWndProc = GLWindowProc;//������   ��Ϣ��Ӧ����
	wndclass.lpszClassName = L"GLWindow";//Ψһ��ʶ��
	wndclass.lpszMenuName = NULL;//�˵�
	wndclass.style = CS_VREDRAW | CS_HREDRAW;//class style  ��ֱ  ˮƽ
	ATOM atom = RegisterClassEx(&wndclass);//ע�ᣬ�᷵��һ�����
	if (!atom)
	{
		MessageBox(NULL,L"Register Fail",L"Error",MB_OK);
		return 0;
	}
	//create window
	RECT rect;
	rect.left = 0;
	rect.right = 800;
	rect.top = 0;
	rect.bottom = 600;
	AdjustWindowRect(&rect,WS_OVERLAPPEDWINDOW,NULL);
	int windowWidth = rect.right - rect.left;
	int windowHeight = rect.bottom - rect.top;
	HWND hwnd = CreateWindowEx(NULL, L"GLWindow", L"OpenGL Window", WS_OVERLAPPEDWINDOW, 100, 100, windowWidth,windowHeight, NULL, NULL, hInstance, NULL);
	// 1.���ⴰ�ڷ�� 2.֮ǰע���Class name 3.�������� 4.�������ڷ��5,������ʼλ�� x.y ���� �߶�  9.�����ڣ�10.�˵� 11.hinstance 12.��������
	//create opengl render context
	HDC dc = GetDC(hwnd);
	PIXELFORMATDESCRIPTOR pfd;//�ظ�ʽ������
	memset(&pfd, 0, sizeof(PIXELFORMATDESCRIPTOR));
	pfd.nVersion = 1;
	pfd.nSize = sizeof(PIXELFORMATDESCRIPTOR);
	pfd.cColorBits = 32;//��Ȼ�����
	pfd.cDepthBits = 24;//��ɫ������
	pfd.cStencilBits = 8;
	pfd.iPixelType = PFD_TYPE_RGBA;
	pfd.iLayerType = PFD_MAIN_PLANE;
	pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;//˫��������ѧУ��ʦ�Ͽν�������������T-T ǰ̨�������ͺ�̨��������ָ�뽻����

	int pixelFormat = ChoosePixelFormat(dc, &pfd);//ѡ��һ����ɫ��ʽ
	SetPixelFormat(dc, pixelFormat, &pfd);//����dc ѡ�����ظ�ʽ����������pixelformat

	HGLRC rc = wglCreateContext(dc);
	wglMakeCurrent(dc, rc);//���ɵ�ǰ��Ⱦ�豸
						   //setup opengl context complete 
	glewInit();
	Init();
	SetViewPortSize(800.0f, 600.0f);
	ShowWindow(hwnd, SW_SHOW);
	UpdateWindow(hwnd);
	MSG msg;
	while (true)
	{
		if (PeekMessage(&msg, NULL, NULL, NULL, PM_REMOVE))//ץ����Ϣ	�������еط���ץ������Ϣ�����κι��ˣ�����Ϣ���ж����Ƴ���
		{
			//������Ϣ
			if (msg.message == WM_QUIT)//�ж��Ƿ����˳���Ϣ
			{
				break;
			}
			TranslateMessage(&msg);//ת����Ϣ����
			DispatchMessage(&msg);
		}
		//draw scene	
		Draw();
		//present scene	
		SwapBuffers(dc);//������������ ǰ��
	}
	return 0; 
}