#pragma once

#include "D3Ddefine.h"

#include <d3d9.h>
#include <d3dx9tex.h>
#include <string>

#define COLOR_SKY1			D3DCOLOR_XRGB(160, 180, 255)
#define COLOR_SKY2			D3DCOLOR_XRGB(140, 140, 255)

class Environment{
public:
	LPDIRECT3DDEVICE9 device;
	D3DXMATRIXA16 matworld;
	//D3DXMATRIXA16 matSun;//תΪȫ�ֱ���

	D3DLIGHT9 light0, light0s, light0n;//���⼰���渨������
	D3DMATERIAL9 material0;//unused

	ID3DXMesh* skyback;					//��ձ���
	ID3DXMesh* sun;

	int basetimezone;					//�ṩ�����յ�ʱ��
	double baselongitude, baselatitude;	//��׼��γ�ȣ���0��0�����괦��γ��
	float skyradius;					//��հ뾶
	double hangle, vangle;				//����Ƕ�


	tm lastmidwinter, nextmidwinter;
	time_t tyearstart, tyearend;
	int thisyear;						//���
	unsigned int ydays;					//һ�������
	double yearpassed;					//һ���Ѷȹ��ı���
	double daypassed;					//һ���Ѷȹ��ı���
	double declination_general, declination_precise;	//��γ��
	double yearangle;					//һ���е�������ת���ĽǶ�
	D3DXVECTOR3 yearvector;				//һ���е�����������
	D3DXVECTOR3 backnormal;				//����γ�Ǵ�0��ʱ�̵ķ�����
	D3DXVECTOR3 sunproj;
	D3DXVECTOR3 basenormal, basedirectionEW;	//����longitudeָ��γ�ȵĵص㣬0��ʱ�̷��ߺͶ�����������
	D3DXVECTOR3 groundnormal, grounddirectionEW;//����longitudeָ��γ�ȵĵص㣬ʵʱ���ߺͶ�����������
	double blockangleEW, blockangleSN;			//���涫�����������ϱ�������
	double equatorangle;				//���ƽ���ϣ���ֱ��㷴��ͬһ���ߵĵ㣬��Ե�����ת���ĽǶ�


	float ambientindex;
	float diffuseindex;
	D3DXVECTOR4 sunLight;
	D3DXMATRIX sunTranslation;
public:
	Environment();
	~Environment();

