#include "geTextBox.h"
#include "geGUIManager.h"

KeyMapper& KeyMapper::getInstance()
{
    static KeyMapper instance;
    return instance;
}

KeyMapper::KeyMapper()
{
    init();
}

void KeyMapper::init()
{
    int asciiCode=65;   //A-Z loop
    for (int scancode=SDL_SCANCODE_A; scancode<=SDL_SCANCODE_Z; scancode++, asciiCode++)
    {
        asciiCodeMap[(SDL_Scancode)scancode]=asciiCode;
    }
    
    //space
    asciiCodeMap[SDL_SCANCODE_SPACE]=32;
    //0
    asciiCodeMap[SDL_SCANCODE_0]=48;
    //-
    asciiCodeMap[SDL_SCANCODE_MINUS]=45;
    //=
    asciiCodeMap[SDL_SCANCODE_EQUALS]=61;

    asciiCodeMap[SDL_SCANCODE_LEFTBRACKET]=91;
    asciiCodeMap[SDL_SCANCODE_RIGHTBRACKET]=93;
    
    asciiCodeMap[SDL_SCANCODE_SEMICOLON] = 59;
    asciiCodeMap[SDL_SCANCODE_APOSTROPHE] = 39;
    
    asciiCodeMap[SDL_SCANCODE_COMMA] = 44;
    asciiCodeMap[SDL_SCANCODE_PERIOD] = 46;
    asciiCodeMap[SDL_SCANCODE_SLASH] = 47;
    asciiCodeMap[SDL_SCANCODE_BACKSLASH] = 92;

    asciiCodeMap[SDL_SCANCODE_KP_DIVIDE] = 47;
    asciiCodeMap[SDL_SCANCODE_KP_MULTIPLY] = 42;
    asciiCodeMap[SDL_SCANCODE_KP_MINUS] = 45;
    asciiCodeMap[SDL_SCANCODE_KP_PLUS] = 43;

    asciiCodeMap[SDL_SCANCODE_BACKSPACE] = 8;
    
    asciiCode=49;   //1-9 loop
    for (int scancode=SDL_SCANCODE_1; scancode<=SDL_SCANCODE_9; scancode++, asciiCode++)
    {
        asciiCodeMap[(SDL_Scancode)scancode]=asciiCode;
    }
    
}

int KeyMapper::getAsciiCode(SDL_Scancode scancode)
{
    return asciiCodeMap[scancode];
}

geTextBox* geTextBox::g_pCurrentSelectedTextBoxPtr=NULL;
geTextBox* geTextBox::g_pCurrentlyActiveTextBoxPtr=NULL;

geTextBox::geTextBox(geFontManager* fontmanager):
	geGUIBase(GEGUI_TEXTBOX, "Text Box", fontmanager)
{
    resetVars();
}

geTextBox::geTextBox(const char* name, geFontManager* fontmanager):
	geGUIBase(GEGUI_TEXTBOX, name, fontmanager)
{
    acceptOnlyNumbers=false;
    resetVars();
}

geTextBox::~geTextBox()
{
	if(geTextBox::g_pCurrentSelectedTextBoxPtr==this)
		geTextBox::g_pCurrentSelectedTextBoxPtr=NULL;
}

void geTextBox::resetVars()
{
    isStartSelection=false;
    isShowSelection=false;
    previousMouseXPosition=0.0f;
    selectionStartXPosition=0.0f;
    isShowCursor=false;
    cursorPositionInPixel=0.0f;
    isControlSelected=false;
    cursorIndexPosition=0;
    startCursorSelectionIndexPosition=0;
    endCursorSelectionIndexPosition=0;
    startStringCharToDisplay=0;
    isClearAll=false;
    memset(internalTextBuffer, 0, sizeof(internalTextBuffer));
}

