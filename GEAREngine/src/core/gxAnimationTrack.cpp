#include "gxAnimationTrack.h"

gxAnimationTrack::gxAnimationTrack()
{
	animationFPS=0;
	numberOfFrames=0;
	currentAnimationFrame=0;
	animationTrack=NULL;
	//m_pObjectPtr=NULL;
}

gxAnimationTrack::~gxAnimationTrack()
{
	//if(m_pObjectPtr)
	//	m_pObjectPtr->setAnimationTrack(NULL);
	GX_DELETE_ARY(animationTrack);
}

matrix4x4f* gxAnimationTrack::allocateTrack()
{
	if(numberOfFrames==0) return NULL;

	if(animationTrack!=NULL)
		return animationTrack;

	animationTrack = new matrix4x4f[numberOfFrames];	//(matrix4x4f*)malloc(sizeof(*animationTrack)*numberOfFrames);
	return animationTrack;
}

//void gxAnimationTrack::setObject3d(object3d* obj)
//{
//	m_pObjectPtr=obj;
//}


void gxAnimationTrack::write(gxFile& file)
{
	file.Write(m_szName);
	file.Write(animationFPS);
	file.Write(numberOfFrames);
	for(int x=0;x<numberOfFrames;x++)
	{
		file.WriteBuffer((unsigned char*)animationTrack[x].getMatrix(), sizeof(float)*16);
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
		matrix4x4f* track=allocateTrack();
		for(int x=0;x<numberOfFrames;x++)
		{
			file.ReadBuffer((unsigned char*)track[x].getMatrix(), sizeof(float)*16);
		}
	}
}