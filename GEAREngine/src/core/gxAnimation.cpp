#include "gxAnimation.h"

gxAnimation::gxAnimation()
{
	activeAnimationSet=NULL;
	isAnimationPlaying=false;

	numberOfFrames=0;
	animationFPS=0;
	currentAnimationFrame=0.0f;
	animationSpeed=1.0f;
}

gxAnimation::~gxAnimation()
{
    removeAllAnimationSet();
}

bool gxAnimation::appendAnimationSet(gxAnimationSet* animationSet)
{
    if(std::find(animationSets.begin(), animationSets.end(), animationSet)==animationSets.end())
    {
        REF_RETAIN(animationSet);
        animationSets.push_back(animationSet);
        return true;
    }
    
    return false;
}

bool gxAnimation::removeAnimationSet(gxAnimationSet* animationSet)
{
    int oldSz = (int)animationSets.size();
    animationSets.erase(std::remove(animationSets.begin(), animationSets.end(), animationSet), animationSets.end());
    if(oldSz!=animationSets.size())
    {
        REF_RELEASE(animationSet);
        return true;
    }
    
    return false;
}

void gxAnimation::removeAllAnimationSet()
{
    for (auto animationSet : animationSets)
    {
        REF_RELEASE(animationSet);
    }
    animationSets.clear();
}

void gxAnimation::update(float dt)
{
	if(activeAnimationSet && isAnimationPlaying)
	{
		float nFramesToPlay=(dt*animationFPS*animationSpeed);
		currentAnimationFrame+=nFramesToPlay;
		if(currentAnimationFrame>=numberOfFrames)
			currentAnimationFrame=0.0f;
	}
}

void gxAnimation::write(gxFile& file)
{
	file.Write((int)animationSets.size());
	for(std::vector<gxAnimationSet*>::iterator it = animationSets.begin(); it != animationSets.end(); ++it)
	{
		gxAnimationSet* animationSet = *it;
		animationSet->write(file);
	}
}

void gxAnimation::read(gxFile& file)
{
	int nAnimSet=0;
	file.Read(nAnimSet);
	for(int x=0;x<nAnimSet;x++)
	{
        gxAnimationSet* animationSet = gxAnimationSet::create("");
		animationSet->read(file);
		appendAnimationSet(animationSet);
        REF_RELEASE(animationSet);
	}
}

gxAnimationSet* gxAnimation::play(gxAnimationSet* animset)
{
	activeAnimationSet = animset;
	isAnimationPlaying=true;
	currentAnimationFrame=0;
	animationFPS=activeAnimationSet->getFPS();
	numberOfFrames=activeAnimationSet->getFrameCount();

	return activeAnimationSet;
}

gxAnimationSet* gxAnimation::play(int animSetIndex)
{
	return play(animationSets[animSetIndex]);
}

void gxAnimation::stop()
{
	isAnimationPlaying=false;
	currentAnimationFrame=0;
}

void gxAnimation::pause()
{
	stop();
}

void gxAnimation::resume()
{
	isAnimationPlaying=true;
}

void gxAnimation::rewind()
{
	currentAnimationFrame=0;

}

void gxAnimation::rewindAll()
{
	currentAnimationFrame=0;
}

bool gxAnimation::isPlaying()
{
	return isAnimationPlaying;
}


extern "C" {
extern DECLSPEC gxAnimationSet* gxAnimation_play(gxAnimation* animation, int animSetIndex)
{
	return animation->play(animSetIndex);
}
extern DECLSPEC void gxAnimation_stop(gxAnimation* animation)
{
	animation->stop();
}

extern DECLSPEC void gxAnimation_pause(gxAnimation* animation)
{
	animation->pause();
}

extern DECLSPEC void gxAnimation_resume(gxAnimation* animation)
{
	animation->resume();
}

extern DECLSPEC void gxAnimation_rewind(gxAnimation* animation)
{
	animation->rewind();
}

extern DECLSPEC void gxAnimation_rewindAll(gxAnimation* animation)
{
	animation->rewindAll();
}

extern DECLSPEC bool gxAnimation_isPlaying(gxAnimation* animation)
{
	return animation->isPlaying();
}

extern DECLSPEC gxAnimationSet* gxAnimation_getAnimationSet(gxAnimation* animation, int index)
{
	return animation->getAnimationSet(index);
}

extern DECLSPEC int gxAnimation_getAnimSetCount(gxAnimation* animation)
{
	return animation->getAnimSetCount();
}

extern DECLSPEC void gxAnimation_appendAnimationSet(gxAnimation* animation, gxAnimationSet* animationSet)
{
	animation->appendAnimationSet(animationSet);
}
}