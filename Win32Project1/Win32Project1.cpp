#include "stdafx.h"
#include "Win32Project1.h"

#define MAX_LOADSTRING 100

// ȫ�ֱ���: 
HINSTANCE hInst;                                // ��ǰʵ��
WCHAR szTitle[MAX_LOADSTRING];                  // �������ı�
WCHAR szWindowClass[MAX_LOADSTRING];            // ����������


												// �˴���ģ���а����ĺ�����ǰ������: 
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPWSTR    lpCmdLine,
	_In_ int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	// TODO: �ڴ˷��ô��롣

	// ��ʼ��ȫ���ַ���
	LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadStringW(hInstance, IDC_WIN32PROJECT1, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// ִ��Ӧ�ó����ʼ��: 
	if (!InitInstance(hInstance, nCmdShow))
	{
		return FALSE;
	}

	HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_WIN32PROJECT1));

	MSG msg;

	// �޸İ�ѭ����Ч�ʸ���
	while (true)
	{
		QueryPerformanceCounter(&stime2);
		if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
		{
			if (msg.message == WM_QUIT)//�˳�����
				break;

			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		//�����Ƿ���С�� TODO��������Ϣ����
		if (IsIconic(mainwnd))
		{
			isiconic = true;
			if (mode != MODE_STOP)
			{
				lastmode = mode;
				mode = MODE_STOP;
			}
		}
		else
		{
			if (isiconic)//����С��״̬�ָ�
			{
				mode = lastmode;
				isiconic = false;
			}
		}

		WCHAR title[40];
		swprintf_s(title, L"world %.1f / %.1ffps", fps, avgfps);
		SetWindowText(mainwnd, title);
		//��ѭ��
		switch (mode)
		{
		case MODE_RUN:
			loopcount++;
			MainLoop();
			break;
		case MODE_STOP:
			Sleep(1000 / STOPCAPTUREFREQ);//����STOPģʽ����ѭ��Ƶ��
			break;
		case MODE_MENU:
			break;
		case MODE_PAUSE:
			break;
		}

		//����֡��
		QueryPerformanceCounter(&etime);
		if (fpslimit > 0)
		{
			float time = 1000.*(double)(etime.QuadPart - stime.QuadPart) / (double)frequency.QuadPart;
			if (1000 / fpslimit >  time)
			{
				Sleep(1000 / fpslimit - time);
			}
		}

		//����֡��
		QueryPerformanceCounter(&etime);//��¼����ʱ��
		if ((loopcount & 0x3f) == 0)//ÿ��n�μ�������fps
		{
			if (etime.QuadPart != stime.QuadPart)
				fps = (double)frequency.QuadPart
				/ (double)(etime.QuadPart - stime.QuadPart);

			float rate = 0.05f;
			if (avgfps == -1)//ƽ��֡��
				avgfps = fps;
			else
				avgfps = avgfps*(1 - rate) + fps*rate;

			if (frametime == -1)//ƽ��֡ʱ��
				frametime = (double)(etime.QuadPart - stime2.QuadPart)
				/ (double)frequency.QuadPart;
			else
				frametime = frametime*(1 - rate) +
				(double)(etime.QuadPart - stime2.QuadPart)
				/ (double)frequency.QuadPart*rate;
		}
		stime.QuadPart = etime.QuadPart;//��¼��ʼʱ��
	}

	return (int)msg.wParam;
}



//
//  ����: MyRegisterClass()
//
//  Ŀ��: ע�ᴰ���ࡣ
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEXW wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_WIN32PROJECT1));
	wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName = NULL;//����ʾ�˵���//MAKEINTRESOURCEW(IDC_WIN32PROJECT1);
	wcex.lpszClassName = szWindowClass;
	wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	return RegisterClassExW(&wcex);
}

