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


void gxAnimationTrack::write(gxFile& file)
{
	file.Write(m_szName);
	file.Write(m_iFPS);
	file.Write(m_nFrames);
	file.WriteBuffer((unsigned char*)m_pTrack, sizeof(matrix4x4f)*m_nFrames);
}

void gxAnimationTrack::read(gxFile& file)
{
	char* name = file.ReadString();
	strcpy(m_szName, name);
	GX_DELETE_ARY(name);
	file.Read(m_iFPS);
	file.Read(m_nFrames);
	if(m_nFrames)
	{
		matrix4x4f* track=allocateTrack();
		file.ReadBuffer((unsigned char*)track, sizeof(matrix4x4f)*m_nFrames);
	}
}