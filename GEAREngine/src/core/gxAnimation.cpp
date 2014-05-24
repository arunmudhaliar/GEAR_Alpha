#include "gxAnimation.h"

gxAnimation::gxAnimation()
{
	m_pActiveAnimationSetPtr=NULL;
	m_bPlay=false;

	m_nFrames=0;
	m_iFPS=0;
	m_fCurrentFrame=0.0f;
	m_fSpeed=1.0f;
}

gxAnimation::~gxAnimation()
{
	m_vAnimationSet.clear();
}

void gxAnimation::appendAnimationSet(gxAnimationSet* animationSet)
{
	m_vAnimationSet.push_back(animationSet);
}

void gxAnimation::update(float dt)
{
	if(m_pActiveAnimationSetPtr && m_bPlay)
	{
		float nFramesToPlay=(dt*m_iFPS*m_fSpeed);
		m_fCurrentFrame+=nFramesToPlay;
		if(m_fCurrentFrame>=m_nFrames)
			m_fCurrentFrame=0.0f;
	}
}

void gxAnimation::write(gxFile& file)
{
	file.Write((int)m_vAnimationSet.size());
	for(std::vector<gxAnimationSet*>::iterator it = m_vAnimationSet.begin(); it != m_vAnimationSet.end(); ++it)
	{
		gxAnimationSet* animationSet = *it;
		animationSet->write(file);
	}
}

void gxAnimation::read(gxFile& file)
{
	int nAnimSet=0;
	file.Read(nAnimSet);
	for(int x=0;x<nAnimSet;x++)
	{
		gxAnimationSet* animationSet = new gxAnimationSet("");
		animationSet->read(file);
		appendAnimationSet(animationSet);
	}
}

gxAnimationSet* gxAnimation::play(gxAnimationSet* animset)
{
	m_pActiveAnimationSetPtr = animset;
	m_bPlay=true;
	m_fCurrentFrame=0;
	m_iFPS=m_pActiveAnimationSetPtr->getFPS();
	m_nFrames=m_pActiveAnimationSetPtr->getFrameCount();

	return m_pActiveAnimationSetPtr;
}

gxAnimationSet* gxAnimation::play(int animSetIndex)
{
	return play(m_vAnimationSet[animSetIndex]);
}

void gxAnimation::stop()
{
	m_bPlay=false;
	m_fCurrentFrame=0;
}

void gxAnimation::pause()
{
	stop();
}

void gxAnimation::resume()
{
	m_bPlay=true;
}

void gxAnimation::rewind()
{
	m_fCurrentFrame=0;

}

void gxAnimation::rewindAll()
{
	m_fCurrentFrame=0;
}

bool gxAnimation::isPlaying()
{
	return m_bPlay;
}


extern "C" {
extern DECLSPEC gxAnimationSet* gxAnimation_play(gxAnimation* animation, int animSetIndex)
{
	return animation->play(animSetIndex);
}
extern DECLSPEC void gxAnimation_stop(gxAnimation* animation)
{
	animation->stop();
}

extern DECLSPEC void gxAnimation_pause(gxAnimation* animation)
{
	animation->pause();
}

extern DECLSPEC void gxAnimation_resume(gxAnimation* animation)
{
	animation->resume();
}

extern DECLSPEC void gxAnimation_rewind(gxAnimation* animation)
{
	animation->rewind();
}

extern DECLSPEC void gxAnimation_rewindAll(gxAnimation* animation)
{
	animation->rewindAll();
}

extern DECLSPEC bool gxAnimation_isPlaying(gxAnimation* animation)
{
	return animation->isPlaying();
}

extern DECLSPEC gxAnimationSet* gxAnimation_getAnimationSet(gxAnimation* animation, int index)
{
	return animation->getAnimationSet(index);
}

extern DECLSPEC int gxAnimation_getAnimSetCount(gxAnimation* animation)
{
	return animation->getAnimSetCount();
}

extern DECLSPEC void gxAnimation_appendAnimationSet(gxAnimation* animation, gxAnimationSet* animationSet)
{
	animation->appendAnimationSet(animationSet);
}
}