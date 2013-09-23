using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;


public class stAnimCommand {
	public stAnimCommand()
	{
		m_cID=-1;
		m_pActiveState=null;
		m_cCurrentStateID=-1;
		m_bLastStateCompleted=false;
		m_cPriority=0;
	    setCategory(-1);
	    m_pszName=null;
	}
	
	//~stAnimCommand();
	
	public void reset()
	{
		m_pvAnimStateLst.Clear();
		m_pszName=null;
	}
	
	public void update(float dt)
	{
	}
	
	public void setID(int cmd_id)	{	m_cID=cmd_id;	}
	public int getID()				{	return m_cID;	}
	
	public string getName()	{	return m_pszName;	}
	public stAnimState getActiveState()	{	return m_pActiveState;	}
	
	public void setCurrentStateID(int stateID)
	{
		if(stateID>=m_pvAnimStateLst.Count)
		{
			Console.WriteLine("clip "+m_pszName+" stateID "+stateID+", animStateCount "+m_pvAnimStateLst.Count);
		}
		
		m_bLastStateCompleted=false;
		m_cCurrentStateID=stateID;
		m_pActiveState=m_pvAnimStateLst[stateID];
		
	    m_pActiveState.setForceAfterThisUpdate(false);
	    m_pActiveState.setCurrentFrame(-1.0f);
		if(m_pActiveState.isFlag((int)stAnimState.EANIMSTATEFLAG.REVERSE))
			m_pActiveState.setCurrentFrame((float)m_pActiveState.getEndFrame());
		else
			m_pActiveState.setCurrentFrame((float)m_pActiveState.getStartFrame());
	}
	
	public int getCurrentStateID()	{	return m_cCurrentStateID;	}
	public int getNoOfState()		{	return m_pvAnimStateLst.Count;	}
	
	public stAnimState getState(int index)	{	return m_pvAnimStateLst[index];	}
	
	public bool transitToNextState()	
	{
		int cur_state=getCurrentStateID();
		if(cur_state==getNoOfState()-1)
		{
			m_bLastStateCompleted=true;
			return false;
		}
		
		setCurrentStateID(++cur_state);
		getActiveState().PlayAnimationClip();
		return true;
	}
	
	public void transitToLastState()	
	{
		int cur_state=getCurrentStateID();
		setCurrentStateID(getNoOfState()-1);
		getActiveState().PlayAnimationClip();
	}
	
	//can not transit to previous state once the command is completed.
	public bool transitToPrevState()
	{
		if (m_bLastStateCompleted) return false;
	
		int cur_state=getCurrentStateID();
		if(cur_state == 0)
		{
			//m_bLastStateCompleted=true;
			return false;
		}
		
		setCurrentStateID(--cur_state);
		getActiveState().PlayAnimationClip();
		return true;
	}
	
	public bool isLastStateCompleted()	{	return m_bLastStateCompleted;	}
	
	public void setName(string name)	{	m_pszName=name;	}
	
	public void addState(stAnimState state)
	{
		m_pvAnimStateLst.Add(state);
	}
	
	public int getPriority()					{   return m_cPriority;     }
	public void setPriority(int priority)		{   m_cPriority=priority;   }
	public int getCategory()                   {   return m_cCategoryID;   }
    public void setCategory(int category)      {   m_cCategoryID=category; }
    
	private int m_cID;
	private int m_cCurrentStateID;
	private string m_pszName;
	private int m_cPriority;
	private List<stAnimState> m_pvAnimStateLst = new List<stAnimState>();
	private stAnimState m_pActiveState;
	private bool m_bLastStateCompleted;
    private int m_cCategoryID;
}
