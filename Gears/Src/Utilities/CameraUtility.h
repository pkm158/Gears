/************************************************************************/
/* Author:  Kemi Peng 
History: 08/03/2015 by Kemi Peng*/
/************************************************************************/

#ifndef CAMARAUTILITY
#define CAMARAUTILITY

#include "GearsCommonHead.h"
#include "IUtility.h"
#include "libmath.h"

#define DEFAULT_CAM_FOV				0.78539f
#define DEFAULT_CAM_FOV_ZOOM		1.0f
#define DEFAULT_CAM_ZNEAR			1.0f
#define DEFAULT_CAM_ZFAR			65535.0f
#define DEFAULT_CAM_ZOOM			1.0f
#define DEFAULT_CAM_ASPECT			1.77778f

class CameraUtility : public IUtility
{
public:
	CameraUtility(void);
	virtual ~CameraUtility(void);

	virtual void postInitiate(void);
	virtual void update( double tt, double dt );

	void		resetCamera();
	
	void		setAt(float x,float y,float z);									
	void		setPos(float x,float y,float z);								
	void		setPosTg(float x, float y, float z){m_postg.Set(x, y, z); m_isMoveToPosTg = true;};							// the position that camera is going to move to

	void		bindCamTo(void){};												// unfinished 
	void		bindCamAtTo(void){};											// unfinished

	void		setAddPos(const CVector &posPlus);								
	void		setAddAt(const CVector &atplus);								

	//void		setDistance(float dis)			{m_.dis = dis;};				
	void		setRotationAngle(float angle);									
	void		addRotationAngle(float addangle);
	void		setElevationAngle(float elevation);								
	void		addElevationAngle(float addelevation);

	void		setAspect(float w,float h) { m_aspect = w/h; };										
	void		setFOV(float fov) { m_fov = fov; };											

	void		setZPlane(float znear,float zfar){m_zNear = znear;m_zFar = zfar;};	
	void		setZoom(float zoom)	{ m_zoom = zoom; };											
	void		setZoomTg(float zoom) { m_zoomtg = zoom; m_isMoveToZoomTg = true; };				
	void		addZoom(float zoomPlus) { m_zoom += zoomPlus; };									

	void		setFOVZoomTg(float fovzoom) { m_fovzmtg = fovzoom; m_isMoveToFovzmTg = true; };		
	void		setFOVZoom(float fovzoom) { m_fovzm = fovzoom; };										
	void		addFOVZoom(float fovzoomplus) { m_fovzm += fovzoomplus; };				
	void		setPosMoveSpd(float spd){ m_posSpd = spd;};
	void		setZoomSpd(float spd){ m_zoomSpd = spd;};
	void		setFOVZmSpd(float spd){ m_fovZmSpd = spd;};

	float		getPosMoveSpd(void) const {return m_posSpd;};
	float		getZoomSpd(void) const {return m_zoomSpd;};
	float		getFovZmSpd(void) const {return m_fovZmSpd;};


	CVector		getPos(void) const{ return m_pos; };							
	CVector		getAt(void) const{ return m_at; };								

	CVector		getPosTg(void) const{ return m_postg; };
	
	float		getAngle() const { return m_rotAngle; };						
	float		getElevation() const { return m_elevAngle; };					
	//float		getDistance();	

	float		getFOV(void)     const	{return m_fov;};						
	float		getFOVZoom(void) const	{return m_fovzm;};						
	float		getFOVZoomTg(void)const {return m_zoomtg;};
	float		getAspect(void)  const	{return m_aspect;};						
	float		getZoom(void)    const	{return m_zoom;};						
	float		getZoomTg(void)  const	{return m_zoomtg;};						

	float		getZFar()	 const	{return m_zFar;};							
	float		getZNear()	 const	{return m_zNear;};				

	

	const CMatrix &	getViewMatrix() {return m_vmat;};
	const CMatrix &	getProjectionMatrix() {return m_pmat;};


	void		setEarthQuake(float updown , float side , int time);	//地震啦,尚未实现

private:
	CMatrix			m_pmat;					// project mat
	CMatrix			m_vmat;					// view mat

	CVector			m_pos;					// position
	CVector			m_at;					// look at
	const CVector	m_up;					// up vector

	// 有点像经度和维度
	float			m_rotAngle;				// rotation angle, between 0 to 2pi
	float			m_elevAngle;			// elevation between -pi/2 to pi/2

	CVector			m_postg;				// the position that camera is going to move to. m_pos will gradually change to m_postg

	float			m_fov;					

	float			m_aspect;
	float			m_zoom;					// simply move camera towards its look at point.				
	float			m_zoomtg;				// m_zoom will gradually change to m_zoomtg.
	float			m_fovzmtg;				// m_fovzm will gradually change to m_fovzmtg
	float			m_fovzm;				// used as telescope

	float			m_zNear;
	float			m_zFar;

	float			m_posSpd;				// speed of moving camera to m_postg
	float			m_zoomSpd;				// speed of moving camera to m_zoomtg
	float			m_fovZmSpd;				// speed of moving camera to m_fovzmtg

	float			m_eq_time;				// unused, for earthquake
	float			m_eq_h;					// 
	float			m_eq_w;					// 

	bool			m_isMoveToPosTg;
	bool			m_isMoveToZoomTg;
	bool			m_isMoveToFovzmTg;
	void calAngles(float &rotAngle, float &elevAngle);					//calculate rotation angle and elevation angle by position and look at
	void calLookAt(CVector &at);					// calculate look at
	void normalizeAngles(void);					//normalize angles; rotation angle, between 0 to 2p;, elevation between -pi/2 to pi/2

};

#endif