//
//   ����: InitInstance(HINSTANCE, int)
//
//   Ŀ��: ����ʵ�����������������
//
//   ע��: 
//
//        �ڴ˺����У�������ȫ�ֱ����б���ʵ�������
//        ��������ʾ�����򴰿ڡ�
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
	hInst = hInstance; // ��ʵ������洢��ȫ�ֱ�����

	//���ó�ʼ���ڴ�С
	screenwidth = GetSystemMetrics(SM_CXSCREEN);
	screenheight = GetSystemMetrics(SM_CYSCREEN);
	float wndscale = 0.6;
	HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
		screenwidth*(1 - wndscale) / 2, screenheight*(1 - wndscale) / 2
		, screenwidth*wndscale, screenheight*wndscale, nullptr, nullptr, hInstance, nullptr);

	if (!hWnd)
	{
		return FALSE;
	}

	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);


	// ��ӵ��Զ����ʼ��
	mainwnd = hWnd;//�洢�����ھ��
	SetWindowText(mainwnd, L"world");
	Init();//��ʼ��
	D3DInit(hWnd, D3DMULTISAMPLE_16_SAMPLES);//D3D��ʼ��

	//D3D���ݳ�ʼ��
	moved = true;
	viewchanged = true;//�����ӽǸ��²���
	shadowchanged = true;
	viewer.SetViewmode(VIEWMODE_CHASE);

	//�������߳�
	tp.start = &startthread;
	tp.pclientcenter = &clientcenter;
	tp.pviewer = &viewer;
	tp.pcursormode = &cursormode;
	InitializeCriticalSection(&cs);
	hThread = CreateThread(NULL, 0, ThreadProc, &tp, 0, &threadID); // �����߳�
	

	return TRUE;
}

