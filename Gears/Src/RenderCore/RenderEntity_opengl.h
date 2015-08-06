/************************************************************************/
/* Author:  Kemi Peng 
History: 08/03/2015 by Kemi Peng*/
/************************************************************************/

#ifndef RENDERENTITY_OPENGL
#define RENDERENTITY_OPENGL

#include "GearsCommonHead.h"
#include "RenderCommonHead.h"
#include "IRenderEntity.h"
#include "GutModel_OpenGL.h"

class RenderEntity_opengl : public IRenderEntity
{
public:
	RenderEntity_opengl(void);
	virtual ~RenderEntity_opengl(void);

	virtual bool init(const char *file, bool hasShadow=false, bool canBeShadowed=false, bool isLight=false,
		bool canBeLighted=true, const char *specMap=NULL, const char *normalMap=NULL, const char *emissionMap = NULL, 
		int diffuseMapNum = 1, float diffuseMapsScale1 = 1.0f, float diffuseMapScale2 = 1.0f, float diffuseMapScale3 = 1.0f);

	virtual void release(void);
	virtual bool loadFromFile(const char *file, const char *specMap = NULL, const char *normalMap = NULL, const char *emissionMap = NULL);
	virtual void render(void);

	virtual void scale(float x, float y, float z);
	virtual void scale(float uni);
	virtual void move(float x, float y, float z);
	virtual void move(const CVector move);
	virtual void rotateX(const float rads);
	virtual void rotateY(const float rads);
	virtual void rotateZ(const float rads);

	virtual const CVector& getPos(void) const{ return m_pos;};
	virtual const CVector& getScale(void) const{ return m_scale;};
	virtual const CVector& getRotate(void) const{ return m_rotate;};

	virtual const CMatrix& getWorldMat(void);

private:
	CGutModel_OpenGL m_model_opengl;
	bool m_isAltered;
	CMatrix m_worldMat;

	CVector m_pos;
	CVector	m_scale;
	CVector m_rotate;

};

#endif