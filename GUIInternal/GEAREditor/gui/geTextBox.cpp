#include "geTextBox.h"
#include "geGUIManager.h"

geTextBox* geTextBox::g_pCurrentSelectedTextBoxPtr=NULL;
geTextBox* geTextBox::g_pCurrentlyActiveTextBoxPtr=NULL;

geTextBox::geTextBox(geFontManager* fontmanager):
	geGUIBase(GEGUI_TEXTBOX, "Text Box", fontmanager)
{
	isStartSelection=false;
	isShowSelection=false;
	previousMouseXPosition=0.0f;
	selectionStartXPosition=0.0f;
	isShowCursor=false;
	cursorPositionInPixel=0.0f;
	isControlSelected=false;
	cursorPosition=0;
	startCursorSelectionPosition=0;
	endCursorSelectionPosition=0;
	startStringCharToDisplay=0;
	startChar=NULL;
}

geTextBox::geTextBox(const char* name, geFontManager* fontmanager):
	geGUIBase(GEGUI_TEXTBOX, name, fontmanager)
{
	isStartSelection=false;
	isShowSelection=false;
	previousMouseXPosition=0.0f;
	selectionStartXPosition=0.0f;
	isShowCursor=false;
	cursorPositionInPixel=0.0f;
	isControlSelected=false;
	cursorPosition=0;
	startCursorSelectionPosition=0;
	endCursorSelectionPosition=0;
	startStringCharToDisplay=0;
	startChar=NULL;
}

geTextBox::~geTextBox()
{
	if(geTextBox::g_pCurrentSelectedTextBoxPtr==this)
		geTextBox::g_pCurrentSelectedTextBoxPtr=NULL;
}

void geTextBox::create(rendererGL10* renderer, geGUIBase* parent, const char* name, float x, float y, float cx, float cy)
{
	createBase(renderer, parent);

	//int width=geFontManager::g_pFontArial10_80Ptr->calculateStringWidthInPixelTillNewLine(name, strlen(name), 0);
	setSize(cx, cy);
	setPos(x, y);

	STRCPY(m_szName, name);
	setClientAreaPrimaryActiveForeColor(0.21f, 0.21f, 0.21f, 1.0f);
	applyPrimaryColorToVBClientArea();
	setColor(&vertexBufferSelectionArea, 0.24f, 0.38f, 0.57f, 1.0f);
	startChar=m_szName;
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

		if(leftX<10 /*&& diff<0*/)
		{
			if(startStringCharToDisplay)
			{
				startStringCharToDisplay--;
				if(startStringCharToDisplay<=0)
					startStringCharToDisplay=0;
				startChar = &m_szName[startStringCharToDisplay];
			}
		}

		float full_width=geFontManager::g_pFontArial10_80Ptr->calculateStringWidthInPixelTillNewLine(startChar, (int)strlen(startChar), 0);
		if(rightX>m_cSize.x-10 /*&& diff>0*/)
		{
			if(full_width>m_cSize.x)
			{
				startStringCharToDisplay++;
				startChar = &m_szName[startStringCharToDisplay];
			}
		}
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
	const float title_vertLst[8] =
	{
		cx,	0,
		0,		0,
		cx,	cy,
		0,		cy,
	};
	memcpy(vertexBufferClientArea.vertexArray, title_vertLst, sizeof(title_vertLst));

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
	startCursorSelectionPosition=0;
	endCursorSelectionPosition=0;
	setCursorPos(cursorPosition);
}

void geTextBox::setCursorPos(int index)
{
	if(index<0) return;

	cursorPosition=index;
	cursorPositionInPixel=0;
	for(int x=0;x<cursorPosition;x++)
		cursorPositionInPixel+=geFontManager::g_pFontArial10_80Ptr->getCharWidth(startChar[x]);
}

float geTextBox::getVirtualEndBound(int& index)
{
	index=0;
	float cursorPos=0;
	for(index=0;index<strlen(startChar);index++)
	{
		float temp=cursorPos;
		cursorPos+=geFontManager::g_pFontArial10_80Ptr->getCharWidth(startChar[index]);
		if(cursorPos>m_cSize.x)
		{
			cursorPos=temp;
			break;
		}
	}

	return cursorPos;
}