//
//  ����: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  Ŀ��:    ���������ڵ���Ϣ��
//
//  WM_COMMAND  - ����Ӧ�ó���˵�
//  WM_PAINT    - ����������
//  WM_DESTROY  - �����˳���Ϣ������
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	char key;
	int key_state;
	POINT cur, bias;
	bool keyvalid;

	switch (message)
	{
	case WM_COMMAND:
	{
		int wmId = LOWORD(wParam);
		// �����˵�ѡ��: 
		switch (wmId)
		{
		case IDM_ABOUT:
			DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
			break;
		case IDM_EXIT:
			DestroyWindow(hWnd);
			break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
	}
	break;
	case WM_PAINT:
	{
		PAINTSTRUCT ps;
		HDC hdc = BeginPaint(hWnd, &ps);
		// TODO: �ڴ˴����ʹ�� hdc ���κλ�ͼ����...

		EndPaint(hWnd, &ps);
	}
	break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	case WM_LBUTTONDOWN:
		if (cursormode == CURSORMODE_NORMAL)//�ػ���겶׽
		{
			CaptureCursor();
			UpdateStatusStr();//���´���״̬�ַ���
		}
		break;
	case WM_ACTIVATE://������С�����������¼�
		if (wParam == WA_INACTIVE)//������겶׽����Init��getforeground��ͬ�����ʼ�����ڲ����ռ����¼���not focused״̬
		{
			focused = false;

			viewer.Inactivate();//�ж�viewer�ļ��̿�����Ϣ������Ϊ��

			ReleaseCursor();
			UpdateStatusStr();//���´���״̬�ַ���
		}
		else if (wParam == WA_ACTIVE)
		{
			focused = true; 
			UpdateStatusStr();//���´���״̬�ַ���
		}
		else if (wParam == WA_CLICKACTIVE)
		{
			focused = true;
			UpdateStatusStr();//���´���״̬�ַ���
		}
		break;
	case WM_KEYDOWN:
		key = wParam;
		//key_state = lParam;

		keyvalid = true;//��ʼ����־Ϊ�Ϸ�����
		
		if (key == VK_ESCAPE)//������겶׽
		{
			if (cursormode == CURSORMODE_CAPTURE)
			{
				ReleaseCursor();
				UpdateStatusStr();//���´���״̬�ַ���
			}
		}
		else if (key == 'F')
		{
			fpsshow = !fpsshow;
		}
		else if (key == 'I')
		{
			infoshow = !infoshow;
		}
		else if (key == 'M')
		{
			ChangeMultiSample();
			swprintf_s(showms, _T("%d�� MULTISAMPLE/ %dMAX     %d��%d"), multisample, maxmmultisample
				, clientrect.right - clientrect.left, clientrect.bottom - clientrect.top);
		}
		else if (key == VK_F3)
			otherinfoshow = !otherinfoshow;
		else if (key == VK_F4)
		{
			FullScreen(!isfullscreen);
			UpdateStatusStr();//���´���״̬�ַ���
		}
		else
			keyvalid = viewer.KeyControlDown(key);

		if (!keyvalid)//������Ч����
		{
			invalidkey = key;
			QueryPerformanceCounter(&invalidtime);
		}
		//if (key == 'T')//�����ַ���ʾ
		//	testchar++;
		//else if (key == 'R')
		//	testchar--;
		break;
	case WM_KEYUP:
		key = wParam;
		key_state = lParam;

		viewer.KeyControlUp(key);
		break;
	case WM_SETCURSOR:
		if (cursormode == CURSORMODE_CAPTURE)
		{
			switch (mode)
			{
			case MODE_RUN:
				SetCursor(LoadCursor(NULL, IDC_ICON));
				break;
			default:
				SetCursor(LoadCursor(NULL, IDC_ARROW));
				break;
			}
		}
		else if(cursormode == CURSORMODE_NORMAL)
			SetCursor(LoadCursor(NULL, IDC_ARROW));
		break;
	case WM_SIZE:
		Get2WndRect();//���´�������
		if (cursormode == CURSORMODE_CAPTURE)
			ClipCursor(&clientrect);//�������ò�������

		if (device)
		{
			//�޸�device����
			d3dpp.BackBufferWidth = clientrect.right - clientrect.left;
			d3dpp.BackBufferHeight = clientrect.bottom - clientrect.top;

			OnLostDevice();
			OnResetDevice();
		}
		SetTextRect();//����text����
		swprintf_s(showms, _T("%d�� MULTISAMPLE/ %dMAX     %d��%d"), multisample, maxmmultisample
			, clientrect.right - clientrect.left, clientrect.bottom - clientrect.top);
		break;
	case WM_MOVE:
		Get2WndRect();//���´�������
		if (cursormode == CURSORMODE_CAPTURE)
			ClipCursor(&clientrect);//�������ò�������
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

// �����ڡ������Ϣ�������
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:
		return (INT_PTR)TRUE;

	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
		{
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		break;
	}
	return (INT_PTR)FALSE;
}

void Init()
{
	//������Ϣ��ʼ��
	if (IsIconic(mainwnd))
		isiconic = true;
	else
		isiconic = false;
	isfullscreen = false;
	focused = GetForegroundWindow() == mainwnd;
	Get2WndRect();
	originwndrect = wndrect;
	//mainwnd;screenwidth;screenheight;//���ھ������Ļ������InitInstance�л��

	//������Ϣ��ʼ��
	loopcount = 0;
	cursorpos.x = -1;
	cursorpos.y = -1;
	fps = -1.0f;
	avgfps = -1.0f;//needed < 0
	fpslimit = -1.0f;
	frametime = -1.0f;
	invalidkey = 0;//needed 0
	memory1 = 0.0f;
	memory2 = 0.0f;
	//testchar = 0;

	//��ʱ
	QueryPerformanceFrequency(&frequency);
	stime.QuadPart = 0;
	etime.QuadPart = 0;
	stime2.QuadPart = 0;
	etime2.QuadPart = 0;

	//ģʽ
	mode = MODE_RUN;//��ʼ������ģʽ
	lastmode = MODE_RUN;
	//��ʼ������״̬������״̬
	if (!focused)//��ʼ������ģʽ��(�����ڲ��ܽ����¼������)
		cursormode = CURSORMODE_NORMAL;
	else
		CaptureCursor();
	//��ʼ����Ϣ��ʾ����
	infoshow = false;
	otherinfoshow = false;
	fpsshow = true;
	UpdateStatusStr();//��ʼ��״̬��ʾ�ַ���

	SetTextRect();//����������ʾ����
}

bool FullScreen(bool tofull)
{
	if (isfullscreen == tofull)
		return false;
	else if(tofull)//ȫ����
	{
		originwndrect = wndrect;
		RECT m_FullScreenRect;
		m_FullScreenRect.left = wndrect.left - clientrect.left;
		m_FullScreenRect.top = wndrect.top - clientrect.top + 22;
		m_FullScreenRect.right = wndrect.right
			- clientrect.right + screenwidth;
		m_FullScreenRect.bottom = wndrect.bottom
			- clientrect.bottom + screenheight;

		//����������
		/*HWND hwnd;
		hwnd = FindWindow(L"Shell_TrayWnd", NULL);
		ShowWindow(hwnd, SW_HIDE);
		hwnd = GetDlgItem(FindWindow(L"Shell_TrayWnd", NULL), 0x130);
		ShowWindow(hwnd, SW_HIDE);*/


		LONG tmp = GetWindowLong(mainwnd, GWL_STYLE);
		tmp &= ~WS_BORDER; 
		tmp |= WS_SYSMENU | WS_MINIMIZEBOX | WS_MAXIMIZEBOX;
		SetWindowLong(mainwnd, GWL_STYLE, tmp);

		MoveWindow(mainwnd, m_FullScreenRect.left, m_FullScreenRect.top
			, m_FullScreenRect.right- m_FullScreenRect.left
			, m_FullScreenRect.bottom- m_FullScreenRect.top, TRUE);

		isfullscreen = true;
	}
	else//ȡ��ȫ��
	{
		//��ʾ������
		/*HWND hwnd;
		hwnd = FindWindow(L"Shell_TrayWnd", NULL);
		ShowWindow(hwnd, SW_SHOW);
		hwnd = GetDlgItem(FindWindow(L"Shell_TrayWnd", NULL), 0x130);
		ShowWindow(hwnd, SW_SHOW);*/

		LONG tmp = GetWindowLong(mainwnd, GWL_STYLE);
		tmp |= WS_POPUPWINDOW;
		tmp |= WS_SYSMENU | WS_MINIMIZEBOX | WS_MAXIMIZEBOX;
		SetWindowLong(mainwnd, GWL_STYLE, tmp);

		MoveWindow(mainwnd, originwndrect.left, originwndrect.top
			, originwndrect.right - originwndrect.left
			, originwndrect.bottom - originwndrect.top, TRUE);
		SetFocus(mainwnd);
		isfullscreen = false;
	}
}

void CaptureCursor()
{
	cursormode = CURSORMODE_CAPTURE;
	if(mode== MODE_RUN)
	{
		SetCursorPos(clientcenter.x, clientcenter.y);//��ָ�����õ������е�
		ClipCursor(&clientrect);
	}
}

void ReleaseCursor()
{
	cursormode = CURSORMODE_NORMAL;
	ClipCursor(NULL);
}

void D3DInit(HWND wnd, D3DMULTISAMPLE_TYPE ms)
{
	HRESULT hr;

	//Direct3D �ӿڶ���
	lpD3D = Direct3DCreate9(D3D_SDK_VERSION);

	//display mode
	hr = lpD3D->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &displaymode);
	if (FAILED(hr))
		MessageBoxW(wnd, L"get displaymode FAILED!", L"", 0);

	//D3DPRESENT_PARAMETERS
	ZeroMemory(&d3dpp, sizeof(d3dpp));
	d3dpp.MultiSampleType = GetMultisampleType(lpD3D, displaymode);//���ö��ز���ģʽ
	if (d3dpp.MultiSampleType > ms)
		d3dpp.MultiSampleType = ms;
	multisample = d3dpp.MultiSampleType;
	swprintf_s(showms, _T("%d�� MULTISAMPLE/ %dMAX     %d��%d"), multisample, maxmmultisample
		, clientrect.right - clientrect.left, clientrect.bottom - clientrect.top);//����multisample��ʾ�����ַ���
	d3dpp.Windowed = TRUE;
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
	d3dpp.BackBufferFormat = displaymode.Format;
	//�رմ�ֱͬ��,(��������֡�ʣ��������ڴ�ռ�ã��ϴ�����cpuռ����)
	d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE; 
	d3dpp.EnableAutoDepthStencil = TRUE;//��Ȼ���
	d3dpp.AutoDepthStencilFormat = D3DFMT_D16;
	//d3dpp.AutoDepthStencilFormat = D3DFMT_D24S8;

	//device
	CreateDevice();	//�����豸
	DeviceInit();	//��ʼ���豸ģʽ

	//viewer
	viewer.SetDevice(device);
	viewer.SetFigure();
	viewer.InitProj(D3DX_PI / 3
		, (float)(clientrect.right - clientrect.left) / (float)(clientrect.bottom - clientrect.top));
	//blocker
	blocker.SetDevice(device);
	blocker.SetGBlock();
	//environment
	environment.SetDevice(device);
	environment.InitSkyBack();
	environment.InitMaterialLight();

	//��������
	D3DXCreateFontW(
		device,
		16, 7, 0, 1, 0,			// �����ַ��Ŀ�ߡ��Ƿ�Ӵ֡�Mipmap�����Ƿ�Ϊб��	
		DEFAULT_CHARSET,        // Ĭ���ַ���
		OUT_TT_ONLY_PRECIS,     // ������ȣ�ʹ��Ĭ��ֵ
		NONANTIALIASED_QUALITY,	// �ı�����
		DEFAULT_PITCH | FF_DONTCARE,
		L"Calibri",
		&font
	);
	D3DXCreateFontW(
		device,
		18, 7, 0, 1, 0,
		DEFAULT_CHARSET,
		OUT_DEFAULT_PRECIS,
		DEFAULT_QUALITY,
		DEFAULT_PITCH | FF_DONTCARE,
		L"΢���ź�",
		&font2
	);
	D3DXCreateFontW(
		device,
		15, 5, 0, 1, 0,
		DEFAULT_CHARSET,
		OUT_DEFAULT_PRECIS,
		NONANTIALIASED_QUALITY,
		DEFAULT_PITCH | FF_DONTCARE,
		L"Tw Cen MT",
		&font3
	);
}

