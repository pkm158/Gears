/************************************************************************/
/* Author:  Kemi Peng 
History: 08/03/2015 by Kemi Peng*/
/************************************************************************/
#include "Utilitymanager.h"
UtilityManager *gUtilityManager = NULL;

UtilityManager::UtilityManager():
						pTimeUtility( new TimeUtility() ),
						pConfigUtility( new ConfigUtility() ),
						pInputUtility( new InputUtility() ),
						pCameraUtility( new CameraUtility() ),
						pStageUtility( new StageUtility() )

{
	pConfigUtility->readConfig();
}

UtilityManager::~UtilityManager()
{
	SAFE_DELETE( pTimeUtility );
	SAFE_DELETE(pInputUtility);
	SAFE_DELETE(pCameraUtility);
	SAFE_DELETE(pStageUtility);
	SAFE_DELETE(pConfigUtility);
}


void UtilityManager::postInitiate( void )
{
	DEBUG_PRINT("Initiate utility manager...");
	pCameraUtility->postInitiate();
	pInputUtility->postInitiate();
	pStageUtility->postInitiate();
}

void UtilityManager::CreateUtilityManager()
{
	DEBUG_PRINT("Create utility manager...");
	if ( NULL==gUtilityManager )
	{
		gUtilityManager = new UtilityManager();
	}
}

UtilityManager * UtilityManager::GetHandle( void )
{
	HR(gUtilityManager);
	return gUtilityManager;
}

void UtilityManager::ReleaseUtilityManager( void )
{
	SAFE_DELETE(gUtilityManager);
}

void UtilityManager::update( double tt, double dt )
{

	pInputUtility->update(tt, dt);
	pCameraUtility->update(tt, dt);
	pStageUtility->update(tt, dt);
}

void		ResetCamera()
{
	gUtilityManager->pCameraUtility->resetCamera();
}													//���������

void		SetCamAt(float x,float y,float z)
{
	gUtilityManager->pCameraUtility->setAt(x, y, z);
}									// �趨�۲��
void		SetCamPos(float x,float y,float z)
{
	gUtilityManager->pCameraUtility->setPos(x, y, z);
}							// �趨λ��
void		SetCamPosTg(float x, float y, float z)
{
	gUtilityManager->pCameraUtility->setPosTg(x, y, z);
}							// �趨��Ҫ�ƶ�����λ��

void		SetCamAddPos(const CVector &posPlus)
{
	gUtilityManager->pCameraUtility->setAddPos(posPlus);
}								// �����λ�õ�ƫ����
void		SetCamAddAt(const CVector &atplus)
{
	gUtilityManager->pCameraUtility->setAddAt(atplus);
}// �۲��ƫ����

//void		setDistance(float dis)			{m_.dis = dis;};				// ���x�O��
void		SetCamRotationAngle(float angle)
{
	gUtilityManager->pCameraUtility->setRotationAngle(angle);
}// �Ƕ��O��
void		AddCamRotationAngle(float addangle)
{
	gUtilityManager->pCameraUtility->addRotationAngle(addangle);
};
void		SetCamElevationAngle(float elevation)
{	gUtilityManager->pCameraUtility->setElevationAngle(elevation);
}// �����O��
void		AddCamElevationAngle(float addelevation)
{
	gUtilityManager->pCameraUtility->addElevationAngle(addelevation);
}

void		SetCamAspect(float w,float h) 
{
	gUtilityManager->pCameraUtility->setAspect(w, h);
}										// �����
void		SetCamFOV(float fov) 
{
	gUtilityManager->pCameraUtility->setFOV(fov);
}// ��Ұ��

void		SetCamZPlane(float znear,float zfar)
{
	gUtilityManager->pCameraUtility->setZPlane(znear, zfar);
}	// �������巶Χ

void		SetCamZoom(float zoom)	
{
	gUtilityManager->pCameraUtility->setZoom(zoom);
}											// zoomֵ��Ĭ��Ϊ1������1������С��1��Զ������С��0,����������ƽ���Զ
void		SetCamZoomTg(float zoom) 
{
	gUtilityManager->pCameraUtility->setZoomTg(zoom);
}											// ����zoomֵ
void		AddCamZoom(float zoomPlus)
{
	gUtilityManager->pCameraUtility->addZoom(zoomPlus);
}								// zoom������

void		SetCamFOVZoomTg(float fovzoom)
{
	gUtilityManager->pCameraUtility->setFOVZoomTg(fovzoom);
}								// ��Ұ��zoom����ֵ��Ĭ��Ϊ1������1������С��1��Զ������С��0���൱����׼��
void		SetCamFOVZoom(float fovzoom) 
{ 
	gUtilityManager->pCameraUtility->setFOVZoom(fovzoom);
}							// ��Ұ��zoomֵ
void		AddCamFOVZoom(float fovzoomplus) 
{ 
	gUtilityManager->pCameraUtility->addFOVZoom(fovzoomplus);
}									// ��Ұ��zoomֵ��������

void		SetCamPosMoveSpd(float spd)
{
	gUtilityManager->pCameraUtility->setPosMoveSpd(spd);
}
void		SetCamZoomSpd(float spd)
{
	gUtilityManager->pCameraUtility->setZoomSpd(spd);
}
void		SetCamFOVZmSpd(float spd)
{
	gUtilityManager->pCameraUtility->setFOVZmSpd(spd);
}

