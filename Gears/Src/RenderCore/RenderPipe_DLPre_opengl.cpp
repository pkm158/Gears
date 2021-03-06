/************************************************************************/
/* Author:  Kemi Peng 
History: 08/03/2015 by Kemi Peng*/
/************************************************************************/
#include "RenderPipe_DLPre_opengl.h"

#include "UtilityManager.h"

RenderPipe_DLPre_opengl::RenderPipe_DLPre_opengl(void) : m_preShadowMapSize(0)
{
	m_zbias.Identity();
	m_zbias[3][2] = DEFAULT_FZBIAS;

	m_remapmat.Identity();
	m_remapmat.Scale(0.5f, 0.5f, 0.5f);
	m_remapmat[3][0] = m_remapmat[3][1] = m_remapmat[3][2] = 0.5f;
	m_remapmat[3][3] = 1.0f;
}

RenderPipe_DLPre_opengl::~RenderPipe_DLPre_opengl(void)
{
	release();
}

bool RenderPipe_DLPre_opengl::init( Device_opengl *device )
{
	m_pDevice = device;

	/************************************************************************/

	m_vertexProgramID[SIMPLE] = loadVertexShaderOpenGL_GLSL(".//Src//Shader//DL_pre_lerpShadow.glvs");
	if ( m_vertexProgramID[SIMPLE]==NULL )
		return false;

	m_fragmentProgramID[SIMPLE] = loadFragmentShaderOpenGL_GLSL(".//Src//Shader//DL_pre_lerpShadow.glfs");
	if ( m_fragmentProgramID==NULL )
		return false;

	m_pipeProgramID[SIMPLE] = glCreateProgram();

	glAttachShader(m_pipeProgramID[SIMPLE], m_vertexProgramID[SIMPLE]);
	glAttachShader(m_pipeProgramID[SIMPLE], m_fragmentProgramID[SIMPLE]);
	glLinkProgram(m_pipeProgramID[SIMPLE]);

	/************************************************************************/

	m_vertexProgramID[SPEC] = m_vertexProgramID[SIMPLE]; 
	if ( m_vertexProgramID[SPEC]==NULL )
		return false;

	m_fragmentProgramID[SPEC] = loadFragmentShaderOpenGL_GLSL(".//Src//Shader//DL_pre_lerpShadow_spec.glfs");
	if ( m_fragmentProgramID==NULL )
		return false;

	m_pipeProgramID[SPEC] = glCreateProgram();

	glAttachShader(m_pipeProgramID[SPEC], m_vertexProgramID[SPEC]);
	glAttachShader(m_pipeProgramID[SPEC], m_fragmentProgramID[SPEC]);
	glLinkProgram(m_pipeProgramID[SPEC]);

	/************************************************************************/

	m_vertexProgramID[NORMAL] = loadVertexShaderOpenGL_GLSL(".//Src//Shader//DL_pre_lerpShadow_normal.glvs");
	if ( m_vertexProgramID[NORMAL]==NULL )
		return false;

	m_fragmentProgramID[NORMAL] = loadFragmentShaderOpenGL_GLSL(".//Src//Shader//DL_pre_lerpShadow_normal.glfs");
	if ( m_fragmentProgramID==NULL )
		return false;

	m_pipeProgramID[NORMAL] = glCreateProgram();

	glAttachShader(m_pipeProgramID[NORMAL], m_vertexProgramID[NORMAL]);
	glAttachShader(m_pipeProgramID[NORMAL], m_fragmentProgramID[NORMAL]);
	glLinkProgram(m_pipeProgramID[NORMAL]);

	/************************************************************************/

	m_vertexProgramID[SPEC_NORMAL] = m_vertexProgramID[NORMAL];
	if ( m_vertexProgramID[SPEC_NORMAL]==NULL )
		return false;

	m_fragmentProgramID[SPEC_NORMAL] = loadFragmentShaderOpenGL_GLSL(".//Src//Shader//DL_pre_lerpShadow_spec_normal.glfs");
	if ( m_fragmentProgramID==NULL )
		return false;

	m_pipeProgramID[SPEC_NORMAL] = glCreateProgram();

	glAttachShader(m_pipeProgramID[SPEC_NORMAL], m_vertexProgramID[SPEC_NORMAL]);
	glAttachShader(m_pipeProgramID[SPEC_NORMAL], m_fragmentProgramID[SPEC_NORMAL]);
	glLinkProgram(m_pipeProgramID[SPEC_NORMAL]);


	return true;
}

void RenderPipe_DLPre_opengl::render( GLuint target )
{
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, target);
	GLuint mrt[BUFFER_NUM];
	for (int idx=0; idx<BUFFER_NUM; idx++)
	{
		mrt[idx] = GL_COLOR_ATTACHMENT0_EXT + idx;
	}
	glDrawBuffers(BUFFER_NUM, mrt);


	renderByProgram(SIMPLE, GetRenderEntityList(ENTITIES_OPAQUE_SIMPLE));
	renderByProgram(SPEC, GetRenderEntityList(ENTITIES_OPAQUE_SPEC));
	renderByProgram(NORMAL, GetRenderEntityList(ENTITIES_OPAQUE_NORMAL));
	renderByProgram(SPEC_NORMAL, GetRenderEntityList(ENTITIES_OPAQUE_SPEC_NORMAL));


	GLuint default_mrt[BUFFER_NUM];
	default_mrt[0] = GL_FRONT;
	for (int idx=1; idx<BUFFER_NUM; idx++)
	{
		default_mrt[idx] = GL_NONE;
	}

	glDrawBuffers(BUFFER_NUM, default_mrt);
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);
}

