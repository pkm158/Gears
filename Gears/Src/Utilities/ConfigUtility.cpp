/************************************************************************/
/* Author:  Kemi Peng 
History: 08/03/2015 by Kemi Peng*/
/************************************************************************/

#include "ConfigUtility.h"

ConfigUtility::ConfigUtility( void ):
							m_windowWidth(1024),
							m_windowHeight(768),
							m_fps(60),
							m_appName( new char[STRTEMP_SIZE] ),
							m_isFullScreen(0),
							m_shadowMapSize(512),
							m_isFullScreenBloom(true),
							m_isShinessBloom(true),
							m_fullscreenBloomFactor(0.5f),
							m_shinessBloomFactor(0.5),
							m_blurIntense_levell(4),
							m_blurIntense_level2(6),
							m_isDepthOfField(true),
							m_depthOfFieldFactors(10, 10, 65, 10),
							configPath(".\\Resources\\config.cfg")
{
	DEBUG_PRINT("Create config utility...");
}

ConfigUtility::~ConfigUtility( void )
{
	SAFE_DELETE_ARRAY( m_appName );
}

void ConfigUtility::readConfig( void )
{
	char	strTemp[STRTEMP_SIZE];

	//app name
	GetPrivateProfileString("COMMON","AppName","0",strTemp,STRTEMP_SIZE,configPath);
	strcmp( "0", strTemp )==0? strcpy_s( m_appName, STRTEMP_SIZE, "Gears" ):strcpy_s( m_appName, STRTEMP_SIZE, strTemp );

	//³¤¿í
	GetPrivateProfileString("COMMON","Window_Width","0",strTemp,STRTEMP_SIZE,configPath);
	m_windowWidth=(int)atoi(strTemp);
	if ( m_windowWidth<=0 )
		m_windowWidth = 1280;

	GetPrivateProfileString("COMMON","Window_Height","0",strTemp,STRTEMP_SIZE,configPath);
	m_windowHeight=(int)atoi(strTemp);
	if ( m_windowHeight<= 0 )
		m_windowHeight = 720;

	//fps
	GetPrivateProfileString("COMMON","FPS","0",strTemp,STRTEMP_SIZE,configPath);
	m_fps=(int)atoi(strTemp);
	if ( m_fps<=0 )
		m_fps =60;

	//is full screen?
	GetPrivateProfileString("COMMON","FullScreen","0",strTemp,STRTEMP_SIZE,configPath);

	if ( strcmp( "Enabled", strTemp )==0 )
	{
		m_isFullScreen = true;
	}
	else
	{
		m_isFullScreen = false;
	}

	//shadowMap
	GetPrivateProfileString("VISUAL_EFFECT","ShadowMapSize","0",strTemp,STRTEMP_SIZE,configPath);
	m_shadowMapSize=(int)atoi(strTemp);
	if ( m_shadowMapSize==0 )
		m_shadowMapSize =512;

	GetPrivateProfileString("VISUAL_EFFECT","Bloom","0",strTemp,STRTEMP_SIZE,configPath);

	GetPrivateProfileString("VISUAL_EFFECT","FullScreenBloom","0",strTemp,STRTEMP_SIZE,configPath);

	if ( strcmp( "Enabled", strTemp )==0 )
	{
		m_isFullScreenBloom = true;
	}
	else
	{
		m_isFullScreenBloom = false;
	}

	GetPrivateProfileString("VISUAL_EFFECT","ShinessBloom","0",strTemp,STRTEMP_SIZE,configPath);

	if ( strcmp( "Enabled", strTemp )==0 )
	{
		m_isShinessBloom = true;
	}
	else
	{
		m_isShinessBloom = false;
	}

	GetPrivateProfileString("VISUAL_EFFECT","FullScreenBloomFactor","-1",strTemp,STRTEMP_SIZE,configPath);
	m_fullscreenBloomFactor =(float)atof(strTemp);
	if ( m_fullscreenBloomFactor < 0 )
		m_fullscreenBloomFactor = 0.5f;
	else if (m_fullscreenBloomFactor > 1.0f)
		m_fullscreenBloomFactor = 1.0f;

	GetPrivateProfileString("VISUAL_EFFECT","ShinessBloomFactor","-1",strTemp,STRTEMP_SIZE,configPath);
	m_shinessBloomFactor =(float)atof(strTemp);
	if ( m_shinessBloomFactor < 0 )
		m_shinessBloomFactor = 0.5f;

	GetPrivateProfileString("VISUAL_EFFECT","BlurIntense_level1","0",strTemp,STRTEMP_SIZE,configPath);
	m_blurIntense_levell=(int)atoi(strTemp);
	if ( m_blurIntense_levell<=0 )
		m_blurIntense_levell =4;

	GetPrivateProfileString("VISUAL_EFFECT","BlurIntense_level2","0",strTemp,STRTEMP_SIZE,configPath);
	m_blurIntense_level2=(int)atoi(strTemp);
	if ( m_blurIntense_level2<=0 )
		m_blurIntense_level2 =8;

	GetPrivateProfileString("VISUAL_EFFECT","DepthOfField","0",strTemp,STRTEMP_SIZE,configPath);
	if ( strcmp( "Enabled", strTemp )==0 )
	{
		m_isDepthOfField = true;
	}
	else
	{
		m_isDepthOfField = false;
	}

	float nearZ, farZ, nearZFactor, farZfactor;
	GetPrivateProfileString("VISUAL_EFFECT","NearZ","-1",strTemp,STRTEMP_SIZE,configPath);
	nearZ =(float)atof(strTemp);
	if ( nearZ < 0 )
		nearZ = 10.0f;

	GetPrivateProfileString("VISUAL_EFFECT","FarZ","-1",strTemp,STRTEMP_SIZE,configPath);
	farZ =(float)atof(strTemp);
	if ( farZ < 0 )
		farZ = 65.0f;

	GetPrivateProfileString("VISUAL_EFFECT","NearZ_Factor","-1",strTemp,STRTEMP_SIZE,configPath);
	nearZFactor =(float)atof(strTemp);
	if ( nearZFactor < 0 )
		nearZFactor = 10.0f;

	GetPrivateProfileString("VISUAL_EFFECT","FarZ_Factor","-1",strTemp,STRTEMP_SIZE,configPath);
	farZfactor =(float)atof(strTemp);
	if ( farZfactor < 0 )
		farZfactor = 10.0f;

	m_depthOfFieldFactors = CVector(nearZ, nearZFactor, farZ, farZfactor);

}