void geTextBox::create(rendererGL10* renderer, geGUIBase* parent, const char* name, float x, float y, float cx, float cy)
{
	createBase(renderer, parent, x, y, cx, cy);

	//int width=geFontManager::g_pFontArial10_80Ptr->calculateStringWidthInPixelTillNewLine(name, strlen(name), 0);
    setName(name);

	setClientAreaPrimaryActiveForeColor(0.21f, 0.21f, 0.21f, 1.0f);
	applyPrimaryColorToVBClientArea();
    setColor(&vertexBufferSelectionArea, 0.24f, 0.38f, 0.57f, 1.0f);
    startChar=internalTextBuffer;
}

void geTextBox::draw()
{
	if(isStartSelection && isControlSelected && geTextBox::g_pCurrentlyActiveTextBoxPtr==this)
	{
		float diff=previousMouseXPosition-m_cPos.x-selectionStartXPosition;
		float leftX=selectionStartXPosition;
		float rightX=selectionStartXPosition+diff;

		if(selectionStartXPosition+diff<selectionStartXPosition)
		{
			rightX=selectionStartXPosition;
			leftX=selectionStartXPosition+diff;
		}

//		if(leftX<10 )
//		{
//			if(startStringCharToDisplay)
//			{
//				startStringCharToDisplay--;
//				if(startStringCharToDisplay<=0)
//					startStringCharToDisplay=0;
//				startChar = &m_szName[startStringCharToDisplay];
//			}
//		}
//
//		float full_width=geFontManager::g_pFontArial10_80Ptr->calculateStringWidthInPixelTillNewLine(&*startChar, (int)strlen(&*startChar), 0);
//		if(rightX>m_cSize.x-10 )
//		{
//			if(full_width>m_cSize.x)
//			{
//				startStringCharToDisplay++;
//				startChar = &m_szName[startStringCharToDisplay];
//			}
//		}
	}

	glPushMatrix();
	glTranslatef(m_cPos.x, m_cPos.y, 0);
	drawRect(&vertexBufferClientArea);
	if(isStartSelection || isShowSelection)
		drawRect(&vertexBufferSelectionArea);
	if(isShowCursor)
	{
		glPushMatrix();
		glTranslatef(cursorPositionInPixel, 0, 0);
		drawLine(vertexBufferCursorLine, 0.8f, 0.8f, 0.8f, 1.0f, 2, false);
		glPopMatrix();
	}
	drawLine(vertexBufferClientAreaArray, 0.13f, 0.13f, 0.13f, 1.0f, 3, false);
	drawLine(&vertexBufferClientAreaArray[4], 0.3f, 0.3f, 0.3f, 1.0f, 3, false);
	geFontManager::g_pFontArial10_80Ptr->drawString(startChar, 0, geFontManager::g_pFontArial10_80Ptr->getLineHeight()-5, m_cSize.x);
	glPopMatrix();
}
	
void geTextBox::onPosition(float x, float y, int flag)
{
}

void geTextBox::onSize(float cx, float cy, int flag)
{
    vertexBufferClientArea.updateRect(0, 0, cx, cy);
	const float clientarea_linevertLst[10] =
	{
		cx,	0,
		0,	0,
		0,	cy-0.5f,
		cx,	cy-0.5f,
		cx,	0,
	};
	memcpy(vertexBufferClientAreaArray, clientarea_linevertLst, sizeof(clientarea_linevertLst));

	vertexBufferCursorLine[0]=0.0f;	vertexBufferCursorLine[1]=0.0f;
	vertexBufferCursorLine[2]=0.0f;	vertexBufferCursorLine[3]=cy;
}

void geTextBox::clearSelection()
{
	isShowSelection=false;
	isStartSelection=false;
	isShowCursor=false;
	startCursorSelectionIndexPosition=0;
	endCursorSelectionIndexPosition=0;
	setCursorPos(cursorIndexPosition);
}

void geTextBox::setCursorPos(int index)
{
	if(index<0)
        return;
    
	cursorIndexPosition=index;
	cursorPositionInPixel=0;
	for(int x=0;x<cursorIndexPosition;x++)
		cursorPositionInPixel+=geFontManager::g_pFontArial10_80Ptr->getCharWidth(startChar[x]);
    
    printf("cursorIndexPosition %d %s %s\n",cursorIndexPosition, &*startChar, internalTextBuffer);
}

