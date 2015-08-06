/************************************************************************/
/* Author:  Kemi Peng 
History: 08/03/2015 by Kemi Peng*/
/************************************************************************/

#ifndef RENDERENTITYMANAGER
#define RENDERENTITYMANAGER

#include "GearsCommonHead.h"
#include "RenderCommonHead.h"
#include "IRenderEntity.h"

#include <list>

#ifndef RENDERENTITYLIST
#define RENDERENTITYLIST
typedef std::list<IRenderEntity *> RenderEntityList;
#endif

enum EntityListType
{
	ENTITIES_ALL,
	ENTITIES_OPAQUE,
	ENTITIES_LIGHT,
	ENTITIES_HAS_SHADOW,
	ENTITIES_OPAQUE_SPEC,
	ENTITIES_OPAQUE_NORMAL,
	ENTITIES_OPAQUE_SPEC_NORMAL,
	ENTITIES_OPAQUE_SIMPLE,
	ENTITIES_OPAQUE_2ND_DIFFUSE,
	ENTITIES_OPAQUE_3RD_DIFFUSE,
	ENTITIES_OPAQUE_EMISSION,
	ENTITIES_NUM,
};


class RenderEntityManager
{
public:
	RenderEntityManager(void);
	virtual ~RenderEntityManager(void);
	
	static void CreateRenderEntityManager(void);
	static void ReleaseRenderEntityManager(void);


	void clear(void);
	void submit(IRenderEntity *pRenderEntity);
	void deSubmit(IRenderEntity *pRenderEntity);

	RenderEntityList	m_lists[ENTITIES_NUM];
};

// only use these global functions.
const RenderEntityList & GetRenderEntityList(EntityListType type);
void ClearRenderLists(void);
void SubmitRenderEntity(IRenderEntity *pRenderEntity);
void DeSubmitRenderEntity(IRenderEntity *pRenderEntity);

extern RenderEntityManager *gpRenderEntityManager;
#endif