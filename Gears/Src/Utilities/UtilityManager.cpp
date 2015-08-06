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
}													//重置摄像机

void		SetCamAt(float x,float y,float z)
{
	gUtilityManager->pCameraUtility->setAt(x, y, z);
}									// 设定观测点
void		SetCamPos(float x,float y,float z)
{
	gUtilityManager->pCameraUtility->setPos(x, y, z);
}							// 设定位置
void		SetCamPosTg(float x, float y, float z)
{
	gUtilityManager->pCameraUtility->setPosTg(x, y, z);
}							// 设定需要移动到的位置

void		SetCamAddPos(const CVector &posPlus)
{
	gUtilityManager->pCameraUtility->setAddPos(posPlus);
}								// 摄像机位置的偏移量
void		SetCamAddAt(const CVector &atplus)
{
	gUtilityManager->pCameraUtility->setAddAt(atplus);
}// 观测点偏移量

//void		setDistance(float dis)			{m_.dis = dis;};				// 距離設定
void		SetCamRotationAngle(float angle)
{
	gUtilityManager->pCameraUtility->setRotationAngle(angle);
}// 角度設定
void		AddCamRotationAngle(float addangle)
{
	gUtilityManager->pCameraUtility->addRotationAngle(addangle);
};
void		SetCamElevationAngle(float elevation)
{	gUtilityManager->pCameraUtility->setElevationAngle(elevation);
}// 仰角設定
void		AddCamElevationAngle(float addelevation)
{
	gUtilityManager->pCameraUtility->addElevationAngle(addelevation);
}

void		SetCamAspect(float w,float h) 
{
	gUtilityManager->pCameraUtility->setAspect(w, h);
}										// 长宽比
void		SetCamFOV(float fov) 
{
	gUtilityManager->pCameraUtility->setFOV(fov);
}// 视野角

void		SetCamZPlane(float znear,float zfar)
{
	gUtilityManager->pCameraUtility->setZPlane(znear, zfar);
}	// 可视菱体范围

void		SetCamZoom(float zoom)	
{
	gUtilityManager->pCameraUtility->setZoom(zoom);
}											// zoom值，默认为1，大于1拉近，小于1拉远，不会小于0,单纯的相机推近推远
void		SetCamZoomTg(float zoom) 
{
	gUtilityManager->pCameraUtility->setZoomTg(zoom);
}											// 最终zoom值
void		AddCamZoom(float zoomPlus)
{
	gUtilityManager->pCameraUtility->addZoom(zoomPlus);
}								// zoom增加量

void		SetCamFOVZoomTg(float fovzoom)
{
	gUtilityManager->pCameraUtility->setFOVZoomTg(fovzoom);
}								// 视野角zoom最终值，默认为1，大于1拉近，小于1拉远，不会小于0，相当于瞄准镜
void		SetCamFOVZoom(float fovzoom) 
{ 
	gUtilityManager->pCameraUtility->setFOVZoom(fovzoom);
}							// 视野角zoom值
void		AddCamFOVZoom(float fovzoomplus) 
{ 
	gUtilityManager->pCameraUtility->addFOVZoom(fovzoomplus);
}									// 视野角zoom值的增加量

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
}// 获得当前位置
CVector		GetCamAt(void) 
{
	return gUtilityManager->pCameraUtility->getAt();
}// 获得当前视点

CVector		GetCamPosTg(void) 
{
	return gUtilityManager->pCameraUtility->getPosTg();
};

float		GetCamAngle() 
{
	return gUtilityManager->pCameraUtility->getAngle();
}// 角度取得
float		GetCamElevation() 
{
	return gUtilityManager->pCameraUtility->getElevation();
};					// 仰角取得
//float		getDistance();	// 距離取得

float		GetCamFOV(void)    
{
	return gUtilityManager->pCameraUtility->getFOV();
}// 視野角取得
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
}// 縦横比取得
float		GetCamZoom(void)  	
{
	return gUtilityManager->pCameraUtility->getZoom();
}// 拡大値
float		GetCamZoomTg(void) 
{
	return gUtilityManager->pCameraUtility->getFOVZoomTg();
}// 拡大値目標値

float		GetCamZFar()
{
	return gUtilityManager->pCameraUtility->getZFar();
}// 最遠点
float		GetCamZNear()	
{
	return gUtilityManager->pCameraUtility->getZNear();
}// 最近点



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