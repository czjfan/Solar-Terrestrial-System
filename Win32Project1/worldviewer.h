#pragma once

#include "D3Ddefine.h"
#include <d3d9.h>
#include <d3dx9tex.h>
#include <string>

//��ɫ
#define COLOR_FIGURE1					D3DCOLOR_XRGB(255, 140, 120)
//viewģʽ
#define VIEWMODE_FIRSTPERSON			1
#define VIEWMODE_CHASE					2
#define VIEWMODE_FREE					3
#define VIEWMODE_OVERLOOK				4

//�˶�����
#define DIRECTION_NONE					0
#define DIRECTION_TOP					1
#define DIRECTION_LEFT					2
#define DIRECTION_RIGHT					3
#define DIRECTION_DOWN					4
#define DIRECTION_TOPLEFT				5
#define DIRECTION_TOPRIGHT				6
#define DIRECTION_BOTTOMLEFT			7
#define DIRECTION_BOTTOMRIGHT			8

//�����ٶ�ģʽ
#define REMNANTSPEEDMODE_OFF			0
#define REMNANTSPEEDMODE_DELAY			1
#define REMNANTSPEEDMODE_SMOOTH			2
#define REMNANTSPEEDMODE_DELAYSMOOTH	3


using std::string;

//�˶�
const WCHAR walkdirection[10] = { L'��', L'��', L'��', L'��', L'��', L'�I', L'�J', L'�L', L'�K', L'�~' };
const char viewdirection[][3] = { "E", "NE", "N", "NW", "W", "SW", "S", "SE", "E"};//�ӽǷ���
const float speednormal = WALKSPEED;		//�����ٶ�
const float speeddelaytime = 0.04f;			//�ٶ�ά�ֲ����ʱ�䡣�����ٶ�ʱ��1
const float speedsmoothtime = 0.1f;			//�ٶȽ�Ϊ0��Ҫ��ʱ�䡣�����ٶ�ʱ��2
const float chasedist = 0.6f;				//׷���ӽ�ˮƽ����
const float chasescreenheight = 0.2f;		//׷���ӽǸ߶�
const float overlookscreenheight = 300.0f;	//���Ӹ߶�

class WViewer {
public:
	LPDIRECT3DDEVICE9 device;

	//��ͼ
	ID3DXMesh* figure;
	ID3DXMesh* figurelight;

	//����
	D3DXMATRIXA16 matView;				//view����
	D3DXMATRIXA16 matWorld;				//�����������󣨰���ƽ�ƺ�ˮƽ��ת��
	D3DXMATRIXA16 matTranslation;		//����ƽ�ƾ���
	D3DXMATRIXA16 matTranslation2;		//����ƽ��΢Сƫ�ƣ�����figurelight����ֹ������Ⱦ���ص�����������
	D3DXMATRIXA16 matHRotate;			//����ˮƽ��ת����
	D3DXMATRIXA16 matShadowWorld;		//Ӱ�Ӿ���

	//���� & ����
	D3DMATERIAL9 figuremtrl;			//�������
	LPDIRECT3DTEXTURE9 g_Texture;		//�����������

	//ʵʱ��Ϣ
	DoubleVec3 viewpos_d;
	DoubleVec3 displacement;			//λ��
	double baselongitude, baselatitude;
	double longitude, latitude;			//�۾���γ��TODO:��תʱ�޸�
	double figurelongitude, figurelatitude;
	DoubleVec3 pos_d;					//double��������λ��
	INDEX2D blockindex;					//�۾�λ���ĸ�����
	D3DXVECTOR3 inblockpos;
	D3DXMATRIX inblockTranslation;

	float figurehangle, figurevangle;	//���ﳯ��(0~360) & (-90~90)
	double hAngle;						//�ӽ�ˮƽ��Ƕ�	(0~360) x����=0
	double basehangle;
	double earthhangle;					//�ڵ����ϵ�ǰλ�ã�ʵ�ʵ�ָ��
	double vAngle;						//�ӽ�����Ƕ�	(-90~90)
	D3DXVECTOR3 viewdirection;			//�۾�����, ���ڸ�������at��eye + viewdirection = at
	D3DXVECTOR3 eye;
	D3DXVECTOR3 at;						//����Ŀ��
	D3DXVECTOR3 up;
	float viewangle;					//��Ұ�Ƕ�
	float viewaspect;					//��Ұ�����

