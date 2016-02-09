#pragma once

#include "../core/basicIncludes.h"

template<class T>
class stLinkNode
{
public:
	stLinkNode(const T& data)
	{
		m_cData=data;
		m_pNext=NULL;
		m_pParent=NULL;
	}
	
	~stLinkNode()
	{
		//destroy the next node
		if(m_pNext)
		{
			GX_DELETE(m_pNext);
			m_pNext=NULL;
		}
	}
	
	void setParent(stLinkNode<T>* parent)	{	m_pParent=parent;	}
	stLinkNode<T>* getParent()				{	return m_pParent;	}

	void setNext(stLinkNode<T>* next)
	{
		m_pNext=next;
		if(m_pNext)
			m_pNext->m_pParent=(stLinkNode<T>*)this;
	}
	stLinkNode<T>* getNext()				{	return m_pNext;		}
	
    T getData()             {   return m_cData; }
    void setData(T data)    {   m_cData=data;   }
private:
	T m_cData;
	stLinkNode<T>* m_pNext;
	stLinkNode<T>* m_pParent;
};


template<class T> class bxLinkedList
{
public:
	bxLinkedList()
	{
		headNode=NULL;
		tailNode=NULL;
		nodeCount=0;
	}
	
	~bxLinkedList()
	{
		clearAll();
	}

	T insertHead(T data)
	{
		stLinkNode<T>* head=getHead();
		//create a new node and pass the data to it
		stLinkNode<T>* newHead=new stLinkNode<T>(data);
		newHead->setNext(head);
		
		//set the head to the newly created pointer
		setHead(newHead);
		
		if(nodeCount==0) setTail(newHead);	//if this is the first node
		
		nodeCount++;
		
		return data;
	}

	T insertTail(T data)
	{
		stLinkNode<T>* tail=getTail();
		//create a new node and pass the data to it
		stLinkNode<T>* newTail=new stLinkNode<T>(data);
		if(tail)
			tail->setNext(newTail);
		
		//set the tail to the newly created pointer
		setTail(newTail);
		
		if(nodeCount==0) setHead(newTail);	//if this is the first node
		
		nodeCount++;
		
		return data;
	}

	T insertAfter(T data, int pos)
	{
		stLinkNode<T>* node=getNode(pos);
		
		if(node==NULL) return NULL;		//if invalid position
		
		stLinkNode<T>* newNode=new stLinkNode<T>(data);
		newNode->setNext(node->getNext());
		node->setNext(newNode);

		if(headNode==tailNode) setTail(newNode);
		
		nodeCount++;
		return data;
	}

    T remove(T data)
    {
        stLinkNode<T>* node=getHead();
        while(node)
        {
            T ret_data=node->getData();
            if(ret_data==data)
            {
                //remove logic goes here
                stLinkNode<T>* parentnode=node->getParent();
                if(parentnode)
                {
                    if(tailNode==node)
                        setTail(parentnode);
                    parentnode->setNext(node->getNext());
                }
                
                if(node==headNode)
                    setHead(node->getNext());
                
                nodeCount--;
                if(nodeCount==0) setTail(NULL);
                node->setNext(NULL);
                node->setParent(NULL);
                node->setData(NULL);
                GX_DELETE(node);				//this will only destroy the node.
                //
                return ret_data;
            }
            node=node->getNext();
        }
        
        return NULL;
    }
    
	//removeAt will delete the node but not the actual data, in failure it will return NULL.
	T removeAt(int pos)
	{
		stLinkNode<T>* node=getNode(pos);
		if(node==NULL) return NULL;		//if invalid position

		stLinkNode<T>* parentnode=node->getParent();
		if(parentnode)
		{
			if(tailNode==node)
				setTail(parentnode);
			parentnode->setNext(node->getNext());
		}

		if(node==headNode)
			setHead(node->getNext());

		nodeCount--;
		if(nodeCount==0) setTail(NULL);

		T ret_data=node->getData();
		node->setNext(NULL);
		node->setParent(NULL);
		node->setData(NULL);
		GX_DELETE(node);				//this will only destroy the node.
		return ret_data;
	}

	T getAt(int pos) const
	{
		stLinkNode<T>* node=getNode(pos);
		return node->getData();
	}

	T operator[](int pos) const		{	return getAt(pos);	}

	int getSize()	{	return nodeCount;	}
	int size()		{	return nodeCount;	}

	bool isEmpty()	{	return (getSize()==0)?true:false;	}

    T find(T data)
    {
        stLinkNode<T>* node=getHead();
        while (node)
        {
            if(data==node->getData())
                return node->getData();
            node=node->getNext();
        }
        
        return NULL;
    }
    
	//clears all the node from the linkedlist.
	void clearAll()
	{
		if(headNode)
		{
			GX_DELETE(headNode);
		}
		headNode=NULL;
		tailNode=NULL;
        nodeCount=0;
	}

	stLinkNode<T>* getHead() const			{	return headNode;	}
    stLinkNode<T>* getTail() const			{	return tailNode;	}

private:
	void setHead(stLinkNode<T>* head)		{	headNode=head;	if(headNode)headNode->setParent(NULL);	}
	void setTail(stLinkNode<T>* tail)	{	tailNode=tail;	}
	
	stLinkNode<T>* getNode(int pos) const
	{
		if(pos<0 || pos>=nodeCount)		//out of bound check
			return NULL;
		
		stLinkNode<T>* node=getHead();
		for(int x=0;x<nodeCount;x++)
		{
			if(x==pos)
				break;					//position reached
			else
				node=node->getNext();	
		}
		
		return node;
	}
	
	
	stLinkNode<T>* headNode;
	stLinkNode<T>* tailNode;
	int				nodeCount;
};