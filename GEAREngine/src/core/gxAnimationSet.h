#ifndef GXANIMATIONSET_H
#define GXANIMATIONSET_H

#include "gxAnimationTrack.h"
#include <vector>
#include "../util/gxFile.h"

class DllExport gxAnimationSet
{
public:
	gxAnimationSet(const char* animationName);
	~gxAnimationSet();

	void appendTrack(gxAnimationTrack* track);
	void update(float dt);

	const char* getAnimationName()		{	return m_szName;	}

	std::vector<gxAnimationTrack*>* getTrackList()	{	return &m_vAnimationTrack;	}

	void write(gxFile& file);
	void read(gxFile& file);

	void setCurrentFrame(int currentFrame);
private:
	std::vector<gxAnimationTrack*> m_vAnimationTrack;
	int m_nFrames;
	int m_iFPS;
	float m_fSpeed;
	float m_fCurrentFrame;
	char m_szName[64];
};

extern "C" {
	DllExport const char* gxAnimationSet_getAnimationName(gxAnimationSet* animSet);
}

#endif