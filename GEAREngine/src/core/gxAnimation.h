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
	std::vector<gxAnimationSet*>* getAnimationSetList()		{	return &m_vAnimationSet; }

	void write(gxFile& file);
	void read(gxFile& file);

	gxAnimationSet* play(int animSetIndex);
	void stop();
	void pause();
	void resume();
	void rewind();
	void rewindAll();
	bool isPlaying(int animSetIndex);

private:
	std::vector<gxAnimationSet*> m_vAnimationSet;
	gxAnimationSet* m_pActiveAnimationSetPtr;		//must not delete this

	bool m_bPlay;
};

#endif