float geTextBox::getVirtualEndBound(int& index)
{
	index=0;
	float cursorPosInPx=0;
	for(index=0;index<strlen(startChar);index++)
	{
		float temp=cursorPosInPx;
		cursorPosInPx+=geFontManager::g_pFontArial10_80Ptr->getCharWidth(startChar[index]);
		if(cursorPosInPx>m_cSize.x)
		{
			cursorPosInPx=temp;
			break;
		}
	}

	return cursorPosInPx;
}

bool geTextBox::onMouseLButtonDown(float x, float y, int nFlag)
{
	if(!isPointInsideClientArea(x, y))
		return false;
//	isStartSelection=true;
	geTextBox::g_pCurrentlyActiveTextBoxPtr=this;

	if(geTextBox::g_pCurrentSelectedTextBoxPtr && geTextBox::g_pCurrentSelectedTextBoxPtr!=this)
		geTextBox::g_pCurrentSelectedTextBoxPtr->clearSelection();
	geTextBox::g_pCurrentSelectedTextBoxPtr=this;

	previousMouseXPosition=x;
	selectionStartXPosition=x;
	float full_width=getVirtualEndBound(cursorIndexPosition);
	if(selectionStartXPosition>=full_width)
	{
		selectionStartXPosition=full_width;
		setCursorPos(cursorIndexPosition);
		startCursorSelectionIndexPosition=0;
		endCursorSelectionIndexPosition=cursorIndexPosition;
	}
	isControlSelected=true;
//	isStartSelection=false;

	return true;
}

bool geTextBox::onMouseLButtonUp(float x, float y, int nFlag)
{
	if(!isPointInsideClientArea(x, y))
		return false;

	if(!isStartSelection)
	{
		//insert cursor
		int len=(int)strlen(startChar);
		float cursorPosInPx=0;
		bool bflag=false;
		for(int m=0;m<len;m++)
		{
			float startX=cursorPosInPx;
			cursorPosInPx+=geFontManager::g_pFontArial10_80Ptr->getCharWidth(startChar[m]);
			float endX=cursorPosInPx;
			if(x>=startX && x<=endX)
			{
				setCursorPos(m);
				bflag=true;
				break;
			}
		}
        
		if(!bflag)
		{
			startCursorSelectionIndexPosition=0;
			endCursorSelectionIndexPosition=len;
			setCursorPos(len);
		}
		isShowCursor=true;
		if(isShowSelection)
		{
            isClearAll=false;
			isControlSelected=false;
			startCursorSelectionIndexPosition=0;
			endCursorSelectionIndexPosition=0;
			isShowSelection=false;
			return true;
		}
        
        float full_width=getVirtualEndBound(cursorIndexPosition);
        vertexBufferSelectionArea.updateRect(0, 0, full_width, m_cSize.y);
        setCursorPos(cursorIndexPosition);
        startCursorSelectionIndexPosition=0;
        endCursorSelectionIndexPosition=cursorIndexPosition;
        isShowSelection=true;
        if(strlen(startChar)==cursorIndexPosition)
        {
            isClearAll=true;
        }
	}


//    isStartSelection=false;
    isControlSelected=false;
    previousMouseXPosition=x;

    return true;
}