bool geTextBox::onMouseLButtonDown(float x, float y, int nFlag)
{
	if(!isPointInsideClientArea(x, y))
		return false;
	//isStartSelection=true;
	geTextBox::g_pCurrentlyActiveTextBoxPtr=this;

	if(geTextBox::g_pCurrentSelectedTextBoxPtr && geTextBox::g_pCurrentSelectedTextBoxPtr!=this)
		geTextBox::g_pCurrentSelectedTextBoxPtr->clearSelection();
	geTextBox::g_pCurrentSelectedTextBoxPtr=this;

	previousMouseXPosition=x;
	selectionStartXPosition=x-m_cPos.x;
	float full_width=getVirtualEndBound(cursorPosition);//geFontManager::g_pFontArial10_80Ptr->calculateStringWidthInPixelTillNewLine(startChar, strlen(startChar), 0);
	if(selectionStartXPosition>full_width)
	{
		selectionStartXPosition=full_width;
		setCursorPos(cursorPosition);
		startCursorSelectionPosition=0;
		endCursorSelectionPosition=cursorPosition;
	}
	isControlSelected=true;
	isStartSelection=false;

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
		float cursorPos=0;
		bool bflag=false;
		for(int m=0;m<len;m++)
		{
			float startX=cursorPos;
			cursorPos+=geFontManager::g_pFontArial10_80Ptr->getCharWidth(startChar[m]);
			float endX=cursorPos;
			if(x-m_cPos.x>startX && x-m_cPos.x<endX)
			{
				setCursorPos(m);
				bflag=true;
				break;
			}
		}
		if(!bflag)
		{
			startCursorSelectionPosition=0;
			endCursorSelectionPosition=len;
			setCursorPos(len);
		}
		isShowCursor=true;
		if(isShowSelection)
		{
			isControlSelected=false;
			startCursorSelectionPosition=0;
			endCursorSelectionPosition=0;
			isShowSelection=false;
			return true;
		}
		//
	}

	if(!isStartSelection)
	{
		float full_width=getVirtualEndBound(cursorPosition);
		//float line_width=geFontManager::g_pFontArial10_80Ptr->calculateStringWidthInPixelTillNewLine(startChar, len, 0);
		const float selection_vertLst[8] =
		{
			full_width,	0,
			0,			0,
			full_width,	m_cSize.y,
			0,			m_cSize.y,
		};
		memcpy(vertexBufferSelectionArea.vertexArray, selection_vertLst, sizeof(selection_vertLst));

		setCursorPos(cursorPosition);
		startCursorSelectionPosition=0;
		endCursorSelectionPosition=cursorPosition;
	}

	isShowSelection=true;

	isStartSelection=false;
	isControlSelected=false;
	previousMouseXPosition=x;

	return true;
}

