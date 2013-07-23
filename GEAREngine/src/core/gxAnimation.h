#ifndef GXANIMATION_H
#define GXANIMATION_H

#include "gxAnimationTrack.h"
#include <vector>

class gxAnimation
{
public:
	gxAnimation();
	~gxAnimation();

	void appendTrack(gxAnimationTrack* track);
	void update(float dt);

private:
	std::vector<gxAnimationTrack*> m_vAnimationTrack;
	int m_nFrames;
	int m_iFPS;
	float m_fSpeed;
	float m_fCurrentFrame;
};

#endif