	//�����˶�
	LARGE_INTEGER frequency;			//��ʱ��Ƶ��
	LARGE_INTEGER endtick;				//���������ʱ��ticks�����ڲ����ٶȼ���
	LARGE_INTEGER lasttick;				//��һ��tick�����ڵ�ǰ���߾������
	float speed;						//��ǰ�ٶ�
	float speedrate;
	int remnantspeedmode;				//�����ٶ�ģʽ
	int curdirection;					//��ǰ�˶�����
	int lastdirection;					//��ǰ����Ϊ0ʱ�洢��һ�����򣬴�������ٶȵ��˶�
	bool wdown, sdown, adown, ddown;	//����״̬
	bool shiftdown;						//����״̬

	D3DXVECTOR3 figureeye;				//�ӽ�λ��(�����۾��������ģ��λ��)
	float viewradius;					//��Ұ����
	float sensitivity;					//��ת������
	double hcos, hsin, vsin, vcos, bhcos, bhsin;		//��������
	bool flashlight;

	int viewmode;						//viewģʽ
	bool viewchanged;
	bool figuremoved;
	bool eyehmoved;
	bool bindview;

public:
	WViewer();
	~WViewer();

	void InitProj(float aspect);	//����ͶӰ�任���������ñ任
	void SetFigure();

	bool SetDevice(LPDIRECT3DDEVICE9 device);	//�����豸
	bool SetViewAngle(float angle);				//����view�Ƕ�
	bool SetAspect(float aspect);				//����aspect
	void SetViewmode(int mode);					//���ù۲�ģʽ
	inline void Refreshtri()
	{
		hcos = cos(hAngle);
		hsin = sin(hAngle);
		vsin = sin(vAngle);
		vcos = cos(vAngle);
	}
	inline void BindView()
	{
		switch (viewmode)
		{
		case VIEWMODE_FIRSTPERSON:
			viewpos_d = pos_d + DoubleVec3(hcos*figureeye.x -hsin*figureeye.y
				, hsin*figureeye.x + hcos*figureeye.y, figureeye.z);
			break;
		case VIEWMODE_CHASE:
			viewpos_d = pos_d + DoubleVec3(-chasedist*hcos, -chasedist*hsin, -chasescreenheight);
			break;
		case VIEWMODE_FREE:
			//viewpos_d = pos_d + DoubleVec3(-chasedist*hcos, -chasedist*hsin, -chasescreenheight);
			break;
		case VIEWMODE_OVERLOOK:
			//viewpos_d = pos_d + DoubleVec3(0.0f, 0.0f, -overlookscreenheight);
			//at = inblockpos;//���ƴ�ֱ�ӽ�
			break;
		}

		bindview = false;
	}
	inline void SetBlockInfo()
	{
		//��������λ��
		blockindex.x = (long long)floor((viewpos_d.x + BLOCKRADIUS) / (long long)(2 * BLOCKRADIUS));
		blockindex.y = (long long)floor((viewpos_d.y + BLOCKRADIUS) / (long long)(2 * BLOCKRADIUS));
		//����inblockpos��ʵ���۾�λ��
		inblockpos = D3DXVECTOR3(
			(float)(viewpos_d.x - blockindex.x * 2 * BLOCKRADIUS)
			, (float)(viewpos_d.y - blockindex.y * 2 * BLOCKRADIUS)
			, 0.0f);
		//����ƽ�ƾ���
		D3DXMatrixIdentity(&inblockTranslation);
		D3DXMatrixTranslation(&inblockTranslation, inblockpos.x, inblockpos.y, 0.0f);//z����ƽ��

		//eyehmoved = false;
	}
	inline void SetViewVector()					//���ݵ�ǰ����λ��pos_d�͹۲�ģʽ���������߲���
	{
		//eye = D3DXVECTOR3(inblockpos.x, inblockpos.y, viewpos_d.z);
		if(viewmode==VIEWMODE_CHASE)//��γ������ԣ���ʹ������
			eye = D3DXVECTOR3(-chasedist*hcos, -chasedist*hsin, -chasescreenheight);
		else if(viewmode==VIEWMODE_FIRSTPERSON)
			eye = D3DXVECTOR3(hcos*figureeye.x - hsin*figureeye.y, hsin*figureeye.x + hcos*figureeye.y, figureeye.z);
		else if(viewmode==VIEWMODE_FREE ||viewmode==VIEWMODE_OVERLOOK)
			eye = D3DXVECTOR3(0.0f, 0.0f, viewpos_d.z);

		//����up
		if (vAngle == -MYPI / 2)
			up = D3DXVECTOR3((float)(hcos), (float)(hsin), 0.0f);
		else if (vAngle == MYPI / 2)
			up = -D3DXVECTOR3((float)(hcos), (float)(hsin), 0.0f);
		else
			up = D3DXVECTOR3(0.0f, 0.0f, -1.0f);

		viewdirection = D3DXVECTOR3((float)(hcos*vcos), (float)(hsin*vcos), (float)(-vsin));
		//����at
		at = eye + viewdirection;//�����ӽ�
	}
	inline bool SetView()//�����ӽǾ���
	{
		if (!device)
			return false;

		/*if (viewchanged)
		{
			SetViewVector();
			D3DXMatrixLookAtLH(&matView, &eye, &at, &up);
			viewchanged = false;
		}*/

		device->SetTransform(D3DTS_VIEW, &matView);

		return true;
	}
	inline bool RefreshView()
	{
		SetViewVector();
		D3DXMatrixLookAtLH(&matView, &eye, &at, &up);

		viewchanged = false;

		return true;
	}
	bool SetProj();								//���ݵ�ǰ��������ͶӰ�任

