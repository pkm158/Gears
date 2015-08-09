/************************************************************************/
/* Author:  Kemi Peng 
History: 08/03/2015 by Kemi Peng*/
/************************************************************************/
#ifndef LIGHTSMANAGER
#define LIGHTSMANAGER
#include "GearsCommonHead.h"
#include "RenderCommonHead.h"
#include <list>

class LightsManager
{
public:
	LightsManager(void);
	~LightsManager(void);

	void pushLights( Light *light);
	void popLights(void);
	void removeLight( Light *light);
	void clearLights(void);

	void setDirLight(const Light &dirLight);
	void setDirLightDirection(float x, float y, float z) {m_dirLight.direction.x = x; m_dirLight.direction.y = y; m_dirLight.direction.z = z;};
	inline const Light& getDirLight(void) const {return m_dirLight;};

	inline const LightList & getLights(void) const {return m_lights;};
private:
	LightList m_lights;
	Light	  m_dirLight;
};


#endif

