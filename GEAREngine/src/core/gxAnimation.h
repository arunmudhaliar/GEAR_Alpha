#ifndef GXANIMATION_H
#define GXANIMATION_H

#include "gxAnimationSet.h"
#include <vector>

class gxAnimationSet;
class gxAnimation
{
public:
	gxAnimation();
	~gxAnimation();

	void appendAnimationSet(gxAnimationSet* animationSet);
	void update(float dt);

	void setActiveAnimationSet(int index);
private:
	std::vector<gxAnimationSet*> m_vAnimationSet;
	gxAnimationSet* m_pActiveAnimationSetPtr;		//must not delete this
};

#endif