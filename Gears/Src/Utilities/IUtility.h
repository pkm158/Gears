/************************************************************************/
/* Author:  Kemi Peng 
History: 08/03/2015 by Kemi Peng*/
/************************************************************************/

#ifndef IUTILITY
#define IUTILITY

class IUtility
{
public:
	enum UtilityType
	{
		UTILITY_CONFIG = 0,
		UTILITY_TIME,
		UTILITY_INPUT,
		UTILITY_CAMERA,
		UTILITY_STAGE
	};
	virtual ~IUtility( void ){};
	virtual void postInitiate(void) = NULL;
	inline UtilityType getUtilityState( void ) const { return m_UtilityType; };
	virtual void update( double tt, double dt ) = NULL;

protected:
	UtilityType m_UtilityType;
};

#endif