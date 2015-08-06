/************************************************************************/
/* Author:  Kemi Peng 
History: 08/03/2015 by Kemi Peng*/
/************************************************************************/

#ifndef TIMEUTILITY
#define TIMEUTILITY

#include "GearsCommonHead.h"
#include "IUtility.h"

class TimeUtility:protected IUtility
{
public:
	TimeUtility( void );
	virtual ~TimeUtility( void );

	virtual void update( double tt, double dt ){};
	virtual void postInitiate(void){};
	void start( void );
	void reset( void );
	void stop( void );
	double getTime( void );

private:
	enum TimeState
	{
		TIMESTATE_RUN = 0,
		TIMESTATE_STOP = 1,
		TIMESTATE_RESET = 2
	};
	long long m_currTimeStamp;
	long long m_preTimeStamp;
	double m_secsPerCnt;
	double	m_stopTimeCount;
	TimeState m_timeState;
	
};
#endif
