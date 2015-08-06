/************************************************************************/
/* Author:  Kemi Peng 
History: 08/03/2015 by Kemi Peng*/
/************************************************************************/
#include "CameraUtility.h"
#include "UtilityManager.h"
CameraUtility::CameraUtility( void ) : m_pos(0,20,-50.1),
									m_postg(0,20,-50.1),
									m_at(0, 0, 0),
									m_fov(DEFAULT_CAM_FOV),
									m_aspect(DEFAULT_CAM_ASPECT),
									m_zoom(DEFAULT_CAM_ZOOM),
									m_zoomtg(1.0f),
									m_fovzmtg(1.0f),
									m_fovzm(DEFAULT_CAM_FOV_ZOOM),
									m_zNear(DEFAULT_CAM_ZNEAR),
									m_zFar(DEFAULT_CAM_ZFAR),
									m_eq_time(0.0f),
									m_eq_h(0.0f),
									m_eq_w(0.0f),
									m_up(0, 1, 0),
									m_rotAngle(0.0f),
									m_elevAngle(0.0f),
									m_posSpd(1.0f),
									m_zoomSpd(0.05f),
									m_fovZmSpd(0.05f),
									m_isMoveToPosTg(false),
									m_isMoveToZoomTg(false),
									m_isMoveToFovzmTg(false)


{
	DEBUG_PRINT("Create a Camera...");
	IUtility::m_UtilityType = UTILITY_CAMERA;
	m_pmat.Identity();
	m_vmat.Identity();
}

CameraUtility::~CameraUtility( void )
{

}

void CameraUtility::postInitiate( void )
{
	DEBUG_PRINT("Initiate a Camera...");
	int width, height;
	GetWindowWH(width, height);
	m_aspect = width/(float)height;

	MatrixPerspectiveFovLH(&m_pmat,m_fov * m_fovzm,m_aspect,m_zNear,m_zFar);
	m_vmat = ViewMatrix(m_pos,m_at,m_up,0.0f);


#ifdef OPENGL_CORE
	glMatrixMode(GL_PROJECTION);
	CMatrix project_mat =GetProjectionMatrix();
	glLoadMatrixf( (float *) &project_mat);
#endif

}

void CameraUtility::update( double tt, double dt )
{
	assert(m_zoomSpd>0.0f);
	assert(m_posSpd>0.0f);
	assert(m_fovZmSpd>0.0f);

	if (m_isMoveToPosTg)
	{
		CVector posDistance = m_postg - m_pos;
		if (posDistance.Length() > m_posSpd*dt)
		{
			posDistance *= (m_posSpd*dt)/posDistance.Length();

			m_pos += posDistance;
		}else
		{
			m_pos = m_postg;
			m_isMoveToPosTg = false;
		}
	}
	
	calAngles(m_rotAngle, m_elevAngle);

	if (m_isMoveToZoomTg)
	{
		if (abs(m_zoom - m_zoomtg) > m_zoomSpd*dt)
		{
			if (m_zoom - m_zoomSpd > 0)
			{
				m_zoom -= m_zoomSpd*dt;
			} else
			{
				m_zoom += m_zoomSpd*dt;
			}
		}else
		{
			m_zoom = m_zoomtg;
			m_isMoveToZoomTg = false;
		}
	}

	if (m_isMoveToFovzmTg)
	{
		if (abs(m_fovzm - m_fovzmtg) > m_fovZmSpd*dt)
		{
			if (m_fovzm - m_fovzmtg > 0)
			{
				m_fovzm -= m_fovZmSpd*dt;
			}else
			{
				m_fovzm += m_fovZmSpd*dt;
			}

		}else
		{
			m_fovzm = m_fovzmtg;
			m_isMoveToFovzmTg = false;
		}
	}
	
	

	CVector calPos = m_pos - m_at;
	calPos *= m_zoom;
	calPos += m_at;

	m_vmat = ViewMatrix(calPos,m_at,m_up,0.0f);
	MatrixPerspectiveFovLH(&m_pmat,m_fov * m_fovzm,m_aspect,m_zNear,m_zFar);
}

void CameraUtility::resetCamera()
{
	int width, height;
	GetWindowWH(width, height);

	m_pos.Set(0, 0, 1, 0);
	m_postg.Set(0, 0, 0, 0);
	m_at.Set(0, 0, 0, 0);
//	m_up.Set(0, 1, 0, 0);
	m_fov		= DEFAULT_CAM_FOV;
	m_aspect	= width/(float)height;
	m_zoom		= DEFAULT_CAM_ZOOM;
	m_zoomtg	= 1.0f;
	m_fovzmtg	= 1.0f;
	m_fovzm		= DEFAULT_CAM_FOV_ZOOM;
	m_zNear		= DEFAULT_CAM_ZNEAR;
	m_zFar		= DEFAULT_CAM_ZFAR;
	m_eq_time	= 0.0f;
	m_eq_h		= 0.0f;
	m_eq_w		= 0.0f;
	m_rotAngle	= 0.0f;
	m_elevAngle = 0.0f;

	m_posSpd	= 1.0f;
	m_zoomSpd	= 0.05f;
	m_fovZmSpd	= 0.05f;

	MatrixPerspectiveFovLH(&m_pmat,m_fov * m_fovzm,m_aspect,m_zNear,m_zFar);
	m_vmat = ViewMatrix(m_pos,m_at,m_up,0.0f);
}

