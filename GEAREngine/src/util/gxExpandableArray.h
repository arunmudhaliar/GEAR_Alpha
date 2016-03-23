#pragma once

#include "../core/basicIncludes.h"

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
        headNode = NULL;
        tailNode = NULL;
        nodeIterator = NULL;
        
        count=0;
        allocationSize=0;
        deltaAllocationSize=0;
    }
    
	ExpandableArray(int initialAllocationSize/*=0*/, int deltaAllocationSize=1)
	{
		allocationSize=0;
		deltaAllocationSize=deltaAllocationSize;

		headNode=tailNode=nodeIterator=NULL;
		count=0;

		Allocate(initialAllocationSize);
	}

	~ExpandableArray()
	{
		GX_DELETE(headNode);
	}

    void Init(int initialAllocationSize=0, int deltaAllocationSize=1)
    {
        allocationSize=0;
		deltaAllocationSize=deltaAllocationSize;
        
		headNode=tailNode=nodeIterator=NULL;
		count=0;
        
		Allocate(initialAllocationSize); 
    }
    
	void Append(T data)
	{
		if(!nodeIterator)
		{
			Allocate(deltaAllocationSize);
		}

		nodeIterator->SetData(data);
		nodeIterator=nodeIterator->GetNext();
		count++;
	}

	//T GetAt(int index)
	//{
	//	if(index>=count) return NULL;
	//	return nodeIterator->GetData();
	//}

	ExpandableArrayNode<T>* GetRoot()	const {	return headNode;		}

	int GetCount()						const {	return count;	}
	void Clear()
	{
		count=0;
		nodeIterator=headNode;
	}


    ExpandableArrayNode<T>* getNode(int pos) const
	{
		if(pos<0 || pos>=count)		//out of bound check
			return NULL;
		
		ExpandableArrayNode<T>* node=GetRoot();
		for(int x=0;x<count;x++)
		{
			if(x==pos)
				break;				//position reached
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
		for(int x=0;x<count;x++)
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
            count--;
            allocationSize--;
            return  ret_data;
		}
        else if(parentnode && !childnode)
        {
            parentnode->SetNext(nodeIterator);
            if(nodeIterator)nodeIterator->SetPrevious(parentnode);
            if(tailNode==node)   //if the last node is about to delete
            {
                tailNode=node->GetPrevious();
            }

            T ret_data=node->GetData();
            node->SetDestroyChild(false);
            GX_DELETE(node);
            count--;
            allocationSize--;
            return  ret_data;
        }
        else if(!parentnode)
        {
            headNode=node->GetNext();
            if(tailNode==node)   //if the last node is about to delete
            {
                tailNode=node->GetPrevious();
            }
            if(headNode)headNode->SetPrevious(NULL);
       		T ret_data=node->GetData();
            node->SetDestroyChild(false);
            GX_DELETE(node);
            count--;
            allocationSize--;
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
            count--;
            allocationSize--;
            return  ret_data;
		}
        else if(parentnode && !childnode)
        {
            parentnode->SetNext(nodeIterator);
            if(nodeIterator)nodeIterator->SetPrevious(parentnode);
            if(tailNode==node)   //if the last node is about to delete
            {
                tailNode=node->GetPrevious();
            }
            T ret_data=node->GetData();
            node->SetDestroyChild(false);
            GX_DELETE(node);
            count--;
            allocationSize--;
            return  ret_data;
        }
        else if(!parentnode)
        {
            headNode=node->GetNext();
            if(tailNode==node)   //if the last node is about to delete
            {
                tailNode=node->GetPrevious();
            }
            if(headNode)headNode->SetPrevious(NULL);
       		T ret_data=node->GetData();
            node->SetDestroyChild(false);
            GX_DELETE(node);
            count--;
            allocationSize--;
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
			if(!headNode)
			{
				headNode = new ExpandableArrayNode<T>();
				tailNode=nodeIterator=headNode;
				allocationSize++;
				continue;
			}

			tailNode->SetNext(new ExpandableArrayNode<T>());
            tailNode->GetNext()->SetPrevious(tailNode);
			tailNode=tailNode->GetNext();
			if(!nodeIterator)nodeIterator=tailNode;
			allocationSize++;
		}

		return true;
	}

	ExpandableArrayNode<T>* headNode;
	ExpandableArrayNode<T>* tailNode;	//not valid after a remove() or removeAt(), be careful while using the pointer
	ExpandableArrayNode<T>* nodeIterator;

	int count;
	int allocationSize;
	int deltaAllocationSize;
};