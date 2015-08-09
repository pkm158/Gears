#include "StageUtility.h"
#include "UtilityManager.h"

StageUtility::StageUtility( void ) : m_stageType(STAGE_MAIN)
{
	DEBUG_PRINT("Create stage utility...");
	IUtility::m_UtilityType = UTILITY_STAGE;

	m_entityNum = 10;
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
	HR( m_pEntity[4].init(".//Resources//statue//statue.GMA", false, true, false, true) );
	m_pEntity[4].move(0, -4,-12);
	m_pEntity[4].rotateX(-RIGHT_ANGLE);
	m_pEntity[4].rotateY(2*RIGHT_ANGLE);

	HR( m_pEntity[5].init(".//Resources//statue//statue.GMA", false, true, false, true) );
	m_pEntity[5].move(0, -4, 0);
	m_pEntity[5].rotateX(-RIGHT_ANGLE);
	m_pEntity[5].rotateY(2*RIGHT_ANGLE);

	HR( m_pEntity[6].init(".//Resources//statue//statue.GMA", false, true, false, true) );
	m_pEntity[6].move(0, -4, 12);
	m_pEntity[6].rotateX(-RIGHT_ANGLE);
	m_pEntity[6].rotateY(2*RIGHT_ANGLE);

	HR( m_pEntity[7].init(".//Resources//statue//statue.GMA", false, true, false, true) );
	m_pEntity[7].move(0, -4, 24);
	m_pEntity[7].rotateX(-RIGHT_ANGLE);
	m_pEntity[7].rotateY(2*RIGHT_ANGLE);

	HR( m_pEntity[8].init(".//Resources//statue//statue.GMA", false, true, false, true) );

	m_pEntity[8].move(0, -4, 36);
	m_pEntity[8].rotateX(-RIGHT_ANGLE);
	m_pEntity[8].rotateY(2*RIGHT_ANGLE);
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

	/************************************************************************/
	/* Deferred rendering                                                   */
	/************************************************************************/

	HR( m_pEntity[9].init(".//Resources//moon//moon.GMA", false, false, false, true, NULL, "moon_normal.dds") );

	m_pEntity[9].scale(7.2);
	m_pEntity[9].rotateX(-RIGHT_ANGLE);

	/************************************************************************/
	/* Deferred rendering                                                   */
	/************************************************************************/

	m_lightArr = new Light[128];
	for ( int i=0; i<128; i++ )
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

		CVector vPos(0, 0, 8.0f, 2.0f);
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

	m_lightManager.clearLights();
	switch (m_stageType)
	{
	case STAGE_MAIN:
		{
			m_lightManager.setDirLightDirection(1, -1, -0.3);
			SetCamPos(0,20,-50);
			for (int i=0; i<4; i++)
			{
				SubmitRenderEntity(&m_pEntity[i]);
			}
		}break;
	case STAGE_DEPTH_OF_FIELD:
		{
			m_lightManager.setDirLightDirection(0.3, -1, 1);
			SetCamPos(10,0,-50);
			for (int i=4; i<9; i++)
			{
				SubmitRenderEntity(&m_pEntity[i]);
			}
		}break;
	case STAGE_DEFERED_RENDERING:
		{
			m_lightManager.setDirLightDirection(1, -1, -0.3);
			SetCamPos(0,20,-50);
			for ( int i=0; i<128; i++ )
			{
				float spin = (float(rand()%1024)/1023.0f+0.0001) * 0.1;
				CMatrix local_mat; 

				CMatrix rot_mat;
				rot_mat.RotateY_Replace_LH(spin);

				CVector vPos = m_lightArr[i].position;

				CVector tmp =  vPos * rot_mat;// * local_mat;

				m_lightArr[i].position = CVector(tmp.GetX(), tmp.GetY(), tmp.GetZ(), tmp.GetW());

				m_lightManager.pushLights(&(m_lightArr[i]));
			}

			SubmitRenderEntity(&m_pEntity[9]);
		}break;
	default:
		ALERT("Stage illegel");
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
		m_stageType = STAGE_MAIN;
	}

	if (IsKeyPressed(DIK_2))
	{
		m_stageType = STAGE_DEPTH_OF_FIELD;
	}

	if (IsKeyPressed(DIK_3))
	{
		m_stageType = STAGE_DEFERED_RENDERING;
	}

	if (IsKeyPressed(DIK_4))
	{
		TurnFullScreenBloom(true);
	}

	if (IsKeyPressed(DIK_5))
	{
		TurnFullScreenBloom(false);
	}

	if (IsKeyPressed(DIK_6))
	{
		TurnShinessBloom(true);
	}

	if (IsKeyPressed(DIK_7))
	{
		TurnShinessBloom(false);
	}

	if (IsKeyPressed(DIK_8))
	{
		TurnDepthOfField(true);
	}

	if (IsKeyPressed(DIK_9))
	{
		TurnDepthOfField(false);
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