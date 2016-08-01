#pragma once

#include "resource.h"
#include "windows.h"
#include "worldviewer.h"
#include "worldblock.h"
#include "environment.h"
//#include "d3dfont.h"

#include <d3d9.h>
#include <d3dx9tex.h>
#include <string>
#include <tchar.h>
#include <time.h>
#include <psapi.h>
#include <thread>

using std::wstring;

//��ɫ
#define COLOR_CLEAR				D3DCOLOR_XRGB(0, 0, 0 )
#define COLOR_BLUE1				D3DCOLOR_XRGB(100, 120, 255)
#define COLOR_RED1				D3DCOLOR_XRGB(255, 120, 100)
#define COLOR_RED2				D3DCOLOR_XRGB(255, 80, 80)
#define COLOR_GREY1				D3DCOLOR_XRGB(240, 240, 240)
#define COLOR_GREY2				D3DCOLOR_XRGB(230, 230, 230)
#define COLOR_DIRECTION			D3DCOLOR_XRGB(230, 230, 230)
	

//���ģʽ
#define CURSORMODE_NORMAL		1
#define CURSORMODE_CAPTURE		2

//����ģʽ
#define MODE_STOP				0
#define MODE_RUN				1
#define MODE_MENU				2
#define MODE_PAUSE				3

#define STOPCAPTUREFREQ			20		// STOPģʽ����ѭ��Ƶ��(����Ƶ��)

#define INVALIDKEY_DELAY		0.4f	//�Ƿ��ַ���ʾ����ʱ�䣨�룩

struct threadparam {
	/*LPD3DXFONT *font;
	float *fps;
	float *avgfps;
	float *frametime;
	time_t *loopcount;
	RECT *text;*/
	bool *start;
	POINT *pclientcenter;
	WViewer *pviewer;
	int *pcursormode; 
};

DWORD WINAPI ThreadProc(LPVOID lpParam)//���̣߳������˶����ӽ��ƶ�
{
	threadparam* tpp = (threadparam*)lpParam;
	POINT cursorpos, bias;
	/*LPD3DXFONT *font = tpp->font;
	float *fps = tpp->fps;
	float *avgfps = tpp->avgfps;
	float *frametime = tpp->frametime;
	time_t *loopcount = tpp->loopcount;
	RECT *text = tpp->text;*/
	bool *start = tpp->start;
	POINT *pclientcenter = tpp->pclientcenter;
	WViewer *pviewer = tpp->pviewer;
	int *pcursormode = tpp->pcursormode;

	WCHAR show[40];
	while (true)
	{
		if (*start)
		{
			pviewer->HandleMove();//�����ӽ��ƶ�
			if (*pcursormode == CURSORMODE_CAPTURE)//�����ӽ���ת
			{
				GetCursorPos(&cursorpos);
				SetCursorPos(pclientcenter->x, pclientcenter->y);//��ָ�����õ������е�

				bias = cursorpos;
				bias.x -= pclientcenter->x;
				bias.y -= pclientcenter->y;
				pviewer->Rotate(bias);
			}
			//����̫�������������ǡ��ƵȻ������ƶ�
			
			/*if ((loopcount & 0x2) == 0)
			{*/
				if (moved)//����ȫ��ƽ�ƾ��󣬵���environmentλ�ã��������۾����λ�ò���
				{
					D3DXMatrixIdentity(&ViewTranslation);
					D3DXMatrixTranslation(&ViewTranslation, viewpos.x, viewpos.y, 0.0f);//z����ƽ��
				}
				if (shadowchanged || moved)
					matSun = ViewTranslation * sunTranslation;//����̫������
			//}
			*start = false;
		}
		else
		{
			//Sleep(1);
		}
	}
}


//���߳�
threadparam tp;//���ݸ����̵߳Ĳ���
DWORD threadID;
HANDLE hThread;
bool startthread;						//�Ƿ��������߳����еı�־
CRITICAL_SECTION cs;					//�ٽ���

//������Ϣ
HWND mainwnd;							//�����ھ��
bool isiconic, isfullscreen, focused;	//����״̬���Ƿ���С�����Ƿ�ȫ�����Ƿ��н���
RECT wndrect, clientrect;				//���������򣬿ͻ�����
POINT clientcenter;						//�ͻ����е㣬�������㣬ÿ������ƻ����λ��
RECT originwndrect;						//�洢ԭ���ڳߴ磬����ȫ���ָ�
int screenwidth, screenheight;			//��Ļ����

//������Ϣ
time_t loopcount;						//ѭ������
POINT cursorpos;						//�������
float fps, avgfps, fpslimit;			//֡��, ֡�����ƣ�<=0��ʾ�����ƣ�
float frametime;						//֡ʱ��
char invalidkey; LARGE_INTEGER invalidtime;//�Ƿ�������¼
float memory1, memory2;					//�ڴ�ռ����
tm Time;								//ʱ��
//wchar_t testchar;						//������Ļ��ʾЧ�����ַ�

//��ʱ
LARGE_INTEGER frequency;				//������Ƶ��
LARGE_INTEGER stime, etime, stime2, etime2;

//ģʽ
int mode, lastmode;						//����ģʽ, �洢��ǰģʽ�����ڻָ���
int cursormode;							//��겶׽ģʽ���Ƿ�׽��
bool infoshow;							//�Ƿ���ʾ��Ϣ
bool otherinfoshow;						//�Ƿ���ʾ������Ϣ
bool fpsshow;							//�Ƿ���ʾfps

