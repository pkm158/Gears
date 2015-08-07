/************************************************************************/
/* Author:  Kemi Peng 
History: 08/03/2015 by Kemi Peng*/
/************************************************************************/

#ifndef CONFIGUTILITY
#define CONFIGUTILITY

#include "GearsCommonHead.h"
#include "IUtility.h"
#include "libmath.h"

class ConfigUtility:protected IUtility
{
public:
	ConfigUtility( void );
	virtual ~ConfigUtility( void );
	
	void readConfig( void );
	virtual void update( double tt, double dt ){};
	virtual void postInitiate(void){};
	// it's all about the size of view port, not the window!!
	inline void setWindowWH(int w, int h) {m_windowWidth = w; m_windowHeight = h;};
	inline void getWindowWH( int&w, int&h ) const { w=m_windowWidth; h=m_windowHeight; };
	inline int getWindowWidth( void ) const { return m_windowWidth; };
	inline int getWindowHeight( void ) const { return m_windowHeight; };
	/////
	inline int getFPS( void ) const { return m_fps;};
	inline const char* getAppName( void ) const { return m_appName; };
	inline bool isFullScreen( void ) const { return m_isFullScreen; };

	inline int getShadowMapSize( void ) const { return m_shadowMapSize; };

	inline bool isFullscreenBloom(void) const {return m_isFullScreenBloom;};
	inline void turnFullScreenBloom(bool state) {m_isFullScreenBloom = state;};
	inline bool isShinessBloom(void) const {return m_isShinessBloom;};
	inline void turnShinessBloom(bool state) {m_isShinessBloom = state;};
	inline float getFullScreenBloomFactor(void) const {return m_fullscreenBloomFactor;};
	inline float getShinessBloomFactor(void) const {return m_shinessBloomFactor;};
	inline int	 getBlurIntenseLevel1(void) const {return m_blurIntense_levell;};
	inline int   getBlurIntenseLevel2(void) const {return m_blurIntense_level2;};
	inline bool	 isDepthOfField(void) const {return m_isDepthOfField;};
	inline void	 turnDepthOfField(bool state) {m_isDepthOfField = state;};
	inline const CVector& getDepthOfFieldFactors(void) const {return m_depthOfFieldFactors;};
	
private:
	static const int STRTEMP_SIZE = 128;

	const char *configPath;
	char *m_appName;
	int m_windowWidth, m_windowHeight;	// view port size, not the window indeed
	int m_fps;
	bool m_isFullScreen;

	int m_shadowMapSize;

	bool m_isFullScreenBloom;
	float m_fullscreenBloomFactor;

	bool m_isShinessBloom;
	float m_shinessBloomFactor;

	int m_blurIntense_levell;
	int m_blurIntense_level2;

	bool m_isDepthOfField;
	CVector m_depthOfFieldFactors;


};

#endif