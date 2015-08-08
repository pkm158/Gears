#include "StageUtility.h"
#include "UtilityManager.h"

StageUtility::StageUtility( void )
{
	DEBUG_PRINT("Create stage utility...");
	IUtility::m_UtilityType = UTILITY_STAGE;

	m_entityNum = 4;
	m_pEntity = new RenderEntity_opengl [m_entityNum];

}

StageUtility::~StageUtility( void )
{
	for (int idx=0; idx<m_entityNum; idx++)
	{
		DeSubmitRenderEntity(&m_pEntity[idx]);
	}
	
	SAFE_DELETE_ARRAY(m_pEntity);
	SAFE_DELETE_ARRAY(m_lightArr);
}

void StageUtility::postInitiate( void )
{
	DEBUG_PRINT("Initiate stage utility...");
 
 	HR( m_pEntity[0].init(".//Resources//sun//sun.GMA", false, false, true, false) );
 
 	m_pEntity[0].move(-12, 6, 3);
 	m_pEntity[0].scale(1.1);
 	m_pEntity[0].rotateX(-RIGHT_ANGLE);
 	SubmitRenderEntity(&m_pEntity[0]);
 
 	HR( m_pEntity[1].init(".//Resources//earth//earth.GMA", true, true, false, true, "Spec.dds", "nasa_earth_normalmap_2k.dds","Night_Lights2.dds", 2) );
 
 	m_pEntity[1].move(0,0, 0);
 	m_pEntity[1].scale(5);
 	m_pEntity[1].rotateX(-RIGHT_ANGLE);
 	SubmitRenderEntity(&m_pEntity[1]);
 
 	HR( m_pEntity[3].init(".//Resources//cobble//cobble.GMA", false, true, false, true) );
 
 	m_pEntity[3].move(0, -4, 0);
 	m_pEntity[3].scale(3);
 	m_pEntity[3].rotateX(RIGHT_ANGLE);
 	SubmitRenderEntity(&m_pEntity[3]);
 
 	HR( m_pEntity[2].init(".//Resources//moon//moon.GMA", false, false, false, true, NULL, "moon_normal.dds") );
 
 	m_pEntity[2].move(7, 1.5, 12);
 	m_pEntity[2].scale(1.2);
 	m_pEntity[2].rotateX(-RIGHT_ANGLE);
 	SubmitRenderEntity(&m_pEntity[2]);

	/************************************************************************/
	/* depth of field                                                       */
	/************************************************************************/
	/*HR( m_pEntity[0].init(".//Resources//statue//statue.GMA", false, true, false, true) );

	m_pEntity[0].move(0, 0,-12);

	m_pEntity[0].rotateX(-RIGHT_ANGLE);
	SubmitRenderEntity(&m_pEntity[0]);

	HR( m_pEntity[1].init(".//Resources//statue//statue.GMA", false, true, false, true) );

	m_pEntity[1].move(0, 0, 0);
	m_pEntity[1].rotateX(-RIGHT_ANGLE);
	SubmitRenderEntity(&m_pEntity[1]);

	HR( m_pEntity[2].init(".//Resources//statue//statue.GMA", false, true, false, true) );
	m_pEntity[2].move(0, 0, 12);
	m_pEntity[2].rotateX(-RIGHT_ANGLE);
	SubmitRenderEntity(&m_pEntity[2]);

	HR( m_pEntity[3].init(".//Resources//statue//statue.GMA", false, true, false, true) );

	m_pEntity[3].move(0, 0, 24);
	m_pEntity[3].rotateX(-RIGHT_ANGLE);
	SubmitRenderEntity(&m_pEntity[3]);

	HR( m_pEntity[4].init(".//Resources//statue//statue.GMA", false, true, false, true) );

	m_pEntity[4].move(0, 0, 36);
	m_pEntity[4].rotateX(-RIGHT_ANGLE);
	SubmitRenderEntity(&m_pEntity[4]);*/
	/************************************************************************/
	/* depth of field                                                       */
	/************************************************************************/

	/************************************************************************/
	/* shadow                                                               */
	/************************************************************************/
	/*HR( m_pEntity[0].init(".//Resources//cobble//cobble.GMA", false, true, false, true) );

	m_pEntity[0].move(0, 0, 0);
	m_pEntity[0].scale(3);
	m_pEntity[0].rotateX(RIGHT_ANGLE);
	SubmitRenderEntity(&m_pEntity[0]);

	HR( m_pEntity[1].init(".//Resources//statue//statue.GMA", true, true, false, true) );

	m_pEntity[1].move(0, 0, 0);
	m_pEntity[1].scale(1);
	m_pEntity[1].rotateX(-RIGHT_ANGLE);
	SubmitRenderEntity(&m_pEntity[1]);*/
	/************************************************************************/
	/* shadow                                                               */
	/************************************************************************/

	m_lightArr = new Light[0];
	for ( int i=0; i<0; i++ )
	{
		float rx = float(rand()%1024)/1023.0f * PI;
		float ry = float(rand()%1024)/1023.0f * PI * 2;
		float rz = float(rand()%1024)/1023.0f * PI;

		Matrix4x4 local_mat; 

		local_mat.RotateX_Replace_LH(rx);
		local_mat.RotateY_LH(ry);
		local_mat.RotateZ_LH(rz);

//		Matrix4x4 rot_mat;
//		rot_mat.RotateY_Replace_LH(spin);

	//	float x = float(rand()%20) - 10;
	//	float z = float(rand()%20) - 10;

		CVector vPos(0, 0, 6, 2.0f);
		vPos =  vPos * local_mat;
		m_lightArr[i].position = vPos;

		m_lightArr[i].ambient.Set(0.0f);

		float r = float(rand()%256);
		float g = float(rand()%256);
		float b = float(rand()%256);

		m_lightArr[i].diffuse[0] = r/256;
		m_lightArr[i].diffuse[1] = g/256;
		m_lightArr[i].diffuse[2] = b/256;
		m_lightArr[i].diffuse[3] = 1.f;

		m_lightManager.pushLights(&(m_lightArr[i]));
	}

	Light light;
	light.lightType = LIGHT_DIRECTIONAL;
	light.ambient = CVector(0.0, 0.0, 0.0);
	light.diffuse = CVector(1.7, 1.7, 1.7);
	light.specular = CVector(0.5, 0.5, 0.5);
	light.direction = CVector(1, -1, -0.3);
	//light.direction = CVector(1, -1, -0.3);
	m_lightManager.setDirLight(light);
}

