#pragma once

#include "IAnimationTrack.h"
#include <vector>
#include "../util/gxFile.h"
#include "../autoReleasePool/autoReleasePool.h"

using namespace GEAR::Memory;

class DECLSPEC gxAnimationSet : public Ref
{
public:
	~gxAnimationSet();
    static gxAnimationSet* create(const std::string& name);
    
	void appendTrack(IAnimationTrack* animationTrack);

    const std::string& getAnimationName()               {	return animationName;   }
	const std::vector<IAnimationTrack*>& getTrackList()	{	return animationTracks;	}

    IAnimationTrack* getAnimationTrack(const std::string& animationName);
    
	void write(gxFile& file);
	void read(gxFile& file);

	void setCRCOfMeshData(int crc)	{	crcOfMeshData = crc;	}
	int getCRCOfMeshData()			{	return crcOfMeshData;	}

	int getFPS()		{	return animationFPS;	}
	int getFrameCount()	{	return numberOfFrames;	}

private:
    gxAnimationSet(const std::string& animationName);
    gxAnimationSet(){}
    
	std::vector<IAnimationTrack*> animationTracks;
	int numberOfFrames;
	int animationFPS;
    std::string animationName;
	int crcOfMeshData;
};

extern "C" {
	DECLSPEC const char* gxAnimationSet_getAnimationName(gxAnimationSet* animSet);
}
