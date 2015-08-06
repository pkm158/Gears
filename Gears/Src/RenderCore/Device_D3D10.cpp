/************************************************************************/
/* Author:  Kemi Peng 
History: 08/03/2015 by Kemi Peng*/
/************************************************************************/
#include "Device_D3D10.h"
#ifdef D3D10_CORE
Device_D3D10::Device_D3D10( HWND hwnd ):
					m_pd3dDevice( NULL ),
					m_pSwapChain( NULL ),
					m_pDepthStencil( NULL ),
					m_pRenderTargetView( NULL ),
					m_pDepthStencilView( NULL ),
					m_pDefaultEffect( NULL ),
					m_pDefaultTechnique( NULL ),
					m_pSamplerState( NULL ),
					m_pCull( NULL ),
					m_pHLSL_Macros( NULL ),
					m_hWnd( hwnd )
{
}

Device_D3D10::~Device_D3D10(void)
{
	SAFE_RELEASE( m_pd3dDevice );
	SAFE_RELEASE( m_pSwapChain );
	SAFE_RELEASE( m_pDepthStencil );
	SAFE_RELEASE( m_pRenderTargetView );
	SAFE_RELEASE( m_pDepthStencilView );
	SAFE_RELEASE( m_pDefaultEffect );
	SAFE_DELETE( m_pDefaultTechnique );
	SAFE_RELEASE( m_pSamplerState );
	SAFE_RELEASE( m_pCull );
	SAFE_DELETE( m_pHLSL_Macros );

}

ID3D10Device* Device_D3D10::getD3DDevice( void )
{
	return m_pd3dDevice;
}

bool Device_D3D10::setDefaultState( void )
{
	{
		D3D10_SAMPLER_DESC desc;
		setDefaultSamplerDesc(desc);

		desc.Filter = D3D10_FILTER_MIN_MAG_MIP_LINEAR;
		desc.AddressU = D3D10_TEXTURE_ADDRESS_WRAP;
		desc.AddressV = D3D10_TEXTURE_ADDRESS_WRAP;
		desc.AddressW = D3D10_TEXTURE_ADDRESS_WRAP;

		m_pd3dDevice->CreateSamplerState( &desc, &m_pSamplerState );

		for ( int i=0; i<4; i++ )
		{
			m_pd3dDevice->PSSetSamplers( i, 1, &m_pSamplerState);
		}
	}
	// rasterizer state
	{
		D3D10_RASTERIZER_DESC desc;
		setDefaultRasterizerDesc(desc);

		desc.CullMode = D3D10_CULL_NONE;
		desc.FrontCounterClockwise = true;

		if ( D3D_OK != m_pd3dDevice->CreateRasterizerState( &desc, &m_pCull ) )
			return false;

		m_pd3dDevice->RSSetState( m_pCull );
	}

	return true;
}

void Device_D3D10::setDefaultSamplerDesc( D3D10_SAMPLER_DESC &desc )
{
	ZeroMemory(&desc, sizeof(desc));

	desc.Filter = D3D10_FILTER_MIN_MAG_MIP_POINT;

	desc.AddressU = D3D10_TEXTURE_ADDRESS_CLAMP;
	desc.AddressV = D3D10_TEXTURE_ADDRESS_CLAMP;
	desc.AddressW = D3D10_TEXTURE_ADDRESS_CLAMP;

	desc.ComparisonFunc = D3D10_COMPARISON_NEVER;

	desc.BorderColor[0] = desc.BorderColor[1] = desc.BorderColor[2] = desc.BorderColor[3] = 0;
	desc.MinLOD = 0;
	desc.MaxLOD = D3D10_FLOAT32_MAX;
	desc.MipLODBias = 0.0f;
	desc.MaxAnisotropy = 16;
}

void Device_D3D10::setDefaultRasterizerDesc( D3D10_RASTERIZER_DESC &desc )
{
	ZeroMemory(&desc, sizeof(desc));

	desc.FillMode = D3D10_FILL_SOLID;
	desc.CullMode = D3D10_CULL_NONE;
	desc.FrontCounterClockwise = FALSE;
	desc.DepthBias = 0;
	desc.DepthBiasClamp = 0.0f;
	desc.SlopeScaledDepthBias = 0.0f;
	desc.DepthClipEnable = TRUE;
	desc.ScissorEnable = FALSE;
	desc.MultisampleEnable = FALSE;
	desc.AntialiasedLineEnable = FALSE;
}

