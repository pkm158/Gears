/************************************************************************/
/* Author:  Kemi Peng 
History: 08/03/2015 by Kemi Peng*/
/************************************************************************/
#include "RenderEntity_opengl.h"

RenderEntity_opengl::RenderEntity_opengl( void ) :
									m_isAltered(false),
									m_pos(0.f, 0.f, 0.f),
									m_rotate(0.f, 0.f, 0.f),
									m_scale(1.f, 1.f, 1.f),
									IRenderEntity(true, true)
{
	m_worldMat.Identity();
}

RenderEntity_opengl::~RenderEntity_opengl( void )
{
	release();
}

bool RenderEntity_opengl::loadFromFile( const char *file, const char *specMap /* = NULL*/, const char *normalMap /*= NULL*/, const char *emissionMap /*= NULL*/ )
{
	if (!file)
	{
		ALERT("file is null!!!");
	}

	if (specMap)
	{
		this->hasSpecMap = true;
	} else 
	{
		this->hasSpecMap = false;
	}

	if (normalMap)
	{
		this->hasNormalMap = true;
	} else 
	{
		this->hasNormalMap = false;
	}

	if (emissionMap)
	{
		this->hasEmissionMap = true;
	} else
	{
		this->hasEmissionMap = false;
	}

	char fileDir [256];
	GetDirFromFile(fileDir, file);

	DEBUG_PRINT("Render entity load from file %s", file);

	CGutModel::SetTexturePath(fileDir);
	HR( IRenderEntity::loadFromFile(file) );

	/************************************************************************/
	/* TO DO. Need to change the gma file format to make it support those textures*/
	/************************************************************************/

	if(specMap != NULL)
	{
		HR(IRenderEntity::loadExternTexture(SPEC_MAP_CHANNEL, specMap));
	}

	if(normalMap != NULL)
	{
		HR(IRenderEntity::loadExternTexture(NORMAL_MAP_CHANNEL, normalMap));
	}

	if (emissionMap != NULL)
	{
		HR(IRenderEntity::loadExternTexture(EMISSION_MAP_CHANNEL, emissionMap));
	}

	return m_model_opengl.ConvertToOpenGLModel(&m_model);
}

void RenderEntity_opengl::render( void )
{
	m_model_opengl.Render();
}

void RenderEntity_opengl::scale( float x, float y, float z )
{
	m_isAltered = true;
	m_scale.x = x;
	m_scale.y = y;
	m_scale.z = z;
}

void RenderEntity_opengl::scale( float uni )
{
	m_isAltered = true;
	scale(uni, uni, uni);
}

void RenderEntity_opengl::move( float x, float y, float z )
{
	m_isAltered = true;
	m_pos.x = x;
	m_pos.y = y;
	m_pos.z = z;
}

void RenderEntity_opengl::move( const CVector move )
{
	m_isAltered = true;
	m_pos = move;
}
void RenderEntity_opengl::rotateX( const float rads )
{
	m_isAltered = true;
	m_rotate.x = rads;
}

void RenderEntity_opengl::rotateY( const float rads )
{
	m_isAltered = true;
	m_rotate.y = rads;
}

void RenderEntity_opengl::rotateZ( const float rads )
{
	m_isAltered = true;
	m_rotate.z = rads;
}

const CMatrix& RenderEntity_opengl::getWorldMat( void )
{
	if (m_isAltered)
	{
		m_worldMat.Identity();

		m_worldMat.RotateX_LH(m_rotate.x);
		m_worldMat.RotateY_LH(m_rotate.y);
		m_worldMat.RotateZ_LH(m_rotate.z);

		m_worldMat.Scale(m_scale.x, m_scale.y, m_scale.z);
		m_worldMat.Move_LH(m_pos.x, m_pos.y, m_pos.z);
		m_isAltered = false;
	}

	return m_worldMat;

}

bool RenderEntity_opengl::init( const char *file, bool hasShadow/*=false*/, bool canBeShadowed/*=false*/, bool isLight/*=false*/,
							   bool canBeLighted/*=true*/,const char *specMap/*=NULL*/, const char * normalMap/*=NULL*/, const char *emissionMap /*= NULL*/,
							   int diffuseMapNum /*= 1*/, float diffuseMapsScale1 /*= 1.0f*/, float diffuseMapScale2 /*= 0.0f*/, float diffuseMapScale3/* = 0.0f*/)
{
	float diffuseMapsScale[3] = {diffuseMapsScale1, diffuseMapScale2, diffuseMapScale3};
	this->hasShadow = hasShadow;
	this->canBeShadowed = canBeShadowed;

	this->isLight = isLight;
	this->canBeLighted = canBeLighted;

	m_diffuseMapNum = diffuseMapNum;
	m_diffuseMapScale = new float [diffuseMapNum];
	for (int idx=0; idx<diffuseMapNum; idx++)
	{
		m_diffuseMapScale[idx] = diffuseMapsScale[idx];
	}
	
	return loadFromFile(file, specMap, normalMap, emissionMap);
}

void RenderEntity_opengl::release( void )
{
	SAFE_DELETE_ARRAY(m_diffuseMapScale);
	m_model_opengl.Release();
}