/************************************************************************/
/* Author:  Kemi Peng 
History: 08/03/2015 by Kemi Peng*/
/************************************************************************/
#include "LightsManager.h"

LightsManager::LightsManager(void)
{
}

LightsManager::~LightsManager(void)
{
	clearLights();
}

void LightsManager::clearLights( void )
{
	m_lights.clear();	
}

void LightsManager::removeLight( Light *light )
{
	m_lights.remove(light);
}

void LightsManager::pushLights( Light *plight )
{
	m_lights.push_back(plight);
}

void LightsManager::popLights( void )
{
	m_lights.pop_back();
}

void LightsManager::setDirLight( const Light& dirLight )
{
	HR(dirLight.lightType == LIGHT_DIRECTIONAL);

	m_dirLight = dirLight;

}