//D3D
LPDIRECT3D9	lpD3D;						//
LPDIRECT3DDEVICE9 device;				//�豸
D3DPRESENT_PARAMETERS d3dpp;			//
D3DDISPLAYMODE displaymode;				//��ʾģʽ
LPDIRECT3DSURFACE9 surf;				//��δʹ��
byte multisample, maxmmultisample;		//���ز���ģʽ
bool mslist[17];						//���ز���ģʽ�����б�

//��Ϣ��ʾ
LPD3DXFONT font, font2, font3;			//����
//CD3DFont d3dfont;
RECT text, text2, text3, text4, text5, text6, text7;//������Ϣ��ʾ����
RECT text8, text9, text10, text11, text12;
WCHAR show[100], showms[100], status1[100], status2[100];//������Ϣ����


//�۲���
WViewer viewer;
//����
WBlocker blocker;
//����
Environment environment;

//
//��Ϣ����
//
//�۾�λ��
	//[WViewer���캯���г�ʼ����Walk()��SetViewmode()��KeyControlDown()��SetViewVector()��ʵʱ����
	//	�����̺߳�SetViewmode()�и���ViewTranslation, SetView()������SetTransform(D3DTS_VIEW,)]
D3DXVECTOR3 viewpos;
//�ӽǾ���ȫ��ƽ�ƾ��󣩣�environment������ʹ�õ�ˮƽƽ�ƾ���
	//[���߳��и���moved��viewposʵʱ�������£����߳��и���moved��shadowchangedʵʱ����matSun
	//	����WViewer���캯������viewpos��ʼ������environment��InitMaterialLight()�г�ʼ��matSun]
D3DXMATRIX ViewTranslation;	
//�ӽ��Ƿ�ı䣬��������view
	//[SetViewmode()��Walk()��Rotate()������Ϊtrue��MainLoop()����������SetView()������
	//	��������Ԫ�ػ��ƽ���������Ϊfalse����InitInstance()�г�ʼ��]
bool viewchanged;
//�ӽ��Ƿ��ƶ�������������Ӱ������viewchanged���Ӽ�
	//[���߳���������������ViewTranslation��matSun�����ݣ���WViewer��Draw()��������������matShadowWorld������
	//	����Walk()������Ϊtrue����MainLoop()������Ԫ�ػ��ƽ���������Ϊfalse����InitInstance()�г�ʼ��Ϊtrue]
bool moved;
//̫���������= ViewTranslation * sunTranslation��
	//[�����߳��и���shadowchanged || moved�������£���InitMaterialLight()�г�ʼ��Ϊtrue
	//	��InitInstance()�г�ʼ��Ϊtrue]
D3DXMATRIX matSun;
//̫��ƽ�ƾ���
	//[�����߳���������������matSun����InitMaterialLight()����sunLight��ʼ��������matSun]
D3DXMATRIX sunTranslation;
D3DXMATRIX sunRotate;			//̫����ת����unused
//̫�������������ǡ��ƵȻ�����λ���Ƿ�ı䣬����������Ӱ����
	//[�����߳���������������matSun����MainLoop()������Ԫ�ػ��ƽ���������Ϊfalse
	//����InitMaterialLight()�г�ʼ��Ϊtrue����InitInstance()�г�ʼ��Ϊtrue
	//����WViewer��Draw()����������matShadowWorld����WBlock��Draw()����������matShadowWorld]
bool shadowchanged;
//̫��λ��
	//[��InitMaterialLight()�и���light0��ʼ������ʼ��sunTranslation��matShadow]
D3DXVECTOR4 sunLight;
//��Ӱ����
	//[��InitMaterialLight()����sunLight��groundPlane��ʼ������groundPlane.d��ֱ̧����
	//����WBlock��Draw()�и���shadowchanged��������matShadowWorld
//����WViewer��Draw()�к�shadowchanged��������matShadowWorld]
D3DXMATRIX matShadow;
//��Ӱ����
D3DMATERIAL9 shadowmtrl;

inline void MainLoop();			//��ѭ��

void Init();					//������״̬��ʼ��

void D3DInit(HWND wnd, D3DMULTISAMPLE_TYPE ms);	//D3D��ʼ��
void CreateDevice();			//�����豸
void DeviceInit();				//�豸��������
void SetTextRect();				//������ʾ������Ϣ�����򣨸��ݴ��ڳߴ磩
_D3DMULTISAMPLE_TYPE GetMultisampleType(LPDIRECT3D9 lp, D3DDISPLAYMODE dm);	//��ö��ز������÷�Χ
void ChangeMultiSample();		//���Ķ��ز���ģʽ

void OnLostDevice(void);		//�豸��ʧ����
void OnResetDevice(void);		//�豸���ô���

inline void FpsShow();			//��ʾfps
inline void InfoShow();			//��ʾ��Ϣ
inline void OtherInfoShow();	//��ʾ������Ϣ
inline void UpdateStatusStr();	//����״̬�ַ���

//ʱ��
inline string GetWDayStr(int wday);	//�õ����ڼ��ַ���
inline void Get2WndRect();			//��ȡʵʱ���ںͿͻ�������
//����
bool FullScreen(bool tofull);	//����ȫ��ģʽ
void CaptureCursor();
void ReleaseCursor();
