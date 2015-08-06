/************************************************************************/
/* Author:  Kemi Peng 
History: 08/03/2015 by Kemi Peng*/
/************************************************************************/

#include "IRenderEntity.h"

IRenderEntity::IRenderEntity(void) :
						hasShadow(false),
						canBeShadowed(false),
						hasSpecMap(false),
						hasNormalMap(false)
{

}

IRenderEntity::IRenderEntity( bool hasShadow, bool canBeShadowed ) :
										hasShadow(hasShadow),
										canBeShadowed(canBeShadowed)
{

}
IRenderEntity::~IRenderEntity(void)
{

}

bool IRenderEntity::loadFromFile( const char *file )
{
	return m_model.Load_ASCII(file);
}

bool IRenderEntity::loadExternTexture( int channel, const char *file )
{
	return m_model.LoadExternTexture(channel, file);
}