	bool SetDevice(LPDIRECT3DDEVICE9 device);
	bool InitSkyBack();
	void InitMaterialLight(const double &longitude, const double &latitude);
	inline void RefreshLight()//���¹���ָ��
	{
		float speedregion = 0.06f;	//̫������ɽ�󣬶��ٱ��л�������diffuse�ݼ�Ϊ0������ambient�ݼ�Ϊstage1end��������
		float stage1end = 0.10f;	//̫������ɽ��speedregion���л��Ⱥ�ambientindex�ݼ�����ı�����
		float stage2end = 0.06f;

		if (vangle <= MYPI)
		{
			diffuseindex = ambientindex = (float)(0.01 + abs(cos(vangle - MYPI / 2) + 1) / 2);
			diffuseindex = ambientindex = 1;
		}
		else if (vangle < (1 + speedregion)*MYPI)//̫����ɽ���һ���ݼ�ʱ��
		{
			diffuseindex = (float)(((1 + speedregion)*MYPI - vangle) / speedregion / MYPI);
			ambientindex = (float)(stage1end + ((1 + speedregion)*MYPI - vangle)
				/ speedregion / MYPI*(1 - stage1end));
		}
		else if (vangle > (2 - speedregion)*MYPI && vangle < 2 * MYPI)//̫��������ǰ���һ������ʱ��
		{
			diffuseindex = (float)((vangle - (2 - speedregion)*MYPI) / speedregion / MYPI);
			ambientindex = (float)(stage1end + (vangle - (2 - speedregion)*MYPI)
				/ speedregion / MYPI*(1 - stage1end));
		}
		else//̫�����ṩ������
		{
			diffuseindex = 0;
			ambientindex = (float)(stage2end + (stage1end - stage2end)*abs(1.5*MYPI - vangle)
					/ (0.5 - speedregion) / MYPI);
		}
	}
	/*
	���㣺�����м��죬���˼��죬�������ߣ���γ��longitude��0�����Ļ�׼���ߺͶ�������
	*/
	inline void SunMoveDay(const double &latitude)
	{
		//����һ��
		thisyear = realdate.tm_year + 1900;

		//һ���м���
		ydays = getDaysinYear(thisyear);

		//һ��ȹ��ı���������ڶ��������Ǿ�ȷ�㷨��Ϊ12/21����ȷ�㷨���ڡ���һ��ͱ��궬���ľ�ȷʱ�䡿
#define TWOMIDWINTER_YEARPASSED
#ifndef TWOMIDWINTER_YEARPASSED
		if (nowdate.tm_mon == 11 && nowdate.tm_mday >= 21)
		yearpassed = nowdate.tm_mday - 21;
		else//TODO�����ж��Ƿ��ѹ�����
		yearpassed = nowdate.tm_yday + 11;
		//����һ���Ѷȹ��ı����������ǵ���תͬʱҲ�ڹ�ת�����»�׼��
		yearpassed += daypassed;
#else
		//TODO ���Ͼ�׼������ת�Ƶ�ͬһ���㣬��������Ƿ�仯����
		lastmidwinter.tm_mon = 11;
		lastmidwinter.tm_mday = 22;
		lastmidwinter.tm_hour = 0;
		lastmidwinter.tm_min = 27;
		lastmidwinter.tm_sec = 53;
		lastmidwinter.tm_yday = 0;
		lastmidwinter.tm_isdst = 0;
		lastmidwinter.tm_year = thisyear - 1900 - 1;//2015  12-22 12:47:55
		nextmidwinter.tm_mon = 11;
		nextmidwinter.tm_mday = 22;
		nextmidwinter.tm_hour = 6;
		nextmidwinter.tm_min = 22;
		nextmidwinter.tm_sec = 38;
		nextmidwinter.tm_yday = 0;
		nextmidwinter.tm_isdst = 0;
		nextmidwinter.tm_year = thisyear - 1900;//2016  12-21 18:44:07
		if (basedate.tm_mon == nextmidwinter.tm_mon && basedate.tm_mday >= nextmidwinter.tm_mday)
		{
			lastmidwinter.tm_year++;
			nextmidwinter.tm_year++;
		}
		else
		{
			;
		}
		tyearstart = mktime(&lastmidwinter);
		tyearend = mktime(&nextmidwinter);
		//�õ���������ʱ��������
		nowtime += SECS_PER_HOUR*(basetimezone - devicetimezone);
		yearpassed = (nowtime - tyearstart) / (double)(tyearend - tyearstart)*ydays;
#endif
		//daypassed = getDaypassed(realdate);//TODO:��ȷֵ��ÿ������ʱ�������

		time_t secReal= mktime(&realdate);
		double baseDaypassed = lastmidwinter.tm_hour / 24.0 + lastmidwinter.tm_min / 24.0 / 60.0
			+ lastmidwinter.tm_sec / 24.0 / 3600.0;//�������Ѷȹ�һ��ı���
		daypassed = baseDaypassed + (double)(secReal - tyearstart) / SECS_PER_SOLARDAY
			- (double)SECS_PER_HOUR*(basetimezone - devicetimezone) / SECS_PER_SOLARDAY;//-�����ֲ�ʱ����
		daypassed = daypassed - (time_t)daypassed;

		//�������߽Ƕ�
		yearangle = yearpassed / ydays * 2 * MYPI;

		//������������
		yearvector = { (float)cos(yearangle), (float)sin(yearangle), 0.0f };

		//��γ��
		declination_general = -cos(yearangle)*ECLIPTICOBLIQUITY_R;
		if (cos(yearangle) == 0)
		{
			if (yearangle < MYPI)
				declination_precise = MYPI / 2;
			else
				declination_precise = 3 * MYPI / 2;
			equatorangle = declination_precise;
		}
		else
		{
			equatorangle = -atan(tan(yearangle) / cos(ECLIPTICOBLIQUITY_R));
			double deccos = sin(yearangle)*sin(equatorangle) - cos(ECLIPTICOBLIQUITY_R)*cos(yearangle)*cos(equatorangle);
			declination_precise = acos(deccos);
		}
		if(cos(declination_precise)<0)
			declination_precise -= MYPI;

		//����γ�Ǵ�0��ص�ķ��������������յ����ߣ������潻�ڸ���γ����
		backnormal = yearvector;

#define PRECISEDIRECTIONEW
#ifndef PRECISEDIRECTIONEW

		double directionyearangle;			//���ڼ��㡮����γ�Ǵ�0��ص�Ķ�������������z�����ĸ����Ƕ�
		directionyearangle = yearpassed - ydays / 4.0;
		if (directionyearangle < 0)
			directionyearangle += ydays;
		//����γ�Ǵ�0��ص�Ķ�������������(����longitude��������������)
		double decd = -cos(directionyearangle / ydays * 2 * MYPI)*ECLIPTICOBLIQUITY_R;
		basedirectionEW = { (float)(sin(yearangle)*cos(decd)), (float)-(cos(yearangle)*cos(decd))
			, (float)sin(decd) };
#else
		if (yearangle > MYPI / 2 && yearangle < 3 * MYPI / 2)
			equatorangle = MYPI - equatorangle;
		else if (yearangle < MYPI / 2)
			equatorangle = -equatorangle;//TODOȡ���ţ������ȡ���ţ���ת����
		else
			equatorangle = 2 * MYPI - equatorangle;
		basedirectionEW = AxisRotate(earthaxis, D3DXVECTOR3(0.0f, -1.0f, 0.0f), equatorangle);
#endif // !PRECISEDIRECTIONEW

		//longitude��0��ص�ķ�����
		basenormal = AxisRotate(basedirectionEW, backnormal, latitude + declination_precise);
	}

