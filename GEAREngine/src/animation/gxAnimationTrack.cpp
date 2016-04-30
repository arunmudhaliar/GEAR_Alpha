#include "gxAnimationTrack.h"

gxAnimationTrack::gxAnimationTrack():
IAnimationTrack()
{
    animationFrames=nullptr;
}

gxAnimationTrack::~gxAnimationTrack()
{
    GX_DELETE_ARY(animationFrames);
}

gxAnimationTrack* gxAnimationTrack::create()
{
    auto newObject = new gxAnimationTrack();
    if(newObject)
    {
        newObject->autoRelease();
        REF_RETAIN(newObject);
        
        return newObject;
    }
    
    return nullptr;
}

matrix4x4f* gxAnimationTrack::allocateFrames()
{
	if(numberOfFrames==0) return NULL;

	if(animationFrames!=NULL)
		return animationFrames;

	animationFrames = new matrix4x4f[numberOfFrames];	//(matrix4x4f*)malloc(sizeof(*animationTrack)*numberOfFrames);
	return animationFrames;
}

int gxAnimationTrack::getTotalTimeInMilliSec()
{
    return SEC_TO_MILLISEC(getTotalTimeInSec());
}

float gxAnimationTrack::getTotalTimeInSec()
{
    return (float)numberOfFrames/(float)getFPS();
}

bool gxAnimationTrack::getFrame(int frame, matrix4x4f& mat)
{
    if(frame>=numberOfFrames)
        return false;
    
    mat=animationFrames[frame];
    
    return true;
}

bool gxAnimationTrack::getFrameFromTime(int timeInMilliSec, matrix4x4f& mat)
{
    int frame = MILLISEC_TO_SEC(timeInMilliSec)*getFPS();
    return getFrame(frame, mat);
}

void gxAnimationTrack::write(gxFile& file)
{
	file.Write(m_szName);
	file.Write(animationFPS);
	file.Write(numberOfFrames);
	for(int x=0;x<numberOfFrames;x++)
	{
		file.WriteBuffer((unsigned char*)animationFrames[x].getMatrix(), sizeof(float)*16);
	}
}

void gxAnimationTrack::read(gxFile& file)
{
	char* name = file.ReadString();
	GX_STRCPY(m_szName, name);
	GX_DELETE_ARY(name);
	file.Read(animationFPS);
	file.Read(numberOfFrames);
	if(numberOfFrames)
	{
		matrix4x4f* track=allocateFrames();
		for(int x=0;x<numberOfFrames;x++)
		{
			file.ReadBuffer((unsigned char*)track[x].getMatrix(), sizeof(float)*16);
		}
	}
}