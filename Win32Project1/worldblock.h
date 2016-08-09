#pragma once

#include "D3Ddefine.h"

#include <d3d9.h>
#include <d3dx9tex.h>
#include <string>

//����ģʽ������ƽ�桢�ļ���ȡ
#define BLOCKMODE_GRID			1
#define BLOCKMODE_PLANE			2
#define BLOCKMODE_FILE			3

//��ɫ
#define COLOR_GROUND			D3DCOLOR_XRGB(240, 240, 240)
#define COLOR_GROUND2			D3DCOLOR_XRGB(80, 255, 30)
#define COLOR_SIGN				D3DCOLOR_XRGB(160, 20, 20)

using std::string;


class WBlocker {
public:
	LPDIRECT3DDEVICE9 device;

	D3DXMATRIXA16 matWorld;			//������������������������ԭ��ľ���
	D3DXMATRIXA16 matShadowWorld;	//��Ӱ���������

	float blockradius;				//����������߳�
	float blockrange;				//��Ұ���룬unused

	//D3DXVECTOR2 blockindex;		//ȫ�ֱ�������

	int blockmode;					//����ģʽ

	//mesh
	ID3DXMesh* generalmesh;			//ͨ������
	ID3DXMesh* signmesh;			//����·��

	//buffer��ͼ
	//IDirect3DVertexBuffer9* g_VB;
	//IDirect3DIndexBuffer9*  g_IB;

	//����
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

		//�ܱ��������
		D3DXMatrixTranslation(&matWorld, 2*blockradius, 0.0f, 0.0f);
		device->SetTransform(D3DTS_WORLD, &matWorld);
		device->SetMaterial(&blockmtrl);//����
		generalmesh->DrawSubset(0);
		device->SetMaterial(&signmtrl);//����
		signmesh->DrawSubset(0);


		//sign��Ӱ,ֻ���������������sign��Ӱ
		if (shadowchanged)
		{
			D3DXMatrixIdentity(&matWorld);
			matShadowWorld = matWorld * matShadow;
		}
		device->SetTransform(D3DTS_WORLD, &matShadowWorld);

		ShadowBegin();
		signmesh->DrawSubset(0);
		ShadowEnd();

		//sign���ƣ�������Ӱ
		D3DXMatrixIdentity(&matWorld);
		device->SetTransform(D3DTS_WORLD, &matWorld);
		device->SetMaterial(&signmtrl);//����
		signmesh->DrawSubset(0);


		//buffer ����
		/*device->SetStreamSource(0, g_VB, 0, sizeof(CUSTOMVERTEX));
		device->SetIndices(g_IB);
		device->SetFVF(FVF_CUSTOM1);
		device->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0,4,0,2);*/

		return true;
	}
};