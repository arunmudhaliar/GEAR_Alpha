using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.IO;

public class stAnimationPackage {
	/*
	 *  stAnimationPackage.cpp
	 *  BMX2
	 *
	 *  Created by arun on 16/07/10.
	 *  Copyright 2010 __MyCompanyName__. All rights reserved.
	 *
	 */
	
	public stAnimationPackage()
	{
		m_pActiveCommand=null;
		m_pNextCommandOnQ=null;
		m_nTotalKeyFrames=0;
		m_cFPS=0;
		m_bAnimationPaused = false;
	}
	
//	~stAnimationPackage()
//	{
//		reset();
//	}
	
	void reset()
	{
		m_pvAnimCommandLst.Clear();
	    m_pActiveCommand=null;
		m_pNextCommandOnQ=null;
		m_nTotalKeyFrames=0;
		m_cFPS=0;
		m_bAnimationPaused = false;
	}
	
	public void readANBFile(BinaryReader reader)
	{
		m_nTotalKeyFrames=reader.ReadInt32();
		m_cFPS=reader.ReadInt32();
		
		int nCommands=0;
		nCommands=reader.ReadInt32();
		
		for(int x=0;x<nCommands;x++)
		{
			stAnimCommand command=new stAnimCommand();
			string name=ZBGlobal.readString(reader);
			command.setName(name);
            Console.WriteLine(name);
			command.setID(x);
			
			int categoryID=0;
			categoryID=reader.ReadInt32();
	        command.setCategory(categoryID);
			int priority=0;
			priority=reader.ReadInt32();
			command.setPriority(priority);
			
			int nState=0;
			nState=reader.ReadInt32();
			for(int y=0;y<nState;y++)
			{
				stAnimState state=new stAnimState();				
				state.setFlag(reader.ReadInt32());
				int startFrame=0;
				startFrame=reader.ReadInt32();
				state.setStartFrame(startFrame);
				int endFrame=0;
				endFrame=reader.ReadInt32();
				state.setEndFrame(endFrame);
				
				state.setAnimSpeed(reader.ReadSingle());
                float reserved_float = 0.0f;
                reserved_float = reader.ReadSingle();			//reserved
                reserved_float = reader.ReadSingle();
				state.setHitFrame(reader.ReadInt32());
				int nUserDefined=reader.ReadInt32();
				state.allocateUserDefinedIntBuffer(nUserDefined);
				for(int m=0;m<nUserDefined;m++)
				{
					state.setUserDefined(reader.ReadInt32(), m);
				}

				command.addState(state);
			}
			
			m_pvAnimCommandLst.Add(command);
		}
	}
	
	protected void readAndAppendANBFile(BinaryReader reader)
	{
		int nTotalKeyFrames=reader.ReadInt32();
		int cFPS=reader.ReadInt32();
		
		int nCommands=0;
		nCommands=reader.ReadInt32();
		
		int highestCategory=0;
		//find highest category
		foreach(stAnimCommand command in m_pvAnimCommandLst)
		{
			if(command.getCategory()>highestCategory)
				highestCategory=command.getCategory();
		}
		//
		int previous_total=m_pvAnimCommandLst.Count;
		
		for(int x=0;x<nCommands;x++)
		{
			stAnimCommand command=new stAnimCommand();
			string name=ZBGlobal.readString(reader);
			command.setName(name);
			command.setID(previous_total+x);
			
			int categoryID=highestCategory;
			categoryID+=reader.ReadInt32();
	        command.setCategory(categoryID);
			int priority=0;
			priority=reader.ReadInt32();
			command.setPriority(priority);
			
			int nState=0;
			nState=reader.ReadInt32();
			for(int y=0;y<nState;y++)
			{
				stAnimState state=new stAnimState();
				
				state.setFlag(reader.ReadInt32());
				int startFrame=0;
				startFrame=reader.ReadInt32();
				state.setStartFrame(startFrame);
				int endFrame=0;
				endFrame=reader.ReadInt32();
				state.setEndFrame(endFrame);
				
				state.setAnimSpeed(reader.ReadSingle());
                float reserved_float = 0.0f;
                reserved_float = reader.ReadSingle();			//reserved
                reserved_float = reader.ReadSingle();
				state.setHitFrame(reader.ReadInt32());
				int nUserDefined=reader.ReadInt32();
				state.allocateUserDefinedIntBuffer(nUserDefined);
				for(int m=0;m<nUserDefined;m++)
				{
					state.setUserDefined(reader.ReadInt32(), m);
				}
				
				command.addState(state);
			}
			
			m_pvAnimCommandLst.Add(command);
		}
	}
	
