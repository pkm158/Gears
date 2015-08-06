/************************************************************************/
/* Author:  Kemi Peng 
History: 08/03/2015 by Kemi Peng*/
/************************************************************************/

#include "RenderPipe_zpass_opengl.h"
#include "UtilityManager.h"
RenderPipe_zpass_opengl::RenderPipe_zpass_opengl( void ) : 
												m_shadowmap_size(512),
												m_lightFOV(DEFAULT_LIGHT_FOV),
												m_lightNearZ(DEFAULT_LIGHT_NEARZ),
												m_lightFarZ(DEFAULT_LIGHT_FARZ),
												m_lightPos(-20, 20, 20),
												m_lightLookAt(0, 0, 0),
												m_lightUp(0, 1, 0),
												m_orthH(DEFAULT_ORTH_H),
												m_orthW(DEFAULT_ORTH_W)
{

}

RenderPipe_zpass_opengl::~RenderPipe_zpass_opengl( void )
{
	release();
}

bool RenderPipe_zpass_opengl::init( Device_opengl *device )
{

	m_vertexProgramID = loadVertexShaderOpenGL_GLSL(".//Src//Shader//ZPass.glvs");
	if ( m_vertexProgramID==NULL )
		return false;

	m_fragmentProgramID = loadFragmentShaderOpenGL_GLSL(".//Src//Shader//ZPass.glfs");
	if ( m_fragmentProgramID==NULL )
		return false;

	m_pipeProgramID = glCreateProgram();

	glAttachShader(m_pipeProgramID, m_vertexProgramID);
	glAttachShader(m_pipeProgramID, m_fragmentProgramID);
	glLinkProgram(m_pipeProgramID);

	MatrixOrthoForLH(&m_light_pmat, m_orthW, m_orthH,m_lightNearZ,m_lightFarZ);
	m_light_vmat = ViewMatrix(m_lightPos,m_lightLookAt,m_lightUp,0.0f);

	return true;
}

void RenderPipe_zpass_opengl::render( GLuint target )
{
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, target);

	glPushAttrib(GL_VIEWPORT_BIT);
	m_shadowmap_size = GetShadowMapSize();
	glViewport(0, 0, m_shadowmap_size, m_shadowmap_size);
	// Çå³ý»­Ãæ
	glClear(GL_DEPTH_BUFFER_BIT);

	glMatrixMode(GL_PROJECTION);
	glLoadMatrixf( (float *) &m_light_pmat );
	glUseProgram(m_pipeProgramID);
	RenderEntityList listForShadow = GetRenderEntityList(ENTITIES_HAS_SHADOW);

	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	for (RenderEntityList::iterator it=listForShadow.begin(); it!=listForShadow.end(); ++it)
	{
		glMatrixMode(GL_MODELVIEW);
		CMatrix worldlMat = (*it)->getWorldMat();

		worldlMat = worldlMat * m_light_vmat;
		glLoadMatrixf( (float *) &worldlMat);
		(*it)->render();
	}
	glCullFace(GL_FRONT);
	glDisable(GL_CULL_FACE);
	glUseProgram(0);
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);
	glPopAttrib();
	glDrawBuffer(GL_TRUE);

	glClear(GL_DEPTH_BUFFER_BIT);
}

void RenderPipe_zpass_opengl::render( void )
{
	render(0);
}
void RenderPipe_zpass_opengl::setLight( CVector lightPos, CVector lightLookAt, CVector lightUp )
{
	m_lightPos = lightPos;
	m_lightLookAt = lightLookAt;
	lightUp = lightUp;

	m_light_vmat = ViewMatrix(m_lightPos,m_lightLookAt,m_lightUp,0.0f);
}

void RenderPipe_zpass_opengl::setLight( CVector lightPos, CVector lightLookAt)
{
	m_lightPos = lightPos;
	m_lightLookAt = lightLookAt;

	m_light_vmat = ViewMatrix(m_lightPos,m_lightLookAt,m_lightUp,0.0f);
}

void RenderPipe_zpass_opengl::setArgs( int shadowMapSize )
{
	m_shadowmap_size = shadowMapSize;
}

void RenderPipe_zpass_opengl::release( void )
{
	if ( m_vertexProgramID )
	{
		glDeleteShader(m_vertexProgramID);
		m_vertexProgramID = NULL;
	}

	if ( m_fragmentProgramID )
	{
		glDeleteShader(m_fragmentProgramID);
		m_fragmentProgramID = NULL;
	}

	if ( m_pipeProgramID )
	{
		glDeleteProgram(m_pipeProgramID);
		m_pipeProgramID = NULL;
	}
}