	inline void HandleSpeed()					//����ǰ�ٶ�
	{
		if (speed == 0.0f)
			return;

		LARGE_INTEGER now;
		QueryPerformanceCounter(&now);
		float time = (float)(now.QuadPart - endtick.QuadPart) / frequency.QuadPart;
		if (curdirection == DIRECTION_NONE)//����������ߣ��Ŵ����ٶȣ������ڼ䱣���ٶȺ㶨
		{
			switch (remnantspeedmode)
			{
			case REMNANTSPEEDMODE_OFF:
				speed = 0.0f;
				break;
			case REMNANTSPEEDMODE_DELAY:
				if (time < speeddelaytime)
				{
					return;
				}
				else
					speed = 0.0f;
				break;
			case REMNANTSPEEDMODE_SMOOTH:
				if (time < speedsmoothtime)
				{
					float percent = (speedsmoothtime - time)
						/ speedsmoothtime;
					speed = speednormal*percent;
				}
				else
					speed = 0.0f;
				break;
			case REMNANTSPEEDMODE_DELAYSMOOTH:
				if (time < speeddelaytime)
				{
					return;
				}
				else
				{
					float percent = ((speeddelaytime + speedsmoothtime) - time) / speedsmoothtime;
					if (percent > 0)
						speed = speednormal*percent;
					else
						speed = 0.0f;
				}
				break;
			}
		}
	}
	inline void Walk()							//����speed��curdirection��������
	{
		if (speed == 0.0f)//û�ٶ��򷵻�
		{
			displacement = DoubleVec3(0.0, 0.0, 0.0f);
			return;
		}

		LARGE_INTEGER now;
		QueryPerformanceCounter(&now);
		double dist = speed*(double)(now.QuadPart - lasttick.QuadPart) / frequency.QuadPart;//�ƶ�����
		lasttick = now;//��¼Ϊ��һ������ʱ���
		dist *= speedrate;
		//�����ǰ�˶�����ΪNONE��ʹ�ô洢����һ���˶�����
		int direction = (curdirection == DIRECTION_NONE) ? lastdirection : curdirection;
		if (viewmode == VIEWMODE_FIRSTPERSON || viewmode == VIEWMODE_CHASE)
		{
			if (direction == DIRECTION_TOP)//ǰ��
			{
				displacement.x = dist*hcos;
				displacement.y = dist*hsin;
				displacement.z = 0.0;
			}
			else if (direction == DIRECTION_DOWN)
			{
				displacement.x = -dist*hcos;
				displacement.y = -dist*hsin;
				displacement.z = 0.0;
			}
			else if (direction == DIRECTION_LEFT)
			{
				displacement.x = -dist*hsin;
				displacement.y = dist*hcos;
				displacement.z = 0.0;
			}
			else if (direction == DIRECTION_RIGHT)
			{
				displacement.x = dist*hsin;
				displacement.y = -dist*hcos;
				displacement.z = 0.0;
			}
			else if (direction == DIRECTION_TOPLEFT)
			{
				double biash = hAngle + MYPI / 8;
				displacement.x = dist*cos(biash);
				displacement.y = dist*sin(biash);
				displacement.z = 0.0;
			}
			else if (direction == DIRECTION_TOPRIGHT)
			{
				double biash = hAngle - MYPI / 8;
				displacement.x = dist*cos(biash);
				displacement.y = dist*sin(biash);
				displacement.z = 0.0;
			}
			else if (direction == DIRECTION_BOTTOMLEFT)
			{
				double biash = hAngle - MYPI / 8;
				displacement.x = -dist*cos(biash);
				displacement.y = -dist*sin(biash);
				displacement.z = 0.0;
			}
			else if (direction == DIRECTION_BOTTOMRIGHT)
			{
				double biash = hAngle + MYPI / 8;
				displacement.x = -dist*cos(biash);
				displacement.y = -dist*sin(biash);
				displacement.z = 0.0;
			}
			pos_d.x += displacement.x;
			pos_d.y += displacement.y;
			pos_d.z += displacement.z;
		}
		else if (viewmode == VIEWMODE_OVERLOOK)
		{
			if (direction == DIRECTION_TOP)//ǰ��
			{
				displacement.x = dist*hcos;
				displacement.y = dist*hsin;
				displacement.z = 0.0;
			}
			else if (direction == DIRECTION_DOWN)
			{
				displacement.x = -dist*hcos;
				displacement.y = -dist*hsin;
				displacement.z = 0.0;
			}
			else if (direction == DIRECTION_LEFT)
			{
				displacement.x = -dist*hcos;
				displacement.y = dist*hsin;
				displacement.z = 0.0;
			}
			else if (direction == DIRECTION_RIGHT)
			{
				displacement.x = dist*hcos;
				displacement.y = -dist*hsin;
				displacement.z = 0.0;
			}
			else if (direction == DIRECTION_TOPLEFT)
			{
				double biash = hAngle + MYPI / 8;
				displacement.x = dist*cos(biash);
				displacement.y = dist*sin(biash);
				displacement.z = 0.0;
			}
			else if (direction == DIRECTION_TOPRIGHT)
			{
				double biash = hAngle - MYPI / 8;
				displacement.x = dist*cos(biash);
				displacement.y = dist*sin(biash);
				displacement.z = 0.0;
			}
			else if (direction == DIRECTION_BOTTOMLEFT)
			{
				double biash = hAngle - MYPI / 8;
				displacement.x = -dist*cos(biash);
				displacement.y = -dist*sin(biash);
				displacement.z = 0.0;
			}
			else if (direction == DIRECTION_BOTTOMRIGHT)
			{
				double biash = hAngle + MYPI / 8;
				displacement.x = -dist*cos(biash);
				displacement.y = -dist*sin(biash);
				displacement.z = 0.0;
			}
			viewpos_d.x += displacement.x;
			viewpos_d.y += displacement.y;
			viewpos_d.z += displacement.z;
		}
		else if (viewmode == VIEWMODE_FREE)
		{
			if (direction == DIRECTION_TOP)//ǰ��
			{
				displacement.x = dist*hcos*vcos;
				displacement.y = dist*hsin*vcos;
				displacement.z = -dist*vsin;
			}
			else if (direction == DIRECTION_DOWN)
			{
				displacement.x = -dist*hcos*vcos;
				displacement.y = -dist*hsin*vcos;
				displacement.z = dist*vsin;
			}
			else if (direction == DIRECTION_LEFT)
			{
				displacement.x = -dist*hsin;
				displacement.y = dist*hcos;
				displacement.z = 0.0f;
			}
			else if (direction == DIRECTION_RIGHT)
			{
				displacement.x = dist*hsin;
				displacement.y = -dist*hcos;
				displacement.z = 0.0f;
			}
			else if (direction == DIRECTION_TOPLEFT)
			{
				double biash = hAngle + MYPI / 8;
				displacement.x = dist*cos(biash)*vcos;
				displacement.y = dist*sin(biash)*vcos;
				displacement.z = -dist*vsin / 2;
			}
			else if (direction == DIRECTION_TOPRIGHT)
			{
				double biash = hAngle - MYPI / 8;
				displacement.x = dist*cos(biash)*vcos;
				displacement.y = dist*sin(biash)*vcos;
				displacement.z = -dist*vsin / 2;
			}
			else if (direction == DIRECTION_BOTTOMLEFT)
			{
				double biash = hAngle - MYPI / 8;
				displacement.x = -dist*cos(biash)*vcos;
				displacement.y = -dist*sin(biash)*vcos;
				displacement.z = dist*vsin / 2;
			}
			else if (direction == DIRECTION_BOTTOMRIGHT)
			{
				double biash = hAngle + MYPI / 8;
				displacement.x = -dist*cos(biash)*vcos;
				displacement.y = -dist*sin(biash)*vcos;
				displacement.z = dist*vsin / 2;
			}
			viewpos_d.x += displacement.x;
			viewpos_d.y += displacement.y;
			viewpos_d.z += displacement.z;
		}

		if (viewmode == VIEWMODE_FIRSTPERSON || viewmode == VIEWMODE_CHASE)
		{
			//moved = true; //��γ������ԣ�������
			bindview = true;
		}
		else
			figuremoved = true;
		eyehmoved = true;
		viewchanged = true;
	}
	inline void HandleMove()					//����ǰ�ƶ�
	{
		HandleSpeed();
		Walk();
	}
	inline bool Rotate(POINT bias)				//����ǰ��ת
	{
		if (bias.x == 0 && bias.y == 0)
		{
			return false;
		}
		double oldh = hAngle, oldv = vAngle;
		double hbias = -bias.x * sensitivity;
		hAngle += hbias;
		if (hAngle < 0)
			hAngle += 2 * MYPI;
		if (hAngle >= 2 * MYPI)
			hAngle -= 2 * MYPI;

		vAngle -= bias.y * sensitivity;
		if (vAngle < -MYPI / 2)
			vAngle = -MYPI / 2;
		if (vAngle > MYPI / 2)
			vAngle = MYPI / 2;

		Refreshtri();

		if (viewmode == VIEWMODE_CHASE || viewmode == VIEWMODE_FIRSTPERSON)
		{
			//ͬ������Ƕ�
			figurehangle += -bias.x * sensitivity;
			figurevangle += -bias.y * sensitivity;
			if (figurehangle < 0)
				figurehangle += (float)(2 * MYPI);
			if (figurehangle >= (float)(2 * MYPI))
				figurehangle -= (float)(2 * MYPI);
			
			if (figurevangle < -(float)(MYPI / 2))
				figurevangle = -(float)(MYPI / 2);
			if (figurevangle >(float)(MYPI / 2))
				figurevangle = (float)(MYPI / 2);

			double radius;
			if (viewmode == VIEWMODE_CHASE)
				radius = chasedist;
			else
				radius = sqrt(figureeye.x*figureeye.x + figureeye.y*figureeye.y);

			/*displacement = DoubleVec3(
				radius*(cos(hAngle)-cos(oldh))
				, radius*(sin(vAngle) - cos(oldv))*cos(oldh)
				, 0.0f);*/
		}
		else
		{
			;//�۾�λ��û�ı䣬������������󣬲���������λ��
		}

		
		if (viewmode == VIEWMODE_CHASE || viewmode == VIEWMODE_FIRSTPERSON)
		{
			eyehmoved = true;
			bindview = true;
			figuremoved = true;
		}
		viewchanged = true;//�����ӽǸ��²���

		return true;
	}
	inline void ViewMove()//����viewpos�ͻ�׼��γ�ȼ���ʵʱ��γ�ȣ���SunMove()ǰ����
	{
		//������ʱ���3D�����ӽ�Ҳƫת��ȡ��ע��
		//if (anti)
		//	displacement = -displacement;


		//��γ��
		if (cos(latitude) == 0)
			latitude = latitude
			+ sqrt(displacement.x*displacement.x + displacement.y*displacement.y) / ONEMETER / AVGRADIUS;
		else
		{
			latitude = latitude + displacement.y / ONEMETER / AVGRADIUS;
			longitude = longitude + displacement.x / ONEMETER / AVGRADIUS / cos(latitude);
		}
		

		double unnormalizedlatitude = latitude;

		//�淶����γ��
		NormalizeLongitudeLatitude(longitude, latitude);

		//���µ���Ƕ�
		if (latitude == MYPI / 2)
			earthhangle = 3 * MYPI / 2;
		else if (latitude == -MYPI / 2)
			earthhangle = MYPI / 2;
		else
			earthhangle = hAngle;

		if (unnormalizedlatitude > MYPI / 2 || unnormalizedlatitude < -MYPI / 2)
		{
			//����ת��
			if (longitude > 0)
				longitude -= MYPI;
			else
				longitude += MYPI;

			anti = !anti;


			if (hAngle < MYPI)
				earthhangle = hAngle + MYPI;
			else
				earthhangle = hAngle - MYPI;

			//������ʱ��3D�����ӽ�Ҳƫת
			hAngle = earthhangle;
			Refreshtri();
			bindview = true;
		}


		//if (anti)//Խ�����ϱ������㣬�Ƕȷ�ת
		//{
		//	if (hAngle < MYPI)
		//		earthhangle = hAngle + MYPI;
		//	else
		//		earthhangle = hAngle - MYPI; 
		//	
		//	//viewchanged = true;
		//	//hAngle = earthhangle;//TODO:3D�����ӽǽǶ��Ƿ�Ҳ��ת
		//}
		//else
		//	earthhangle = hAngle;
	}
	void DirectionControl();					//����ʵʱ����״̬����curdirection,������speed������
	bool KeyControlDown(int key);				//���°����Ĵ���
	void KeyControlUp(int key);					//̧�𰴼��Ĵ���
	void Inactivate();							//����ʧȥ�����Ĵ���
	string GetViewmodeStr();					//��õ�ǰviewģʽ���ַ���
	int ChangeViewmode();						//�л�viewģʽ
	inline void SetFigureMat()
	{
		//����ƽ�ƾ���
		D3DXMatrixIdentity(&matTranslation); 
		//D3DXMatrixTranslation(&matTranslation
		//	, (float)(pos_d.x - blockindex.x * 2 * BLOCKRADIUS)
		//	, (float)(pos_d.y - blockindex.y * 2 * BLOCKRADIUS)
		//	, 0.0f);//��ֱ�������ƣ�TODO��ֱ����traslation����double
		//			//����ƽ��ƫת����
		//D3DXMatrixIdentity(&matTranslation2);
		//D3DXMatrixTranslation(&matTranslation2
		//	, (float)(pos_d.x - blockindex.x * 2 * BLOCKRADIUS) + TINYBIAS
		//	, (float)(pos_d.y - blockindex.y * 2 * BLOCKRADIUS)
		//	, 0);
		if (viewmode == VIEWMODE_CHASE|| viewmode == VIEWMODE_FIRSTPERSON)
		{
			D3DXMatrixTranslation(&matTranslation
				, 0.0f
				, 0.0f
				, 0.0f);//��ֱ�������ƣ�TODO��ֱ����traslation����double
						//����ƽ��ƫת����
			D3DXMatrixIdentity(&matTranslation2);
			D3DXMatrixTranslation(&matTranslation2
				, 0.0f + TINYBIAS
				, 0.0f
				, 0);
		}
		else if (viewmode == VIEWMODE_FREE || viewmode == VIEWMODE_OVERLOOK)
		{
			D3DXMatrixTranslation(&matTranslation
				, (float)(pos_d.x - viewpos_d.x)
				, (float)(pos_d.y - viewpos_d.y)
				, 0.0f);//��ֱ�������ƣ�TODO��ֱ����traslation����double
						//����ƽ��ƫת����
			D3DXMatrixIdentity(&matTranslation2);
			D3DXMatrixTranslation(&matTranslation2
				, (float)(pos_d.x - viewpos_d.x) + TINYBIAS
				, (float)(pos_d.y - viewpos_d.y)
				, 0);
		}
		//����ˮƽ��ת����
		D3DXMatrixIdentity(&matHRotate);
		D3DXMatrixRotationZ(&matHRotate, figurehangle);
		//�����������
		D3DXMatrixIdentity(&matWorld);
		D3DXMatrixMultiply(&matWorld, &matTranslation, &matWorld);
		D3DXMatrixMultiply(&matWorld, &matHRotate, &matWorld);
		//������Ӱ����
		matShadowWorld = matShadow*matTranslation;

		figuremoved = false;
	}
	inline void Render()
	{
		if (!device)
			return;

		if (!figure)
			return;

		if (figuremoved)
			SetFigureMat();
		//
		//shadow ����
		//
		if (!sundown)
		{
			if (shadowchanged)
				matShadowWorld = matShadow*matTranslation;//TODO������������
			device->SetTransform(D3DTS_WORLD, &matShadowWorld);

			ShadowBegin();
			figure->DrawSubset(0);
			ShadowEnd();
		}

		//
		//�۲��߻���
		//
		device->SetTransform(D3DTS_WORLD, &matWorld);
		device->SetMaterial(&figuremtrl);
		figure->DrawSubset(0);

		if (flashlight)
		{
			DrawLight();
		}

	}
	inline void DrawLight()
	{
		if (!figurelight)
			return;

		device->SetTransform(D3DTS_WORLD, &matTranslation);

		//�����Դ��ͼ
		device->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
		//device->SetRenderState(D3DRS_SEPARATEALPHABLENDENABLE, TRUE);
		//����1��D3DBLEND_DESTCOLOR & D3DBLEND_SRCALPHA
		//����2��D3DBLEND_SRCALPHA & D3DBLEND_INVSRCALPHA || D3DBLEND_INVDESTCOLOR & D3DBLEND_SRCALPHA
		//����3��D3DBLEND_SRCALPHA & D3DBLEND_SRCALPHA
		device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_DESTCOLOR);
		device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_SRCALPHA);
		device->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);//�ر�ZBUFFER��д�룬����Ӱ��˸
		device->SetRenderState(D3DRS_FOGENABLE, FALSE);//�ر�������Ӱȫ��ɫ
		device->SetRenderState(D3DRS_LIGHTING, FALSE);//�رչ���

		device->SetTexture(0, g_Texture); 
		float db2 = DEPTHBIAS;
		float sdb2 = SLOPESCALEDEPTHBIAS;
		if (depthbiasable)
		{
			device->SetRenderState(D3DRS_DEPTHBIAS, *((DWORD*)&db2));
			device->SetRenderState(D3DRS_SLOPESCALEDEPTHBIAS, *((DWORD*)&sdb2));
		}
		figurelight->DrawSubset(0); 
		device->SetTransform(D3DTS_WORLD, &matTranslation2);
		figurelight->DrawSubset(0);
		device->SetRenderState(D3DRS_SLOPESCALEDEPTHBIAS, 0);
		device->SetRenderState(D3DRS_DEPTHBIAS, 0);
		device->SetTexture(0, 0);

		device->SetRenderState(D3DRS_LIGHTING, TRUE);
		device->SetRenderState(D3DRS_FOGENABLE, TRUE);
		device->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);//�ر�ZBUFFER��д�룬����Ӱ��˸
		device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
		device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
		device->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);

	}
};