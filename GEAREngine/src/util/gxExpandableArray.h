#ifndef GXEXPANDABLEARRAY_H
#define GXEXPANDABLEARRAY_H

#include "../core/basicIncludes.h"
//#define GX_DELETE(x) if(x){ delete x; x=NULL;}

template<class T>
class ExpandableArrayNode
{
public:
	ExpandableArrayNode()
	{
		next=NULL;
        previous=NULL;
		data=NULL;
        m_bDestroyChild=true;
	}

	~ExpandableArrayNode()
	{
        if(m_bDestroyChild)
            GX_DELETE(next);
	}

	void SetData(T _data)                           {	data=_data;             }
	T GetData()                                     {	return data;            }
	ExpandableArrayNode<T>* GetNext()               {	return next;            }
	ExpandableArrayNode<T>* GetPrevious()           {	return previous;        }
	void SetNext(ExpandableArrayNode<T>* _next)     {	next=_next;             }
    void SetPrevious(ExpandableArrayNode<T>* _prev) {   previous=_prev;         }
    void SetDestroyChild(bool bFlag)                {   m_bDestroyChild=bFlag;  }
    
private:
	T data;
	ExpandableArrayNode<T>* next;
   	ExpandableArrayNode<T>* previous;
    bool m_bDestroyChild;
};

template<class T>
class ExpandableArray
{
public:
    ExpandableArray()
    {
        m_pHead = NULL;
        m_pTail = NULL;
        m_pIteratorPtr = NULL;
        
        m_cCount=0;
        m_cAllocationSize=0;
        m_cDeltaAllocationSize=0;
    }
    
	ExpandableArray(int initialAllocationSize/*=0*/, int deltaAllocationSize=1)
	{
		m_cAllocationSize=0;
		m_cDeltaAllocationSize=deltaAllocationSize;

		m_pHead=m_pTail=m_pIteratorPtr=NULL;
		m_cCount=0;

		Allocate(initialAllocationSize);
	}

	~ExpandableArray()
	{
		GX_DELETE(m_pHead);
	}

    void Init(int initialAllocationSize=0, int deltaAllocationSize=1)
    {
        m_cAllocationSize=0;
		m_cDeltaAllocationSize=deltaAllocationSize;
        
		m_pHead=m_pTail=m_pIteratorPtr=NULL;
		m_cCount=0;
        
		Allocate(initialAllocationSize); 
    }
    
	void Append(T data)
	{
		if(!m_pIteratorPtr)
		{
			Allocate(m_cDeltaAllocationSize);
		}

		m_pIteratorPtr->SetData(data);
		m_pIteratorPtr=m_pIteratorPtr->GetNext();
		m_cCount++;
	}

	//T GetAt(int index)
	//{
	//	if(index>=m_cCount) return NULL;
	//	return m_pIteratorPtr->GetData();
	//}

	ExpandableArrayNode<T>* GetRoot()	const {	return m_pHead;		}

	int GetCount()						const {	return m_cCount;	}
	void Clear()
	{
		m_cCount=0;
		m_pIteratorPtr=m_pHead;
	}


    ExpandableArrayNode<T>* getNode(int pos) const
	{
		if(pos<0 || pos>=m_cCount)		//out of bound check
			return NULL;
		
		ExpandableArrayNode<T>* node=GetRoot();
		for(int x=0;x<m_cCount;x++)
		{
			if(x==pos)
				break;					//position reached
			else
				node=node->GetNext();	
		}
		
		return node;
	}

    ExpandableArrayNode<T>* find(T data) const
    {
        int pos=-1;
        return getNode(data, pos);
    }
    
    ExpandableArrayNode<T>* getNode(T data, int& pos) const
	{
        pos=-1;
		if(data==NULL) return NULL;
		
		ExpandableArrayNode<T>* node=this->GetRoot();
		for(int x=0;x<m_cCount;x++)
		{
            pos=x;
			if(data==node->GetData())
				return node;					//position reached
			else
				node=node->GetNext();	
		}
		
		return NULL;
	}
    
