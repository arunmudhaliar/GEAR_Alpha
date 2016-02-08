#pragma once

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
	gxAnimationSet* getActiveAnimationSet()             {	return activeAnimationSet;          }
	std::vector<gxAnimationSet*>* getAnimationSetList()	{	return &animationSets;              }
	gxAnimationSet* getAnimationSet(int index)          {	return animationSets[index];        }
	int getAnimSetCount()                               {	return (int)animationSets.size();	}

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

	float getCurrentFrame()		{	return currentAnimationFrame;	}
	int getFrameCount()			{	return numberOfFrames;          }

private:
	std::vector<gxAnimationSet*> animationSets;
	gxAnimationSet* activeAnimationSet;		//must not delete this

	bool isAnimationPlaying;
	int numberOfFrames;
	int animationFPS;
	float animationSpeed;
	float currentAnimationFrame;
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