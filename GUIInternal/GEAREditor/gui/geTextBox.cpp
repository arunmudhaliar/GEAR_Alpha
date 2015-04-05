#include "geTextBox.h"
#include "geGUIManager.h"

geTextBox* geTextBox::g_pCurrentSelectedTextBoxPtr=NULL;
geTextBox* geTextBox::g_pCurrentlyActiveTextBoxPtr=NULL;

geTextBox::geTextBox(geFontManager* fontmanager):
	geGUIBase(GEGUI_TEXTBOX, "Text Box", fontmanager)
{
	m_bStartSelection=false;
	m_bShowSelection=false;
	m_fPrevMouseXPos=0.0f;
	m_fSelectionStartXPos=0.0f;
	m_bShowCursor=false;
	m_fCursorPosInPixels=0.0f;
	m_bControlSelected=false;
	m_iCursorPos=0;
	m_iStartCursorSelectionPos=0;
	m_iEndCursorSelectionPos=0;
	m_iStartStringCharToDisplay=0;
	m_pStartCharPtr=NULL;
}

geTextBox::geTextBox(const char* name, geFontManager* fontmanager):
	geGUIBase(GEGUI_TEXTBOX, name, fontmanager)
{
	m_bStartSelection=false;
	m_bShowSelection=false;
	m_fPrevMouseXPos=0.0f;
	m_fSelectionStartXPos=0.0f;
	m_bShowCursor=false;
	m_fCursorPosInPixels=0.0f;
	m_bControlSelected=false;
	m_iCursorPos=0;
	m_iStartCursorSelectionPos=0;
	m_iEndCursorSelectionPos=0;
	m_iStartStringCharToDisplay=0;
	m_pStartCharPtr=NULL;
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
	setColor(&m_cVBSelectionArea, 0.24f, 0.38f, 0.57f, 1.0f);
	m_pStartCharPtr=m_szName;
}

void geTextBox::draw()
{
	if(m_bStartSelection && m_bControlSelected && geTextBox::g_pCurrentlyActiveTextBoxPtr==this)
	{
		float diff=m_fPrevMouseXPos-m_cPos.x-m_fSelectionStartXPos;
		float leftX=m_fSelectionStartXPos;
		float rightX=m_fSelectionStartXPos+diff;

		if(m_fSelectionStartXPos+diff<m_fSelectionStartXPos)
		{
			rightX=m_fSelectionStartXPos;
			leftX=m_fSelectionStartXPos+diff;
		}

		if(leftX<10 /*&& diff<0*/)
		{
			if(m_iStartStringCharToDisplay)
			{
				m_iStartStringCharToDisplay--;
				if(m_iStartStringCharToDisplay<=0)
					m_iStartStringCharToDisplay=0;
				m_pStartCharPtr = &m_szName[m_iStartStringCharToDisplay];
			}
		}

		float full_width=geFontManager::g_pFontArial10_80Ptr->calculateStringWidthInPixelTillNewLine(m_pStartCharPtr, (int)strlen(m_pStartCharPtr), 0);
		if(rightX>m_cSize.x-10 /*&& diff>0*/)
		{
			if(full_width>m_cSize.x)
			{
				m_iStartStringCharToDisplay++;
				m_pStartCharPtr = &m_szName[m_iStartStringCharToDisplay];
			}
		}
	}

	glPushMatrix();
	glTranslatef(m_cPos.x, m_cPos.y, 0);
	drawRect(&m_cVBClientArea);
	if(m_bStartSelection || m_bShowSelection)
		drawRect(&m_cVBSelectionArea);
	if(m_bShowCursor)
	{
		glPushMatrix();
		glTranslatef(m_fCursorPosInPixels, 0, 0);
		drawLine(m_cVBCursorLine, 0.8f, 0.8f, 0.8f, 1.0f, 2, false);
		glPopMatrix();
	}
	drawLine(m_cVBClientAreaLine, 0.13f, 0.13f, 0.13f, 1.0f, 3, false);
	drawLine(&m_cVBClientAreaLine[4], 0.3f, 0.3f, 0.3f, 1.0f, 3, false);
	geFontManager::g_pFontArial10_80Ptr->drawString(m_pStartCharPtr, 0, geFontManager::g_pFontArial10_80Ptr->getLineHeight()-5, m_cSize.x);
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
	memcpy(m_cVBClientArea.m_cszVertexList, title_vertLst, sizeof(title_vertLst));

	const float clientarea_linevertLst[10] =
	{
		cx,	0,
		0,	0,
		0,	cy-0.5f,
		cx,	cy-0.5f,
		cx,	0,
	};
	memcpy(m_cVBClientAreaLine, clientarea_linevertLst, sizeof(clientarea_linevertLst));

	m_cVBCursorLine[0]=0.0f;	m_cVBCursorLine[1]=0.0f;
	m_cVBCursorLine[2]=0.0f;	m_cVBCursorLine[3]=cy;
}