void CreateDevice()
{
	device = NULL;
	HRESULT hr = lpD3D->CreateDevice(
		D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, mainwnd
		, D3DCREATE_SOFTWARE_VERTEXPROCESSING, &d3dpp, &device
	);

	if (FAILED(hr))
		MessageBoxW(mainwnd, L"Device create FAILED!", L"", 0);
}

void DeviceInit()
{
	//������ɫģʽ
	device->SetRenderState(D3DRS_SHADEMODE, D3DSHADE_PHONG);
	//�޳�ģʽ����ʱ������
	device->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);//���޳�
	//��������
	device->SetRenderState(D3DRS_LIGHTING, TRUE);
	//������Ȳ���  
	device->SetRenderState(D3DRS_ZENABLE, TRUE);
	device->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESS);
	device->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
	//���þ��淴�����ģ��  
	//device->SetRenderState(D3DRS_SPECULARENABLE, TRUE);
	//���ö�����ɫ���ڹ��ռ���
	device->SetRenderState(D3DRS_COLORVERTEX, TRUE);
	//������ɫ��Դ
	//device->SetRenderState(D3DRS_DIFFUSEMATERIALSOURCE, D3DMCS_MATERIAL);
	//device->SetRenderState(D3DRS_AMBIENTMATERIALSOURCE, D3DMCS_MATERIAL);
	//device->SetRenderState(D3DRS_SPECULARMATERIALSOURCE, D3DMCS_MATERIAL);
	//������
	device->SetRenderState(D3DRS_MULTISAMPLEANTIALIAS, TRUE);//�������ز���
	//device->SetRenderState(D3DRS_ANTIALIASEDLINEENABLE, FALSE);//δ֪
	//device->SetRenderState((_D3DRENDERSTATETYPE)40, TRUE);//������
	//��
	_D3DFOGMODE fogmode = D3DFOG_LINEAR;//��ģʽ
	float density = 0.001f;
	float start = FOGSTART;
	float end = FOGEND;
	device->SetRenderState(D3DRS_FOGENABLE, TRUE);
	device->SetRenderState(D3DRS_FOGCOLOR, COLOR_SKY1);
	device->SetRenderState(D3DRS_FOGTABLEMODE, fogmode);
	if (fogmode == D3DFOG_LINEAR)
	{
		device->SetRenderState(D3DRS_FOGSTART, *(DWORD *)(&start));
		device->SetRenderState(D3DRS_FOGEND, *(DWORD *)(&end));
	}
	else
		device->SetRenderState(D3DRS_FOGDENSITY, *(DWORD *)(&density));
	//ALPHABLEND
	device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
}

