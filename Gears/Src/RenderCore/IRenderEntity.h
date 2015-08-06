/************************************************************************/
/* Author:  Kemi Peng 
History: 08/03/2015 by Kemi Peng*/
/************************************************************************/

#ifndef IRENDERENTITY
#define IRENDERENTITY
#include "GearsCommonHead.h"
#include "RenderCommonHead.h"
#include "GutModel.h"

class IRenderEntity {
public:
	IRenderEntity(void);
	IRenderEntity(bool hasShadow, bool canBeShadowed);
	virtual ~IRenderEntity(void);

	virtual bool loadFromFile(const char *file);
	virtual bool loadExternTexture(int channel, const char *file);
	virtual void render(void) = NULL;

	virtual void scale(float x, float y, float z) = NULL;
	virtual void scale(float uni) = NULL;
	virtual void move(float x, float y, float z) = NULL;
	virtual void rotateX(const float rads) = NULL;
	virtual void rotateY(const float rads) = NULL;
	virtual void rotateZ(const float rads) = NULL;
	virtual const CMatrix& getWorldMat(void) = NULL;

	virtual int getDiffuseMapNum(void) const { return m_diffuseMapNum; };
	virtual float getDiffuseMapScale(int idx) const { HR(idx < m_diffuseMapNum); return m_diffuseMapScale[idx]; };

	/************************************************************************/
	/* hasShadow:		if false does not have shadow
	canBeShadowed:	if false does not draw shadow on it
	isLight:			if true, will be effect by shininess bloom effect
	canBeLighted		if false, light won't effect this object, it is self-luminous
	diffuseMapNum	how many diffuse map it has*/
	/************************************************************************/
	bool hasShadow;
	bool canBeShadowed;
	bool isLight;
	bool canBeLighted;
	bool hasSpecMap;
	bool hasNormalMap;
	bool hasEmissionMap;

protected:
	CGutModel m_model;
	int m_diffuseMapNum;
	float *m_diffuseMapScale;
};

#endif