float x = 1;			// holy fucking shit
void StageUtility::update( double tt, double dt )
{

	/*for ( int i=0; i<1; i++ )
	{
		float rx = float(rand()%1024)/1023.0f * PI;
		float ry = float(rand()%1024)/1023.0f * PI * 2;
		float rz = float(rand()%1024)/1023.0f * PI;
		float spin = (float(rand()%1024)/1023.0f+0.0001) * 0;
		Matrix4x4 local_mat; 

		local_mat.RotateX_Replace_LH(rx);
		local_mat.RotateY_LH(ry);
		local_mat.RotateZ_LH(rz);

		Matrix4x4 rot_mat;
		rot_mat.RotateY_Replace_LH(spin);

		Vector4 vPos(0.0f, 0.0f, 6.0, 2.0);

		Vector4 tmp =  vPos * rot_mat * local_mat;

		m_lightArr[i].position = CVector(tmp.GetX(), tmp.GetY(), tmp.GetZ(), tmp.GetW());

	}*/

	for (int i=0; i<m_entityNum; i++)
	{
		SubmitRenderEntity(&m_pEntity[i]);
	}

	if (IsKeyPressed(DIK_I))
	{
		for (int idx=0; idx<m_entityNum-1; idx++)
		{
			m_pEntity[idx].rotateX(m_pEntity[idx].getRotate().x - 0.1);
		}
		
	}

	if (IsKeyPressed(DIK_K))
	{
		for (int idx=0; idx<m_entityNum-1; idx++)
		{
			m_pEntity[idx].rotateX(m_pEntity[idx].getRotate().x + 0.1);
		}
	}


 	if (IsKeyPressed(DIK_J) )
 	{
		for (int idx=0; idx<m_entityNum; idx++)
		{
 			m_pEntity[idx].rotateY(m_pEntity[idx].getRotate().y - 0.1);
		}
 	}
 
 	if (IsKeyPressed(DIK_L))
 	{
		for (int idx=0; idx<m_entityNum; idx++)
		{
 			m_pEntity[idx].rotateY(m_pEntity[idx].getRotate().y + 0.1);
		}
 	}

	if (IsKeyPressed(DIK_Z))
	{
		x+=0.1;
		m_pEntity[0].scale(x);
	}

	if ( IsKeyPressed(DIK_X))
	{
		x-=0.1;
		m_pEntity[0].scale(x);
	}

	CVector b = CVector(1,0,0);
	if (GetKeyState(KLEFT))
	{
		for (int idx=0; idx<m_entityNum; idx++)
		{
			CVector a = m_pEntity[idx].getPos();
			m_pEntity[idx].move(a-b);
		}

		
	}

	if (GetKeyState(KRIGHT))
	{
		for (int idx=0; idx<m_entityNum; idx++)
		{
			CVector a = m_pEntity[idx].getPos();
			m_pEntity[idx].move(a+b);
		}
		
	}

	if (IsKeyPressed(DIK_1))
	{
		turnShinessBloom(false);
	}

	if (IsKeyPressed(DIK_2))
	{
		turnShinessBloom(true);
	}

	b = CVector(0,0,1);
	if (GetKeyState(KUP))
	{
		for (int idx=0; idx<m_entityNum; idx++)
		{
			CVector a = m_pEntity[idx].getPos();
			m_pEntity[idx].move(a+b);
		}
	}

	if (GetKeyState(KDOWN))
	{
		for (int idx=0; idx<m_entityNum; idx++)
		{
			CVector a = m_pEntity[idx].getPos();
			m_pEntity[idx].move(a-b);
		}
	}

	if (IsKeyPressed(DIK_Q) )
	{
		AddCamRotationAngle(-0.01);
	}

	if (IsKeyPressed(DIK_E))
	{
		AddCamRotationAngle(0.01);
	}

}