    //removeAt will delete the node but not the actual data, in failure it will return NULL.
	T removeAt(int pos)
	{
		ExpandableArrayNode<T>* node=getNode(pos);
		if(node==NULL) return NULL;		//if invalid position
        
		ExpandableArrayNode<T>* parentnode=getNode(pos-1);
        ExpandableArrayNode<T>* childnode=getNode(pos+1);
        
		if(parentnode && childnode)
		{
            parentnode->SetNext(childnode);
            childnode->SetPrevious(parentnode);
       		T ret_data=node->GetData();
            node->SetDestroyChild(false);
            GX_DELETE(node);
            m_cCount--;
            m_cAllocationSize--;
            return  ret_data;
		}
        else if(parentnode && !childnode)
        {
            parentnode->SetNext(m_pIteratorPtr);
            if(m_pIteratorPtr)m_pIteratorPtr->SetPrevious(parentnode);
            if(m_pTail==node)   //if the last node is about to delete
            {
                m_pTail=node->GetPrevious();
            }

            T ret_data=node->GetData();
            node->SetDestroyChild(false);
            GX_DELETE(node);
            m_cCount--;
            m_cAllocationSize--;
            return  ret_data;
        }
        else if(!parentnode)
        {
            m_pHead=node->GetNext();
            if(m_pTail==node)   //if the last node is about to delete
            {
                m_pTail=node->GetPrevious();
            }
            if(m_pHead)m_pHead->SetPrevious(NULL);
       		T ret_data=node->GetData();
            node->SetDestroyChild(false);
            GX_DELETE(node);
            m_cCount--;
            m_cAllocationSize--;
            return  ret_data;
        }
        
        return NULL;
	}
    
    T remove(T data)
    {
        int pos=-1;
        ExpandableArrayNode<T>* node=getNode(data, pos);
		if(node==NULL) return NULL;		//if invalid position
        
		ExpandableArrayNode<T>* parentnode=getNode(pos-1);
        ExpandableArrayNode<T>* childnode=getNode(pos+1);
        
		if(parentnode && childnode)
		{
            parentnode->SetNext(childnode);
            childnode->SetPrevious(parentnode);
       		T ret_data=node->GetData();
            node->SetDestroyChild(false);
            GX_DELETE(node);
            m_cCount--;
            m_cAllocationSize--;
            return  ret_data;
		}
        else if(parentnode && !childnode)
        {
            parentnode->SetNext(m_pIteratorPtr);
            if(m_pIteratorPtr)m_pIteratorPtr->SetPrevious(parentnode);
            if(m_pTail==node)   //if the last node is about to delete
            {
                m_pTail=node->GetPrevious();
            }
            T ret_data=node->GetData();
            node->SetDestroyChild(false);
            GX_DELETE(node);
            m_cCount--;
            m_cAllocationSize--;
            return  ret_data;
        }
        else if(!parentnode)
        {
            m_pHead=node->GetNext();
            if(m_pTail==node)   //if the last node is about to delete
            {
                m_pTail=node->GetPrevious();
            }
            if(m_pHead)m_pHead->SetPrevious(NULL);
       		T ret_data=node->GetData();
            node->SetDestroyChild(false);
            GX_DELETE(node);
            m_cCount--;
            m_cAllocationSize--;
            return  ret_data;
        }
        
        return NULL;
    }
    
private:

	bool Allocate(int sz)
	{
		if(sz<=0) return false;

		while(sz--)
		{
			if(!m_pHead)
			{
				m_pHead = new ExpandableArrayNode<T>();
				m_pTail=m_pIteratorPtr=m_pHead;
				m_cAllocationSize++;
				continue;
			}

			m_pTail->SetNext(new ExpandableArrayNode<T>());
            m_pTail->GetNext()->SetPrevious(m_pTail);
			m_pTail=m_pTail->GetNext();
			if(!m_pIteratorPtr)m_pIteratorPtr=m_pTail;
			m_cAllocationSize++;
		}

		return true;
	}

	ExpandableArrayNode<T>* m_pHead;
	ExpandableArrayNode<T>* m_pTail;	//not valid after a remove() or removeAt(), be careful while using the pointer
	ExpandableArrayNode<T>* m_pIteratorPtr;

	int m_cCount;
	int m_cAllocationSize;
	int m_cDeltaAllocationSize;
};
#endif