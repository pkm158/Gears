/************************************************************************/
/* Author:  Kemi Peng 
History: 08/03/2015 by Kemi Peng*/
/************************************************************************/
#ifndef RENDERPIPE_POS_OPENGL
#define RENDERPIPE_POS_OPENGL

#define DEFAULT_FZBIAS 0.05f
#define PCF_SAMPLES 16

#include "IRenderpipe.h"
#include "RenderEntityManager.h"

class RenderPipe_DLPre_opengl :
	public IRenderPipe
{
public:
	RenderPipe_DLPre_opengl(void);
	virtual ~RenderPipe_DLPre_opengl(void);

	bool init(Device_opengl *device);
	void release(void);

	void render(void);
	// target is a buffer which contains 8 different textures
	void render(GLuint target);
	void setArgs( const CMatrix& light_vmat, const CMatrix& light_pmat, int shadowMapSize, GLuint shadowTexture );

private:
	enum
	{
		SIMPLE = 0,
		SPEC,
		NORMAL,
		SPEC_NORMAL,
		PIPE_NUM
	};
	GLuint m_vertexProgramID[PIPE_NUM];
	GLuint m_fragmentProgramID[PIPE_NUM];

	GLuint m_pipeProgramID[PIPE_NUM];

	GLuint m_shadowTexture;

	CMatrix m_zbias;
	CMatrix m_remapmat;
	CMatrix m_light_vmat;
	CMatrix m_light_pmat;
	CVector m_vTextureSize;

	
	CVector m_PCFOffset[PCF_SAMPLES];
	int m_preShadowMapSize;

	void renderByProgram(int type, const RenderEntityList& listToRender);


};

#endif