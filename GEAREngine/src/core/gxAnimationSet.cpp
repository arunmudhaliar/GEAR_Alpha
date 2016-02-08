#include "gxAnimationSet.h"

gxAnimationSet::gxAnimationSet(const char* animationName)
{
	GX_STRCPY(m_szName, animationName);
	numberOfFrames=0;
	animationFPS=0;
	crcOfMeshData=0;
}

gxAnimationSet::~gxAnimationSet()
{
	for(std::vector<gxAnimationTrack*>::iterator it = animationTracks.begin(); it != animationTracks.end(); ++it)
	{
		gxAnimationTrack* animationTrack = *it;
		GX_DELETE(animationTrack);
	}
	animationTracks.clear();
}

void gxAnimationSet::appendTrack(gxAnimationTrack* track)
{
	animationFPS=track->getFPS();
	if(track->getTotalFrames()>numberOfFrames)
		numberOfFrames=track->getTotalFrames();
	animationTracks.push_back(track);
}

void gxAnimationSet::write(gxFile& file)
{
	file.Write(m_szName);
	file.Write((int)animationTracks.size());
	for(std::vector<gxAnimationTrack*>::iterator it = animationTracks.begin(); it != animationTracks.end(); ++it)
	{
		gxAnimationTrack* animationTrack = *it;
		animationTrack->write(file);
	}
}

void gxAnimationSet::read(gxFile& file)
{
	char* name=file.ReadString();
	GX_STRCPY(m_szName, name);
	GX_DELETE_ARY(name);
	int nTrack=0;
	file.Read(nTrack);
	for(int x=0;x<nTrack;x++)
	{
		gxAnimationTrack* animationTrack = new gxAnimationTrack();
		animationTrack->read(file);
		appendTrack(animationTrack);
	}
}

extern "C" {
extern DECLSPEC const char* gxAnimationSet_getAnimationName(gxAnimationSet* animSet)
{
	return animSet->getAnimationName();
}
}