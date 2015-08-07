/************************************************************************/
/* Author:  Kemi Peng 
History: 08/03/2015 by Kemi Peng*/
/************************************************************************/

#ifndef RENDERCOMMONHEAD
#define RENDERCOMMONHEAD

#include "libmath.h"
#include <list>
#ifdef WIN32

#ifdef D3D10_CORE
#include <d3d10.h>
#endif

#ifdef OPENGL_CORE
#include "glew.h"
#include "wglew.h"
#include <GL/gl.h>
#pragma comment(lib, "opengl32.lib")
#pragma comment(lib, "glu32.lib")
#pragma comment(lib, "glew32.lib")
#endif

#endif
#define DIFF0_MAP_CHANNEL		0
#define DIFF1_MAP_CHANNEL		1
#define DIFF2_MAP_CHANNEL		2
#define EMISSION_MAP_CHANNEL	3
#define NORMAL_MAP_CHANNEL		4
#define SPEC_MAP_CHANNEL		5
#define ENVIRONMENT_MAP_CHANNEL 6
#define SHADOW_MAP_CHANNEL		7
#define NOTSPECIALFIED_CHANNEL  8

// for fixed pipeline material
#define MAX_NUM_TEXTURES 9
#define MAX_TEXCOORDS 9

// auto gen map channel ID
#define MAP_SPHEREMAP 32
#define MAP_CUBEMAP   33
#define MAP_NOOVERWRITE 0xff

#define D3D_OK   S_OK


#define DXUTERR_NODIRECT3D              MAKE_HRESULT(SEVERITY_ERROR, FACILITY_ITF, 0x0901)
#define DXUTERR_NOCOMPATIBLEDEVICES     MAKE_HRESULT(SEVERITY_ERROR, FACILITY_ITF, 0x0902)
#define DXUTERR_MEDIANOTFOUND           MAKE_HRESULT(SEVERITY_ERROR, FACILITY_ITF, 0x0903)
#define DXUTERR_NONZEROREFCOUNT         MAKE_HRESULT(SEVERITY_ERROR, FACILITY_ITF, 0x0904)
#define DXUTERR_CREATINGDEVICE          MAKE_HRESULT(SEVERITY_ERROR, FACILITY_ITF, 0x0905)
#define DXUTERR_RESETTINGDEVICE         MAKE_HRESULT(SEVERITY_ERROR, FACILITY_ITF, 0x0906)
#define DXUTERR_CREATINGDEVICEOBJECTS   MAKE_HRESULT(SEVERITY_ERROR, FACILITY_ITF, 0x0907)
#define DXUTERR_RESETTINGDEVICEOBJECTS  MAKE_HRESULT(SEVERITY_ERROR, FACILITY_ITF, 0x0908)
#define DXUTERR_DEVICEREMOVED           MAKE_HRESULT(SEVERITY_ERROR, FACILITY_ITF, 0x090A)

struct STREAM_OUT_VERTEX
{
	CVector Pos;
	CVector Norm;
	CVector Tex;
	CVector Tangent;
};

struct Mtrl 
{
	Mtrl( void )
		:ambient( CVector(0.f,0.f,0.f,1.f) ), diffuse( CVector(0.f,0.f,0.f,1.f) ), spec( CVector(0.f,0.f,0.f,1.f) ),
		emissive( CVector(0.f,0.f,0.f,1.f) ), specPower(8.0f){}
	Mtrl(const CVector& a, const CVector& d, 
		const CVector& s, const CVector& e, float power)
		:ambient(a), diffuse(d), emissive(e), spec(s), specPower(power){}

	CVector ambient;
	CVector diffuse;
	CVector spec;
	CVector emissive;
	float   specPower;
};

#define LIGHT_POINT			0
#define LIGHT_SPOT			1
#define LIGHT_DIRECTIONAL	2

struct Light
{
	// position
	CVector position;
	// color & intensity
	union{
		struct
		{
			CVector ambient;
			CVector diffuse;
			CVector specular;
			CVector direction;
		};
		float dirLightData[4][4];
	};
	// attenuation
	CVector attenuation;

	// type & enable/disable
	int	   lightType;
	int	   enabled;
	// int	   padding[2];

	Light(void)
	{
		position.Set(0.0f, 0.0f, 0.0f, 1.0f);
		direction.Set(0.0f, 0.0f, 1.0f, 0.0f);

		ambient.Set(1.0f);
		diffuse.Set(1.0f);
		specular.Set(0.0f);

		attenuation.Set(1.0f, 0.0f, 0.0f);

		lightType = LIGHT_POINT;
		enabled = false;
	}
};

struct sVertexDecl
{
	// position
	int m_iPositionOffset;
	int m_iNumPositionElements;
	// normal
	int m_iNormalOffset;
	int m_iNumNormalElements;
	// color
	int m_iColorOffset;
	int m_iNumColorElements;
	// texcoord
	int m_iTexcoordOffset[MAX_TEXCOORDS];
	int m_iNumTexcoordElements[MAX_TEXCOORDS];
	// tangent
	int m_iTangentOffset;
	int m_iNumTangentElements;
	// binormal
	int m_iBiNormalOffset;
	int m_iBiNormalElements;
	// stride
	int m_iVertexSize;

	sVertexDecl(void)
	{
		m_iPositionOffset = 0;
		m_iNumPositionElements = 3;
		m_iNormalOffset = -1;
		m_iNumNormalElements = 0;
		m_iColorOffset = -1;
		m_iNumColorElements = 0;
		for ( int i=0; i<MAX_TEXCOORDS; i++ )
		{
			m_iTexcoordOffset[i] = -1;
			m_iNumTexcoordElements[i] = 0;
		}
		m_iTangentOffset = -1;
		m_iNumTangentElements = 0;
		m_iBiNormalOffset = -1;
		m_iBiNormalElements = 0;
		m_iVertexSize = 0;
	}

	void SetVertexFormat(int PosElems, int NormalElems, int ColorElems, 
		int Tex0Elems=0, int Tex1Elems=0, int Tex2Elems=0, int Tex3Elems=0);
};

typedef std::list<Light *> LightList;

#endif