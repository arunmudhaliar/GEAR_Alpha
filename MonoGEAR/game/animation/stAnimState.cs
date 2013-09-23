using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

public class stAnimState {
	
/*
 *  stAnimState.h
 *  BMX2
 *
 *  Created by arun on 16/07/10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

	public enum EANIMSTATEFLAG
	{
		PLAYONCE		= (1<<0),
		LOOP			= (1<<1),
		REVERSE			= (1<<2),
		OVERRIDE		= (1<<3),
		HOLD_ON_COMPLETE= (1<<4),
		
		BLEND_ANIMATION = (1<<5),
        CAMERA_SHAKE 	= (1<<6),
		
		HIT_STATE		= (1<<7),
		RESTRICT_INPUT	= (1<<8)
	};
	
	public stAnimState()
	{
		m_pszName=null;
		m_cStartFrame=0;
		m_cEndFrame=0;
		//m_pUserDefined=NULL;
		m_eFlags=0;
		m_fCurFrame=0;
        m_cPriority=0;
        m_fFrameElapseTime=0.0f;
        m_bForceToNextStateAfterThisUpdate=false;
		m_fAnimSpeed=1.0f;
	}
	
//	~stAnimState()
//	{
//		GX_DELETE(m_pszName);
//		//GX_DELETE(m_pUserDefined);
//	}
	
	public void setFlag(int flag)	{	m_eFlags|=flag;					}
	public int getFlag()			{	return m_eFlags;				}
	public bool isFlag(int flag)	{	return (bool)((m_eFlags&flag)>0);	}
	public void resetFlags()		{	m_eFlags=0;						}
	
	public void setCurrentFrame(float frame)
	{
        if((int)m_fCurFrame!=(int)frame)
        {
            m_fFrameElapseTime=0.0f;
        }
        m_fCurFrame=frame;
	}
	public float getCurrentFrame()	{	return m_fCurFrame;				}
	
	public int getStartFrame()		{	return m_cStartFrame;			}
	public int getEndFrame()		{	return m_cEndFrame;				}

	
	public void setStartFrame(int startFrame)	{	m_cStartFrame=startFrame;	}
	public void setEndFrame(int endFrame)		{	m_cEndFrame=endFrame;		}

    public void setFrameElapseTime(float elapse)   {   m_fFrameElapseTime=elapse;  }
    public float getFrameElapseTime()              {   return m_fFrameElapseTime;  }
	
    public void setForceAfterThisUpdate(bool flag) {   m_bForceToNextStateAfterThisUpdate=flag;    }
    public bool isForceAfterThisUpdate()           {   return m_bForceToNextStateAfterThisUpdate;  }
    
	public void setAnimationClip(string animationClipName, IntPtr animationPtr)
	{
		m_pAnimationClipName=animationClipName;
		m_pAnimationPtr=animationPtr;
	}
	
	public string getAnimationClipName()
	{
		return m_pAnimationClipName;
	}

    public IntPtr getAnimationPtr()
	{
		return m_pAnimationPtr;
	}
	
	public void PlayAnimationClip()
	{
		//if(m_pAnimationPtr.IsPlaying(m_pAnimationClipName)) return;
		
		//Debug.Log("play animation "+m_pAnimationClipName);
		if(isFlag((int)EANIMSTATEFLAG.BLEND_ANIMATION))
		{
			Console.WriteLine("blending....");
			//arun_check_for_mono m_pAnimationPtr.CrossFade(m_pAnimationClipName, 0.1f);
		}
		else
		{
            //arun_check_for_mono m_pAnimationPtr.Stop();
            //arun_check_for_mono m_pAnimationPtr.PlayQueued(m_pAnimationClipName);
		}
	}
	
	public void setAnimSpeed(float speed)	{	m_fAnimSpeed=speed;		}
	public float getAnimSpeed()				{	return m_fAnimSpeed;	}
	
	public void setHitFrame(int frame)		{	m_iHitFrame=frame;	}
	public int getHitFrame()				{	return m_iHitFrame;	}
	
	public void allocateUserDefinedIntBuffer(int nUserDefined)
	{
		if(nUserDefined<=0) return;
		m_iszUserDefined=new int[nUserDefined];
	}
	public void setUserDefined(int val, int index)	{	m_iszUserDefined[index]=val;	}
	public int getUserDefined(int index)	{	return m_iszUserDefined[index];	}
	
	private string m_pszName;
	private int m_cStartFrame;
	private int m_cEndFrame;
	private int m_cPriority;
	private int m_eFlags;
	private float m_fCurFrame;
	private int[] m_iszUserDefined = null;
	private float m_fAnimSpeed;
	private int m_iHitFrame;
	
	//void* m_pUserDefined;
    private float m_fFrameElapseTime; //use for override default behaviour of current frame calculation
    private bool m_bForceToNextStateAfterThisUpdate;
	
	//for unity
	private string m_pAnimationClipName;
    private IntPtr m_pAnimationPtr;   //Animation
}
