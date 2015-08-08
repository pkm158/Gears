/************************************************************************/
/* Author:  Kemi Peng 
History: 08/03/2015 by Kemi Peng*/
/************************************************************************/

#include "RenderCore.h"
#include "Window_Win32.h"
#include "UtilityManager.h"
#include "RenderPipeManager.h"
RenderCore::RenderCore()
{
}

RenderCore::~RenderCore()
{
#ifdef D3D10_CORE
	SAFE_DELETE(m_pDevice_D3D10);
#endif

#ifdef OPENGL_CORE
	SAFE_DELETE(m_pDevice_opengl);
#endif
	RenderEntityManager::ReleaseRenderEntityManager();
	RenderPipeManager::ReleaseRenderPipeManager();
}

void RenderCore::initDevice( WindowCallBackManager *winCallBack )
{
#ifdef D3D10_CORE
	DEBUG_PRINT("initiate d3d10 device...");
	m_pDevice_D3D10 =  new Device_D3D10((((Window_Win32 *)Window_Win32::GetEngineWindow())->getWindowHandleWin32())); //���ڹ��캯����new����Ϊ���ܻ����gWindow_Win32Ϊnull�����
	HR( m_pDevice_D3D10->initD3D() );
	winCallBack->addReset( m_pDevice_D3D10 );
#endif

#ifdef OPENGL_CORE
	DEBUG_PRINT("initiate opengl device...");
	m_pDevice_opengl = new Device_opengl((((Window_Win32 *)Window_Win32::GetEngineWindow())->getWindowHandleWin32()));
	HR(m_pDevice_opengl->initOpengl());
	winCallBack->addReset(m_pDevice_opengl);
#endif

	RenderEntityManager::CreateRenderEntityManager();
	RenderPipeManager::CreateRenderPipeManager();

	RenderPipeManager::GetHandle()->init(m_pDevice_opengl);

//	HR(m_renderPipe.init(m_pDevice_opengl));
}

void RenderCore::draw( void )
{
	float dt = 0.001f;
	double tt = 0.f;
	CVector vClearColor( 0.4f, 0.5f, 0.6f, 1.0f );

	bool isShowFrame = true;

#ifdef D3D10_CORE
	ID3D10Device* device;
	device = m_pDevice_D3D10->getD3DDevice();
	// `�����ɫ`
	device->ClearRenderTargetView( m_pDevice_D3D10->getRenderTargetView(), (float *)&vClearColor );
	// `���Depth/Stencil buffer`
	device->ClearDepthStencilView( m_pDevice_D3D10->getDepthStencilView(), D3D10_CLEAR_DEPTH | D3D10_CLEAR_STENCIL, 1.0f, 0 );

	// ��ʼ�»�ͼָ��
	// ��������


	//
	// �ѱ���backbuffer��ʾ����
	m_pDevice_D3D10->getSwapChain()->Present( 0, 0 );
#endif

#ifdef OPENGL_CORE
	
	glClearColor(1.0f, 0.0f, 0.0f, 1.0f);
 	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

 	glEnable(GL_DEPTH_TEST);
	glDisable(GL_ALPHA_TEST);
	glDepthFunc(GL_LEQUAL);

	GetRenderPipe(PIPE_MAIN)->render();

 	SwapBuffers(m_pDevice_opengl->getHDC());
#endif
}