void SetTextRect()
{
	//text4, 11δʹ��

	int margin = 2, rowmargin = 6;//��Ե��
	int rowscreenheight = 16, rowwidth = 200;//�иߣ��п�
	text.top = margin;//�������
	text.bottom = text.top + rowscreenheight;
	text.left = rowmargin;
	text.right = text.left + 300;
	text2.top = text.bottom;//���½�text
	text2.bottom = text2.top + 3*rowscreenheight;
	text2.left = rowmargin;
	text2.right = text2.left + rowwidth;
	text3.top = text2.bottom;//���½�text2
	text3.bottom = text3.top + rowscreenheight;
	text3.left = rowmargin;
	text3.right = text3.left + rowwidth;

	text5.bottom = clientrect.bottom - clientrect.top - margin;//�ײ����
	text5.top = text5.bottom - rowscreenheight;
	text5.left = rowmargin;
	text5.right = text5.left + 300;
	text6.bottom = clientrect.bottom - clientrect.top - margin;//�ײ��м�
	text6.top = text6.bottom - rowscreenheight;
	text6.left = (wndrect.right - wndrect.left) / 2 - rowwidth / 2;
	text6.right = text6.left + rowwidth;
	text12.bottom = clientrect.bottom - clientrect.top - margin;//�ײ��м�
	text12.top = text12.bottom - rowscreenheight;
	text12.left = (wndrect.right - wndrect.left) / 2 - rowwidth / 2 - 40;
	text12.right = text12.left + 40;
	text7.bottom = clientrect.bottom - clientrect.top - margin;//�ײ��Ҳ�
	text7.top = text7.bottom - rowscreenheight;
	text7.right = clientrect.right - clientrect.left - rowmargin;
	text7.left = text7.right - 200;


	text8.top = margin;//�����Ҳ�
	text8.bottom = text8.top + rowscreenheight;
	text8.right = clientrect.right - clientrect.left - rowmargin;
	text8.left = text7.right - 300; 
	text9.top = text8.bottom;//���½�text8
	text9.bottom = text9.top + rowscreenheight;
	text9.right = clientrect.right - clientrect.left - rowmargin;
	text9.left = text7.right - 300;
	text10.top = text9.bottom;//���½�text9
	text10.bottom = text10.top + rowscreenheight;
	text10.right = clientrect.right - clientrect.left - rowmargin;
	text10.left = text7.right - 300;
}

