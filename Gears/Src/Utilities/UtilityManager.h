/************************************************************************/
/* Author:  Kemi Peng 
History: 08/03/2015 by Kemi Peng*/
/************************************************************************/

#ifndef UTILITYMANAGER
#define UTILITYMANAGER

#include "GearsCommonHead.h"
#include "TimeUtility.h"
#include "ConfigUtility.h"
#include "InputUtility.h"
#include "CameraUtility.h"
#include "StageUtility.h"

class UtilityManager
{
public:
	UtilityManager();
	~UtilityManager();

	static void CreateUtilityManager(void);
	static UtilityManager *GetHandle( void );
	static void ReleaseUtilityManager( void );

	void postInitiate(void);	//some initiation only can be done after the initiation of render core

	void update( double tt, double dt);

	CameraUtility *pCameraUtility;
	ConfigUtility *pConfigUtility;
	StageUtility  *pStageUtility;
	InputUtility  *pInputUtility;
	TimeUtility   *pTimeUtility;
};

extern UtilityManager *gUtilityManager;

/************************************************************************/
/* camera function                                                      */
/************************************************************************/

void		ResetCamera();									

void		SetCamAt(float x,float y,float z);					
void		SetCamPos(float x,float y,float z);				// the position that camera is going to move to
void		SetCamPosTg(float x, float y, float z);
void		BindCamTo(void);								// unfinished				
void		BindCamAtTo(void);								// unfinished	

void		SetCamAddPos(const CVector &posPlus);							
void		SetCamAddAt(const CVector &atplus);

//void		setDistance(float dis)			{m_.dis = dis;};	
void		SetCamRotationAngle(float angle);
void		AddCamRotationAngle(float addangle);
void		SetCamElevationAngle(float elevation);
void		AddCamElevationAngle(float addelevation);

void		SetCamAspect(float w,float h);		
void		SetCamFOV(float fov);

void		SetCamZPlane(float znear,float zfar);

void		SetCamZoom(float zoom);				// simply move camera towards its look at point.
void		SetCamZoomTg(float zoom);			//  m_zoom will gradually change to m_zoomtg.
void		AddCamZoom(float zoomPlus);			

void		SetCamFOVZoomTg(float fovzoom);		// like a telecope
void		SetCamFOVZoom(float fovzoom);		// m_fovzm will gradually change to m_fovzmtg
void		AddCamFOVZoom(float fovzoomplus) ;

void		SetCamPosMoveSpd(float spd);
void		SetCamZoomSpd(float spd);
void		SetCamFOVZmSpd(float spd);
float		GetCamPosMoveSpd(void);
float		GetCamZoomSpd(void);
float		GetCamFovZmSpd(void);


CVector		GetCamPos(void);
CVector		GetCamAt(void);

CVector		GetCamPosTg(void);

float		GetCamAngle();
float		GetCamElevation();

float		GetCamFOV(void);
float		GetCamFOVZoom(void);
float		GetCamFOVZoomTg(void);
float		GetCamAspect(void);
float		GetCamZoom(void);
float		GetCamZoomTg(void);

float		GetCamZFar();
float		GetCamZNear();

const CMatrix &	GetViewMatrix();
const CMatrix &	GetProjectionMatrix();

/************************************************************************/
/* configuration function                                               */
/************************************************************************/

void		SetWindowWH(int w, int h);
void		GetWindowWH( int&w, int&h );
int			GetWindowWidth( void );
int			GetWindowHeight( void );
int			GetFPS( void );
const char* GetAppName( void );
bool		IsFullScreen( void );

int			GetShadowMapSize( void );

bool		IsFullscreenBloom(void);
void		turnFullScreenBloom(bool state);
bool		IsShinessBloom(void);
void		turnShinessBloom( bool state );
float		GetFullScreenBloomFactor(void);
float		GetShinessBloomFactor(void);
int			GetBlurIntenseLevel1(void);
int			GetBlurIntenseLevel2(void);
bool		IsDepthOfField(void);
void		turnDepthOfField(bool state);
const CVector& GetDepthOfFieldFactors(void);

/************************************************************************/
/* stage manager function												*/
/************************************************************************/
const LightsManager& GetLightManager(void);

/************************************************************************/
/* input function														*/
/************************************************************************/
int GetKeyState(KeyType key);
bool IsKeyPressed(unsigned char key);

/************************************************************************/
/*timer function														*/
/************************************************************************/
void TimerStart( void );
void TimerReset( void );
void TimerStop( void );
double GetTime( void );

#endif
