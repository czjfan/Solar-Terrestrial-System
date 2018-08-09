#pragma once

#include "resource.h"
//#include "windows.h"
#include "worldviewer.h"
#include "worldblock.h"
#include "environment.h"

#include <d3d9.h>
#include <d3dx9tex.h>
#include <string>
#include <tchar.h>
#include <psapi.h>
#include <thread>

using std::wstring;

//��ɫ
#define COLOR_CLEAR					D3DCOLOR_XRGB(0, 0, 0 )
#define COLOR_RED1					D3DCOLOR_XRGB(255, 120, 100)
#define COLOR_RED2					D3DCOLOR_XRGB(255, 80, 80)
#define COLOR_BLUE1					D3DCOLOR_XRGB(100, 120, 255)
#define COLOR_GREY1					D3DCOLOR_XRGB(240, 240, 240)
#define COLOR_GREY2					D3DCOLOR_XRGB(230, 230, 230)
#define COLOR_DIRECTION				D3DCOLOR_XRGB(230, 230, 230)
	
//����ģʽ���Ƿ���������̿��ƣ�
#define CONTROLMODE_NORMAL			1
#define CONTROLMODE_CAPTURE			2

//����ģʽ
#define MODE_STOP					0
#define MODE_RUN					1
#define MODE_MENU					2
#define MODE_PAUSE					3

#define STOPCAPTUREFREQ				40		// STOPģʽ����ѭ��Ƶ��(����Ƶ��)

#define INVALIDKEY_DELAY			0.4f	//�Ƿ��ַ���ʾ����ʱ�䣨�룩

struct threadparam {
	bool *start;
	POINT *pclientcenter;
	WViewer *pviewer;
	int *pcontrolmode; 
};

DWORD WINAPI ThreadProc(LPVOID lpParam)//���̣߳������˶����ӽ��ƶ�
{
	POINT cursorpos, bias;

	threadparam* tpp = (threadparam*)lpParam;
	bool *start = tpp->start;
	POINT *pclientcenter = tpp->pclientcenter;
	WViewer *pviewer = tpp->pviewer;
	int *pcontrolmode = tpp->pcontrolmode;

	while (true)
	{
		if (*start)
		{
			pviewer->HandleMove();//�����ӽ��ƶ�
			if (*pcontrolmode == CONTROLMODE_CAPTURE)//�����ӽ���ת
			{
				GetCursorPos(&cursorpos);
				SetCursorPos(pclientcenter->x, pclientcenter->y);//��ָ�����õ������е�

				bias = cursorpos;
				bias.x -= pclientcenter->x;
				bias.y -= pclientcenter->y;
				pviewer->Rotate(bias);
			}			
			
			//if (viewer.viewchanged)//����ȫ��ƽ�ƾ��󣬱���environment���۾����λ�ò���
			//{
			//	//D3DXMatrixIdentity(&ViewTranslation); 
			//	//D3DXMatrixTranslation(&ViewTranslation, viewpos.x, viewpos.y, 0.0f);//z����ƽ��
			//	D3DXMatrixIdentity(&inblockTranslation);
			//	D3DXMatrixTranslation(&inblockTranslation
			//		, inblockpos.x, inblockpos.y, 0.0f);//z����ƽ��
			//}
			//if (shadowchanged || moved)
			//	matSun = inblockTranslation * sunTranslation;//����̫�����󣬱���̫�����۾����λ�ò���

			//������n��仯
			//environment.ViewMove();//����ʵʱ��γ��
			//if (nowtime / 2 != lasttime / 2)
			//{
			//	localtime_s(&nowdate, &nowtime);//��ȡ��ǰʱ��
			//	environment.SunMove();
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
threadparam tp;							//���ݸ����̵߳Ĳ���
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
float fps, avgfps, fpslimit;			//֡��, ֡�����ƣ�fpslimit<=0��ʾ�����ƣ�
float frametime, avgframetime;			//֡ʱ��
char invalidkey; LARGE_INTEGER invalidtime;//�Ƿ�������¼
float memory1, memory2;					//�ڴ�ռ����
tm dateGMT, devicedate, basedate, lastdate, realdate;//ʱ�䡣basedate���ṩ������ʱ����ʱ��
time_t nowtime, basetime, lasttime, realtime;		//ʱ��
int devicetimezone;						//�豸��ʱ��
wchar_t testchar;						//������Ļ��ʾЧ�����ַ�
bool rotated;							//�ӽ��Ƿ���ת

//��ʱ
LARGE_INTEGER frequency;				//������Ƶ��
LARGE_INTEGER stime, etime, stime2, etime2;

//ģʽ
int mode, lastmode;						//����ģʽ; �洢��ǰģʽ�����ڻָ���
int controlmode;						//��겶׽ģʽ
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
D3DCAPS9 caps;							//�豸����
float depthbias;						//��Ӱzdeptḩ����棬��ֹzfighting
float sdepthbias;						//��ֹ��������Ӱ���ɼ�
bool depthbiasable;						//depthbias�Ƿ����

//��Ϣ��ʾ
LPD3DXFONT font, font2, font3;			//����
RECT text, text2, text3, text4, text5, text6, text7;//������Ϣ��ʾ����
RECT text8, text9, text10, text11, text12;
WCHAR show[1024], showms[100], status1[100], status2[100];//������Ϣ����


//�۲���
WViewer viewer;
//����
WBlocker blocker;
//����
Environment environment;

//
//��Ϣ����
//
bool anti;
bool shadowchanged;
D3DXMATRIX matShadow;				//��Ӱ����
D3DMATERIAL9 shadowmtrl;			//��Ӱ����
bool sundown;						//̫���Ƿ���ɽ

//��ѭ��
inline void MainLoop();			

//��ʼ��
void Init();					//������״̬��ʼ��
void D3DInit(HWND wnd, D3DMULTISAMPLE_TYPE ms);	//D3D��ʼ��
void CreateDevice();			//�����豸
void DeviceInit();				//�豸��������
void SetTextRect();				//������ʾ������Ϣ�����򣨸��ݴ��ڳߴ磩
_D3DMULTISAMPLE_TYPE GetMultisampleType(LPDIRECT3D9 lp, D3DDISPLAYMODE dm);	//��ö��ز������÷�Χ�������������ز���
void ChangeMultiSample();		//���Ķ��ز���ģʽ

//�豸����
void OnLostDevice();			//�豸��ʧ����
void OnResetDevice();			//�豸���ô���

//��ʾ
inline void FpsShow();			//��ʾfps
inline void InfoShow();			//��ʾ��Ϣ
inline void OtherInfoShow();	//��ʾ������Ϣ
inline void UpdateStatusStr();	//����״̬,k�ַ���

//ʱ��
inline string GetWDayStr(int wday);	//�õ����ڼ��ַ���
inline void Get2WndRect();			//��ȡʵʱ���ںͿͻ�������
inline int GetRadianDValue(double radian);
inline int GetRadianMValue(double radian);
inline double GetRadianSValue(double radian);
inline int GetRadianSValueI(double radian);

//����
bool FullScreen(bool tofull);	//����ȫ��ģʽ
void CaptureControl();
void ReleaseControl();
