#ifndef DEVICE_D3D9
#define DEVICE_D3D9


#include "GearsCommonHead.h"
#include "RenderCommonHead.h"

class Device_D3D9
{
public:
	Device_D3D9( void );
	~Device_D3D9(void);

	bool initD3D( HWND hwnd );

	LPDIRECT3DDEVICE9 getD3DDevice( void );


private:
	LPDIRECT3DDEVICE9 m_pDevice;
	LPDIRECT3D9 m_pDirect;
	D3DPRESENT_PARAMETERS m_D3DPresent;
};

#endif