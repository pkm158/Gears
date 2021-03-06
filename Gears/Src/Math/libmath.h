/************************************************************************/
/* Author:  Kemi Peng 
History: 08/03/2015 by Kemi Peng*/
/************************************************************************/
#ifndef LIBMATH_H
#define LIBMATH_H

#include "Matrix4x4_Reference.h"
#include "Vector4_Reference.h"

typedef Matrix4x4CPU CMatrix;
typedef Vector4CPU CVector;

#define PI 3.141593f
#define EPSILON 1e-6

#define RIGHT_ANGLE 1.57079633

inline static CMatrix  MatrixPerspectiveFovLH( CMatrix* pOut,float fovX,float aspect,float znear,float zfar )
{
	float e = 1.0f / tanf(fovX / 2.0f);
	float aspectInv = 1.0f / aspect;
	float fovy = 2.0f * atanf(aspectInv / e);
	float w = 1.0f / tanf(0.5f * fovy);
	float h = w / aspectInv;

	pOut->m_00 = w;
	pOut->m_11 = h;
	pOut->m_22 = zfar / (zfar - znear);
	pOut->m_32 = -znear * zfar / (zfar - znear);
	pOut->m_23 = 1;
	pOut->m_01 = pOut->m_02 = pOut->m_03 = 0;
	pOut->m_10 = pOut->m_12 = pOut->m_13 = 0;
	pOut->m_20 = pOut->m_21 = 0;
	pOut->m_30 = pOut->m_31 = pOut->m_33 = 0;

	return *pOut;
}

inline static CMatrix ViewMatrix(const CVector& from,  const CVector& at, const CVector& world_up,  const float roll)
{
	if(at.x==from.x && at.y==from.y && at.z==from.z)
	{
		return CMatrix::IdentityMatrix();

	}
	CMatrix view = CMatrix::IdentityMatrix();
	CVector up, right, view_dir;

	CVector ss;
	ss.x=at.x-from.x;
	ss.y=at.y-from.y;
	ss.z=at.z-from.z;
	ss.w=0;
	view_dir = VectorNormalize(ss);
	right = Vector3CrossProduct(world_up, view_dir);
	up = Vector3CrossProduct(view_dir, right);

	right = VectorNormalize(right);
	up = VectorNormalize(up);

	view[0][0] = right.x;
	view[1][0] = right.y;
	view[2][0] = right.z;
	view[0][1] = up.x;
	view[1][1] = up.y;
	view[2][1] = up.z;
	view[0][2] = view_dir.x;
	view[1][2] = view_dir.y;
	view[2][2] = view_dir.z;

	view[3][0] = -VectorDotf(right, from);
	view[3][1] = -VectorDotf(up, from);
	view[3][2] = -VectorDotf(view_dir, from);

	// Set roll
	if (roll != 0.0f) 
	{

		float	cosine, sine;

		cosine = (float) cosf(-roll);
		sine = (float) sinf(-roll);
		CMatrix ret = CMatrix::IdentityMatrix();
		ret[0][0] = cosine;
		ret[1][1] = cosine;
		ret[0][1] = -sine;
		ret[1][0] = sine;

		view = view *ret;
	}
	return view;
}

inline CMatrix  static MatrixOrthoForLH(CMatrix* pOut, float w, float h, float znear, float zfar)
{
	pOut->m_00 = 2/w;
	pOut->m_11 = 2/h;
	pOut->m_22 = 1 / (zfar - znear);
	pOut->m_32 = znear / (znear - zfar);
	pOut->m_33 = 1;

	pOut->m_01 = pOut->m_02 = pOut->m_03 = 0;
	pOut->m_10 = pOut->m_12 = pOut->m_13 = 0;
	pOut->m_20 = pOut->m_21 = pOut->m_23 = 0;
	pOut->m_30 = pOut->m_31 = 0;

	return *pOut;
}


#endif