bool geTextBox::onMouseMove(float x, float y, int flag)
{
	if(!isPointInsideClientArea(x, y))
		return false;

//	if(flag&MK_LBUTTON)
//		isStartSelection=true;
//
//	if(isStartSelection && isControlSelected && geTextBox::g_pCurrentlyActiveTextBoxPtr==this && (flag&MK_LBUTTON))
//	{
//		float diff=x-m_cPos.x-selectionStartXPosition;
//		float leftX=selectionStartXPosition;
//		float rightX=selectionStartXPosition+diff;
//
//		if(selectionStartXPosition+diff<selectionStartXPosition)
//		{
//			rightX=selectionStartXPosition;
//			leftX=selectionStartXPosition+diff;
//		}
//
//		if(leftX<10 /*&& diff<0*/)
//		{
//			if(startStringCharToDisplay)
//			{
//				startStringCharToDisplay--;
//				if(startStringCharToDisplay<=0)
//					startStringCharToDisplay=0;
//				startChar = &m_szName[startStringCharToDisplay];
//			}
//		}
//
//		float full_width=geFontManager::g_pFontArial10_80Ptr->calculateStringWidthInPixelTillNewLine(startChar, (int)strlen(startChar), 0);
//		if(rightX>m_cSize.x-10 /*&& diff>0*/)
//		{
//			if(full_width>m_cSize.x)
//			{
//				startStringCharToDisplay++;
//				startChar = &m_szName[startStringCharToDisplay];
//			}
//		}
//
//		//
//		int len=0;//strlen(startChar);
//		float cursorPos=getVirtualEndBound(len);
//		full_width	=cursorPos;//reassign the virtual full width
//		for(int m=len-1;m>=0;m--)
//		{
//			//float endX=cursorPos;
//			cursorPos-=geFontManager::g_pFontArial10_80Ptr->getCharWidth(startChar[m]);
//			if(leftX>cursorPos)
//			{
//				startCursorSelectionPosition=m;
//				leftX=cursorPos;
//				break;
//			}
//		}
//
//		cursorPos=0;
//		for(int m=0;m<len;m++)
//		{
//			//float startX=cursorPos;
//			cursorPos+=geFontManager::g_pFontArial10_80Ptr->getCharWidth(startChar[m]);
//			if(rightX<cursorPos)
//			{
//				endCursorSelectionPosition=m+1;
//				rightX=cursorPos;
//				break;
//			}
//		}
//
//		if(selectionStartXPosition+diff<selectionStartXPosition)
//		{
//			setCursorPos(startCursorSelectionPosition);
//		}
//		else
//		{
//			setCursorPos(endCursorSelectionPosition);
//		}
//
//		if(rightX>full_width && diff>0)
//		{
//			rightX=full_width;
//			//if(rightX>m_cSize.x)
//			//	rightX=m_cSize.x;
//			setCursorPos((int)strlen(startChar));
//		}
//
////		const float selection_vertLst[8] =
////		{
////			rightX,		0,
////			leftX,		0,
////			rightX,		m_cSize.y,
////			leftX,		m_cSize.y,
////		};
////		memcpy(vertexBufferSelectionArea.vertexArray, selection_vertLst, sizeof(selection_vertLst));
//        vertexBufferSelectionArea.updateRect(leftX, 0, rightX-leftX, m_cSize.y);
//	}
//	previousMouseXPosition=x;
//
//	return true;
    
    return true;
}

void geTextBox::onCancelEngagedControls()
{

}

bool replaceSelection(char* str, int start, int end);

