#include "Device_D3D9.h"

Device_D3D9::Device_D3D9()
{

}
Device_D3D9::~Device_D3D9(void)
{
//	SAFE_DELETE( m_pDevice );
//	SAFE_DELETE( m_pDirect );
}


bool Device_D3D9::initD3D( HWND hwnd )
{
	HWND hWnd = hwnd;

	RECT rect;
	GetWindowRect(hWnd, &rect);

	long w = rect.right - rect.left;
	long h = rect.bottom - rect.top;
	if ( w==0 || h==0 )
		return false;


	// `���һ��D3D9����, ����Ψһ������ȥ����������������ͼ��D3D9 Device.`
//	if( NULL == ( m_pDirect = Direct3DCreate9( D3D_SDK_VERSION ) ) )
		return false;

	ZeroMemory( &m_D3DPresent, sizeof(m_D3DPresent) );
	m_D3DPresent.Windowed = TRUE;
	m_D3DPresent.SwapEffect = D3DSWAPEFFECT_DISCARD;
	m_D3DPresent.BackBufferFormat = D3DFMT_UNKNOWN;
	m_D3DPresent.BackBufferCount = 1; // `�ṩһ��backbuffer`
	m_D3DPresent.EnableAutoDepthStencil = TRUE;
	m_D3DPresent.AutoDepthStencilFormat = D3DFMT_D24S8;
	m_D3DPresent.MultiSampleType = D3DMULTISAMPLE_NONE;
	m_D3DPresent.hDeviceWindow = hWnd;

	bool device_initialized = false;

	/*

	`������4�ֲ�ͬ��������d3d9`
	1. D3DDEVTYPE_HAL + D3DCREATE_HARDWARE_VERTEXPROCESSING 
	`��ȫ����GPU����`
	2. D3DDEVTYPE_HAL + D3DCREATE_MIXED_VERTEXPROCESSING	
	`Vertex����CPU��GPU������, ������GPU.`
	3. D3DDEVTYPE_HAL + D3DCREATE_SOFTWARE_VERTEXPROCESSING 
	`Vertex��CPU, ������GPU.`
	4. D3DDEVTYPE_REF + D3DCREATE_SOFTWARE_VERTEXPROCESSING	
	`��ȫ��CPU����, ���Ǹ��ǳ�����ģʽ.`

	*/

	const int device_types = 4;

	struct sDeviceType
	{
		D3DDEVTYPE type;
		DWORD behavior;
	};

	sDeviceType device_type[device_types] = 
	{
		{D3DDEVTYPE_HAL, D3DCREATE_HARDWARE_VERTEXPROCESSING},
		{D3DDEVTYPE_HAL, D3DCREATE_MIXED_VERTEXPROCESSING},
		{D3DDEVTYPE_HAL, D3DCREATE_SOFTWARE_VERTEXPROCESSING},
		{D3DDEVTYPE_REF, D3DCREATE_SOFTWARE_VERTEXPROCESSING}
	};

	for ( int type=0; type<device_types; type++ )
	{
		// `����ȥ��һ��Direct3D9�豸`
		if( m_pDirect->CreateDevice( D3DADAPTER_DEFAULT, device_type[type].type, hWnd,
			device_type[type].behavior, &m_D3DPresent, &m_pDevice )==D3D_OK )
		{
			device_initialized = true;
			break;
		}
	}
	return device_initialized;
}

LPDIRECT3DDEVICE9 Device_D3D9::getD3DDevice( void )
{
	return m_pDevice;
}
