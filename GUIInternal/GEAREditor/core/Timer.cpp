//
//  untitled.mm
//  GEARv1.0
//
//  Created by Samarth on 24/06/10.
//  Copyright 2010 __MyCompanyName__. All rights reserved.
//

#include "Timer.h"
#if defined(TARGET_IPHONE_SIMULATOR) || defined(TARGET_OS_IPHONE)
	#include <CoreFoundation/CFDate.h>
#elif defined(WIN32)
	#include <Windows.h>
	#pragma comment( lib, "Winmm.lib")
#elif defined(ANDROID)
#include <time.h>
static long _getTime(void)
{
    struct timeval  now;

    gettimeofday(&now, NULL);
    return (long)(now.tv_sec*1000 + now.tv_usec/1000);
}

#endif

float	Timer::m_fFPS=0.0f;             //frames
float	Timer::m_fDT=0.0f;              //in sec
int		Timer::m_iDT=0;                 //in milli sec
float	Timer::m_fElapsedTime=0.0f;     //in sec
double	Timer::m_fPrevTime=0.0f;
float   Timer::m_fAveragingTime=0.5f;   //in sec
int     Timer::m_iFrameCount=0;
double  Timer::m_fLastTime=0.0f;

void Timer::init()
{
	reset();
}

void Timer::update()
{
	double curTime=0.0;
#if defined(TARGET_IPHONE_SIMULATOR) || defined(TARGET_OS_IPHONE)
	curTime=CFAbsoluteTimeGetCurrent();
#elif defined(WIN32)
	curTime=(double)timeGetTime()/1000.0;
#elif defined(ANDROID)
	curTime=(double)_getTime()/1000.0;
#endif
	m_fDT=(float)(curTime-m_fPrevTime);
	if(m_fDT<=0.0f)
	{
		m_fDT=0.03f;
	}
	m_iDT=(int)(m_fDT*1000.0f);
	m_fElapsedTime+=m_fDT;
    
    if(((float)(curTime-m_fLastTime))>=m_fAveragingTime)
    {
        m_fFPS=m_iFrameCount/((float)(curTime-m_fLastTime));
        m_iFrameCount=0;
        m_fLastTime=curTime;
    }
    else
    {
        m_iFrameCount++;
    }
	m_fPrevTime=curTime;
}

void Timer::update(float targetFPS)
{
	double curTime=0.0;
#if defined(TARGET_IPHONE_SIMULATOR) || defined(TARGET_OS_IPHONE)
	curTime=CFAbsoluteTimeGetCurrent();
#elif defined(WIN32)
	curTime=(double)timeGetTime()/1000.0;
#elif defined(ANDROID)
	curTime=(double)_getTime()/1000.0;
#endif
	m_fDT=(float)(curTime-m_fPrevTime);
	if(m_fDT<=0.0f)
	{
		m_fDT=0.03f;
	}
	while(m_fDT<(1.0f/targetFPS))
	{
	#if defined(TARGET_IPHONE_SIMULATOR) || defined(TARGET_OS_IPHONE)
		curTime=CFAbsoluteTimeGetCurrent();
	#elif defined(WIN32)
		curTime=(double)timeGetTime()/1000.0;
	#elif defined(ANDROID)
		curTime=(double)_getTime()/1000.0;
	#endif
		m_fDT=(float)(curTime-m_fPrevTime);
	}
	m_iDT=(int)(m_fDT*1000.0f);
	m_fElapsedTime+=m_fDT;
	m_fFPS=1.0f/m_fDT;
	m_fPrevTime=curTime;
}

void Timer::reset()
{
	m_fFPS=0.0f;		
	m_fDT=0.0f;		
	m_iDT=0;			
	m_fElapsedTime=0.0f;
#if defined(TARGET_IPHONE_SIMULATOR) || defined(TARGET_OS_IPHONE)
	m_fPrevTime=CFAbsoluteTimeGetCurrent();
#elif defined(WIN32)
	m_fPrevTime=(double)timeGetTime()/1000.0;
#elif defined(ANDROID)
	m_fPrevTime=(double)_getTime()/1000.0;
#endif
    m_fLastTime=m_fPrevTime;
}

double Timer::getCurrentTimeInSec()
{
#if defined(TARGET_IPHONE_SIMULATOR) || defined(TARGET_OS_IPHONE)
	return CFAbsoluteTimeGetCurrent();
#elif defined(WIN32)
	return (double)timeGetTime()/1000.0;
#elif defined(ANDROID)
	return (double)_getTime()/1000.0;
#endif
}

unsigned long Timer::getCurrentTimeInMilliSec()
{
#if defined(TARGET_IPHONE_SIMULATOR) || defined(TARGET_OS_IPHONE)
	return CFAbsoluteTimeGetCurrent()*1000;
#elif defined(WIN32)
	return timeGetTime();
#elif defined(ANDROID)
	return _getTime();
#endif
}