int geTextBox::keyMapping(int32_t scancode, uint16_t mod)
{
    bool isCAPS = (mod&KMOD_CAPS);
    bool isSHIFT = (mod&KMOD_LSHIFT) || (mod&KMOD_RSHIFT);
    bool isCTRL = (mod&KMOD_LCTRL) || (mod&KMOD_RCTRL);
    
    if(scancode==SDL_SCANCODE_BACKSPACE)
        return KeyMapper::getInstance().getAsciiCode((SDL_Scancode)scancode);
    else if(scancode==SDL_SCANCODE_LSHIFT || scancode==SDL_SCANCODE_RSHIFT)
        return 0;
    else if(scancode==SDL_SCANCODE_CAPSLOCK)
        return 0;
    
    if(acceptOnlyNumbers)
    {
        if((scancode>=SDL_SCANCODE_1 && scancode<=SDL_SCANCODE_0 && !isSHIFT))
            return KeyMapper::getInstance().getAsciiCode((SDL_Scancode)scancode);
        if(scancode==SDL_SCANCODE_MINUS && isSHIFT)
        {
            long diff = startChar-internalTextBuffer;
            if(diff==0 && cursorIndexPosition==0 && !(geUtil::isSubString(internalTextBuffer, "-") || geUtil::isSubString(internalTextBuffer, "+")))
                return KeyMapper::getInstance().getAsciiCode((SDL_Scancode)scancode);
            else
                return 0;
        }
        if(scancode==SDL_SCANCODE_EQUALS && isSHIFT)
        {
            long diff = startChar-internalTextBuffer;
            if(diff==0 && cursorIndexPosition==0 && !(geUtil::isSubString(internalTextBuffer, "-") || geUtil::isSubString(internalTextBuffer, "+")))
                return 43;
            else
                return 0;
        }
        if(scancode==SDL_SCANCODE_PERIOD && !isSHIFT)
        {
            if(!(geUtil::isSubString(internalTextBuffer, ".")))
                return KeyMapper::getInstance().getAsciiCode((SDL_Scancode)scancode);
            else
                return 0;
        }
        return 0;
    }
    
    int ascii_code = 0;
    //for ascii chars
    if(scancode>=SDL_SCANCODE_A && scancode<=SDL_SCANCODE_Z)
    {
        ascii_code = KeyMapper::getInstance().getAsciiCode((SDL_Scancode)scancode);

        if((!isCAPS && !isSHIFT) || (isCAPS && isSHIFT))
            ascii_code+=32;
        
        return ascii_code;
    }
    
    //for shift key
    if(isSHIFT)
    {
        int oneTozero[] ={ 33, 64, 35, 36, 37, 94, 38, 42, 40, 41};
        if(scancode>=SDL_SCANCODE_1 && scancode<=SDL_SCANCODE_0)
            return oneTozero[scancode-SDL_SCANCODE_1];
        else if(scancode==SDL_SCANCODE_LEFTBRACKET)
            return 123;
        else if(scancode==SDL_SCANCODE_RIGHTBRACKET)
            return 125;
        else if(scancode==SDL_SCANCODE_SLASH)
            return 63;
        else if(scancode==SDL_SCANCODE_BACKSLASH)
            return 124;
        else if(scancode==SDL_SCANCODE_APOSTROPHE)
            return 34;
        else if(scancode==SDL_SCANCODE_SEMICOLON)
            return 58;
        else if(scancode==SDL_SCANCODE_COMMA)
            return 60;
        else if(scancode==SDL_SCANCODE_PERIOD)
            return 62;
        else if(scancode==SDL_SCANCODE_MINUS)
            return 95;
        else if(scancode==SDL_SCANCODE_EQUALS)
            return 43;
    }
    
    return KeyMapper::getInstance().getAsciiCode((SDL_Scancode)scancode);
}

void geTextBox::doRealign()
{
    if(cursorIndexPosition<=0)
    {
        cursorIndexPosition=0;
        //check if there are any chars in the left side
        long diff = startChar-internalTextBuffer;
        if(diff>0)
        {
            int tempIndex=cursorIndexPosition;
            float remaining_width = getVirtualEndBound(tempIndex);
            long iterator = diff;
            float cursorPosInPx = remaining_width;
            for(long m=diff-1;m>=0;m--, iterator--)
            {
                float temp=cursorPosInPx;
                cursorPosInPx+=geFontManager::g_pFontArial10_80Ptr->getCharWidth(internalTextBuffer[m]);
                if(cursorPosInPx>m_cSize.x)
                {
                    iterator--;
                    cursorPosInPx=temp;
                    break;
                }
            }
            
            if(iterator<0)
            {
                printf("ERROR\n");
            }
            startStringCharToDisplay=(int)iterator;
            startChar=internalTextBuffer+iterator;
            setCursorPos((int)strlen(startChar)-tempIndex);
        }
    }
    else if(cursorIndexPosition>=strlen(startChar))
    {
        
    }
    
    setCursorPos(cursorIndexPosition);
}

