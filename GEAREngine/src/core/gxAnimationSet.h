#ifndef GXANIMATIONSET_H
#define GXANIMATIONSET_H

#include "gxAnimationTrack.h"
#include <vector>
#include "../util/gxFile.h"

class DECLSPEC gxAnimationSet
{
public:
	gxAnimationSet(const char* animationName);
	~gxAnimationSet();

	void appendTrack(gxAnimationTrack* track);
	//void update(float dt);

	const char* getAnimationName()		{	return m_szName;	}

	std::vector<gxAnimationTrack*>* getTrackList()	{	return &m_vAnimationTrack;	}

	void write(gxFile& file);
	void read(gxFile& file);

	void setCRCOfMeshData(int crc)	{	m_iCRC_of_mesh_data = crc;	}
	int getCRCOfMeshData()			{	return m_iCRC_of_mesh_data;	}

	int getFPS()		{	return m_iFPS;		}
	int getFrameCount()	{	return m_nFrames;	}

private:
	std::vector<gxAnimationTrack*> m_vAnimationTrack;
	int m_nFrames;
	int m_iFPS;
	char m_szName[256];
	int m_iCRC_of_mesh_data;
};

extern "C" {
	DECLSPEC const char* gxAnimationSet_getAnimationName(gxAnimationSet* animSet);
}

#endif