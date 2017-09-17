#pragma once

//
//  MDropData.h
//  Copyright 2017 GEAR3D
//  Gear3D
//
//  Created by Arun Mudaliar.
//
//

#include <vector>
#include "../gui/geGUIBase.h"

class geGUIBase;
class MDropData {
public:
    MDropData(std::vector<geGUIBase*>* actualGUIObjectList, geGUIBase* sourceObject, unsigned int wndID) {
        this->actualGUIObjectList = actualGUIObjectList;
        sourceGUIObject = sourceObject;
        sourceWindowID = wndID;
    }

    ~MDropData() {
        actualGUIObjectList->clear();
        GE_DELETE(actualGUIObjectList);
    }

    std::vector<geGUIBase*>* getActualDataList()    {   return actualGUIObjectList;     }
    geGUIBase* getSourcePtr()                       {   return sourceGUIObject;         }
    unsigned int getSourceWindowID()                         {   return sourceWindowID;          }
    
private:
    unsigned int sourceWindowID;
    geGUIBase* sourceGUIObject;
    std::vector<geGUIBase*>* actualGUIObjectList;
};