void Device_D3D10::setDefaultBlendDesc( D3D10_BLEND_DESC &desc )
{
	ZeroMemory(&desc, sizeof(desc));

	desc.AlphaToCoverageEnable = FALSE;

	desc.SrcBlend = D3D10_BLEND_ONE;
	desc.DestBlend = D3D10_BLEND_ZERO;
	desc.BlendOp = D3D10_BLEND_OP_ADD;

	desc.SrcBlendAlpha = D3D10_BLEND_ONE;
	desc.DestBlendAlpha = D3D10_BLEND_ZERO;
	desc.BlendOpAlpha = D3D10_BLEND_OP_ADD;

	for ( int i=0; i<8; i++ )
	{
		desc.RenderTargetWriteMask[i] = D3D10_COLOR_WRITE_ENABLE_ALL;
	}
}

bool Device_D3D10::reset( void )
{
	if ( m_pd3dDevice==NULL )
		return false;

	DWORD hr;

	// `获得窗口大小`
	RECT rect;
	GetWindowRect( m_hWnd, &rect );

	int width = rect.right - rect.left;
	int height = rect.bottom - rect.top;

	// `先把原来设置用来绘图的framebuffer和depthbuffer拿走`
	m_pd3dDevice->OMSetRenderTargets(0, NULL, NULL);

	// `释放framebuffer和depthbuffer的资源`
	SAFE_RELEASE(m_pRenderTargetView);
	SAFE_RELEASE(m_pDepthStencil);
	SAFE_RELEASE(m_pDepthStencilView);

	DXGI_MODE_DESC desc;
	ZeroMemory(&desc, sizeof(desc));

	desc.Width = width;
	desc.Height = height;
	desc.RefreshRate.Numerator = 60;
	desc.RefreshRate.Denominator = 1;
	desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
	desc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_PROGRESSIVE;
	// `改变swapchain的大小`
	if ( D3D_OK!=m_pSwapChain->ResizeBuffers(1, width, height, DXGI_FORMAT_R8G8B8A8_UNORM_SRGB, 0) )
		return false;

	// `取出第一个display buffer`
	ID3D10Texture2D *pBuffer = NULL;
	if ( D3D_OK!=m_pSwapChain->GetBuffer( 0, __uuidof( ID3D10Texture2D ), (LPVOID*)&pBuffer ) )
		return false;
	// `打开一个合乎display buffer格式的RenderTargetView`
	hr = m_pd3dDevice->CreateRenderTargetView( pBuffer, NULL, &m_pRenderTargetView );
	pBuffer->Release();
	if( FAILED( hr ) )
		return false;

	// `设置Depth/Stencil buffer的数据格式`
	D3D10_TEXTURE2D_DESC descDepth;
	descDepth.Width = width;
	descDepth.Height = height;
	descDepth.MipLevels = 1;
	descDepth.ArraySize = 1;
	descDepth.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	descDepth.SampleDesc.Count = 1;
	descDepth.SampleDesc.Quality = 0;
	descDepth.Usage = D3D10_USAGE_DEFAULT;
	descDepth.BindFlags = D3D10_BIND_DEPTH_STENCIL;
	descDepth.CPUAccessFlags = 0;
	descDepth.MiscFlags = 0;
	// `分配Depth/Stencil buffer的空间`
	hr = m_pd3dDevice->CreateTexture2D( &descDepth, NULL, &m_pDepthStencil );
	if( FAILED(hr) )
		return false;

	// `设置Depth/Stencil View的格式`
	D3D10_DEPTH_STENCIL_VIEW_DESC descDSV;
	descDSV.Format = descDepth.Format;
	descDSV.ViewDimension = D3D10_DSV_DIMENSION_TEXTURE2D;
	descDSV.Texture2D.MipSlice = 0;
	// `打开DepthStencil Buffer的View`
	hr = m_pd3dDevice->CreateDepthStencilView( m_pDepthStencil, &descDSV, &m_pDepthStencilView );
	if( FAILED(hr) )
		return false;

	// `设置绘图的结果会存放在什么地方`
	// RenderTargetView = RGBA Color Buffer resource
	// DepthStencilView = Depth/Stencil Buffer resource
	m_pd3dDevice->OMSetRenderTargets(1, &m_pRenderTargetView, m_pDepthStencilView);

	// `Viewport用来指定3D绘图的画布范围`
	// `把Viewport设置成和整个窗口一样大`
	D3D10_VIEWPORT vp;
	vp.Width = width;
	vp.Height = height;
	vp.MinDepth = 0.0f;
	vp.MaxDepth = 0.5f;
	vp.TopLeftX = 0;
	vp.TopLeftY = 0;

	m_pd3dDevice->RSSetViewports( 1, &vp );

	return true;
}

