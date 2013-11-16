//
//  SoundSource.h
//  testOpenAL
//
//  Created by arun on 31/05/11.
//  Copyright 2011 __MyCompanyName__. All rights reserved.
//

#ifndef SOUNDSOURCE_H
#define SOUNDSOURCE_H

#include "../core/basicIncludes.h"



#if defined(USE_OPENAL)
	#include <OpenAL/al.h>
	#include <OpenAL/alc.h>
#elif defined(USE_OPENSL)
	#include <SLES/OpenSLES.h>
	#include <SLES/OpenSLES_Android.h>
#endif

#if defined(USE_OPENAL)
	#include <OpenAL/al.h>
	#include <OpenAL/alc.h>
#elif defined(USE_OPENSL)
	#include <SLES/OpenSLES.h>
	#include <SLES/OpenSLES_Android.h>
#endif

#include "SoundSample.h"

#if defined(USE_OPENSL)
    void bqPlayerCallback(SLBufferQueueItf bq, void *context);
#endif

class DllExport SoundSource
{
public:
    SoundSource();    
    ~SoundSource();
    
    unsigned int getSourceID()              {   return m_iSourceId;     }
    //void setSourceID(unsigned int srcId)    {   m_iSourceId=srcId;      }
    
    void createSource(SoundSample* sample, bool bInstance);
    
    void play(bool bLoop=true);
    void stop();
    
	void pause();
	void resume();
	
    void setPosition(int x, int y, int z)
    {
#if defined(USE_OPENAL)
        alSource3f(m_iSourceId, AL_POSITION, x, y, z);
#elif defined(USE_OPENSL)
        SL3DLocationItf locationIntf=m_pSoundSamplePtr->getLocationInterface();
        if(locationIntf)
        {
        	SLVec3D coords;
        	coords.x=x;        coords.y=y;        coords.z=z;
        	(*locationIntf)->SetLocationCartesian(locationIntf, &coords);
        }
#endif
    }
    
    void setVolume(float scale) //0 to 1
    {
#if defined(USE_OPENAL)
        alSourcef(m_iSourceId, AL_GAIN, scale);
#endif
    }
    
    void setPitch(float scale)  //0 to 1
    {
#if defined(USE_OPENAL)
        alSourcef(m_iSourceId, AL_PITCH, scale);
#endif
    }
    
    void setReferenceDistance(float distance)
    {
#if defined(USE_OPENAL)
        alSourcef(m_iSourceId, AL_REFERENCE_DISTANCE, distance);
#endif
    }
    
    void setMaxDistance(float distance)
    {
#if defined(USE_OPENAL)
        alSourcef(m_iSourceId, AL_MAX_DISTANCE, distance);
#endif
    }
    
    bool isPlaying()    
    {
#if defined(USE_OPENAL)
        ALint state;
        alGetSourcei(m_iSourceId, AL_SOURCE_STATE, &state);
        return ((state == AL_PLAYING) || (state == AL_LOOPING));
#elif defined(USE_OPENSL)
        //GetPlayState
        SLresult result;
        SLuint32 state;
        result = (*m_pSoundSamplePtr->getPlayInterface())->GetPlayState(m_pSoundSamplePtr->getPlayInterface(), &state);

        return ((state==SL_PLAYSTATE_PLAYING));
#endif
    }
    
    void resetFlags()   {   m_bPaused=false;    }
    bool isPaused()     {   return m_bPaused;   }
    
    SoundSample* getSamplePtr()   {   return m_pSoundSamplePtr;   }
#if defined(USE_OPENSL)
    bool isLoopable()	{	return m_bLoop;	}
#endif
    
private:
    unsigned int m_iSourceId;
    bool m_bPaused;
    SoundSample* m_pSoundSamplePtr; //must not delete this pointer

#if defined(USE_OPENSL)
    bool m_bLoop;	//used for android
#endif
};

#endif
