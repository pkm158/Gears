/************************************************************************/
/* Author:  Kemi Peng 
History: 08/03/2015 by Kemi Peng*/
/************************************************************************/

#include "TimeUtility.h"

TimeUtility::TimeUtility( void ):
					m_currTimeStamp( 0 ),
					m_preTimeStamp( 0 ),
					m_stopTimeCount( 0 ),
					m_timeState( TIMESTATE_RESET )
{
	DEBUG_PRINT("create time utility...");
	m_UtilityType = IUtility::UTILITY_TIME;

	long long  cntsPerSec;
#ifdef WIN32
	QueryPerformanceFrequency((LARGE_INTEGER*)&cntsPerSec);
#endif
	m_secsPerCnt = 1.0f / (double)cntsPerSec;
}

TimeUtility::~TimeUtility( void )
{

}

void TimeUtility::start( void )
{
	if ( m_timeState!=TIMESTATE_RUN )
	{
		m_timeState = TIMESTATE_RUN;
#ifdef WIN32
		QueryPerformanceCounter((LARGE_INTEGER*)&m_preTimeStamp);
#endif
	}
}

void TimeUtility::reset( void )
{
	m_timeState = TIMESTATE_RESET;
}

void TimeUtility::stop( void )
{
	if ( m_timeState!=TIMESTATE_STOP )
	{
		m_timeState = TIMESTATE_STOP;
#ifdef WIN32
		QueryPerformanceCounter((LARGE_INTEGER*)&m_currTimeStamp);
#endif
		m_stopTimeCount = (m_currTimeStamp - m_preTimeStamp)*m_secsPerCnt;
	}	
}

double TimeUtility::getTime( void )
{
	switch( m_timeState )
	{
	case TIMESTATE_RUN :
		{
#ifdef WIN32
			QueryPerformanceCounter((LARGE_INTEGER*)&m_currTimeStamp);
#endif
			return (m_currTimeStamp - m_preTimeStamp)*m_secsPerCnt;
		}break;
	case TIMESTATE_STOP :
		{
			return m_stopTimeCount;
		}break;
	case TIMESTATE_RESET :
		{
			return 0;
		}break;
	default:
		return 0;
		break;
	}
}