	protected void updateAnimationPackage(float dt)
	{
		if(m_bAnimationPaused == true)
			return;
		processCmdOnQ();
	
		stAnimCommand active_cmd = getActiveCommand();
	    if(active_cmd==null) return;
		if(active_cmd.isLastStateCompleted()) return;
	
		//update the current frame
		stAnimState active_state= active_cmd.getActiveState();
		
		//float nFrameToSkip = callback_overideFrameElapseTime(dt);
		//int end_frame = active_state.getEndFrame();
		//int start_frame = active_state.getStartFrame();
		
		//float calc_frame=0.0f;
		bool bEndCrossed=false;
	    bool bEndReached=false;
			
		if(active_state.isFlag((int)stAnimState.EANIMSTATEFLAG.REVERSE))
		{
//			calc_frame=active_state->getCurrentFrame()-nFrameToSkip;
//			bEndCrossed=((int)calc_frame<start_frame);
//	        bEndReached=((int)calc_frame==start_frame);
		}
		else
		{
//			calc_frame=active_state->getCurrentFrame()+nFrameToSkip;
//			bEndCrossed=((int)calc_frame>end_frame);
//	        bEndReached=((int)calc_frame==end_frame);
            //arun_check_for_mono bEndReached=!active_state.getAnimationPtr().IsPlaying(active_state.getAnimationClipName());
		}
			
		if(bEndCrossed || bEndReached)
		{
			if(!active_state.isFlag((int)stAnimState.EANIMSTATEFLAG.LOOP))
			{
//				active_state->setCurrentFrame(active_state->isFlag(stAnimState::REVERSE)?start_frame:end_frame);
				//chk if hold flag is there
				if(active_state.isFlag((int)stAnimState.EANIMSTATEFLAG.HOLD_ON_COMPLETE))
				{
					callback_holdReached();
				}
				else
				{
	                bool bForce=false;
	                if(bEndCrossed)
					{
	                    bForce=true;
	                }
	                else if(bEndReached)
	                {
	                    if(active_state.isForceAfterThisUpdate())
	                    {
	                        bForce=true;
	                        active_state.setForceAfterThisUpdate(false);
	                    }
	                    else
	                    {
	                        active_state.setForceAfterThisUpdate(true);
	                    }
	                }
	                    
	                if(bForce)
	                {
	                    //transit to next state
	                    int previous_stateID=active_cmd.getCurrentStateID();
	                    bool bStateTransit=active_cmd.transitToNextState();
	                    if(bStateTransit)
	                        callback_transitToNextState(previous_stateID);
	                    else
	                        callback_commandCompleted(active_cmd.getID());
	                }
				}
			}
			else
			{
				active_state.PlayAnimationClip();
				callback_loopCompleted(active_cmd.getCurrentStateID());
//	            if(bEndCrossed)
//				{
////	                int d=active_state->isFlag(stAnimState::REVERSE)?(unsigned int)start_frame-calc_frame:(unsigned int)calc_frame-start_frame;
////	                int diff=end_frame-start_frame;
////	                int dd=(diff>0)?d%(end_frame-start_frame):0;
////	                int frame=active_state->isFlag(stAnimState::REVERSE)?(end_frame-(dd)):(start_frame + (0));
////	                active_state->setCurrentFrame((float)frame);
////	                callback_loopCompleted(active_cmd->getCurrentStateID());
//	            }
//	            else
//	            {
////	                active_state->setCurrentFrame(calc_frame);
//	            }
			}
		}
		else
		{
//			active_state->setCurrentFrame(calc_frame);
		}
//		
//	//	DEBUG_PRINT("cur frame = %d", (int)active_state->getCurrentFrame());
	}
	
	public bool setNextCommandID(int cmd_id)
	{
	    if(m_pvAnimCommandLst.Count==0) return false;
	    
		stAnimCommand nextCommandOnQ=m_pvAnimCommandLst[cmd_id];
		if(nextCommandOnQ==m_pNextCommandOnQ) return false;
		if (m_pNextCommandOnQ != null && m_pNextCommandOnQ.getPriority() > nextCommandOnQ.getPriority()) return false;
	
		
		m_pNextCommandOnQ=nextCommandOnQ;
		m_pNextCommandOnQ.setCurrentStateID(0);
		return true;
	}
	
	stAnimCommand getNextCommandOnQ()
	{
		return m_pNextCommandOnQ;
	}
	
