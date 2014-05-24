//
//  SoundSample.h
//  testOpenAL
//
//  Created by arun on 31/05/11.
//  Copyright 2011 __MyCompanyName__. All rights reserved.
//
#ifndef SOUNDSAMPLE_H
#define SOUNDSAMPLE_H

//#if defined(TARGET_IPHONE_SIMULATOR) || defined(TARGET_OS_IPHONE)
//	#define USE_OPENAL	1
//#elif defined(ANDROID)
//	#define USE_OPENSL	1
//#endif

#include "../core/basicIncludes.h"

#if defined(USE_OPENAL)
	#include <OpenAL/al.h>
	#include <OpenAL/alc.h>
#elif defined(USE_OPENSL)
	#include <SLES/OpenSLES.h>
	#include <SLES/OpenSLES_Android.h>
#endif

#include <stdio.h>

#include "../util/gxFile.h"
#include "../core/gxDebug.h"

class DECLSPEC SoundSample
{
public:
    SoundSample(const char* filename);
    ~SoundSample();
    
    const char* getName()   {   return m_cszName;   }
    unsigned int getBufferID()              {   return m_iBufferId;     }
    int getFrequency()                      {   return m_iFrequency;    }
    int getBufferSize()						{	return m_iSize;			}
    
#if defined(USE_OPENAL)
    bool loadFromFile(gxFile& file);
#elif defined(USE_OPENSL)
    bool loadFromFile(gxFile& file, const SLEngineItf engineEngine, const SLObjectItf outputMixObject);

    SLObjectItf getPlayerObject()			{	return m_pPlayer;		}
    SLPlayItf getPlayInterface()			{	return m_pPlayerPlay;	}
    SLBufferQueueItf getQueueInterface()	{	return m_pQueue;		}
//    SLSeekItf getSeekInterface()			{	return m_pPlayerSeek;	}
    SL3DLocationItf getLocationInterface()	{	return m_pLocationItf;		}

    unsigned char* getRawSoundBuffer()		{	return m_pRawSoundBuffer;	}
#endif

private:
    char m_cszName[256];
    unsigned int m_iBufferId;
    int m_iFrequency;
    int m_iFormat;
    int m_iSize;

#if defined(USE_OPENSL)
    SLBufferQueueItf m_pQueue;
    SLObjectItf m_pPlayer;
    SLPlayItf m_pPlayerPlay;
    SLVolumeItf m_pVolume;
    SL3DLocationItf m_pLocationItf;
//	SLSeekItf m_pPlayerSeek;
    unsigned char* m_pRawSoundBuffer;	//need to delete manually for android
#endif
};

#endif
