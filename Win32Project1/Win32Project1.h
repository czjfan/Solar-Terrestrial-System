#pragma once

//#include <afx.h>
#include "resource.h"
#include "windows.h"
#include "worldviewer.h"

#include <d3d9.h>
#include <d3dx9tex.h>
#include <string>

#pragma comment(lib,"d3d9.lib")
#pragma comment(lib,"d3dx9.lib")
#pragma comment(lib,"d3dx9d.lib")

using std::wstring;

//��ɫ
#define COLOR_CLEAR				D3DCOLOR_XRGB(252, 252, 252)
#define COLOR_GROUND			D3DCOLOR_XRGB(220, 220, 220)
	
#define FVF_CUSTOM1				D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_DIFFUSE


HWND mainwnd;//������Ϣ
int width, height;
time_t ticks;
POINT cursor;

LPDIRECT3D9	lpD3D;//D3D�豸
LPDIRECT3DDEVICE9 lpDevice;
D3DPRESENT_PARAMETERS d3dpp;
D3DDISPLAYMODE displaymode;
LPDIRECT3DSURFACE9 surf;
HRESULT hr;

LPD3DXFONT font;//����
RECT text, text2;
WCHAR show[100];

//�ӽ�
D3DXVECTOR3 pos;			//λ��
D3DXVECTOR3 viewdirection;	//��Ұ����

//��ͼ
IDirect3DVertexBuffer9* g_VB;
IDirect3DIndexBuffer9*  g_IB;

ID3DXMesh* mesh1;

//�۲���
WViewer viewer;

struct CUSTOMVERTEX
{
	D3DXVECTOR3 pos;     //����λ��
	D3DXVECTOR3 normal;  //���㷨��

	DWORD color;
};

void D3DInit(HWND wnd);
void Light();			//���ʺ͹���
void Pic();				//ͼԪ
void View();			//�ӽǣ�����任
void Paint();			//����