void ChangeMultiSample()
{
	multisample = d3dpp.MultiSampleType + 1;
	while (multisample != d3dpp.MultiSampleType)//�õ���һ�����ʵĶ��ز���ģʽ
	{
		if (mslist[multisample] == true)
			break;
		multisample++;
		if (multisample > D3DMULTISAMPLE_16_SAMPLES || multisample > maxmmultisample)//Խ��򳬹������ز���
			multisample = D3DMULTISAMPLE_NONE;
	}
	//d3dpp.MultiSampleType = (D3DMULTISAMPLE_TYPE)multisample;

	//������д�����D3D����(D3DPOOL_MANAGED��D3DPOOL_DEFAULT���ص���Դ����Ҫ�ͷš��ؽ�)
	lpD3D->Release();
	font->Release();
	font2->Release();
	font3->Release();
	device->Release();
	//���´���D3D����
	D3DInit(mainwnd, (D3DMULTISAMPLE_TYPE)multisample);
}

void OnLostDevice()
{
	//D3DPOOL_DEFAULT���ص���Դ(LPD3DXFont��LPD3DXSprite)��Ҫ���ͷţ����ؽ���
	font->OnLostDevice();
	font2->OnLostDevice();
	font3->OnLostDevice();
}

void OnResetDevice(void)
{
	if (FAILED(device->Reset(&d3dpp)))
	{
		return;
	}

	//D3DPOOL_DEFAULT���ص���Դ(LPD3DXFont��LPD3DXSprite)��Ҫ���ͷţ����ؽ���
	font->OnResetDevice();
	font2->OnResetDevice();
	font3->OnResetDevice();

	DeviceInit();//�豸������ʼ��

	//viewer
	viewer.SetDevice(device);//��viewer����device
	viewer.SetView();//����view�任
	//����ͶӰ�任
	viewer.InitProj(D3DX_PI / 3, (float)(clientrect.right - clientrect.left) / (float)(clientrect.bottom - clientrect.top));
	
	//blocker
	blocker.SetDevice(device);
	//blocker.SetGBlock();//����Ҫ���»�ͼ

	//environment
	environment.SetDevice(device);
	//environment.InitSkyBack();//����Ҫ���»�ͼ
	environment.InitMaterialLight();
}

