#include "gxAnimationSet.h"
#include "gxAnimationTrack.h"

gxAnimationSet* gxAnimationSet::create(const std::string& name)
{
    auto newObject = new gxAnimationSet(name);
    if(newObject)
    {
        newObject->autoRelease();
        REF_RETAIN(newObject);
        
        return newObject;
    }
    
    return nullptr;
}

gxAnimationSet::gxAnimationSet(const std::string& animationName):
    Ref()
{
	this->animationName=animationName;
	numberOfFrames=0;
	animationFPS=0;
	crcOfMeshData=0;
}

gxAnimationSet::~gxAnimationSet()
{
    for(auto animationTrack : animationTracks)
	{
		REF_RELEASE(animationTrack);
	}
	animationTracks.clear();
}

void gxAnimationSet::appendTrack(IAnimationTrack* animationTrack)
{
	animationFPS=animationTrack->getFPS();
	if(animationTrack->getTotalFrames()>numberOfFrames)
		numberOfFrames=animationTrack->getTotalFrames();
    
    REF_RETAIN(animationTrack);
	animationTracks.push_back(animationTrack);
}

void gxAnimationSet::write(gxFile& file)
{
	file.Write(animationName.c_str());
	file.Write((int)animationTracks.size());
	for(auto animationTrack : animationTracks)
	{
        file.Write(animationTrack->getAnimationTrackType());
		animationTrack->write(file);
	}
}

IAnimationTrack* gxAnimationSet::getAnimationTrack(const std::string& animationName)
{
    for(auto animationTrack : animationTracks)
    {
        if(animationName.compare(animationTrack->getName())==0)
        {
            return animationTrack;
        }
    }
    
    return nullptr;
}

void gxAnimationSet::read(gxFile& file)
{
	char* name=file.ReadString();
    animationName.assign(name, strlen(name));
	GX_DELETE_ARY(name);
	int nTrack=0;
	file.Read(nTrack);
	for(int x=0;x<nTrack;x++)
	{
        int animationTrackType = 0;
        file.Read(animationTrackType);
        
        switch (animationTrackType)
        {
            case 1:
            {
                gxAnimationTrack* animationTrack = gxAnimationTrack::create();
                animationTrack->read(file);
                appendTrack(animationTrack);
                REF_RELEASE(animationTrack);
            }
                break;
                
            default:
                printf("\nERROR: Unkown animation track type %d\n", animationTrackType);
                break;
        }
	}
}

extern "C" {
extern DECLSPEC const char* gxAnimationSet_getAnimationName(gxAnimationSet* animSet)
{
	return animSet->getAnimationName().c_str();
}
}
