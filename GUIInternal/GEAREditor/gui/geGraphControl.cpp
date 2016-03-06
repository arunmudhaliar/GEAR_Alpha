#include "geGraphControl.h"
#include <float.h>

geGraphControl::geGraphControl(geFontManager* fontmanager):
	geGUIBase(GEGUI_GRAPHCONTROL, "GraphControl", fontmanager)
{
    divisions = 1;
    animationTrack = nullptr;
    showValueIndex = 0;
    drawSelection=false;
    dragEnabled=false;
    
    horizontalScrollBar = new geScrollBar(fontmanager);
}

geGraphControl::~geGraphControl()
{
    GX_DELETE(horizontalScrollBar);
}

void geGraphControl::create(rendererGL10* renderer, geGUIBase* parent, float x, float y, float cx, float cy, int lowerLimit, int upperLimit)
{
	createBase(renderer, parent);

    horizontalScrollBar->create(renderer, this, this, geScrollBar::HORIZONTAL);

    setSizable(true);
	setSize(cx, 3);
	setPos(x, y);


    this->lowerLimit = lowerLimit;
    this->upperLimit = upperLimit;
    this->divisions = (upperLimit-lowerLimit)*0.01f;
	setMouseBoundCheck(true);
    
    normalKeyVertexBuffer.setRect(gxRectf(-2, -4, 4, 8), vector4f(0.14509f, 0.47843f, 0.227451f, 1.0f), vector4f(0.06275f, 0.25882f, 0.11373f, 1.0f));
    hoverKeyVertexBuffer.setRect(gxRectf(-2, -4, 4, 8), vector4f(0.63922f, 0.66275f, 0.2353f, 1.0f), vector4f(0.41176f, 0.42745f, 0.12549f, 1.0f));
    selectedKeyVertexBuffer.setRect(gxRectf(-2, -4, 4, 8), vector4f(0.56078f, 0.16471f, 0.07451f, 1.0f), vector4f(0.32549f, 0.09412f, 0.04314f, 1.0f));
    
    selectionRectVertexBuffer.setRect(gxRectf(vector2f(0, 0), vector2f(1, 1)), vector4f(0.7f, 0.7f, 0.7f, 0.15f));
    
    //time scale
    float graph_width = (upperLimit-lowerLimit)*divisions;
    timeScaleVertexBuffer.setRect(gxRectf(0, 0, graph_width, 14), vector4f(0.16f, 0.16f, 0.16f, 1.0f));
    timeScaleShadeVertexBuffer.setRect(gxRectf(0, 0, graph_width, 7), vector4f(0.13f, 0.13f, 0.13f, 1.0f), vector4f(0.13f, 0.13f, 0.13f, 0.0f));
    
    //red rect
    const float clientarea_linevertLst[8] =
    {
        1,	0,
        graph_width,	0,
        graph_width,	cy-0.2f,
        1,	cy-0.2f
    };
    memcpy(vertexBufferClientAreaArray, clientarea_linevertLst, sizeof(clientarea_linevertLst));
    
    horizontalScrollBar->setConetentSize(graph_width);
}

