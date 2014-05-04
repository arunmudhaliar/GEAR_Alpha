#ifndef LINKEDLIST_H
#define LINKEDLIST_H

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
		m_pHead=NULL;
		m_pTail=NULL;
		m_nCnt=0;
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
		
		if(m_nCnt==0) setTail(newHead);	//if this is the first node
		
		m_nCnt++;
		
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
		
		if(m_nCnt==0) setHead(newTail);	//if this is the first node
		
		m_nCnt++;
		
		return data;
	}

	T insertAfter(T data, int pos)
	{
		stLinkNode<T>* node=getNode(pos);
		
		if(node==NULL) return NULL;		//if invalid position
		
		stLinkNode<T>* newNode=new stLinkNode<T>(data);
		newNode->setNext(node->getNext());
		node->setNext(newNode);

		if(m_pHead==m_pTail) setTail(newNode);
		
		m_nCnt++;
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
                    if(m_pTail==node)
                        setTail(parentnode);
                    parentnode->setNext(node->getNext());
                }
                
                if(node==m_pHead)
                    setHead(node->getNext());
                
                m_nCnt--;
                if(m_nCnt==0) setTail(NULL);
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
			if(m_pTail==node)
				setTail(parentnode);
			parentnode->setNext(node->getNext());
		}

		if(node==m_pHead)
			setHead(node->getNext());

		m_nCnt--;
		if(m_nCnt==0) setTail(NULL);

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

	int getSize()	{	return m_nCnt;	}
	int size()		{	return m_nCnt;	}

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
		if(m_pHead)
		{
			GX_DELETE(m_pHead);
		}
		m_pHead=NULL;
		m_pTail=NULL;
        m_nCnt=0;
	}

	stLinkNode<T>* getHead() const			{	return m_pHead;	}
    stLinkNode<T>* getTail() const			{	return m_pTail;	}

private:
	void setHead(stLinkNode<T>* head)		{	m_pHead=head;	if(m_pHead)m_pHead->setParent(NULL);	}
	void setTail(stLinkNode<T>* tail)	{	m_pTail=tail;	}
	
	stLinkNode<T>* getNode(int pos) const
	{
		if(pos<0 || pos>=m_nCnt)		//out of bound check
			return NULL;
		
		stLinkNode<T>* node=getHead();
		for(int x=0;x<m_nCnt;x++)
		{
			if(x==pos)
				break;					//position reached
			else
				node=node->getNext();	
		}
		
		return node;
	}
	
	
	stLinkNode<T>* m_pHead;
	stLinkNode<T>* m_pTail;
	int				m_nCnt;
};
#endif