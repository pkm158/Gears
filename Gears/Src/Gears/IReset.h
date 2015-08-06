/************************************************************************/
/* Author:  Kemi Peng 
History: 08/03/2015 by Kemi Peng*/
/************************************************************************/
#ifndef IRESET
#define IRESET

class IReset
{
public:
	virtual ~IReset( void ){};
	virtual bool reset( void ) = NULL;
};

#endif