#ifndef GXANIMATIONSET_H
#define GXANIMATIONSET_H

#include "gxAnimationTrack.h"
#include <vector>

class gxAnimationSet
{
public:
	gxAnimationSet();
	~gxAnimationSet();

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