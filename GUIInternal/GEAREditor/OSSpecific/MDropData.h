#ifndef MDROPDATA_H
#define MDROPDATA_H

//#if !defined(__APPLE__) //disable Drag-Drop
//#include <Ole2.h>
#include "../gui/geGUIBase.h"

class geGUIBase;
class MDropData// : public IDataObject
{
public:

	MDropData(std::vector<geGUIBase*>* actualDataObjectList, geGUIBase* sourceObject)
	{
		m_vpActualDataObjectPtrList=actualDataObjectList;
		m_pSourceObjectPtr=sourceObject;
	}

	~MDropData()
	{
		m_vpActualDataObjectPtrList->clear();
		GE_DELETE(m_vpActualDataObjectPtrList);
	}

	std::vector<geGUIBase*>* getActualDataList()	{	return m_vpActualDataObjectPtrList;	}
	geGUIBase* getSourcePtr()						{	return m_pSourceObjectPtr;			}

private:
	geGUIBase* m_pSourceObjectPtr;
	std::vector<geGUIBase*>* m_vpActualDataObjectPtrList;
};

#endif