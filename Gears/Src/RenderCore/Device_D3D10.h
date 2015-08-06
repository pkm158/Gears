/************************************************************************/
/* Author:  Kemi Peng 
History: 08/03/2015 by Kemi Peng*/
/************************************************************************/
#ifndef DEVICE_D3D10
#define DEVICE_D3D10

#ifdef D3D10_CORE

#include "GearsCommonHead.h"
#include "RenderCommonHead.h"
#include "IReset.h"

class Device_D3D10 : public IReset
{
public:
	Device_D3D10( HWND hwnd );
	virtual ~Device_D3D10(void);

	bool initD3D( void );
	virtual bool reset( void );

	bool setDefaultState( void );

	void setDefaultSamplerDesc( D3D10_SAMPLER_DESC &desc );

	void setDefaultRasterizerDesc( D3D10_RASTERIZER_DESC &desc );

	void setDefaultBlendDesc( D3D10_BLEND_DESC &desc );

	ID3D10Device* getD3DDevice( void );

	inline ID3D10RenderTargetView* getRenderTargetView( void ){
		return m_pRenderTargetView;
	}

	inline ID3D10DepthStencilView* getDepthStencilView( void ){
		return m_pDepthStencilView;
	}

	inline IDXGISwapChain*		   getSwapChain( void ){
		return m_pSwapChain;
	}
	
	inline ID3D10RasterizerState*  getRasterState( void ){
		return m_pCull;
	}


private:
	HWND					m_hWnd; 
	ID3D10Device			*m_pd3dDevice;
	IDXGISwapChain			*m_pSwapChain;

	ID3D10Texture2D			*m_pDepthStencil;
	ID3D10RenderTargetView	*m_pRenderTargetView;
	ID3D10DepthStencilView	*m_pDepthStencilView;

	ID3D10Effect			*m_pDefaultEffect;
	ID3D10EffectTechnique	*m_pDefaultTechnique;
	ID3D10SamplerState		*m_pSamplerState;
	ID3D10BlendState		*m_pBlendState;
	ID3D10RasterizerState	*m_pCull;
//		OutputSprite
	

	D3D10_SHADER_MACRO		*m_pHLSL_Macros;
};

#endif

#endif