#include "gxAnimation.h"

gxAnimation::gxAnimation()
{
	m_pActiveAnimationSetPtr=NULL;
}

gxAnimation::~gxAnimation()
{
	for(std::vector<gxAnimationSet*>::iterator it = m_vAnimationSet.begin(); it != m_vAnimationSet.end(); ++it)
	{
		gxAnimationSet* animationSet = *it;
		GX_DELETE(animationSet);
	}
	m_vAnimationSet.clear();
}

void gxAnimation::appendAnimationSet(gxAnimationSet* animationSet)
{
	m_vAnimationSet.push_back(animationSet);
}

void gxAnimation::setActiveAnimationSet(int index)
{
	m_pActiveAnimationSetPtr = m_vAnimationSet[index];
}

void gxAnimation::update(float dt)
{
	if(m_pActiveAnimationSetPtr)
		m_pActiveAnimationSetPtr->update(dt);
}