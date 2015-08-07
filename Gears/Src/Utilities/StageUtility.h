#ifndef STAGEUTILITY
#define STAGEUTILITY

#include "GearsCommonHead.h"
#include "IUtility.h"
#include "RenderEntityManager.h"
#include "RenderEntity_opengl.h"
#include "CameraUtility.h"
#include "InputUtility.h"
#include "LightsManager.h"

class StageUtility : protected IUtility
{
public:
	StageUtility(void);
	virtual ~StageUtility(void);

	virtual void update( double tt, double dt );
	virtual void postInitiate(void);

	inline const LightsManager& getLightManager(void) {return m_lightManager;};
private:
	LightsManager	m_lightManager;

	Light			*m_lightArr;

	int				m_entityNum;

#ifdef OPENGL_CORE
	RenderEntity_opengl *m_pEntity;
#endif
};

#endif