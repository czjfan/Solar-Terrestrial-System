#pragma once

#include "D3Ddefine.h"

#include <d3d9.h>
#include <d3dx9tex.h>
#include <string>

#define BLOCKMODE_GRID			1
#define BLOCKMODE_PLANE			2
#define BLOCKMODE_XFILE			3

#define COLOR_GROUND			D3DCOLOR_XRGB(240, 240, 240)
#define COLOR_GROUND2			D3DCOLOR_XRGB(80, 255, 30)
#define COLOR_SIGN				D3DCOLOR_XRGB(160, 20, 20)

using std::string;


class WBlocker {
public:
	LPDIRECT3DDEVICE9 device;

	D3DXMATRIXA16 matWorld;
	D3DXMATRIXA16 matShadowWorld;//��Ӱ���������

	float blockradius;				//����������߳�
	float blockrange;				//��Ұ���룬unused

	int blockmode;

	//mesh
	ID3DXMesh* generalmesh;			//ͨ������
	ID3DXMesh* signmesh;			//����·��
	//buffer��ͼ
	//IDirect3DVertexBuffer9* g_VB;
	//IDirect3DIndexBuffer9*  g_IB;

	D3DMATERIAL9 blockmtrl;
	D3DMATERIAL9 signmtrl;

public:
	WBlocker();
	~WBlocker();

	bool SetDevice(LPDIRECT3DDEVICE9 device);
	bool SetGBlock();
	inline bool Draw()
	{
		if (!device)
			return false;

		if (!generalmesh)
			return false;

		//�������
		D3DXMatrixIdentity(&matWorld);
		device->SetTransform(D3DTS_WORLD, &matWorld);
		device->SetMaterial(&blockmtrl);//����
		generalmesh->DrawSubset(0);
		//sign����
		device->SetMaterial(&signmtrl);//����
		signmesh->DrawSubset(0);


		D3DXMatrixTranslation(&matWorld, 2*blockradius, 0.0f, 0.0f);
		device->SetTransform(D3DTS_WORLD, &matWorld);
		device->SetMaterial(&blockmtrl);//����
		generalmesh->DrawSubset(0);
		device->SetMaterial(&signmtrl);//����
		signmesh->DrawSubset(0);



		//sign��Ӱ,ֻ���������������sign��Ӱ
		//D3DXMatrixTranslation(&matWorld, 0, 0, -SHADOWHEIGHT_MIN);
		if (shadowchanged)
		{
			D3DXMatrixIdentity(&matWorld);
			matShadowWorld = matWorld * matShadow;
		}
		device->SetTransform(D3DTS_WORLD, &matShadowWorld);

		device->SetRenderState(D3DRS_STENCILENABLE, TRUE);//����Stencil buffer
														  //device->Clear(0, NULL, D3DCLEAR_STENCIL, 0, 0, 0);//���stencil??�Ƿ���Ҫ
		device->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);//alpha blend the shadow
		device->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);//�ر�ZBUFFER��д�룬����Ӱ��˸
		device->SetRenderState(D3DRS_FOGENABLE, FALSE);//�ر�������Ӱȫ��ɫ
		device->SetRenderState(D3DRS_DIFFUSEMATERIALSOURCE, D3DMCS_MATERIAL);//���ò���Ϊ��ɫ��Դ��������ɫ����

		device->SetMaterial(&shadowmtrl);
		//device->SetTexture(0, 0);//??
		signmesh->DrawSubset(0);

		device->SetRenderState(D3DRS_DIFFUSEMATERIALSOURCE, D3DMCS_COLOR1);
		device->SetRenderState(D3DRS_FOGENABLE, TRUE);
		device->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
		device->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
		device->SetRenderState(D3DRS_STENCILENABLE, FALSE);

		//buffer ����
		/*device->SetStreamSource(0, g_VB, 0, sizeof(CUSTOMVERTEX));
		device->SetIndices(g_IB);
		device->SetFVF(FVF_CUSTOM1);
		device->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0,4,0,2);*/

		return true;
	}
};