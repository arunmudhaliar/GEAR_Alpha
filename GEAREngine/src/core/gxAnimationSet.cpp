#include "gxAnimationSet.h"

gxAnimationSet::gxAnimationSet(const char* animationName)
{
	GX_STRCPY(m_szName, animationName);
	m_nFrames=0;
	m_iFPS=0;
	m_iCRC_of_mesh_data=0;
}

gxAnimationSet::~gxAnimationSet()
{
	for(std::vector<gxAnimationTrack*>::iterator it = m_vAnimationTrack.begin(); it != m_vAnimationTrack.end(); ++it)
	{
		gxAnimationTrack* animationTrack = *it;
		GX_DELETE(animationTrack);
	}
	m_vAnimationTrack.clear();
}

void gxAnimationSet::appendTrack(gxAnimationTrack* track)
{
	m_iFPS=track->getFPS();
	if(track->getTotalFrames()>m_nFrames)
		m_nFrames=track->getTotalFrames();
	m_vAnimationTrack.push_back(track);
}

void gxAnimationSet::write(gxFile& file)
{
	file.Write(m_szName);
	file.Write((int)m_vAnimationTrack.size());
	for(std::vector<gxAnimationTrack*>::iterator it = m_vAnimationTrack.begin(); it != m_vAnimationTrack.end(); ++it)
	{
		gxAnimationTrack* animationTrack = *it;
		animationTrack->write(file);
	}
}

void gxAnimationSet::read(gxFile& file)
{
	char* name=file.ReadString();
	GX_STRCPY(m_szName, name);
	GX_DELETE_ARY(name);
	int nTrack=0;
	file.Read(nTrack);
	for(int x=0;x<nTrack;x++)
	{
		gxAnimationTrack* animationTrack = new gxAnimationTrack();
		animationTrack->read(file);
		appendTrack(animationTrack);
	}
}

extern "C" {
extern DECLSPEC const char* gxAnimationSet_getAnimationName(gxAnimationSet* animSet)
{
	return animSet->getAnimationName();
}
}