void geGraphControl::setTrack(IAnimationTrack* track)
{
    if(track==nullptr)
        return;
    
    //code to restrict the graph to total no of frames.
//    this->upperLimit = track->getTotalFrames();
//    this->divisions = lowerLimit+(upperLimit-lowerLimit)*0.2f;
//    graphMatrix.setScale(divisions, divisions, divisions);

    animationTrack=track;
    memset(verticalScale, 0, sizeof(verticalScale));
    memset(midPoint, 0, sizeof(midPoint));
    
    //min-max
    memset(minExtend, 0, sizeof(minExtend));
    memset(maxExtend, 0, sizeof(maxExtend));
    
    minExtend[0] = minExtend[1] = minExtend[2] = minExtend[3] = minExtend[4] = minExtend[5] = minExtend[6] = minExtend[7] = minExtend[8] = 1e16f;
    maxExtend[0] = maxExtend[1] = maxExtend[2] = maxExtend[3] = maxExtend[4] = maxExtend[5] = maxExtend[6] = maxExtend[7] = maxExtend[8] = -1e16f;
    
    auto matrixPtr = animationTrack->getFrames();
    for(int x=0;x<animationTrack->getTotalFrames();x++)
    {
        auto mat = &matrixPtr[x];
        vector3f value[]=
        {
            mat->getPosition(),
            mat->getRotation(),
            mat->getScale()
        };
        float* vPtr = &value[0].x;
        
        for (int y=0; y<9; y++)
        {
            if(minExtend[y]>*(vPtr+y))
                minExtend[y]=*(vPtr+y);
            if(maxExtend[y]<*(vPtr+y))
                maxExtend[y]=*(vPtr+y);
        }
    }
    
//    for(int x=0;x<animationTrack->getTotalFrames();x++)
//    {
//        auto mat = &matrixPtr[x];
//        vector3f value[]=
//        {
//            mat->getPosition(),
//            mat->getRotation(),
//            mat->getScale()
//        };
//        
//        printf("%3.2f, %3.2f, %3.2f\n", value[1].x, value[1].y, value[1].z);
//    }
    
    float topMargin=getMainWindow()->getTopMarginOffsetHeight();
    for (int x=0; x<9; x++)
    {
        float diff = maxExtend[x]-minExtend[x];
        if(std::abs(diff)<1.0f)
        {
            verticalScale[x]=1.0f;
        }
        else
        {
            verticalScale[x]=(m_cSize.y-topMargin)/diff;
        }
        midPoint[x]=(minExtend[x]+diff*0.5f);
        verticalScale[x]*=0.85f;
    }
}

