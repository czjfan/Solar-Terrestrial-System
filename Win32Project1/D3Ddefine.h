#pragma once

#include <d3d9.h>
#include <d3dx9tex.h>

#pragma comment(lib,"d3d9.lib")
#pragma comment(lib,"d3dx9.lib")

//����
#define FVF_CUSTOM1				(D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_DIFFUSE)
#define FVF_CUSTOM2				(D3DFVF_XYZ | D3DFVF_DIFFUSE)
#define FVF_CUSTOM3				(D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_DIFFUSE | D3DFVF_TEX1)

//������
#define ONEMETER				0.1f				//1�׵ľ���

//�߶ȶ���
#define SKYRADIUS				1500.0f				//��հ뾶
#define VIEWRADIUS				10000.0f			//������Զ���루Զ���棩��Ӱ�������
#define BLOCKRADIUS				200.0f				//һ������İ뾶�������ΰ�߳���
#define BLOCKRANGE				50.0f				//������ʾ��Χ��������Χ�����岻����
#define FOGSTART				20.0f				//��ʼ��
#define FOGEND					BLOCKRANGE			//��100%Ũ�ȵľ��룬������Χ���ɼ�
#define PERSPECTIVEZNEAR_MIN	0.02f				//��С������0.03
#define SHADOWHEIGHT_MIN		0.0f				//��Ӱ��С��ظ߶ȣ�С��������˸���ݲ�ʹ��depthbias����
#define WALKSPEED				0.4f				//ÿ���˶��ľ���
#define DEFAULTFOVY				(D3DX_PI / 3)		//��׶���¼н�
#define MAXSPHERELOOPS			110					//������徫��
#define DEPTHBIAS				-0.0000005f
#define DEPTHBIAS2				-0.0000010f
#define SLOPESCALEDEPTHBIAS		-1.10f				//��ֵԽ�ӽ�0Խ���׽����뿴��˸��ԽԶ��0��ӰԽ���׸ǹ�����
#define TINYBIAS				0.000001f			//΢Сƽ���������ص�������


#define RADIAN2DEGREE			(180.0f / D3DX_PI)

//��ɫ
#define COLOR_BLACK				D3DCOLOR_XRGB(0, 0, 0)
#define COLOR_WHITE				D3DCOLOR_XRGB(255, 255, 255)

#define COLOR_SUN				D3DCOLOR_XRGB(254, 254, 248)
#define COLOR_YELLOWGRASS		D3DCOLOR_XRGB(145, 185, 22)


struct INDEX2D
{
	long long int x;
	long long int y;
};

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
	D3DXVECTOR3 normal;  //���㷨��
	DWORD color;    
	float tu, tv;
};

const D3DXPLANE groundPlane = D3DXPLANE(0.0f, 0.0f, -1.0f, -SHADOWHEIGHT_MIN);

//�������
extern time_t loopcount;

extern LPDIRECT3DDEVICE9 device;

extern INDEX2D blockindex;
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

extern D3DCAPS9 caps;
extern float depthbias;
extern float sdepthbias;
extern bool depthbiasable;

//extern WViewer viewer;
//extern WBlocker blocker;
//extern Environment environment;


inline void ShadowBegin()
{
	if (!device)
		return;

	//device->SetRenderState(D3DRS_STENCILENABLE, TRUE);//����Stencil buffer
	//device->Clear(0, NULL, D3DCLEAR_STENCIL, 0, 0, 0);//���stencil??�Ƿ���Ҫ
	device->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);//alpha blend the shadow
	device->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);//�ر�ZBUFFER��д��
	device->SetRenderState(D3DRS_FOGENABLE, FALSE);//�ر�������Ӱȫ��ɫ
	device->SetRenderState(D3DRS_DIFFUSEMATERIALSOURCE, D3DMCS_MATERIAL);//���ò���Ϊ��ɫ��Դ��������ɫ����

	//depthbias
	if (depthbiasable)
	{
		device->SetRenderState(D3DRS_DEPTHBIAS, *((DWORD*)&depthbias));
		device->SetRenderState(D3DRS_SLOPESCALEDEPTHBIAS, *((DWORD*)&sdepthbias));
	}
	device->SetMaterial(&shadowmtrl);
}
inline void ShadowEnd()
{
	device->SetRenderState(D3DRS_SLOPESCALEDEPTHBIAS, 0);
	device->SetRenderState(D3DRS_DEPTHBIAS, 0);

	device->SetRenderState(D3DRS_DIFFUSEMATERIALSOURCE, D3DMCS_COLOR1);
	device->SetRenderState(D3DRS_FOGENABLE, TRUE);
	device->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
	device->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
	//device->SetRenderState(D3DRS_STENCILENABLE, FALSE);
}

void CreateSphere(ID3DXMesh **obj, int finess, float radius, D3DCOLOR color);