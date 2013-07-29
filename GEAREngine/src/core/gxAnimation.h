#ifndef GXANIMATION_H
#define GXANIMATION_H

#include "gxAnimationSet.h"
#include <vector>

class gxAnimationSet;
class DllExport gxAnimation
{
public:
	gxAnimation();
	~gxAnimation();

	void appendAnimationSet(gxAnimationSet* animationSet);
	void update(float dt);

	gxAnimationSet* setActiveAnimationSet(int index);
	gxAnimationSet* getActiveAnimationSet()		{	return m_pActiveAnimationSetPtr;	}
	std::vector<gxAnimationSet*>* getAnimationSetList()		{	return &m_vAnimationSet; }

private:
	std::vector<gxAnimationSet*> m_vAnimationSet;
	gxAnimationSet* m_pActiveAnimationSetPtr;		//must not delete this
};

#endif