#include "gxAnimationSet.h"

gxAnimationSet::gxAnimationSet(const char* animationName)
{
	GX_STRCPY(m_szName, animationName);
	m_nFrames=0;
	m_iFPS=0;
	m_fCurrentFrame=0.0f;
	m_fSpeed=1.0f;
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

void gxAnimationSet::update(float dt)
{
	float nFramesToPlay=(dt*m_iFPS*m_fSpeed);
	int curFrame=(int)m_fCurrentFrame;

	for(std::vector<gxAnimationTrack*>::iterator it = m_vAnimationTrack.begin(); it != m_vAnimationTrack.end(); ++it)
	{
		gxAnimationTrack* animationTrack = *it;
		animationTrack->setCurrentFrame(curFrame);
	}

	m_fCurrentFrame+=nFramesToPlay;
	if(m_fCurrentFrame>=m_nFrames)
		m_fCurrentFrame=0.0f;
}