bool Device_D3D10::initD3D( void )
{
	HRESULT hr = S_OK;

	// `获得窗口大小`
	RECT rc;
	GetClientRect( m_hWnd, &rc );
	UINT width = rc.right - rc.left;
	UINT height = rc.bottom - rc.top;

	UINT createDeviceFlags = 0;
#ifdef _DEBUG
	createDeviceFlags |= D3D10_CREATE_DEVICE_DEBUG;
#endif

	// `设置Frontbuffer/Backbuffer大小及格式`
	DXGI_SWAP_CHAIN_DESC sd;
	ZeroMemory( &sd, sizeof(sd) );
	sd.BufferCount = 1;
	sd.BufferDesc.Width = width;
	sd.BufferDesc.Height = height;
	sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
	sd.BufferDesc.RefreshRate.Numerator = 60;
	sd.BufferDesc.RefreshRate.Denominator = 1;
	sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	sd.OutputWindow = m_hWnd;
	sd.SampleDesc.Count = 1;
	sd.SampleDesc.Quality = 0;
	//sd.SwapEffect = DXGI_SWAP_EFFECT_SEQUENTIAL;
	//sd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
	sd.Windowed = TRUE;

	// `打开Frontbuffer/Backbuffer`
	hr = D3D10CreateDeviceAndSwapChain( NULL, D3D10_DRIVER_TYPE_HARDWARE, NULL, createDeviceFlags, D3D10_SDK_VERSION, &sd, &m_pSwapChain, &m_pd3dDevice );
	if( FAILED(hr) ) 
		return false;

	// `取出第一个display buffer`
	ID3D10Texture2D *pBuffer = NULL;
	hr = m_pSwapChain->GetBuffer( 0, __uuidof( ID3D10Texture2D ), (LPVOID*)&pBuffer );
	if( FAILED(hr) ) 
		return false;
	// `打开一个合乎display buffer格式的RenderTargetView`
	hr = m_pd3dDevice->CreateRenderTargetView( pBuffer, NULL, &m_pRenderTargetView );

	pBuffer->Release();
	if( FAILED( hr ) ) 
		return false;

	// `设置Depth/Stencil buffer的数据格式`
	D3D10_TEXTURE2D_DESC descDepth;
	descDepth.Width = width;
	descDepth.Height = height;
	descDepth.MipLevels = 1;
	descDepth.ArraySize = 1;
	descDepth.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	descDepth.SampleDesc.Count = 1;
	descDepth.SampleDesc.Quality = 0;
	descDepth.Usage = D3D10_USAGE_DEFAULT;
	descDepth.BindFlags = D3D10_BIND_DEPTH_STENCIL;
	descDepth.CPUAccessFlags = 0;
	descDepth.MiscFlags = 0;
	// `分配Depth/Stencil buffer的空间`
	hr = m_pd3dDevice->CreateTexture2D( &descDepth, NULL, &m_pDepthStencil );
	if( FAILED(hr) )
		return false;

	// `设置Depth/Stencil View的格式`
	D3D10_DEPTH_STENCIL_VIEW_DESC descDSV;
	descDSV.Format = descDepth.Format;
	descDSV.ViewDimension = D3D10_DSV_DIMENSION_TEXTURE2D;
	descDSV.Texture2D.MipSlice = 0;
	// `打开DepthStencil Buffer的View`
	hr = m_pd3dDevice->CreateDepthStencilView( m_pDepthStencil, &descDSV, &m_pDepthStencilView );
	if( FAILED(hr) )
		return false;

	setDefaultState();

	// `设置绘图的结果会存放在什么地方`
	// RenderTargetView = RGBA Color Buffer resource
	// DepthStencilView = Depth/Stencil Buffer resource
	m_pd3dDevice->OMSetRenderTargets(1, &m_pRenderTargetView, m_pDepthStencilView);

	setDefaultState();

	// `Viewport用来指定3D绘图的画布范围, 在此把Viewport设置成和整个窗口一样大`
	D3D10_VIEWPORT vp;

	vp.Width = width;
	vp.Height = height;
	vp.MinDepth = 0.0f;
	vp.MaxDepth = 1.0f;
	vp.TopLeftX = 0;
	vp.TopLeftY = 0;

	m_pd3dDevice->RSSetViewports( 1, &vp );

	return true;
}
#endif