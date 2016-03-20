#pragma once

#include "IAnimationTrack.h"
#include <vector>
#include "../util/gxFile.h"

class DECLSPEC gxAnimationSet
{
public:
	gxAnimationSet(const char* animationName);
	~gxAnimationSet();

	void appendTrack(IAnimationTrack* track);
	//void update(float dt);

	const char* getAnimationName()                  {	return m_szName;            }
	std::vector<IAnimationTrack*>& getTrackList()	{	return animationTracks;	}

	void write(gxFile& file);
	void read(gxFile& file);

	void setCRCOfMeshData(int crc)	{	crcOfMeshData = crc;	}
	int getCRCOfMeshData()			{	return crcOfMeshData;	}

	int getFPS()		{	return animationFPS;	}
	int getFrameCount()	{	return numberOfFrames;	}

private:
    gxAnimationSet(){}
    
	std::vector<IAnimationTrack*> animationTracks;
	int numberOfFrames;
	int animationFPS;
	char m_szName[256];
	int crcOfMeshData;
};

extern "C" {
	DECLSPEC const char* gxAnimationSet_getAnimationName(gxAnimationSet* animSet);
}