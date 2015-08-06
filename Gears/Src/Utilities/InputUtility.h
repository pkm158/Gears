/************************************************************************/
/* Author:  Kemi Peng 
History: 08/03/2015 by Kemi Peng*/
/************************************************************************/

#ifndef INPUTUTILITY
#define INPUTUTILITY
#include "StandardInput.h"
#include "GearsCommonHead.h"
#include "IUtility.h"
#include "PadInput.h"
enum KeyType
{
	KUP = 0,
	KLEFT,
	KDOWN,
	KRIGHT,
	KA,
	KB,
	KX,
	KY,
	KLB,
	KRB,
	KLT,
	KRT,
	KBACK,
	KNEXT
};

class InputUtility : protected IUtility
{
public:
	InputUtility(void);
	virtual ~InputUtility(void);
	virtual void update( double tt, double dt );
	virtual void postInitiate(void);
	int getKeyState(KeyType key) const;
	bool isKey(unsigned char key) const;

private:
	PadInput m_padInput;
	StandardInput m_standardInput;
	
};

#endif