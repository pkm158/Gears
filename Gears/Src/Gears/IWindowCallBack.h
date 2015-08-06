/************************************************************************/
/* Author:  Kemi Peng 
History: 08/03/2015 by Kemi Peng*/
/************************************************************************/
#ifndef IWINDOWCALLBACK
#define IWINDOWCALLBACK

class IWindowCallBack
{
public:
	virtual ~IWindowCallBack( void ){};
	virtual void onLost( void ) = NULL;
	virtual void onReset( void ) = NULL;
};

#endif