#ifndef GXANIMATION_H
#define GXANIMATION_H

#include "gxAnimationSet.h"
#include <vector>
#include "../util/gxFile.h"

class gxAnimationSet;
class DllExport gxAnimation
{
public:
	gxAnimation();
	~gxAnimation();

	void appendAnimationSet(gxAnimationSet* animationSet);
	void update(float dt);

	//gxAnimationSet* setActiveAnimationSet(int index);
	gxAnimationSet* getActiveAnimationSet()		{	return m_pActiveAnimationSetPtr;	}
	std::vector<gxAnimationSet*>* getAnimationSetList	()		{	return &m_vAnimationSet; }
	gxAnimationSet* getAnimationSet(int index)	{	return m_vAnimationSet[index];	}
	int getAnimSetCount()	{	return m_vAnimationSet.size();	}

	void write(gxFile& file);
	void read(gxFile& file);

	gxAnimationSet* play(int animSetIndex);
	void stop();
	void pause();
	void resume();
	void rewind();
	void rewindAll();
	bool isPlaying();

	float getCurrentFrame()		{	return m_fCurrentFrame;	}
	int getFrameCount()			{	return m_nFrames;		}

private:
	std::vector<gxAnimationSet*> m_vAnimationSet;
	gxAnimationSet* m_pActiveAnimationSetPtr;		//must not delete this

	bool m_bPlay;
	int m_nFrames;
	int m_iFPS;
	float m_fSpeed;
	float m_fCurrentFrame;
};


extern "C" {
	DllExport gxAnimationSet* gxAnimation_play(gxAnimation* animation, int animSetIndex);
	DllExport void gxAnimation_stop(gxAnimation* animation);
	DllExport void gxAnimation_pause(gxAnimation* animation);
	DllExport void gxAnimation_resume(gxAnimation* animation);
	DllExport void gxAnimation_rewind(gxAnimation* animation);
	DllExport void gxAnimation_rewindAll(gxAnimation* animation);
	DllExport bool gxAnimation_isPlaying(gxAnimation* animation);

	DllExport gxAnimationSet* gxAnimation_getAnimationSet(gxAnimation* animation, int index);
	DllExport int gxAnimation_getAnimSetCount(gxAnimation* animation);
	DllExport void gxAnimation_appendAnimationSet(gxAnimation* animation, gxAnimationSet* animationSet);
}

#endif