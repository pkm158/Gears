/************************************************************************/
/* Author:  Kemi Peng 
History: 08/03/2015 by Kemi Peng*/
/************************************************************************/

#ifndef RENDERPIPE_ZPASS_OPENGL
#define RENDERPIPE_ZPASS_OPENGL

#include "GearsCommonHead.h"
#include "RenderCommonHead.h"
#include "IRenderPipe.h"


#define DEFAULT_LIGHT_FOV 0.78539f
#define DEFAULT_LIGHT_NEARZ 0.5f
#define DEFAULT_LIGHT_FARZ	1000.0f
#define DEFAULT_ORTH_W	25
#define DEFAULT_ORTH_H	25

class RenderPipe_zpass_opengl : public IRenderPipe
{
public:
	RenderPipe_zpass_opengl(void);
	virtual ~RenderPipe_zpass_opengl(void);

	bool init(Device_opengl *device);
	void release(void);

	void render(void);
	void render(GLuint target);

	void setLight(CVector lightPos, CVector lightLookAt, CVector lightUp);
	void setLight( CVector lightPos, CVector lightLookAt);

	const CMatrix& getLightViewMatrix(void) const { return m_light_vmat; };
	const CMatrix& getLightProjMatrix(void) const { return m_light_pmat; };

	void setArgs(int shadowMapSize);
	int getShadowMapSize(void) const {return m_shadowmap_size;};


private:
	GLuint m_vertexProgramID;
	GLuint m_fragmentProgramID;

	GLuint m_pipeProgramID;

	int m_shadowmap_size;

	float m_lightFOV;
	float m_lightNearZ;
	float m_lightFarZ;

	int m_orthW, m_orthH;

	CVector m_lightPos;
	CVector m_lightLookAt;
	CVector m_lightUp;

	CMatrix m_light_pmat;
	CMatrix m_light_vmat;
};

#endif