void CameraUtility::setEarthQuake( float updown , float side , int time )
{
	m_eq_h		= updown;
	m_eq_w		= side;
	m_eq_time	=time;
}

void CameraUtility::setAt( float x,float y,float z )
{
	m_at.Set(x, y, z);
	calAngles(m_rotAngle, m_elevAngle);
}

void CameraUtility::setPos( float x,float y,float z )
{
	m_pos.Set(x, y, z);
	calAngles(m_rotAngle, m_elevAngle);
}

void CameraUtility::setAddPos( const CVector &posPlus )
{
	m_pos += posPlus;
	calAngles(m_rotAngle, m_elevAngle);
}

void CameraUtility::setAddAt( const CVector &atplus )
{
	m_at += atplus;
	calAngles(m_rotAngle, m_elevAngle);
}

void CameraUtility::setRotationAngle( float angle )
{
	m_rotAngle = angle;
	normalizeAngles();
	calLookAt(m_at);
}

void CameraUtility::setElevationAngle( float elevation )
{
	m_elevAngle += elevation;
	normalizeAngles();
	calLookAt(m_at);
}


void CameraUtility::addRotationAngle( float addangle )
{
	m_rotAngle += addangle;
	normalizeAngles();
	calLookAt(m_at);
}

void CameraUtility::addElevationAngle( float addelevation )
{
	m_elevAngle += addelevation;
	normalizeAngles();
	calLookAt(m_at);
}

void CameraUtility::calAngles(float &rotAngle, float &elevAngle)
{
	CVector pos2Zero = m_at - m_pos;	
	if (abs(pos2Zero.LengthXZ()) <= EPSILON)
	{
		if (pos2Zero.y > 0)
		{
			elevAngle = PI/2;
		} else if (pos2Zero.y < 0)
		{
			elevAngle = 3*PI/2;
		} else
		{
			ALERT("摄像机位置和坐标重合了！");
		}

	} else
	{
		elevAngle = atanf(pos2Zero.y/pos2Zero.LengthXZ());	
	}

	if (abs(pos2Zero.z) <= EPSILON)
	{
		if (pos2Zero.x > 0)
		{
			rotAngle = PI/2;
		} else if (pos2Zero.x < 0)
		{
			rotAngle = 3*PI/2;
		} else
		{
			rotAngle = 0.0f;
			ALERT("Error, camera up vector is same as directional vector！");
		}

	} else
	{
		if (pos2Zero.x > 0)
		{
			if (pos2Zero.z > 0)
			{
				rotAngle = atanf(pos2Zero.x/pos2Zero.z);		
			} else
			{
				rotAngle = PI + atanf(pos2Zero.x/pos2Zero.z);	
			}

		} else if (pos2Zero.x < 0)
		{
			rotAngle = atanf(pos2Zero.x/pos2Zero.z);

			if (pos2Zero.z < 0)
			{
				rotAngle = PI + atanf(pos2Zero.x/pos2Zero.z);	
			} else
			{
				rotAngle = 2*PI + atanf(pos2Zero.x/pos2Zero.z);	
			}

		} else
		{
			rotAngle = 0.0f;
		}

	}

}

void CameraUtility::calLookAt(CVector &at)
{
	CVector pos2Zero = m_at - m_pos;	

	float preRotAngle = 0.0f;
	float preElevAngle= 0.0f;

	calAngles(preRotAngle, preElevAngle);
	pos2Zero.RotateY(m_rotAngle - preRotAngle); 

	float distance = pos2Zero.Length();
	pos2Zero.y = sinf(m_elevAngle) * distance;

	float distanceXZ = cosf(m_elevAngle) * distance;

	pos2Zero.z = cosf(m_rotAngle) * distanceXZ;
	pos2Zero.x = sinf(m_rotAngle) * distanceXZ;

	at = pos2Zero + m_pos;

}

void CameraUtility::normalizeAngles( void )
{
	while (m_rotAngle < 0)
	{
		m_rotAngle += 2*PI;
	}
	while (m_rotAngle > 2*PI)
	{
		m_rotAngle -= 2*PI;
	}

	if (m_elevAngle < -PI/2)	
	{
		m_elevAngle = -PI/2;
	} else if (m_elevAngle > PI/2)
	{
		m_elevAngle = PI/2;
	}
}