	/*
	���㣺longitude������ʵʱ���ߺͶ�������������ȷ��̫��λ�õ�ˮƽ����ֱ��λ��
	*/
	inline void SunMove(const double &latitude)
	{
		SunMoveDay(latitude);//�޼��������¼����յ�λ�ò���

		//longitudeָ��γ�ȵĵص㣬ʵʱ���ߺͶ�����������
		groundnormal = AxisRotate(earthaxis, basenormal, daypassed * 2 * MYPI);
		grounddirectionEW = AxisRotate(earthaxis, basedirectionEW, daypassed * 2 * MYPI);
		//ʵʱ�ϱ���������
		D3DXVECTOR3 grounddirectionSN;
		D3DXVec3Cross(&grounddirectionSN, &grounddirectionEW, &groundnormal);
		//̫�������ڵ�����ͶӰ
		sunproj = -VecProj(yearvector, groundnormal);

		//����̫��ʵʱ��λ��
		if (D3DXVec3Length(&sunproj) == 0.0)//����ֱ��TODO
		{
			hangle = 0.0f;
			vangle = MYPI / 2;
		}
		else
		{
			//��λ��
			D3DXVec3Normalize(&sunproj, &sunproj);
			D3DXVec3Normalize(&grounddirectionSN, &grounddirectionSN);
			D3DXVec3Normalize(&grounddirectionEW, &grounddirectionEW);

			blockangleEW = acos(
				min((sunproj.x*grounddirectionEW.x + sunproj.y*grounddirectionEW.y + sunproj.z*grounddirectionEW.z), 1.0)
			);
			/*blockangleSN = acos(
				D3DXVec3Dot(&sunproj, &grounddirectionSN)
				/ D3DXVec3Length(&sunproj) 
				/ D3DXVec3Length(&grounddirectionSN)
			); */
			blockangleSN = acos(
				min((sunproj.x*grounddirectionSN.x + sunproj.y*grounddirectionSN.y + sunproj.z*grounddirectionSN.z), 1.0)
			);
			//ʵʱˮƽ̫����λ��
			if (blockangleEW > MYPI / 2 && blockangleSN > MYPI / 2)
				hangle = blockangleEW + MYPI;
			else if (blockangleEW > MYPI / 2)
				hangle = MYPI / 2 - blockangleSN;
			else if (blockangleSN > MYPI / 2)
				hangle = blockangleSN + MYPI / 2;
			else
				hangle = blockangleSN + MYPI / 2;
			//ʵʱ��ֱ̫����λ��
			D3DXVECTOR3 antiyearvector = -yearvector;
			vangle = asin(D3DXVec3Dot(&groundnormal, &antiyearvector) / D3DXVec3Length(&groundnormal) / D3DXVec3Length(&yearvector));
			if (vangle < 0)
			{
				if (blockangleEW <= MYPI / 2)
					vangle = MYPI - vangle;
				else
					vangle = 2 * MYPI + vangle;
			}
			else
			{
				if (blockangleEW <= MYPI / 2)
					vangle = MYPI - vangle;
				else
					;
			}
		}

		if (anti)//̫��ת��
		{
			//������ʱ���3D�����ӽ�Ҳƫת��ȡ��ע��
			/*if (hangle < MYPI)
				hangle += MYPI;
			else
				hangle -= MYPI;*/
		}


		//��Դλ�ø���
		RefreshLight();//���¹���ָ��
	}
	inline void setPos()
	{
		double sunh = 0; 	//̫����ˮƽ����
		if (vangle > MYPI && vangle < 2 * MYPI)//̫���ڵ�ƽ����
		{
			sundown = true;//̫������ɽ

			sunh = SUNDIST;//����̫�������Դ����Զ�ˣ���Ӱ��̫��ģ�ͷ�λ

			if (light0.Type == D3DLIGHT_DIRECTIONAL)
				light0.Direction.z = 0;
			else
				light0.Position.z = 0;//�߶ȿ����ڵ�ƽ��
									  /*light0.Diffuse.r = sunlightD.r*abs(cos(vangle));
									  light0.Diffuse.g = sunlightD.g*abs(cos(vangle));
									  light0.Diffuse.b = sunlightD.b*abs(cos(vangle)); */
		}
		else
		{
			sundown = false;

			sunh = SUNDIST*abs(cos(vangle));

			if (light0.Type == D3DLIGHT_DIRECTIONAL)
				light0.Direction.z = (float)(SUNDIST*sin(vangle));
			else
				light0.Position.z = (float)(-SUNDIST*sin(vangle));
			//light0.Diffuse = sunlightD;
		}

		if (light0.Type == D3DLIGHT_DIRECTIONAL)
		{
			light0.Direction.x = -(float)(sunh*cos(hangle));
			light0.Direction.y = -(float)(sunh*sin(hangle));

			light0.Diffuse.r = sunlightD_D.r*diffuseindex;
			light0.Diffuse.g = sunlightD_D.g*diffuseindex;
			light0.Diffuse.b = sunlightD_D.b*diffuseindex;
			light0.Ambient.r = sunlightA.r*ambientindex;
			light0.Ambient.g = sunlightA.g*ambientindex;
			light0.Ambient.b = sunlightA.b*ambientindex;
		}
		else
		{
			light0.Position.x = (float)(sunh*cos(hangle));//̫���ⲻ��Ҫ - viewpos_d.x TODO:������Դ
			light0.Position.y = (float)(sunh*sin(hangle));//̫���ⲻ��Ҫ - viewpos_d.y

			light0.Diffuse.r = sunlightD.r*diffuseindex;
			light0.Diffuse.g = sunlightD.g*diffuseindex;
			light0.Diffuse.b = sunlightD.b*diffuseindex;
			light0.Ambient.r = sunlightA.r*ambientindex;
			light0.Ambient.g = sunlightA.g*ambientindex;
			light0.Ambient.b = sunlightA.b*ambientindex;
		}


		//���渨���������SIDESUNANGLE
		light0s.Diffuse = light0.Diffuse;
		light0n.Diffuse = light0.Diffuse;
		if (light0.Type == D3DLIGHT_DIRECTIONAL)
		{
			light0s.Direction.x = -(float)(sunh*cos(hangle + SIDESUNANGLE));
			light0s.Direction.y = -(float)(sunh*sin(hangle + SIDESUNANGLE));
			light0s.Direction.z = light0.Direction.z + SIDESUN_UP;
			light0n.Direction.x = -(float)(sunh*cos(hangle - SIDESUNANGLE));
			light0n.Direction.y = -(float)(sunh*sin(hangle - SIDESUNANGLE));
			light0n.Direction.z = light0s.Direction.z;
		}
		else
		{
			light0s.Position.x = (float)(sunh*cos(hangle + SIDESUNANGLE));
			light0s.Position.y = (float)(sunh*sin(hangle + SIDESUNANGLE));
			light0s.Position.z = light0.Position.z - SIDESUN_UP;
			light0n.Position.x = (float)(sunh*cos(hangle - SIDESUNANGLE));
			light0n.Position.y = (float)(sunh*sin(hangle - SIDESUNANGLE));
			light0n.Position.z = light0s.Position.z;
		}

		//�����Դ TODO :�Ƿ���Ҫ
		device->SetLight(0, &light0);
		device->SetLight(1, &light0s);
		device->SetLight(2, &light0n);
		device->LightEnable(0, TRUE);
		device->LightEnable(1, TRUE);
		device->LightEnable(2, TRUE);

		//��Ӱǿ�ȣ�͸���ȣ�
		shadowmtrl.Diffuse.a = (float)(0.05 + MAXSHADOWINTENSITY*abs(sin(vangle)));

		//̫������
		sunLight = D3DXVECTOR4(
			(float)(SUNDIST*abs(cos(vangle))*cos(hangle))
			, (float)(SUNDIST*abs(cos(vangle))*sin(hangle))
			, (float)(-SUNDIST*sin(vangle))
			, 1);//����ȫ��̫����Դλ�ã����ڻ���Ӱ����ʵ�ʷ���Ĺ�Դ��ͬ��
		D3DXMatrixIdentity(&sunTranslation);
		D3DXMatrixTranslation(&sunTranslation, sunLight.x, sunLight.y, sunLight.z);
		//��Ӱ����
		D3DXMatrixShadow(
			&matShadow,
			&sunLight,
			&groundPlane);//������Ӱ����
		shadowchanged = true;//��Ӱ�ı��־
	}
	//���õ�ǰλ����ʵʱ����ṩ������ʱ����ʱ��
	inline void SetTime(const double &longitude)
	{
		realtime = nowtime + (time_t)(SECS_PER_SOLARDAY*longitude / 2 / MYPI);
		gmtime_s(&realdate, &realtime);//��ʵʱ��

		basetime = nowtime + SECS_PER_HOUR*basetimezone;
		gmtime_s(&basedate, &basetime);//�ṩ������ʱ����ʱ��
	}
	
	inline bool Render()
	{
		if (!device)
			return false;

		if (!skyback)
			return false;

		//�����
		//device->SetTransform(D3DTS_WORLD, inblockTranslation);
		device->SetTransform(D3DTS_WORLD, &matworld);//��γ������ԣ���ʹ����������
		device->SetRenderState(D3DRS_LIGHTING, FALSE);//��ʱ�رչ���
		device->SetRenderState(D3DRS_FOGENABLE, FALSE);//��ʱ�ر���

		//device->SetMaterial(&material0);
		skyback->DrawSubset(0);

		if (!sun)
			return false;
		//��̫��
		//device->SetTransform(D3DTS_WORLD, &matSun);
		device->SetTransform(D3DTS_WORLD, &sunTranslation);//��γ������ԣ���ʹ����������
		sun->DrawSubset(0);

		device->SetRenderState(D3DRS_LIGHTING, TRUE);
		device->SetRenderState(D3DRS_FOGENABLE, TRUE);

		return true;
	}
};