	bool processCmdOnQ()
	{
		if(m_pNextCommandOnQ==null) return false;
		
		logPrint("processCmdOnQ");
		
		stAnimCommand nextCommandOnQ=m_pNextCommandOnQ;
		stAnimCommand activeCommand=getActiveCommand();
		
		if(activeCommand==null)
		{
			setActiveCommand(nextCommandOnQ);
			getActiveCommand().setCurrentStateID(0);
			getActiveCommand().getActiveState().PlayAnimationClip();
			callback_commandChangedTo(getActiveCommand().getID());
			m_pNextCommandOnQ=null;
			return true;
		}
		
		//chk if the new state can set as active state
		if(nextCommandOnQ.getNoOfState()==0) return false;
		
		//chk if the active cmd is completed or not
		if(getActiveCommand().isLastStateCompleted())
		{
			setActiveCommand(nextCommandOnQ);
			getActiveCommand().setCurrentStateID(0);
			getActiveCommand().getActiveState().PlayAnimationClip();
			callback_commandChangedTo(getActiveCommand().getID());
			m_pNextCommandOnQ=null;
			return true;
		}
		
		//if the cmd is not comleted and the incoming cmd is having equal or higher priority then set to next cmd
		stAnimState activeState=activeCommand.getActiveState();
		//chk the priority
		if(nextCommandOnQ.getPriority()>=activeCommand.getPriority())
		{
			//chk if the state can be overide or not
			if(activeState.isFlag((int)stAnimState.EANIMSTATEFLAG.OVERRIDE))
			{
				setActiveCommand(nextCommandOnQ);
				getActiveCommand().setCurrentStateID(0);
				getActiveCommand().getActiveState().PlayAnimationClip();
				callback_commandChangedTo(getActiveCommand().getID());
				m_pNextCommandOnQ=null;
				return true;
			}
		}
		
		return false;
	}
	
	protected bool forceActiveHoldOrLoopCmdToNextState()
	{	
		if (getActiveCommand().getActiveState().isFlag((int)(stAnimState.EANIMSTATEFLAG.HOLD_ON_COMPLETE | stAnimState.EANIMSTATEFLAG.LOOP))) 
		{
			int previous_stateID=getActiveCommand().getCurrentStateID();
			bool bStateTransit=getActiveCommand().transitToNextState();
			if(bStateTransit)
				callback_transitToNextState(previous_stateID);
			
			return bStateTransit;
		}
		return false;
	}
	
	protected bool forceActiveCmdToNextState()
	{	
		int previous_stateID=getActiveCommand().getCurrentStateID();
		bool bStateTransit=getActiveCommand().transitToNextState();
		if(bStateTransit)
			callback_transitToNextState(previous_stateID);
		else
			callback_commandCompleted(getActiveCommand().getID());
		return bStateTransit;
	}

	protected void forceActiveCmdToLastState()
	{	
		int previous_stateID=getActiveCommand().getCurrentStateID();
		getActiveCommand().transitToLastState();
	}

	protected bool forceActiveCmdToPrevState()
	{
		int previous_stateID=getActiveCommand().getCurrentStateID();
		bool bStateTransit=getActiveCommand().transitToPrevState();
		if(bStateTransit)
			callback_transitToPrevState(previous_stateID);
		
		return bStateTransit;
	}
	
	protected bool forceActiveCmdToCompletetion()
	{
		stAnimCommand activeCmd=getActiveCommand();
		if(activeCmd==null) return false;
		
		while(activeCmd.transitToNextState()){}
		callback_commandCompleted(activeCmd.getID());
		return true;
	}
	
	protected void pauseAnimation()
	{
		m_bAnimationPaused = true;
	}
	
	protected void resumeAnimation()
	{
		m_bAnimationPaused = false;
	}
	
	stAnimCommand getCommand(int index)
	{
		if(index>=m_pvAnimCommandLst.Count) return null;
		
		return m_pvAnimCommandLst[index];
	}
	
	protected void logPrint(string str)
	{
		if(m_bDebugLog)
		{
			Console.WriteLine(str);
		}
	}
	
	public stAnimCommand getActiveCommand()				{	return m_pActiveCommand;	}
	void setActiveCommand(stAnimCommand command)	{	m_pActiveCommand=command;	}
	
	public virtual void callback_transitToNextState(int stateIndex){}
	public virtual void callback_transitToPrevState(int stateIndex){}
	public virtual void callback_commandCompleted(int commandId){}
	public virtual void callback_loopCompleted(int stateIndex){}
	public virtual void callback_holdReached(){}
	public virtual void callback_commandChangedTo(int commandId){}
    public virtual float callback_overideFrameElapseTime(float dt)
	{
	    return dt * (float)m_cFPS;
	}
	
	protected List<stAnimCommand> getAnimCommandList()	{	return m_pvAnimCommandLst;	}
	
	public void enableAnimPackageDebugLog()		{	m_bDebugLog=true;	}
	public void disableAnimPackageDebugLog()	{	m_bDebugLog=false;	}
	
	private int m_nTotalKeyFrames;
	private int m_cFPS;
	private List<stAnimCommand> m_pvAnimCommandLst = new List<stAnimCommand>();
	private stAnimCommand m_pActiveCommand;
	private stAnimCommand m_pNextCommandOnQ;		//for future enhancements
	private bool m_bAnimationPaused;
	private bool m_bDebugLog=false;
}