void RenderPipe_DLPre_opengl::render( void )
{
	render(0);
}
void RenderPipe_DLPre_opengl::release( void )
{
	for (int idx=0; idx<PIPE_NUM; idx++)
	{
		if ( m_vertexProgramID[idx] )
		{
			glDeleteShader(m_vertexProgramID[idx]);
			m_vertexProgramID[idx] = NULL;
		}

		if ( m_fragmentProgramID[idx] )
		{
			glDeleteShader(m_fragmentProgramID[idx]);
			m_fragmentProgramID[idx] = NULL;
		}

		if ( m_pipeProgramID[idx] )
		{
			glDeleteProgram(m_pipeProgramID[idx]);
			m_pipeProgramID[idx] = NULL;
		}
	}
}

void RenderPipe_DLPre_opengl::setArgs( const CMatrix& light_vmat, const CMatrix& light_pmat, int shadowMapSize, GLuint shadowTexture )
{
	m_light_vmat = light_vmat;
	m_light_pmat = light_pmat;

	if (m_preShadowMapSize != shadowMapSize)
	{
		m_vTextureSize = (shadowMapSize, shadowMapSize, 1.0f/(float)shadowMapSize, 1.0f/(float)shadowMapSize);

		int index = 0;
		for ( int y = -1; y < 3; y++ )
		{
			for ( int x = -1; x < 3; x++ )
			{
				m_PCFOffset[index].Set((float)x/(float)shadowMapSize, (float)y/(float)shadowMapSize, 0.0f, 0.0f);
				index++;
			}
		}

		m_preShadowMapSize = shadowMapSize;
	}

	if (m_shadowTexture != shadowTexture)
	{
		m_shadowTexture = shadowTexture;
		CGutModel_OpenGL::SetTextureOverwrite(SHADOW_MAP_CHANNEL, m_shadowTexture);
	}
}


void RenderPipe_DLPre_opengl::renderByProgram( int type, const RenderEntityList& listToRender )
{
	glUseProgram(m_pipeProgramID[type]);
	glMatrixMode(GL_PROJECTION);
	glLoadMatrixf( (float *) &(GetProjectionMatrix()));
	CMatrix view_matrix = GetViewMatrix();

	Light dirLight = GetLightManager().getDirLight();
	CVector camPos = GetCamPos();
	GLint reg = glGetUniformLocation(m_pipeProgramID[type], "u_light");
	if (reg >= 0)
		glUniform4fv(reg, 4, (float *)&dirLight.dirLightData);

	reg = glGetUniformLocation(m_pipeProgramID[type], "u_textureSize");
	if (reg >= 0)
		glUniform4fv(reg, 1, (float *)&m_vTextureSize);

	reg = glGetUniformLocation(m_pipeProgramID[type], "u_camPos");
	if (reg >= 0)
		glUniform4fv(reg, 1, (float *)&camPos);

	reg = glGetUniformLocation(m_pipeProgramID[type], "u_shadowSampler");
	if (reg >= 0)
		glUniform1i(reg, SHADOW_MAP_CHANNEL);

	reg = glGetUniformLocation(m_pipeProgramID[type], "u_diffuseSampler");
	if (reg >= 0)
		glUniform1i(reg, DIFF0_MAP_CHANNEL);

	reg = glGetUniformLocation(m_pipeProgramID[type], "u_specSampler");
	if (reg >= 0)
		glUniform1i(reg, SPEC_MAP_CHANNEL);

	reg = glGetUniformLocation(m_pipeProgramID[type], "u_normalSampler");
	if (reg >= 0)
		glUniform1i(reg, NORMAL_MAP_CHANNEL);
	
	reg = glGetUniformLocation(m_pipeProgramID[type], "u_texoffset");
	if (reg >= 0)
		glUniform4fv(reg, PCF_SAMPLES, (float *)m_PCFOffset);

	GLint regIsShadowed = glGetUniformLocation(m_pipeProgramID[type], "u_isShadowed");
	GLint regCanBeLighted = glGetUniformLocation(m_pipeProgramID[type], "u_canBeLighted");
	GLint regDiffScale = glGetUniformLocation(m_pipeProgramID[type], "u_diffScale");

	for (RenderEntityList::const_iterator it=listToRender.begin(); it!=listToRender.end(); ++it)
	{
		if (regIsShadowed >= 0)
			glUniform1i(regIsShadowed, (*it)->canBeShadowed);

		if (regCanBeLighted >= 0)
			glUniform1i(regCanBeLighted, (*it)->canBeLighted);

		CMatrix worldMat = (*it)->getWorldMat();

		CMatrix light_wvp_matrix = worldMat * m_light_vmat * m_zbias * m_light_pmat * m_remapmat;

		glMatrixMode(GL_TEXTURE); 
		glActiveTexture(GL_TEXTURE0_ARB);
		glLoadMatrixf( (float *) &light_wvp_matrix );

		glActiveTexture(GL_TEXTURE1_ARB);
		glLoadMatrixf( (float *) &worldMat);
		glActiveTexture(GL_TEXTURE0_ARB);


		glMatrixMode(GL_MODELVIEW);
		CMatrix worldlMatView = worldMat * view_matrix;
		glLoadMatrixf( (float *) &worldlMatView);

		if (regDiffScale >= 0)
			glUniform1f(regDiffScale, (*it)->getDiffuseMapScale(0));
		(*it)->render();
	}
glUseProgram(0);


	
}