void geGraphControl::draw()
{
    //applyClipIfIamOnMainWindow();
    
    glPushMatrix();
    glTranslatef(m_cPos.x+graphOffset.x, m_cPos.y, 0);
    
    glPushMatrix();
    glScalef(divisions, 1.0f, 1.0f);
    float delta = 0;
    char timeLineLabel[64];
    float animationFPS=60.0f;
    
    if(animationTrack)
    {
        animationFPS=animationTrack->getFPS();
    }
    
    for(int x = 0; x<(int)(upperLimit-lowerLimit); x++)
    {
        float line[4] = {delta, 0, delta, m_cSize.y};
        drawLine(line, 0.22f, 0.22f, 0.22f, 1, 2, false);
        delta+=lowerLimit;
    }
    glPopMatrix();
    

    //time scale
    drawRect(&timeScaleVertexBuffer);
    
    float topMargin=getMainWindow()->getTopMarginOffsetHeight();
    
    //time stamps
    for(int x = 0; x<((upperLimit-lowerLimit)-1); x++)
    {
        float plotX = x*lowerLimit*divisions;
        float line[4] = {plotX, 0, plotX, (m_cSize.y-topMargin)};
        
        if(lowerLimit*divisions>40.0f)  //40 pixels
        {
            drawLine(line, 0.27f, 0.27f, 0.27f, 1, 2, false);
            
            sprintf(timeLineLabel, "%4.3f", x/animationFPS);
            geFontManager::g_pFontArial10_80Ptr->drawString(timeLineLabel, plotX+3, geFontManager::g_pFontArial10_80Ptr->getLineHeight()-6, m_cSize.x);
        }
        else
        {
            int denominator=2;
            if(lowerLimit*divisions<=1.5f)
                denominator=128;
            else if(lowerLimit*divisions<=3.0f)
                denominator=64;
            else if(lowerLimit*divisions<=6.0f)
                denominator=32;
            else if(lowerLimit*divisions<=10.0f)
                denominator=16;
            else if(lowerLimit*divisions<=20.0f)
                denominator=8;
            else if(lowerLimit*divisions<=30.0f)
                denominator=4;
            
            
            if(x%denominator==0)
            {
                drawLine(line, 0.27f, 0.27f, 0.27f, 1, 2, false);
                
                sprintf(timeLineLabel, "%4.3f", x/animationFPS);
                geFontManager::g_pFontArial10_80Ptr->drawString(timeLineLabel, plotX+3, geFontManager::g_pFontArial10_80Ptr->getLineHeight()-6, m_cSize.x);
            }
        }
    }
    
    //draw the last frame in white color
    int last_index = ((upperLimit-lowerLimit)-1);
    float last_plotX = last_index*lowerLimit*divisions;
    float last_line[4] = {last_plotX, 0, last_plotX, (m_cSize.y-topMargin)};
    
    drawLine(last_line, 0.6f, 0.6f, 0.6f, 1, 2, false);
        
    sprintf(timeLineLabel, "%4.3f", last_index/animationFPS);
    geFontManager::g_pFontArial10_80Ptr->drawString(timeLineLabel, last_plotX-3, geFontManager::g_pFontArial10_80Ptr->getLineHeight()*3, m_cSize.x, false, false, 0, -90);
    //
    
    //middle line
    float line[4] = {-graphOffset.x, (m_cSize.y-topMargin)*0.5f, -graphOffset.x+m_cSize.x, (m_cSize.y-topMargin)*0.5f};
    drawLine(line, 0.22f, 0.22f, 0.22f, 1, 2, false);

    //pivot line
    float pivotLine[4] = {pivot.x, 0, pivot.x, (m_cSize.y-topMargin)};
    drawLine(pivotLine, 0.5f, 0.0f, 0.0f, 1, 2, false);
    
    sprintf(timeLineLabel, "%4.3f s", (pivot.x/divisions)/animationFPS);
    geFontManager::g_pFontArial10_80Ptr->drawString(timeLineLabel, pivot.x-3, (m_cSize.y-topMargin)*0.5f, m_cSize.x, false, false, 0, -90);

    
    if(animationTrack)
    {
        auto matrixPtr = animationTrack->getFrames();
        for(int x=0;x<animationTrack->getTotalFrames();x++)
        {
            auto mat = &matrixPtr[x];
            vector3f value[]=
            {
                mat->getPosition(),
                mat->getRotation(),
                mat->getScale()
            };
            float* vPtr = &value[0].x;
            vPtr+=showValueIndex;
            glPushMatrix();
            
            float plotX = x*lowerLimit*divisions;
            float plotY = ((*vPtr) - midPoint[showValueIndex])* verticalScale[showValueIndex] + (m_cSize.y-topMargin)*0.5f;
            glTranslatef(plotX, plotY, 0);
            
            //draw the animation node
            if(pivot.x>plotX-2 && pivot.x<plotX+2)    //4px is the width of the animation node
            {
                float line[4] = {0, -plotY, 0, -plotY+(m_cSize.y-topMargin)};
                drawLine(line, 0.63922f, 0.66275f, 0.2353f, 1, 2, false);

                drawRect(&hoverKeyVertexBuffer);
            }
            else
            {
                const gxRectf& rect = selectionRectVertexBuffer.getRect();
                if(plotX>rect.m_pos.x && plotX<rect.m_pos.x+rect.m_size.x &&
                   plotY>rect.m_pos.y && plotY<rect.m_pos.y+rect.m_size.y)
                {
                    drawRect(&selectedKeyVertexBuffer);
                }
                else
                {
                    drawRect(&normalKeyVertexBuffer);
                }
            }

            glPopMatrix();
        }
    }
    
    //draw selection
    if(drawSelection)
    {
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        drawRect(&selectionRectVertexBuffer);
        glDisable(GL_BLEND);
    }
    
    //timescale shade
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    drawRect(&timeScaleShadeVertexBuffer);
    glDisable(GL_BLEND);
    
    //translate back
    glTranslatef(-graphOffset.x, -graphOffset.y, 0);
    
    
    char buffer[256];
    sprintf(buffer, "divisions : %3.2f, pivot.x = %f, graphOffset.x = %f", divisions, pivot.x, graphOffset.x);
    geFontManager::g_pFontArial10_84Ptr->drawString(buffer, 80.0f, (m_cSize.y-topMargin)-geFontManager::g_pFontArial10_84Ptr->getLineHeight()*2, m_cSize.x);
    if(animationTrack)
    {
        sprintf(buffer, "fps : %d, nFrames = %d", animationTrack->getFPS(), animationTrack->getTotalFrames());
        geFontManager::g_pFontArial10_84Ptr->drawString(buffer, 80.0f, (m_cSize.y-topMargin)-geFontManager::g_pFontArial10_84Ptr->getLineHeight(), m_cSize.x);

        sprintf(buffer, "%4.2f", minExtend[showValueIndex]);
        geFontManager::g_pFontArial10_84Ptr->drawString(buffer, 2.0f, geFontManager::g_pFontArial10_84Ptr->getLineHeight()*1.5f, m_cSize.x);

        sprintf(buffer, "%4.2f", midPoint[showValueIndex]);
        geFontManager::g_pFontArial10_84Ptr->drawString(buffer, 2.0f, (m_cSize.y-topMargin)*0.5f-3.0f, m_cSize.x);

        sprintf(buffer, "%4.2f", maxExtend[showValueIndex]);
        geFontManager::g_pFontArial10_84Ptr->drawString(buffer, 2.0f, (m_cSize.y-topMargin), m_cSize.x);
    }
    
    //drawLine(vertexBufferClientAreaArray, 0.17f, 0, 0, 1.0f, 4, true);

    glPopMatrix();
    
    horizontalScrollBar->draw();
}

