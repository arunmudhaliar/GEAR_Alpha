#include "gxAnimation.h"

gxAnimation::gxAnimation()
{
	m_pActiveAnimationSetPtr=NULL;
	m_bPlay=false;
}

gxAnimation::~gxAnimation()
{
	/*
	for(std::vector<gxAnimationSet*>::iterator it = m_vAnimationSet.begin(); it != m_vAnimationSet.end(); ++it)
	{
		gxAnimationSet* animationSet = *it;
		GX_DELETE(animationSet);
	}
	*/
	m_vAnimationSet.clear();
}

void gxAnimation::appendAnimationSet(gxAnimationSet* animationSet)
{
	m_vAnimationSet.push_back(animationSet);
}

//gxAnimationSet* gxAnimation::setActiveAnimationSet(int index)
//{
//	m_pActiveAnimationSetPtr = m_vAnimationSet[index];
//	return m_pActiveAnimationSetPtr;
//}

void gxAnimation::update(float dt)
{
	if(m_pActiveAnimationSetPtr && m_bPlay)
		m_pActiveAnimationSetPtr->update(dt);
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

gxAnimationSet* gxAnimation::play(int animSetIndex)
{
	m_pActiveAnimationSetPtr = m_vAnimationSet[animSetIndex];
	m_bPlay=true;
	return m_pActiveAnimationSetPtr;
}

void gxAnimation::stop()
{
	m_bPlay=false;
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
	if(m_pActiveAnimationSetPtr)
	{
		m_pActiveAnimationSetPtr->setCurrentFrame(0);
	}
}

void gxAnimation::rewindAll()
{
	for(std::vector<gxAnimationSet*>::iterator it = m_vAnimationSet.begin(); it != m_vAnimationSet.end(); ++it)
	{
		gxAnimationSet* animationSet = *it;
		animationSet->setCurrentFrame(0);
	}
}

bool gxAnimation::isPlaying(int animSetIndex)
{
	return m_bPlay;
}