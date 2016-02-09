#pragma once

//#if !defined(__APPLE__) //disable Drag-Drop
//#include <Ole2.h>
#include "../gui/geGUIBase.h"

class geGUIBase;
class MDropData// : public IDataObject
{
public:

	MDropData(std::vector<geGUIBase*>* actualGUIObjectList, geGUIBase* sourceObject)
	{
		this->actualGUIObjectList = actualGUIObjectList;
		sourceGUIObject = sourceObject;
	}

	~MDropData()
	{
		actualGUIObjectList->clear();
		GE_DELETE(actualGUIObjectList);
	}

	std::vector<geGUIBase*>* getActualDataList()	{	return actualGUIObjectList;     }
	geGUIBase* getSourcePtr()						{	return sourceGUIObject;			}

private:
	geGUIBase* sourceGUIObject;
	std::vector<geGUIBase*>* actualGUIObjectList;
};