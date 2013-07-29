#ifndef GXANIMATIONSET_H
#define GXANIMATIONSET_H

#include "gxAnimationTrack.h"
#include <vector>

class gxAnimationSet
{
public:
	gxAnimationSet(const char* animationName);
	~gxAnimationSet();

	void appendTrack(gxAnimationTrack* track);
	void update(float dt);

	const char* getAnimationName()		{	return m_szName;	}

	std::vector<gxAnimationTrack*>* getTrackList()	{	return &m_vAnimationTrack;	}

private:
	std::vector<gxAnimationTrack*> m_vAnimationTrack;
	int m_nFrames;
	int m_iFPS;
	float m_fSpeed;
	float m_fCurrentFrame;
	char m_szName[64];
};

#endif