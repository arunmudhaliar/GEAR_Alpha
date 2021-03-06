#ifndef GXANIMATION_H
#define GXANIMATION_H

#include "gxAnimationSet.h"
#include <vector>
#include "../util/gxFile.h"

class gxAnimationSet;
class DECLSPEC gxAnimation
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
	int getAnimSetCount()	{	return (int)m_vAnimationSet.size();	}

	void write(gxFile& file);
	void read(gxFile& file);

	gxAnimationSet* play(gxAnimationSet* animset);
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
	DECLSPEC gxAnimationSet* gxAnimation_play(gxAnimation* animation, int animSetIndex);
	DECLSPEC void gxAnimation_stop(gxAnimation* animation);
	DECLSPEC void gxAnimation_pause(gxAnimation* animation);
	DECLSPEC void gxAnimation_resume(gxAnimation* animation);
	DECLSPEC void gxAnimation_rewind(gxAnimation* animation);
	DECLSPEC void gxAnimation_rewindAll(gxAnimation* animation);
	DECLSPEC bool gxAnimation_isPlaying(gxAnimation* animation);

	DECLSPEC gxAnimationSet* gxAnimation_getAnimationSet(gxAnimation* animation, int index);
	DECLSPEC int gxAnimation_getAnimSetCount(gxAnimation* animation);
	DECLSPEC void gxAnimation_appendAnimationSet(gxAnimation* animation, gxAnimationSet* animationSet);
}

#endif