void geGraphControl::onSize(float cx, float cy, int flag)
{
    horizontalScrollBar->setPos(0, cy-SCROLLBAR_SIZE);
    horizontalScrollBar->setSize(cx, SCROLLBAR_SIZE);
    float graph_width = (upperLimit-lowerLimit)*divisions;
    horizontalScrollBar->setConetentSize(graph_width);
    
    float topMargin=0.0f;
    if(getMainWindow())
        topMargin = getMainWindow()->getTopMarginOffsetHeight();
    
    //time scale
    if(timeScaleVertexBuffer.getRect().m_size.x<cx)
    {
        timeScaleVertexBuffer.updateRect(0, 0, cx, 14);
        timeScaleShadeVertexBuffer.updateRect(0, 0, cx, 7); //half the height of timescale
    }
    
    if(animationTrack)
    {
        for (int x=0; x<9; x++)
        {
            float diff = maxExtend[x]-minExtend[x];
            if(std::abs(diff)>0.000001f)
            {
                verticalScale[x]=(m_cSize.y-topMargin)/diff;
            }
            else
            {
                verticalScale[x]=1.0f;
            }
            midPoint[x]=(minExtend[x]+diff*0.5f);
            verticalScale[x]*=0.9f;
        }
    }
}

void geGraphControl::onResizeComplete()
{
    horizontalScrollBar->setPos(0, m_cSize.y-SCROLLBAR_SIZE);
    horizontalScrollBar->setSize(m_cSize.x, SCROLLBAR_SIZE);
    
    float graph_width = (upperLimit-lowerLimit)*divisions;
    horizontalScrollBar->setConetentSize(graph_width);
    
    geGUIBase::onResizeComplete();
}

void geGraphControl::onCancelEngagedControls()
{
    horizontalScrollBar->CancelEngagedControls();
    geGUIBase::onCancelEngagedControls();
}

void geGraphControl::onScrollBarChange(geScrollBar* scrollbar)
{
    float scrollBar_ratio = scrollbar->getScrollGrabberPos()/(scrollbar->getSize().x-scrollbar->getScrollGrabberSize());
    float displacement = scrollBar_ratio * (1.0f - scrollbar->getActualRatio()) * scrollbar->getContentSize();
    graphOffset.x = -displacement;
}

bool geGraphControl::onMouseLButtonDown(float x, float y, int nFlag)
{
    if(horizontalScrollBar->MouseLButtonDown(x, y, nFlag))
        return false;

    mousePrevPos=mousePos;
    mousePos.set(x, y);
    
    if(y>timeScaleVertexBuffer.getRect().m_size.y)
    {
        selectionRectVertexBuffer.reset();
        drawSelection = true;
        previousClickPivot = vector2f(x, y);
    }
    
    return true;
}

bool geGraphControl::onMouseLButtonUp(float x, float y, int nFlag)
{
    horizontalScrollBar->MouseLButtonUp(x, y, nFlag);

    mousePrevPos=mousePos;
    mousePos.set(x, y);

    drawSelection = false;
    return true;
}

