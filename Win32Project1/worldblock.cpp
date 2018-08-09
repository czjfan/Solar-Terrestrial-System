#include "stdafx.h"
#include "worldblock.h"

WBlocker::WBlocker()	:blockradius(BLOCKRADIUS),
						blockrange(BLOCKRANGE),
						blockmode(BLOCKMODE_PLANE),
						generalmesh(NULL),
						signmesh(NULL)
{
	D3DXMatrixIdentity(&matWorld);
}

WBlocker::~WBlocker()
{
}

bool WBlocker::SetDevice(LPDIRECT3DDEVICE9 device)
{
	this->device = device;

	return (device != NULL);
}

bool WBlocker::SetGBlock()
{
	if (!device)
		return false;

	//��������
	CUSTOMVERTEX1 g_vertices[4] =
	{
		{ D3DXVECTOR3(-blockradius, -blockradius, 0.0f),
			D3DXVECTOR3(0.0f, 0.0f, -1.0f), COLOR_GROUND4 }
		,{ D3DXVECTOR3(blockradius, blockradius, 0.0f),
			D3DXVECTOR3(0.0f, 0.0f, -1.0f),COLOR_GROUND4 }
		,{ D3DXVECTOR3(-blockradius, blockradius, 0.0f),
			D3DXVECTOR3(0.0f, 0.0f, -1.0f), COLOR_GROUND4 }
		,{ D3DXVECTOR3(blockradius, -blockradius, 0.0f),
			D3DXVECTOR3(0.0f, 0.0f, -1.0f), COLOR_GROUND4 }
	};
	//������������
	WORD index[6] =
	{
		0, 2, 1, 0, 1, 3
	};

	void* pVertices = NULL;
	void* pIndex = NULL;
	HRESULT hr;

	if (generalmesh)
		generalmesh->Release();
	//mesh��ʽ
	hr = D3DXCreateMeshFVF(2, 4, D3DXMESH_MANAGED,
		FVF_CUSTOM1,
		device,
		&generalmesh);
	generalmesh->LockVertexBuffer(0, &pVertices);
	memcpy(pVertices, g_vertices, sizeof(g_vertices));
	generalmesh->UnlockVertexBuffer();

	generalmesh->LockIndexBuffer(0, &pIndex);
	memcpy(pIndex, index, sizeof(index));
	generalmesh->UnlockIndexBuffer();



	ZeroMemory(&blockmtrl, sizeof(D3DMATERIAL9));
	blockmtrl.Diffuse = { 0.55f, 1.0f, 0.12f, 1.0f };//������
	blockmtrl.Ambient = { 0.55f, 1.0f, 0.12f, 1.0f };//������
	blockmtrl.Specular = { 0.0f, 0.0f, 0.0f, 0.0f };//���淴��
	//blockmtrl.Emissive={ 1.0f, 1.0f, 1.0f, 1.0f };//�Է���
	blockmtrl.Power = 1.0f;


	//sign mesh
	float signwidth = 0.03f;
	float signheight = -0.1f;
	//��������
	CUSTOMVERTEX1 sign_vertices[20] =
	{
		{ D3DXVECTOR3(signwidth, signwidth, signheight),
		D3DXVECTOR3(0.0f, 0.0f, -1.0f), COLOR_SIGN }
		,{ D3DXVECTOR3(signwidth, -signwidth, signheight),
		D3DXVECTOR3(0.0f, 0.0f, -1.0f), COLOR_SIGN }
		,{ D3DXVECTOR3(-signwidth, -signwidth, signheight),
		D3DXVECTOR3(0.0f, 0.0f, -1.0f), COLOR_SIGN }
		,{ D3DXVECTOR3(-signwidth, signwidth, signheight),
		D3DXVECTOR3(0.0f, 0.0f, -1.0f), COLOR_SIGN }

		,{ D3DXVECTOR3(signwidth, signwidth, 0.0f),
		D3DXVECTOR3(0.0f, 1.0f, 0.0f), COLOR_SIGN }
		,{ D3DXVECTOR3(-signwidth, signwidth, 0.0f),
		D3DXVECTOR3(0.0f, 1.0f, 0.0f), COLOR_SIGN }
		,{ D3DXVECTOR3(-signwidth, signwidth, signheight),
		D3DXVECTOR3(0.0f, 1.0f, 0.0f), COLOR_SIGN }
		,{ D3DXVECTOR3(signwidth, signwidth, signheight),
		D3DXVECTOR3(0.0f, 1.0f, 0.0f), COLOR_SIGN }

		,{ D3DXVECTOR3(-signwidth, signwidth, 0.0f),
		D3DXVECTOR3(-1.0f, 0.0f, 0.0f), COLOR_SIGN }
		,{ D3DXVECTOR3(-signwidth, -signwidth, 0.0f),
		D3DXVECTOR3(-1.0f, 0.0f, 0.0f), COLOR_SIGN }
		,{ D3DXVECTOR3(-signwidth, -signwidth, signheight),
		D3DXVECTOR3(-1.0f, 0.0f, 0.0f), COLOR_SIGN }
		,{ D3DXVECTOR3(-signwidth, signwidth, signheight),
		D3DXVECTOR3(-1.0f, 0.0f, 0.0f), COLOR_SIGN }

		,{ D3DXVECTOR3(-signwidth, -signwidth, 0.0f),
		D3DXVECTOR3(0.0f, -1.0f, 0.0f), COLOR_SIGN }
		,{ D3DXVECTOR3(signwidth, -signwidth, 0.0f),
		D3DXVECTOR3(0.0f, -1.0f, 0.0f), COLOR_SIGN }
		,{ D3DXVECTOR3(signwidth, -signwidth, signheight),
		D3DXVECTOR3(0.0f, -1.0f, 0.0f), COLOR_SIGN }
		,{ D3DXVECTOR3(-signwidth, -signwidth, signheight),
		D3DXVECTOR3(0.0f, -1.0f, 0.0f), COLOR_SIGN }

		,{ D3DXVECTOR3(signwidth, -signwidth, 0.0f),
		D3DXVECTOR3(1.0f, 0.0f, 0.0f), COLOR_SIGN }
		,{ D3DXVECTOR3(signwidth, signwidth, 0.0f),
		D3DXVECTOR3(1.0f, 0.0f, 0.0f), COLOR_SIGN }
		,{ D3DXVECTOR3(signwidth, signwidth, signheight),
		D3DXVECTOR3(1.0f, 0.0f, 0.0f), COLOR_SIGN }
		,{ D3DXVECTOR3(signwidth, -signwidth, signheight),
		D3DXVECTOR3(1.0f, 0.0f, 0.0f), COLOR_SIGN }
	};
	//������������
	WORD sign_index[30] =
	{
		0,1,2,0,2,3
		,4,6,5,4,7,6//y+
		,8,10,9,8,11,10//x-
		,12,14,13,12,15,14//y-
		,16,18,17,16,19,18//x+
	};

	void* signVertices = NULL;
	void* signIndex = NULL;

	if (signmesh)
		signmesh->Release();
	//mesh��ʽ
	hr = D3DXCreateMeshFVF(10, 20, D3DXMESH_MANAGED,
		FVF_CUSTOM1,
		device,
		&signmesh);
	signmesh->LockVertexBuffer(0, &signVertices);
	memcpy(signVertices, sign_vertices, sizeof(sign_vertices));
	signmesh->UnlockVertexBuffer();

	signmesh->LockIndexBuffer(0, &signIndex);
	memcpy(signIndex, sign_index, sizeof(sign_index));
	signmesh->UnlockIndexBuffer();

	//sign����
	ZeroMemory(&signmtrl, sizeof(D3DMATERIAL9));
	signmtrl.Diffuse = { 1.0f, 0.2f, 0.2f, 0.5f };//������
	signmtrl.Ambient = { 1.0f, 0.4f, 0.4f, 1.0f };//������
	signmtrl.Specular = { 0.0f, 0.0f, 0.0f, 0.0f };//���淴��
	//signmtrl.Emissive={ 1.0f, 1.0f, 1.0f, 1.0f };//�Է���
	signmtrl.Power = 1.0f;


	//buffer��ʽ
	//g_VB = 0; //�����嶥��  
	//g_IB = 0; //��������  
	////create vertex buffer.
	//hr = device->CreateVertexBuffer(sizeof(g_vertices), D3DUSAGE_WRITEONLY
	//	, FVF_CUSTOM1, D3DPOOL_MANAGED, &g_VB, NULL);
	//if (FAILED(hr))
	//{
	//	//return E_FAIL;
	//}
	////fill vertex buffer.
	//���ö�������
	//if (FAILED(g_VB->Lock(0, sizeof(g_vertices), &pVertices, 0)))
	//{
	//	//return E_FAIL;
	//}
	//memcpy(pVertices, g_vertices, sizeof(g_vertices));
	//g_VB->Unlock();
	//������������
	//device->CreateIndexBuffer(sizeof(index), D3DUSAGE_WRITEONLY, 
	//	D3DFMT_INDEX16, D3DPOOL_MANAGED, &g_IB, NULL);
	//g_IB->Lock(0, sizeof(index), &pIndex, 0);
	//memcpy(pIndex, index, sizeof(index));
	//g_IB->Unlock();
	return true;
}
