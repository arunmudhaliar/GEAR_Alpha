#pragma once
//
//  geToolBarSeperator.h
//  Copyright 2017 GEAR3D
//  Gear3D
//
//  Created by Arun Mudaliar.
//
//

#include "geToolBar.h"

class geToolBarSeperator : public geGUIBase {
public:
    geToolBarSeperator(rendererGL10* renderer, geGUIBase* parent, float width, geFontManager* fontmanager):
        geGUIBase(GEGUI_TOOLBAR_SEPERATOR, "ToolBar Seperator", fontmanager)
    {
        createBase(renderer, parent, m_cPos.x, m_cPos.y, width, parent->getSize().y);

        setColor(&vertexBufferClientArea, 0.2, 0.2, 0.2, 1.0f, EGRADIENT_VERTICAL_UP, 0.45f);
        isImageLoaded = false;
    }

    virtual ~geToolBarSeperator()
    {
    }

    virtual void draw()
    {
        glPushMatrix();
        glTranslatef(m_cPos.x, m_cPos.y, 0);
        drawRect(&vertexBufferClientArea);
        drawLine(vertexBufferClientAreaArray, 0.1, 0.1, 0.1, 1.0f, 2, false);
        glPopMatrix();
    }
    
protected:
    virtual void onSize(float cx, float cy, int flag)
    {
    //  const float title_vertLst[8] = {
    //      cx-1, 0,
    //      -1, 0,
    //      cx-1, cy,
    //      -1, cy,
    //  };
    //
    //  memcpy(vertexBufferClientArea.vertexArray, title_vertLst, sizeof(title_vertLst));
        vertexBufferClientArea.updateRect(-1, 0, cx, cy);
        const float clientarea_linevertLst[4] = {
            cx-1, 0,
            cx-1, cy,
        };
        memcpy(vertexBufferClientAreaArray, clientarea_linevertLst, sizeof(clientarea_linevertLst));
    }

private:
    float vertexBufferClientAreaArray[4];
    Sprite2Dx sprite;
    bool isImageLoaded;
};