float		GetCamPosMoveSpd(void) 
{
	return gUtilityManager->pCameraUtility->getPosMoveSpd();
}
float		GetCamZoomSpd(void) 
{
	return gUtilityManager->pCameraUtility->getZoomSpd();
}
float		GetCamFovZmSpd(void) 
{
	return gUtilityManager->pCameraUtility->getFovZmSpd();
}


CVector		GetCamPos(void) 
{
	return gUtilityManager->pCameraUtility->getPos();
}// ��õ�ǰλ��
CVector		GetCamAt(void) 
{
	return gUtilityManager->pCameraUtility->getAt();
}// ��õ�ǰ�ӵ�

CVector		GetCamPosTg(void) 
{
	return gUtilityManager->pCameraUtility->getPosTg();
};

float		GetCamAngle() 
{
	return gUtilityManager->pCameraUtility->getAngle();
}// �Ƕ�ȡ��
float		GetCamElevation() 
{
	return gUtilityManager->pCameraUtility->getElevation();
};					// ����ȡ��
//float		getDistance();	// ���xȡ��

float		GetCamFOV(void)    
{
	return gUtilityManager->pCameraUtility->getFOV();
}// ҕҰ��ȡ��
float		GetCamFOVZoom(void) 
{
	return gUtilityManager->pCameraUtility->getFOVZoom();
}
float		GetCamFOVZoomTg(void) 
{
	return gUtilityManager->pCameraUtility->getFOVZoomTg();
}
float		GetCamAspect(void)	
{
	return gUtilityManager->pCameraUtility->getAspect();
}// �k���ȡ��
float		GetCamZoom(void)  	
{
	return gUtilityManager->pCameraUtility->getZoom();
}// ����
float		GetCamZoomTg(void) 
{
	return gUtilityManager->pCameraUtility->getFOVZoomTg();
}// ����Ŀ�˂�

float		GetCamZFar()
{
	return gUtilityManager->pCameraUtility->getZFar();
}// ���h��
float		GetCamZNear()	
{
	return gUtilityManager->pCameraUtility->getZNear();
}// �����



const CMatrix &	GetViewMatrix() 
{
	return gUtilityManager->pCameraUtility->getViewMatrix();
}
const CMatrix &	GetProjectionMatrix() 
{
	return gUtilityManager->pCameraUtility->getProjectionMatrix();
}

void BindCamTo( void )
{

}

void BindCamAtTo( void )
{

}

void SetWindowWH( int w, int h )
{
	gUtilityManager->pConfigUtility->setWindowWH(w, h);
}

void GetWindowWH( int&w, int&h )
{
	return gUtilityManager->pConfigUtility->getWindowWH(w, h);
}

int GetWindowWidth( void )
{
	return gUtilityManager->pConfigUtility->getWindowWidth();
}

int GetWindowHeight( void )
{
	return gUtilityManager->pConfigUtility->getWindowHeight();
}

int GetFPS( void )
{
	return gUtilityManager->pConfigUtility->getFPS();
}

const char* GetAppName( void )
{
	return gUtilityManager->pConfigUtility->getAppName();
}

bool IsFullScreen( void )
{
	return gUtilityManager->pConfigUtility->isFullScreen();
}

int GetShadowMapSize( void )
{
	return gUtilityManager->pConfigUtility->getShadowMapSize();
}

bool IsFullscreenBloom( void )
{
	return gUtilityManager->pConfigUtility->isFullscreenBloom();
}

bool IsShinessBloom( void )
{
	return gUtilityManager->pConfigUtility->isShinessBloom();
}

float GetFullScreenBloomFactor( void )
{
	return gUtilityManager->pConfigUtility->getFullScreenBloomFactor();
}

float GetShinessBloomFactor( void )
{
	return gUtilityManager->pConfigUtility->getShinessBloomFactor();
}

int GetBlurIntenseLevel1( void )
{
	return gUtilityManager->pConfigUtility->getBlurIntenseLevel1();
}

int GetBlurIntenseLevel2( void )
{
	return gUtilityManager->pConfigUtility->getBlurIntenseLevel2();
}

bool IsDepthOfField( void )
{
	return gUtilityManager->pConfigUtility->isDepthOfField();
}

const CVector& GetDepthOfFieldFactors( void )
{
	return gUtilityManager->pConfigUtility->getDepthOfFieldFactors();
}

const LightsManager& GetLightManager( void )
{
	return gUtilityManager->pStageUtility->getLightManager();
}

int GetKeyState( KeyType key )
{
	return gUtilityManager->pInputUtility->getKeyState(key);
}

bool IsKeyPressed( unsigned char key )
{
	return gUtilityManager->pInputUtility->isKey(key);
}

void TimerStart( void )
{
	gUtilityManager->pTimeUtility->start();
}

void TimerReset( void )
{
	gUtilityManager->pTimeUtility->reset();
}

void TimerStop( void )
{
	gUtilityManager->pTimeUtility->stop();
}

double GetTime( void )
{
	return gUtilityManager->pTimeUtility->getTime();
}