bool geTextBox::onMouseMove(float x, float y, int flag)
{
	if(!isPointInsideClientArea(x, y))
		return false;

	if(flag&MK_LBUTTON)
		isStartSelection=true;

	if(isStartSelection && isControlSelected && geTextBox::g_pCurrentlyActiveTextBoxPtr==this && (flag&MK_LBUTTON))
	{
		float diff=x-m_cPos.x-selectionStartXPosition;
		float leftX=selectionStartXPosition;
		float rightX=selectionStartXPosition+diff;

		if(selectionStartXPosition+diff<selectionStartXPosition)
		{
			rightX=selectionStartXPosition;
			leftX=selectionStartXPosition+diff;
		}

		if(leftX<10 /*&& diff<0*/)
		{
			if(startStringCharToDisplay)
			{
				startStringCharToDisplay--;
				if(startStringCharToDisplay<=0)
					startStringCharToDisplay=0;
				startChar = &m_szName[startStringCharToDisplay];
			}
		}

		float full_width=geFontManager::g_pFontArial10_80Ptr->calculateStringWidthInPixelTillNewLine(startChar, (int)strlen(startChar), 0);
		if(rightX>m_cSize.x-10 /*&& diff>0*/)
		{
			if(full_width>m_cSize.x)
			{
				startStringCharToDisplay++;
				startChar = &m_szName[startStringCharToDisplay];
			}
		}

		//
		int len=0;//strlen(startChar);
		float cursorPos=getVirtualEndBound(len);
		full_width	=cursorPos;//reassign the virtual full width
		for(int m=len-1;m>=0;m--)
		{
			//float endX=cursorPos;
			cursorPos-=geFontManager::g_pFontArial10_80Ptr->getCharWidth(startChar[m]);
			if(leftX>cursorPos)
			{
				startCursorSelectionPosition=m;
				leftX=cursorPos;
				break;
			}
		}

		cursorPos=0;
		for(int m=0;m<len;m++)
		{
			//float startX=cursorPos;
			cursorPos+=geFontManager::g_pFontArial10_80Ptr->getCharWidth(startChar[m]);
			if(rightX<cursorPos)
			{
				endCursorSelectionPosition=m+1;
				rightX=cursorPos;
				break;
			}
		}

		if(selectionStartXPosition+diff<selectionStartXPosition)
		{
			setCursorPos(startCursorSelectionPosition);
		}
		else
		{
			setCursorPos(endCursorSelectionPosition);
		}

		if(rightX>full_width && diff>0)
		{
			rightX=full_width;
			//if(rightX>m_cSize.x)
			//	rightX=m_cSize.x;
			setCursorPos((int)strlen(startChar));
		}

		const float selection_vertLst[8] =
		{
			rightX,		0,
			leftX,		0,
			rightX,		m_cSize.y,
			leftX,		m_cSize.y,
		};
		memcpy(vertexBufferSelectionArea.vertexArray, selection_vertLst, sizeof(selection_vertLst));
	}
	previousMouseXPosition=x;

	return true;
}

void geTextBox::onCancelEngagedControls()
{

}

bool replaceSelection(char* str, int start, int end);

bool geTextBox::onKeyDown(int charValue, int flag)
{
	if(geTextBox::g_pCurrentlyActiveTextBoxPtr!=this)
		return false;

//	//replace the selection block
//	if(endCursorSelectionPosition>=strlen(startChar))
//	{
//		endCursorSelectionPosition=endCursorSelectionPosition;
//	}

	bool bReplaced=bReplaced=replaceSelection(startChar, startCursorSelectionPosition, endCursorSelectionPosition);

	if(bReplaced)
	{
		cursorPosition=startCursorSelectionPosition;
		startCursorSelectionPosition=0;
		endCursorSelectionPosition=0;
		isStartSelection=false;
		isShowSelection=false;
	}

	for(int x=(int)strlen(startChar);x>=cursorPosition;x--)
	{
		startChar[x+1]=startChar[x];
	}
	if(charValue==8)
	{
		if(!bReplaced && cursorPosition)
			bReplaced=replaceSelection(startChar, cursorPosition-1, cursorPosition);
		if(bReplaced)
			cursorPosition--;
		if(cursorPosition-1<=0)
			cursorPosition=0;
		setCursorPos(cursorPosition);
	}
	else
	{
		startChar[cursorPosition]=charValue;
		float full_width=geFontManager::g_pFontArial10_80Ptr->calculateStringWidthInPixelTillNewLine(startChar, (int)strlen(m_szName), 0);
		if(full_width>m_cSize.x)
		{
			int temp=0;
			getVirtualEndBound(temp);
			if(cursorPosition==temp)
			{
				startStringCharToDisplay++;
				startChar=&m_szName[startStringCharToDisplay];
				setCursorPos(cursorPosition);
			}
			else
				setCursorPos(cursorPosition+1);
		}
		else
			setCursorPos(cursorPosition+1);
		
		if(guiObserver)
			guiObserver->onTextChange(this);
	}

	return false;	//handled by special case
}

bool replaceSelection(char* str, int start, int end)
{
	if(end>start)
	{
		int remaining_len=(int)strlen(&str[end]);
		if(remaining_len==0)
		{
			strcpy(&str[start], "");
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
	if(guiObserver)
		guiObserver->onTextChange(this);
}