bool geGraphControl::onMouseRButtonDown(float x, float y, int nFlag)
{
    mousePrevPos=mousePos;
    mousePos.set(x, y);

    return true;
}

void geGraphControl::onMouseRButtonUp(float x, float y, int nFlag)
{
    mousePrevPos=mousePos;
    mousePos.set(x, y);
}

bool geGraphControl::onMouseMButtonDown(float x, float y, int nFlag)
{
    mousePrevPos=mousePos;
    mousePos.set(x, y);
    
    if(y>timeScaleVertexBuffer.getRect().m_size.y)
    {
        dragEnabled = true;
        previousClickPivot = vector2f(x, y);
    }
    
    return true;
}

void geGraphControl::onMouseMButtonUp(float x, float y, int nFlag)
{
    dragEnabled=false;
    
    mousePrevPos=mousePos;
    mousePos.set(x, y);
}

bool geGraphControl::onMouseMove(float x, float y, int flag)
{
    horizontalScrollBar->MouseMove(x, y, flag);

    mousePrevPos=mousePos;
    mousePos.set(x, y);

    if(y<=timeScaleVertexBuffer.getRect().m_size.y && (flag&MK_LBUTTON))
    {
        previousPivot = pivot;
        pivot = vector2f(x-graphOffset.x, y);
    }
    
    if(y>timeScaleVertexBuffer.getRect().m_size.y)
    {
        if(drawSelection)
        {
            selectionRectVertexBuffer.updateRect(MIN(-graphOffset.x+previousClickPivot.x, -graphOffset.x+x), MIN(previousClickPivot.y, y), std::abs(x-previousClickPivot.x), std::abs(y-previousClickPivot.y));
        }
        else if((flag&MK_MBUTTON) && dragEnabled)
        {
            graphOffset.x+=mousePos.x-mousePrevPos.x;
            
            if(graphOffset.x>0)
                graphOffset.x=0;
            else if (graphOffset.x<-(upperLimit-lowerLimit)*divisions+m_cSize.x)
                graphOffset.x=-(upperLimit-lowerLimit)*divisions+m_cSize.x;
            
            //TODO: a minor bug while grab and scroll. FIX IT
            horizontalScrollBar->setScrollGrabberPos(-(graphOffset.x)/divisions, false);
        }
    }
    
    return true;
}

void geGraphControl::onMouseWheel(int zDelta, int x, int y, int flag)
{
    //horizontalScrollBar->scrollMouseWheel(zDelta, x, y, flag);

    mousePrevPos=mousePos;
    mousePos.set(x, y);

    float old_division = divisions;
    float diff = (upperLimit-lowerLimit);
    float delta = powf(divisions, 0.25f);
    if(zDelta<0)
    {
        float updated_division = divisions-delta;
        
        if(diff*updated_division<m_cSize.x)
        {
            divisions=m_cSize.x/diff;
        }
        else
        {
            divisions=updated_division;
        }
    }
    else
    {
        float updated_division = divisions+delta;

        divisions=updated_division;
        if(divisions>upperLimit)
            divisions=m_cSize.x/diff;
    }

    float pivot_index_position = pivot.x/old_division;
    pivot.x = pivot_index_position*divisions;

    float graph_width = (upperLimit-lowerLimit)*divisions;
    timeScaleVertexBuffer.updateRect(0, 0, graph_width, 14);
    timeScaleShadeVertexBuffer.updateRect(0, 0, graph_width, 7);
    
    horizontalScrollBar->setConetentSize(graph_width);
}

void geGraphControl::showPositionValues(int coordinate)
{
    if(coordinate<0 || coordinate>2)
        return;
    showValueIndex = 3*0+coordinate;
}

void geGraphControl::showRotationValues(int coordinate)
{
    if(coordinate<0 || coordinate>2)
        return;
    showValueIndex = 3*1+coordinate;
}

void geGraphControl::showScaleValues(int coordinate)
{
    if(coordinate<0 || coordinate>2)
        return;
    showValueIndex = 3*2+coordinate;
}

