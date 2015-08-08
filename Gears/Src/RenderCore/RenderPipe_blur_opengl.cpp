/************************************************************************/
/* Author:  Kemi Peng 
History: 08/03/2015 by Kemi Peng*/
/************************************************************************/

#include "RenderPipe_blur_opengl.h"
#include "UtilityManager.h"

RenderPipe_blur_opengl::RenderPipe_blur_opengl(void)
{
	const int num_samples = KERNELSIZE;
	m_blurLevel = BLUR_INTENSE_L1;

	// calculate the other 8 pixals around it, and their weight, change those attributes can change the effect of blur
	// it is better to have a mechanism to control them rather than to set them as constant value
	m_weight[0] = m_weight[8] = 1.0f;
	m_weight[1] = m_weight[7] = 1.5f;
	m_weight[2] = m_weight[6] = 2.0f;
	m_weight[3] = m_weight[5] = 3.0f;
	m_weight[4] = 4.0f;	

	float sum = 0.0f;
	for (int i=0; i<num_samples; i++ )
	{
		sum += m_weight[i];

		m_uvOffset[i] = (float)(1 - num_samples)/2.0f + i;
	}

	for (int i=0; i<num_samples; i++ )
	{
		m_weight[i] /= sum;
	}
}

RenderPipe_blur_opengl::~RenderPipe_blur_opengl(void)
{
	release();
}

bool RenderPipe_blur_opengl::init( Device_opengl *device )
{
	m_pDevice = device;

	int w, h;
	GetWindowWH(w, h);

	m_bufferWidth = w;
	m_bufferHeight = h;

	int blur_intense_l1 = GetBlurIntenseLevel1();
	int blur_intense_l2 = GetBlurIntenseLevel2();


	// for slightly blur, bigger size for the blurred picture.
	if(!Device_opengl::CreateRenderTargetOpenGL(
		m_bufferWidth/blur_intense_l1, m_bufferHeight/blur_intense_l1, GL_RGBA8, &m_interBufferID[0], &m_interTextureID[0]) )
	{
		return false;
	}
	// for heaviely blur, smaller size for the blurred picture.
	if(!Device_opengl::CreateRenderTargetOpenGL(
		m_bufferWidth/blur_intense_l2, m_bufferHeight/blur_intense_l2, GL_RGBA8, &m_interBufferID[1], &m_interTextureID[1]) )
	{
		return false;
	}

	m_vertexProgramID = loadVertexShaderOpenGL_GLSL(".\\Src\\Shader\\bloom.glvs");
	if ( m_vertexProgramID==NULL )
		return false;

	m_fragmentProgramID = loadFragmentShaderOpenGL_GLSL(".\\Src\\Shader\\blur.glfs");
	if ( m_fragmentProgramID==NULL )
		return false;

	m_pipeProgramID = glCreateProgram();

	glAttachShader(m_pipeProgramID, m_vertexProgramID);
	glAttachShader(m_pipeProgramID, m_fragmentProgramID);
	glLinkProgram(m_pipeProgramID);

	return true;
}

void RenderPipe_blur_opengl::release( void )
{
	for(int idx=0; idx<=BLUR_NUM; idx++)
	{
		glDeleteFramebuffersEXT(1, &m_interBufferID[idx]);
		m_interBufferID[idx] = NULL;
		glDeleteTextures(1, &m_interTextureID[idx]);
		m_interTextureID[idx] = NULL;
	}

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

void RenderPipe_blur_opengl::render( void )
{
	render(0);
}

void RenderPipe_blur_opengl::render( GLuint target )
{
	glDepthMask(GL_FALSE);
	// only render a texture to target, no view_projection_matrix needed
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();

	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();

	int sizeScale;

	switch (m_blurLevel)
	{
	case BLUR_L1:
		{
			sizeScale = GetBlurIntenseLevel1();
		} break;
	case BLUR_L2:
		{
			sizeScale = GetBlurIntenseLevel2();
		} break;
	default:
		{
			HR(false);
		} break;
	}
	// m_bufferWidth and m_bufferHeight is the width and height of the main viewport.
	// bufferWidth and bufferHeight is the width and height of this blurred buffer.
	int bufferWidth = m_bufferWidth / sizeScale;
	int bufferHeight= m_bufferHeight / sizeScale;

	float fTexelW = 1.0f/(float)bufferWidth;
	float fTexelH = 1.0f/(float)bufferHeight;

	const int num_samples = KERNELSIZE;

	CVector texOffsetX[num_samples];
	CVector texOffsetY[num_samples];

	//tweak  m_uvOffset and m_weight can change the blur effect.
	for ( int i=0; i<num_samples; i++ )
	{
		texOffsetX[i].Set(m_uvOffset[i] * fTexelW, 0.0f, 0.0f, m_weight[i]);
		texOffsetY[i].Set(0.0f, m_uvOffset[i] * fTexelH, 0.0f, m_weight[i]);
	}

	glUseProgram(m_pipeProgramID);

	GLint regOffset = glGetUniformLocation(m_pipeProgramID, "u_texOffset");

	GLuint interBufferID = m_interBufferID[m_blurLevel];
	GLuint interTextureID = m_interTextureID[m_blurLevel];

	glPushAttrib(GL_VIEWPORT_BIT);
	// horizontal blur first, then vertical blur. Of course it can be finished in one step, but that require reading 81 pixels, and the efficiency is no better.
	glViewport(0, 0, bufferWidth, bufferHeight);
	// `horizontal blur
	{
		glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, interBufferID);
		glBindTexture(GL_TEXTURE_2D, m_texture);
		if ( regOffset>=0 )
			glUniform4fv(regOffset, num_samples, (float *)texOffsetX);
		
		renderScreenQuad();
	}
	// `vertical blur
	{
		glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, target);
		glBindTexture(GL_TEXTURE_2D, interTextureID);
		if ( regOffset>=0 )
			glUniform4fv(regOffset, num_samples, (float *)texOffsetY);
		renderScreenQuad();
	}

	glUseProgram(0);

	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();

	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);
	glDepthMask(GL_TRUE);

	glPopAttrib();

}

void RenderPipe_blur_opengl::setArgs( BLUR_LEVEL blurLevel, GLuint texture )
{
	m_blurLevel = blurLevel;
	m_texture = texture;
}