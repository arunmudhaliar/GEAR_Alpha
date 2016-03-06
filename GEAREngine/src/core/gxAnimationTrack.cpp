#include "gxAnimationTrack.h"

gxAnimationTrack::gxAnimationTrack():
IAnimationTrack()
{
	animationFPS=0;
	numberOfFrames=0;
	currentAnimationFrame=0;
}

gxAnimationTrack::~gxAnimationTrack()
{
	//GX_DELETE_ARY(animationTrack);
}

matrix4x4f* gxAnimationTrack::allocateFrames()
{
	if(numberOfFrames==0) return NULL;

	if(animationFrames!=NULL)
		return animationFrames;

	animationFrames = new matrix4x4f[numberOfFrames];	//(matrix4x4f*)malloc(sizeof(*animationTrack)*numberOfFrames);
	return animationFrames;
}

bool gxAnimationTrack::getFrame(int frame, matrix4x4f& mat)
{
    if(frame>=numberOfFrames)
        return false;
    
    mat=animationFrames[frame];
    
    return true;
}

bool gxAnimationTrack::getFrameFromTime(float time, matrix4x4f& mat)
{
    //Not implemented
    assert(false);
    return false;
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