bool geTextBox::onKeyDown(int charValue, int flag)
{
	if(geTextBox::g_pCurrentlyActiveTextBoxPtr!=this)
		return false;

//	//replace the selection block
//	if(endCursorSelectionPosition>=strlen(startChar))
//	{
//		endCursorSelectionPosition=endCursorSelectionPosition;
//	}
    int ascii_val=0;
    if(!(charValue==SDL_SCANCODE_LEFT || charValue==SDL_SCANCODE_RIGHT))
    {
        ascii_val = keyMapping(charValue, flag);
        if(ascii_val==0)
            return false;
    }
    
    bool bReplaced=false;
    if(isClearAll)
    {
        startChar=internalTextBuffer;
        startStringCharToDisplay=0;
        startCursorSelectionIndexPosition=0;
        endCursorSelectionIndexPosition=0;
        bReplaced = replaceSelection(internalTextBuffer, 0, (int)strlen(internalTextBuffer));
        isClearAll=false;
        printf("CLEAR ALL\n");
    }
    else
    {
        bReplaced = replaceSelection(startChar, startCursorSelectionIndexPosition, endCursorSelectionIndexPosition);
    }
    
	if(bReplaced)
	{
        printf("REPLACED\n");
		cursorIndexPosition=startCursorSelectionIndexPosition;
		startCursorSelectionIndexPosition=0;
		endCursorSelectionIndexPosition=0;
		isStartSelection=false;
		isShowSelection=false;
	}

	if(ascii_val==8)    //backspace
	{
		if(!bReplaced && cursorIndexPosition)
			bReplaced=replaceSelection(startChar, cursorIndexPosition-1, cursorIndexPosition);
		if(bReplaced)
			cursorIndexPosition--;
        doRealign();
	}
    else if(charValue==SDL_SCANCODE_LEFT || charValue==SDL_SCANCODE_RIGHT)
    {
        if(charValue==SDL_SCANCODE_LEFT)
            cursorIndexPosition--;
        else
            cursorIndexPosition++;
        doRealign();
    }
	else
	{
        if(cursorIndexPosition<strlen(startChar))
        {
            for(int x=(int)strlen(startChar);x>=cursorIndexPosition;x--)
            {
                startChar[x+1]=startChar[x];
            }
        }
		startChar[cursorIndexPosition]=ascii_val;
		float full_width=geFontManager::g_pFontArial10_80Ptr->calculateStringWidthInPixelTillNewLine(startChar, (int)strlen(startChar), 0);
		if(full_width>m_cSize.x)
		{
			int temp=0;
			getVirtualEndBound(temp);
			if(cursorIndexPosition==temp)
			{
				startStringCharToDisplay++;
				startChar=&internalTextBuffer[startStringCharToDisplay];
				setCursorPos(cursorIndexPosition);
			}
			else
            {
                startStringCharToDisplay++;
                startChar=&internalTextBuffer[startStringCharToDisplay];
                setCursorPos(cursorIndexPosition);
            }
		}
		else
			setCursorPos(cursorIndexPosition+1);
	}

    m_szName.assign(internalTextBuffer, strlen(internalTextBuffer));
    if(guiObserver)
        guiObserver->onTextChange(this);

	return false;	//handled by special case
}

bool replaceSelection(char* str, int start, int end)
{
	if(end>start && start>=0)
	{
		int remaining_len=(int)strlen(&str[end]);
		if(remaining_len==0)
		{
			//strcpy(&str[start], "");
            for (int x=start; x<end; x++)
            {
                str[x]='\0';
            }
		}
		else
		{
			for(int x=start;x<end;x++)
			{
				str[x]='\0';
			}

			for(int x=0;x<remaining_len;x++)
			{
				str[start+x]=str[end+x];
				str[end+x]='\0';
			}
		}
		return true;
	}

	return false;
}

bool geTextBox::onKeyUp(int charValue, int flag)
{
	if(geTextBox::g_pCurrentlyActiveTextBoxPtr!=this)
		return false;

	return false;	//handled by special case
}

void geTextBox::onSetName()
{
    resetVars();
    
    if(acceptOnlyNumbers)
    {
        auto val = geUtil::getFloat(m_szName.c_str());
        char buffer[256];
        sprintf(buffer, "%f", val);
        m_szName.assign(buffer, strlen(buffer));
        strcpy(internalTextBuffer, m_szName.c_str());
    }
    else
        strcpy(internalTextBuffer, m_szName.c_str());
    
	if(guiObserver)
		guiObserver->onTextChange(this);
}