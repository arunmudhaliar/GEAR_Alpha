#include "gxAnimationTrack.h"

gxAnimationTrack::gxAnimationTrack()
{
	m_iFPS=0;
	m_nFrames=0;
	m_iCurrentFrame=0;
	m_pTrack=NULL;
	//m_pObjectPtr=NULL;
}

gxAnimationTrack::~gxAnimationTrack()
{
	//if(m_pObjectPtr)
	//	m_pObjectPtr->setAnimationTrack(NULL);
	GX_DELETE_ARY(m_pTrack);
}

matrix4x4f* gxAnimationTrack::allocateTrack()
{
	if(m_nFrames==0) return NULL;

	if(m_pTrack!=NULL)
		return m_pTrack;

	m_pTrack = new matrix4x4f[m_nFrames];
	return m_pTrack;
}

//void gxAnimationTrack::setObject3d(object3d* obj)
//{
//	m_pObjectPtr=obj;
//}