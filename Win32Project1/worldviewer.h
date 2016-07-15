#pragma once

#include <d3d9.h>
#include <d3dx9tex.h>

#define VIEWMODE_FIRSTPERSON	1
#define VIEWMODE_CHASE			2
#define VIEWMODE_FREE			3
#define VIEWMODE_OVERLOOK		4

//�˶�
const float division = 0.1f;
const float chasedist = 4.0f;
const float chaseheight = 3.0f;
const float overlookheight = 400.0f;

class WViewer {
public:
	LPDIRECT3DDEVICE9 device;

	D3DXVECTOR3 up;

	D3DXVECTOR3 pos;		//�۲���λ��
	D3DXVECTOR3 at;
	D3DXVECTOR3 viewpos;

	float viewheight;		//�ӽǸ߶�(����߶�)
	float viewdist;			//��Ұ����
	float viewangle;		//��Ұ�Ƕ�
	float viewaspect;		//��Ұ�����
	float hAngle;			//ˮƽ��Ƕ�	(0~360) x����=0
	float vAngle;			//����Ƕ�	(-90~90)

	int viewmode;			//�۲�ģʽ

public:
	WViewer();
	~WViewer();

	bool SetDevice(LPDIRECT3DDEVICE9 device);
	bool SetAspect(float aspect);
	bool SetViewmode(int mode);
	bool SetView(LPDIRECT3DDEVICE9 dvc = NULL);

	void Walk(int key);
	void Rotate(POINT bias);
	void KeyControl(int key);
};