void MainLoop()
{
	if (!device)
		return;

	//������ͼ�δ����Ƶ����̴߳���
	//viewer.HandleMove();//�����ӽ��ƶ�
	//if (cursormode == CURSORMODE_CAPTURE)//�����ӽ���ת
	//{
	//	GetCursorPos(&cursorpos);

	//	POINT bias = cursorpos;
	//	bias.x -= clientcenter.x;
	//	bias.y -= clientcenter.y;
	//	viewer.Rotate(bias);//�ӽ���ת
	//	//��ָ�����õ������е�
	//	SetCursorPos(clientcenter.x, clientcenter.y);
	//}


	// �ӽǴ�����ʾ����
	if(viewchanged)
		viewer.SetView();//�����ӽǣ�������viewchangedΪfalse
	// Clear the buffer.   
	device->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, COLOR_CLEAR, 1.0f, 0);
	// Begin the scene. Start rendering.   
	if (SUCCEEDED(device->BeginScene()))
	{
		//��Ⱦ״̬��ʼ��
		device->SetRenderState(D3DRS_LIGHTING, TRUE);//����
		device->SetRenderState(D3DRS_FOGENABLE, TRUE);//��

		//����
		if (viewer.viewmode == VIEWMODE_OVERLOOK)
			device->SetRenderState(D3DRS_FOGENABLE, FALSE);//��ʱ�ر���,TODO
		startthread = false;//�������̱߳�־
		blocker.Draw();
		viewer.Draw();
		environment.Draw();

		//���ñ�־
		viewchanged = false;//����view�ı��־Ϊfalse
		moved = false;
		shadowchanged = false;//������Ӱ��־Ϊfalse

		startthread = true;//��ʼ���̱߳�־

		//��Ϣ��ʾ
		if (fpsshow)
			FpsShow();
		if (infoshow)
			InfoShow();
		if (otherinfoshow)
			OtherInfoShow();
		
		// End the scene. Stop rendering.   
		device->EndScene();
	}

	HRESULT hr;
	// Display the scene.   
	hr = device->Present(NULL, NULL, NULL, NULL);

	// Render failed, try to reset device
	if (hr == D3DERR_DEVICELOST)
	{
		if (device->TestCooperativeLevel() == D3DERR_DEVICENOTRESET)//�����豸��ʧ
		{
			OnLostDevice();
			OnResetDevice();
		}
	}
}

void FpsShow()
{
	//fps��ʾ��ɨ�����
	swprintf_s(show, _T("FPS  %.1f/ %.1f  %.3fms  %lld"), fps, avgfps, frametime*1000., loopcount);
	font->DrawTextW(NULL, show, -1, &text, DT_LEFT, COLOR_RED1);
}

void UpdateStatusStr()
{
	if (focused)
		wcscpy_s(status1, L"focused: true ");
	else
		wcscpy_s(status1, L"focused: false ");
	if (isfullscreen)
		wcscat_s(status1, L" fullscreen: true ");
	else
		wcscat_s(status1, L" fullscreen: false ");
	if (cursormode == CURSORMODE_CAPTURE)
		wcscpy_s(status2, L" captured: true ");
	else
		wcscpy_s(status2, L" captured: false ");
}

void InfoShow()
{
	WCHAR show2[100] = { 0 };
	//pos��ʾ��(ɨ�����)
	swprintf_s(show, _T("POS %.3f, %.3f, %.3f")
		, viewer.pos.x, viewer.pos.y, viewer.pos.z);
	//eye��λ��ʾ��(ɨ�����)
	swprintf_s(show2, _T("\n\rEYE %.3f, %.3f, %.3f")
		, viewpos.x, viewpos.y, viewpos.z);
	wcscat_s(show, show2);
	//�ӽ���ʾ��(ɨ�����)
	swprintf_s(show2, _T("\n\rH %.2f��, V %.2f��")
		, viewer.hAngle * RADIAN2DEGREE, viewer.vAngle * RADIAN2DEGREE);
	wcscat_s(show, show2);
	font->DrawTextW(NULL, show, -1, &text2, DT_LEFT, COLOR_WHITE);//����������ʾ

	//viewmode���ٶ���ʾ
	swprintf_s(show, _T("VIEW %S    SPEED %.2f")
		, viewer.GetViewmodeStr().c_str(), viewer.speed);
	font->DrawTextW(NULL, show, -1, &text3, DT_LEFT, COLOR_GREY1);

	//����״̬��������ʾ��ɨ�����
	//swprintf_s(show, _T("%lc   %d"), testchar, testchar); //������ʾЧ��
	if (viewer.speed == 0.0f)
		swprintf_s(show, _T("%lc"), walkdirection[9]);
	else
		swprintf_s(show, _T("%lc"), walkdirection[viewer.curdirection]);
	font2->DrawTextW(NULL, show, -1, &text6, DT_CENTER, COLOR_DIRECTION);
}