void geTextBox::clearSelection()
{
	m_bShowSelection=false;
	m_bStartSelection=false;
	m_bShowCursor=false;
	m_iStartCursorSelectionPos=0;
	m_iEndCursorSelectionPos=0;
	setCursorPos(m_iCursorPos);
}

void geTextBox::setCursorPos(int index)
{
	if(index<0) return;

	m_iCursorPos=index;
	m_fCursorPosInPixels=0;
	for(int x=0;x<m_iCursorPos;x++)
		m_fCursorPosInPixels+=geFontManager::g_pFontArial10_80Ptr->getCharWidth(m_pStartCharPtr[x]);
}

float geTextBox::getVirtualEndBound(int& index)
{
	index=0;
	float cursorPos=0;
	for(index=0;index<strlen(m_pStartCharPtr);index++)
	{
		float temp=cursorPos;
		cursorPos+=geFontManager::g_pFontArial10_80Ptr->getCharWidth(m_pStartCharPtr[index]);
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
	//m_bStartSelection=true;
	geTextBox::g_pCurrentlyActiveTextBoxPtr=this;

	if(geTextBox::g_pCurrentSelectedTextBoxPtr && geTextBox::g_pCurrentSelectedTextBoxPtr!=this)
		geTextBox::g_pCurrentSelectedTextBoxPtr->clearSelection();
	geTextBox::g_pCurrentSelectedTextBoxPtr=this;

	m_fPrevMouseXPos=x;
	m_fSelectionStartXPos=x-m_cPos.x;
	float full_width=getVirtualEndBound(m_iCursorPos);//geFontManager::g_pFontArial10_80Ptr->calculateStringWidthInPixelTillNewLine(m_pStartCharPtr, strlen(m_pStartCharPtr), 0);
	if(m_fSelectionStartXPos>full_width)
	{
		m_fSelectionStartXPos=full_width;
		setCursorPos(m_iCursorPos);
		m_iStartCursorSelectionPos=0;
		m_iEndCursorSelectionPos=m_iCursorPos;
	}
	m_bControlSelected=true;
	m_bStartSelection=false;

	return true;
}

bool geTextBox::onMouseLButtonUp(float x, float y, int nFlag)
{
	if(!isPointInsideClientArea(x, y))
		return false;

	if(!m_bStartSelection)
	{
		//insert cursor
		int len=(int)strlen(m_pStartCharPtr);
		float cursorPos=0;
		bool bflag=false;
		for(int m=0;m<len;m++)
		{
			float startX=cursorPos;
			cursorPos+=geFontManager::g_pFontArial10_80Ptr->getCharWidth(m_pStartCharPtr[m]);
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
			m_iStartCursorSelectionPos=0;
			m_iEndCursorSelectionPos=len;
			setCursorPos(len);
		}
		m_bShowCursor=true;
		if(m_bShowSelection)
		{
			m_bControlSelected=false;
			m_iStartCursorSelectionPos=0;
			m_iEndCursorSelectionPos=0;
			m_bShowSelection=false;
			return true;
		}
		//
	}

	if(!m_bStartSelection)
	{
		float full_width=getVirtualEndBound(m_iCursorPos);
		//float line_width=geFontManager::g_pFontArial10_80Ptr->calculateStringWidthInPixelTillNewLine(m_pStartCharPtr, len, 0);
		const float selection_vertLst[8] =
		{
			full_width,	0,
			0,			0,
			full_width,	m_cSize.y,
			0,			m_cSize.y,
		};
		memcpy(m_cVBSelectionArea.m_cszVertexList, selection_vertLst, sizeof(selection_vertLst));

		setCursorPos(m_iCursorPos);
		m_iStartCursorSelectionPos=0;
		m_iEndCursorSelectionPos=m_iCursorPos;
	}

	m_bShowSelection=true;

	m_bStartSelection=false;
	m_bControlSelected=false;
	m_fPrevMouseXPos=x;

	return true;
}

bool geTextBox::onMouseMove(float x, float y, int flag)
{
	if(!isPointInsideClientArea(x, y))
		return false;

	if(flag&MK_LBUTTON)
		m_bStartSelection=true;

	if(m_bStartSelection && m_bControlSelected && geTextBox::g_pCurrentlyActiveTextBoxPtr==this && (flag&MK_LBUTTON))
	{
		float diff=x-m_cPos.x-m_fSelectionStartXPos;
		float leftX=m_fSelectionStartXPos;
		float rightX=m_fSelectionStartXPos+diff;

		if(m_fSelectionStartXPos+diff<m_fSelectionStartXPos)
		{
			rightX=m_fSelectionStartXPos;
			leftX=m_fSelectionStartXPos+diff;
		}

		if(leftX<10 /*&& diff<0*/)
		{
			if(m_iStartStringCharToDisplay)
			{
				m_iStartStringCharToDisplay--;
				if(m_iStartStringCharToDisplay<=0)
					m_iStartStringCharToDisplay=0;
				m_pStartCharPtr = &m_szName[m_iStartStringCharToDisplay];
			}
		}

		float full_width=geFontManager::g_pFontArial10_80Ptr->calculateStringWidthInPixelTillNewLine(m_pStartCharPtr, (int)strlen(m_pStartCharPtr), 0);
		if(rightX>m_cSize.x-10 /*&& diff>0*/)
		{
			if(full_width>m_cSize.x)
			{
				m_iStartStringCharToDisplay++;
				m_pStartCharPtr = &m_szName[m_iStartStringCharToDisplay];
			}
		}

		//
		int len=0;//strlen(m_pStartCharPtr);
		float cursorPos=getVirtualEndBound(len);
		full_width	=cursorPos;//reassign the virtual full width
		for(int m=len-1;m>=0;m--)
		{
			//float endX=cursorPos;
			cursorPos-=geFontManager::g_pFontArial10_80Ptr->getCharWidth(m_pStartCharPtr[m]);
			if(leftX>cursorPos)
			{
				m_iStartCursorSelectionPos=m;
				leftX=cursorPos;
				break;
			}
		}

		cursorPos=0;
		for(int m=0;m<len;m++)
		{
			//float startX=cursorPos;
			cursorPos+=geFontManager::g_pFontArial10_80Ptr->getCharWidth(m_pStartCharPtr[m]);
			if(rightX<cursorPos)
			{
				m_iEndCursorSelectionPos=m+1;
				rightX=cursorPos;
				break;
			}
		}

		if(m_fSelectionStartXPos+diff<m_fSelectionStartXPos)
		{
			setCursorPos(m_iStartCursorSelectionPos);
		}
		else
		{
			setCursorPos(m_iEndCursorSelectionPos);
		}

		if(rightX>full_width && diff>0)
		{
			rightX=full_width;
			//if(rightX>m_cSize.x)
			//	rightX=m_cSize.x;
			setCursorPos((int)strlen(m_pStartCharPtr));
		}

		const float selection_vertLst[8] =
		{
			rightX,		0,
			leftX,		0,
			rightX,		m_cSize.y,
			leftX,		m_cSize.y,
		};
		memcpy(m_cVBSelectionArea.m_cszVertexList, selection_vertLst, sizeof(selection_vertLst));
	}
	m_fPrevMouseXPos=x;

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
//	if(m_iEndCursorSelectionPos>=strlen(m_pStartCharPtr))
//	{
//		m_iEndCursorSelectionPos=m_iEndCursorSelectionPos;
//	}

	bool bReplaced=bReplaced=replaceSelection(m_pStartCharPtr, m_iStartCursorSelectionPos, m_iEndCursorSelectionPos);

	if(bReplaced)
	{
		m_iCursorPos=m_iStartCursorSelectionPos;
		m_iStartCursorSelectionPos=0;
		m_iEndCursorSelectionPos=0;
		m_bStartSelection=false;
		m_bShowSelection=false;
	}

	for(int x=(int)strlen(m_pStartCharPtr);x>=m_iCursorPos;x--)
	{
		m_pStartCharPtr[x+1]=m_pStartCharPtr[x];
	}
	if(charValue==8)
	{
		if(!bReplaced && m_iCursorPos)
			bReplaced=replaceSelection(m_pStartCharPtr, m_iCursorPos-1, m_iCursorPos);
		if(bReplaced)
			m_iCursorPos--;
		if(m_iCursorPos-1<=0)
			m_iCursorPos=0;
		setCursorPos(m_iCursorPos);
	}
	else
	{
		m_pStartCharPtr[m_iCursorPos]=charValue;
		float full_width=geFontManager::g_pFontArial10_80Ptr->calculateStringWidthInPixelTillNewLine(m_pStartCharPtr, (int)strlen(m_szName), 0);
		if(full_width>m_cSize.x)
		{
			int temp=0;
			getVirtualEndBound(temp);
			if(m_iCursorPos==temp)
			{
				m_iStartStringCharToDisplay++;
				m_pStartCharPtr=&m_szName[m_iStartStringCharToDisplay];
				setCursorPos(m_iCursorPos);
			}
			else
				setCursorPos(m_iCursorPos+1);
		}
		else
			setCursorPos(m_iCursorPos+1);
		
		if(m_pGUIObserver)
			m_pGUIObserver->onTextChange(this);
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
	if(m_pGUIObserver)
		m_pGUIObserver->onTextChange(this);
}