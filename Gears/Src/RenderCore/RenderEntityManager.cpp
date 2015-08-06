/************************************************************************/
/* Author:  Kemi Peng 
History: 08/03/2015 by Kemi Peng*/
/************************************************************************/
#include "RenderEntityManager.h"
RenderEntityManager *gpRenderEntityManager = NULL;

RenderEntityManager::RenderEntityManager( void )
{
}

RenderEntityManager::~RenderEntityManager( void )
{
	clear();
}

void RenderEntityManager::CreateRenderEntityManager( void )
{
	DEBUG_PRINT("Create the render entity manager...");
	if (NULL == gpRenderEntityManager)
	{
		gpRenderEntityManager = new RenderEntityManager();
	}
}

void RenderEntityManager::ReleaseRenderEntityManager( void )
{
	SAFE_DELETE(gpRenderEntityManager);
}

void RenderEntityManager::submit( IRenderEntity *pRenderEntity )
{
	if(!pRenderEntity) ALERT("pRenderEntity is null!!!");

	m_lists[ENTITIES_ALL].push_back(pRenderEntity);
	m_lists[ENTITIES_OPAQUE].push_back(pRenderEntity);

	if (pRenderEntity->isLight)
	{
		m_lists[ENTITIES_LIGHT].push_back(pRenderEntity);
	}


	if (pRenderEntity->hasShadow)
	{
		m_lists[ENTITIES_HAS_SHADOW].push_back(pRenderEntity);
	}

	if (pRenderEntity->hasSpecMap)
	{	
		if (pRenderEntity->hasNormalMap)
		{
			m_lists[ENTITIES_OPAQUE_SPEC_NORMAL].push_back(pRenderEntity);
		} else
		{
			m_lists[ENTITIES_OPAQUE_SPEC].push_back(pRenderEntity);
		}
	} else
	{
		if (pRenderEntity->hasNormalMap)
		{
			m_lists[ENTITIES_OPAQUE_NORMAL].push_back(pRenderEntity);
		} else 
		{
			m_lists[ENTITIES_OPAQUE_SIMPLE].push_back(pRenderEntity);
		}
	}

	if (pRenderEntity->getDiffuseMapNum() == 2)
	{
		m_lists[ENTITIES_OPAQUE_2ND_DIFFUSE].push_back(pRenderEntity);
	}

	if (pRenderEntity->getDiffuseMapNum() == 3)
	{
		m_lists[ENTITIES_OPAQUE_2ND_DIFFUSE].push_back(pRenderEntity);
		m_lists[ENTITIES_OPAQUE_3RD_DIFFUSE].push_back(pRenderEntity);
	}

	if (pRenderEntity->hasEmissionMap)
	{
		m_lists[ENTITIES_OPAQUE_EMISSION].push_back(pRenderEntity);
	}
	
}

void RenderEntityManager::deSubmit( IRenderEntity *pRenderEntity )
{
	if(!pRenderEntity) ALERT("pRenderEntity is null!!!");

	m_lists[ENTITIES_ALL].remove(pRenderEntity);
	m_lists[ENTITIES_OPAQUE].remove(pRenderEntity);

	if (pRenderEntity->isLight)
	{
		m_lists[ENTITIES_LIGHT].remove(pRenderEntity);
	}


	if (pRenderEntity->hasShadow)
	{
		m_lists[ENTITIES_HAS_SHADOW].remove(pRenderEntity);
	}

	if (pRenderEntity->hasSpecMap)
	{	
		if (pRenderEntity->hasNormalMap)
		{
			m_lists[ENTITIES_OPAQUE_SPEC_NORMAL].remove(pRenderEntity);
		} else
		{
			m_lists[ENTITIES_OPAQUE_SPEC].remove(pRenderEntity);
		}
	} else
	{
		if (pRenderEntity->hasNormalMap)
		{
			m_lists[ENTITIES_OPAQUE_NORMAL].remove(pRenderEntity);
		} else 
		{
			m_lists[ENTITIES_OPAQUE_SIMPLE].remove(pRenderEntity);
		}
	}

	if (pRenderEntity->getDiffuseMapNum() == 2)
	{
		m_lists[ENTITIES_OPAQUE_2ND_DIFFUSE].remove(pRenderEntity);
	}

	if (pRenderEntity->getDiffuseMapNum() == 3)
	{
		m_lists[ENTITIES_OPAQUE_2ND_DIFFUSE].remove(pRenderEntity);
		m_lists[ENTITIES_OPAQUE_3RD_DIFFUSE].remove(pRenderEntity);
	}

	if (pRenderEntity->hasEmissionMap)
	{
		m_lists[ENTITIES_OPAQUE_EMISSION].remove(pRenderEntity);
	}
}

void RenderEntityManager::clear( void )
{
	for (int idx=0; idx<ENTITIES_NUM; idx++)
	{
		m_lists[idx].clear();
	}
}

const RenderEntityList & GetRenderEntityList( EntityListType type )
{
	return gpRenderEntityManager->m_lists[type];
}

void ClearRenderLists( void )
{
	gpRenderEntityManager->clear();
}

void SubmitRenderEntity( IRenderEntity *pRenderEntity )
{
	gpRenderEntityManager->submit(pRenderEntity);
}

void DeSubmitRenderEntity( IRenderEntity *pRenderEntity )
{
	gpRenderEntityManager->deSubmit(pRenderEntity);
}