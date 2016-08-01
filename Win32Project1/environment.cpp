#include "stdafx.h"
#include "environment.h"

Environment::Environment()
{
	device = NULL;
	//D3DXMatrixIdentity(&matworld);

	skyback = NULL;
	sun = NULL;
	skyradius = SKYRADIUS;
}

Environment::~Environment()
{
}

bool Environment::SetDevice(LPDIRECT3DDEVICE9 device)
{
	this->device = device;

	return (device != NULL);
}

bool Environment::InitSkyBack()
{
	if (!device)
		return false;

	//��������
	CUSTOMVERTEX2 g_vertices[8] =
	{
		{ D3DXVECTOR3(-skyradius, -skyradius, 20.0f), COLOR_SKY1 }
		,{ D3DXVECTOR3(-skyradius, skyradius, 20.0f), COLOR_SKY1 }
		,{ D3DXVECTOR3(-skyradius, skyradius, -skyradius), COLOR_SKY2 }
		,{ D3DXVECTOR3(-skyradius, -skyradius, -skyradius), COLOR_SKY2 }


		,{ D3DXVECTOR3(skyradius, -skyradius, 20.0f), COLOR_SKY1 }
		,{ D3DXVECTOR3(skyradius, skyradius, 20.0f), COLOR_SKY1 }
		,{ D3DXVECTOR3(skyradius, skyradius, -skyradius), COLOR_SKY2 }
		,{ D3DXVECTOR3(skyradius, -skyradius, -skyradius), COLOR_SKY2 }
	};
	//������������
	WORD index[30] =
	{
		0, 2, 1, 0, 3, 2//x-
		,4, 5, 6, 4, 6, 7//x+
		,0, 4, 7, 0, 7, 3//y-
		,1, 6, 5, 1, 2, 6//y+
		,3, 7, 6, 3, 6, 2//��
	};

	void* pVertices = NULL;
	void* pIndex = NULL;
	HRESULT hr;

	if (skyback)
		skyback->Release();
	//mesh��ʽ
	hr = D3DXCreateMeshFVF(10, 8, D3DXMESH_MANAGED,
		FVF_CUSTOM2,
		device,
		&skyback);
	skyback->LockVertexBuffer(0, &pVertices);
	memcpy(pVertices, g_vertices, sizeof(g_vertices));
	skyback->UnlockVertexBuffer();

	skyback->LockIndexBuffer(0, &pIndex);
	memcpy(pIndex, index, sizeof(index));
	skyback->UnlockIndexBuffer();


	return true;
}

void Environment::InitMaterialLight()
{
	CreateSphere(&sun, 15, 50, COLOR_SUN);

	ZeroMemory(&material0, sizeof(D3DMATERIAL9));
	material0.Diffuse = { 1.0f, 1.0f, 1.0f, 1.0f };//������
	material0.Ambient = { 1.0f, 1.0f, 1.0f, 0.1f };//������
	material0.Specular = { 0.0f, 0.0f, 0.0f, 0.0f };//���淴��
	//material0.Emissive={ 1.0f, 1.0f, 1.0f, 1.0f };//�Է���
	material0.Power = 1.0f;

	ZeroMemory(&light0, sizeof(D3DLIGHT9));
	light0.Type = D3DLIGHT_POINT;
	light0.Range = 3000.0f;
	light0.Diffuse = { 1.0f, 1.0f, 0.9f, 1.0f };
	light0.Ambient = { 0.1f, 0.1f, 0.1f, 0.3f };
	light0.Specular = { 0.6f, 0.6f, 0.6f, 1.0f };
	light0.Position = D3DXVECTOR3(-500.0f, 0.0f, -500.0f);
	light0.Attenuation0 = 0.0f;
	light0.Attenuation1 = 0.001f;
	
	//̫������
	sunLight = D3DXVECTOR4(light0.Position, 1);//����ȫ��̫��λ��
	D3DXMatrixIdentity(&sunTranslation);
	D3DXMatrixTranslation(&sunTranslation, sunLight.x, sunLight.y, sunLight.z);
	matSun = ViewTranslation * sunTranslation;
	//��Ӱ����
	D3DXMatrixShadow(
		&matShadow,
		&sunLight,
		&groundPlane);//������Ӱ����
	D3DXMATRIXA16 elevate;
	D3DXMatrixIdentity(&elevate);
	D3DXMatrixTranslation(&elevate, 0, 0, groundPlane.d);//��Ӱ̧��
	matShadow = elevate*matShadow;
	shadowchanged = true;//��Ӱ�ı��־

	//��Ӱ����
	ZeroMemory(&shadowmtrl, sizeof(D3DMATERIAL9));
	shadowmtrl.Ambient = { 0.0f,0.0f,0.0f,0.0f };
	shadowmtrl.Diffuse = { 0.0f,0.0f,0.0f,0.2f };// 50% transparency.
	shadowmtrl.Emissive = { 0.0f,0.0f,0.0f,0.0f };
	shadowmtrl.Specular = { 0.0f,0.0f,0.0f,0.0f };
	shadowmtrl.Power = 1.0f;

	/*light1.Phi = D3DX_PI / 4.0;
	light1.Theta = D3DX_PI / 8.0f;
	light1.Falloff = 1.0f;*/
	//light1.Direction = D3DXVECTOR3(0, 0, 1);

	device->SetLight(0, &light0);
	device->LightEnable(0, TRUE);
}


