#pragma once

#include <d3d9.h>
#include <d3dx9tex.h>

#pragma comment(lib,"d3d9.lib")
#pragma comment(lib,"d3dx9.lib")

//����
#define FVF_CUSTOM1				(D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_DIFFUSE)
#define FVF_CUSTOM2				(D3DFVF_XYZ | D3DFVF_DIFFUSE)
#define FVF_CUSTOM3				(D3DFVF_XYZ | D3DFVF_DIFFUSE | D3DFVF_TEX1)

//�߶ȶ���
#define SKYRADIUS				1500.0f				//��հ뾶
#define VIEWRADIUS				10000.0f				//������Զ���루Զ���棩����10000.0f
#define BLOCKRADIUS				200.0f				//һ������İ뾶
#define BLOCKRANGE				50.0f				//������ʾ��Χ��������Χ�����岻����
#define FOGSTART				20.0f				//��ʼ��
#define FOGEND					BLOCKRANGE			//��100%Ũ�ȵľ��룬������Χ���ɼ�
#define PERSPECTIVEZNEAR_MIN	0.03f				//��С������
#define SHADOWHEIGHT_MIN		0.004f				//��Ӱ��С��ظ߶ȣ�С��������˸
#define WALKSPEED				0.4f
#define DEFAULTFOVY				(D3DX_PI / 3)
#define MAXSPHERELOOPS			100

#define RADIAN2DEGREE			(180.0f / D3DX_PI)

//��ɫ
#define COLOR_BLACK				D3DCOLOR_XRGB(0, 0, 0)
#define COLOR_SUN				D3DCOLOR_XRGB(254, 254, 248)
#define COLOR_WHITE				D3DCOLOR_XRGB(255, 255, 255)

//�������
extern time_t loopcount;

extern D3DXVECTOR3 viewpos;
extern D3DXMATRIX ViewTranslation;
extern bool viewchanged;
extern bool moved;
extern D3DXMATRIX matSun;
extern D3DXMATRIX sunTranslation;
extern bool shadowchanged;
extern D3DXVECTOR4 sunLight;
extern D3DXMATRIX matShadow;
extern D3DMATERIAL9 shadowmtrl;
extern LPDIRECT3DDEVICE9 device;

struct CUSTOMVERTEX1
{
	D3DXVECTOR3 pos;     //����λ��
	D3DXVECTOR3 normal;  //���㷨��
	
	DWORD color;
};
struct CUSTOMVERTEX2
{
	D3DXVECTOR3 pos;     //����λ��

	DWORD color;
};
struct CUSTOMVERTEX3
{
	D3DXVECTOR3 pos;     //����λ��
	DWORD color;    
	float tu, tv;
};

const D3DXPLANE groundPlane = D3DXPLANE(0.0f, 0.0f, -1.0f, -SHADOWHEIGHT_MIN);

inline void DrawShadow(ID3DXMesh * obj, const D3DXMATRIXA16 & matWorld)
{
	if (!device)
		return;

	D3DXMATRIXA16 W = matWorld;
	W *= matShadow;
	device->SetTransform(D3DTS_WORLD, &matWorld);


	device->SetMaterial(&shadowmtrl);
	device->SetTexture(0, 0);

	device->SetRenderState(D3DRS_STENCILENABLE, TRUE);//����Stencil buffer
	device->Clear(0, NULL, D3DCLEAR_STENCIL, 0, 0, 0);//���stencil??�Ƿ���Ҫ
	device->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);//alpha blend the shadow
	device->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);//�ر�ZBUFFER��д��
													  // Disable depth buffer so that z-fighting doesn't occur when we
													  // render the shadow on top of the floor.
													  //device->SetRenderState(D3DRS_ZENABLE, FALSE);//û��disable depth buffer
	device->SetRenderState(D3DRS_FOGENABLE, FALSE);//�ر���
	device->SetRenderState(D3DRS_DIFFUSEMATERIALSOURCE, D3DMCS_MATERIAL);//���ò���Ϊ��ɫ��Դ

	obj->DrawSubset(0);

	device->SetRenderState(D3DRS_DIFFUSEMATERIALSOURCE, D3DMCS_COLOR1);
	device->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
	device->SetRenderState(D3DRS_FOGENABLE, TRUE);
	device->SetRenderState(D3DRS_ZENABLE, TRUE);
	device->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
	device->SetRenderState(D3DRS_STENCILENABLE, FALSE);
}

void CreateSphere(ID3DXMesh **obj, int finess, float radius, D3DCOLOR color);