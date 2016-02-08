#pragma once

#include "basicIncludes.h"

class DECLSPEC Timer
{
public:
	
	static void init();				//should be called when game initializes
	static void update();			//update the timer variables for each frame
	static void update(float targetFPS);
	static void reset();			//resets the timer variables
	
	static float getDtinSec()		{	return m_fDT;			}
	static int getDTinMilliSec()	{	return m_iDT;			}
	static float getFPS()			{	return m_fFPS;			}
	static float getElapsedTime()	{	return m_fElapsedTime;	}
	
	static double getCurrentTimeInSec();
	static unsigned long getCurrentTimeInMilliSec();
	
	static void setTimeScale(float timescale)	{ m_fTimeScale=timescale; }
	static float getTimeScale() { return m_fTimeScale; }

private:
	static float	m_fFPS;			//frames
	static float	m_fDT;			//in sec
	static int		m_iDT;			//in milli sec
	static float	m_fElapsedTime;	//in sec
	static double	m_fPrevTime;	//stored for internal purpose
    
    static float    m_fAveragingTime;   //calculate after this much amount of secs
    static double   m_fLastTime;        //for internal calculations of averaging fps
    static int      m_iFrameCount;      //frame counter

	static float	m_fTimeScale;	//global time scale for the engine (especially for shaders)
};