void OtherInfoShow()
{
	//���ز���ģʽ�Ϳͻ����ߴ���ʾ��(ȫ���ַ������棬��������)
	font->DrawTextW(NULL, showms, -1, &text5, DT_LEFT, COLOR_GREY2);

	//���·Ƿ���������ʾ����
	if (invalidkey)
	{
		LARGE_INTEGER now;
		QueryPerformanceCounter(&now);
		if ((double)(now.QuadPart - invalidtime.QuadPart) / (double)frequency.QuadPart > INVALIDKEY_DELAY)
		{
			invalidkey = 0;
		}
		else
		{
			swprintf_s(show, _T("%lc"), invalidkey);
			font2->DrawTextW(NULL, show, -1, &text12, DT_LEFT, COLOR_RED2);
		}
	}

	//��ǰʱ����ʾ��(��ʱ����)
	if ((loopcount & 0x3F) == 0)
	{
		time_t rawtime;
		time(&rawtime);
		localtime_s(&Time, &rawtime);//��ȡ��ǰʱ��
	}
	swprintf_s(show, _T("%d-%02d-%02d  %S   %02d:%02d:%02d")
		, Time.tm_year + 1900, Time.tm_mon + 1, Time.tm_mday, GetWDayStr(Time.tm_wday).c_str()
		, Time.tm_hour, Time.tm_min, Time.tm_sec);//ʱ���ַ���
	font3->DrawTextW(NULL, show, -1, &text7, DT_RIGHT, COLOR_GREY2);

	//�ڴ�ռ������ʾ��(��ʱ����)
	if ((loopcount & 0xFF) == 0)
	{
		HANDLE handle = GetCurrentProcess();
		PROCESS_MEMORY_COUNTERS pmc;
		GetProcessMemoryInfo(handle, &pmc, sizeof(pmc));

		memory1 = pmc.WorkingSetSize / 1000000.;//�ڴ�ռ����
		memory2 = pmc.PagefileUsage / 1000000.;	//�����ڴ�ռ����
	}
	swprintf_s(show, _T("%.1fMB, %.1fMB"), memory1, memory2);
	font->DrawTextW(NULL, show, -1, &text8, DT_RIGHT, COLOR_GREY2);

	//����״̬��ʾ��(ȫ���ַ������棬��������)
	//UpdateStatusStr();
	font->DrawTextW(NULL, status1, -1, &text9, DT_RIGHT, COLOR_GREY2);
	font->DrawTextW(NULL, status2, -1, &text10, DT_RIGHT, COLOR_GREY2);
}

_D3DMULTISAMPLE_TYPE GetMultisampleType(LPDIRECT3D9 lp, D3DDISPLAYMODE dm)
{
	if (!lp)
		return D3DMULTISAMPLE_NONE;

	int multisample = 0;
	while (multisample<=16)//ѭ�����ԣ����¶��ز���ģʽ�����б������ز�������ǰ���ز���ģʽ��Ϊ���
	{
		if (lp->CheckDeviceMultiSampleType(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL
			, dm.Format, true, (_D3DMULTISAMPLE_TYPE)multisample, NULL) != D3D_OK)
			mslist[multisample] = false;
		else
		{
			maxmmultisample = multisample;
			mslist[multisample] = true;
		}

		multisample++;
	}

	return (_D3DMULTISAMPLE_TYPE)maxmmultisample;
}

string GetWDayStr(int wday)
{
	switch (wday)
	{
	case 0:
		return "SUN";
		break;
	case 1:
		return "MON";
		break;
	case 2:
		return "TUE";
		break;
	case 3:
		return "WED";
		break;
	case 4:
		return "THR";
		break;
	case 5:
		return "FRI";
		break;
	case 6:
		return "SAT";
		break;
	default:
		return "UNK";
	}
}

void Get2WndRect()
{
	GetClientRect(mainwnd, &clientrect);//�õ�client����ߴ�
	POINT clienttl = { 0, 0 };
	ClientToScreen(mainwnd, &clienttl);//���client�������Ͻǵ���Ļ����
	//�õ�client��ʵ��Ļ����
	clientrect.left = clienttl.x;
	clientrect.top = clienttl.y;
	clientrect.right += clientrect.left;
	clientrect.bottom += clientrect.top;
	clientcenter.x = (clientrect.left + clientrect.right) / 2;
	clientcenter.y = (clientrect.top + clientrect.bottom) / 2;

	GetWindowRect(mainwnd, &wndrect);//�õ���������
}
