#ifndef GEGUIBASE_H
#define GEGUIBASE_H

#include "../renderer/rendererGL10.h"
#include "../util/geDefines.h"
#include "../util/geVector2.h"
#include <vector>
#include "../util/geUtil.h"
#include "../win32/MDropSource.h"

#define GEGUI_BASE				3000
#define GEGUI_LAYOUT_MANAGER	GEGUI_BASE+1
#define GEGUI_LAYOUT			GEGUI_BASE+2
#define GEGUI_WINDOW			GEGUI_BASE+3
#define GEGUI_BUTTON			GEGUI_BASE+4
#define GEGUI_PUSH_BUTTON		GEGUI_BASE+5
#define GEGUI_TOOLBAR_BUTTON	GEGUI_BASE+6
#define GEGUI_TOOLBAR			GEGUI_BASE+7
#define GEGUI_TREEVIEW			GEGUI_BASE+8
#define GEGUI_TREEVIEW_NODE		GEGUI_BASE+9
#define GEGUI_TEXTBOX			GEGUI_BASE+10
#define GEGUI_HORIZONTAL_SLIDER	GEGUI_BASE+11
#define GEGUI_COLOR_CONTROL		GEGUI_BASE+12
#define GEGUI_TEXTURE_THUMBNAIL	GEGUI_BASE+13
#define GEGUI_TOOLBAR_SEPERATOR	GEGUI_BASE+14

struct stVertexBuffer
{
	float m_cszVertexList[8];		//4*2
	float m_cszVertexColorList[16];	//rgba 4*4
};

class MGUIObserver;
class geGUIBase
{
private:
public:
	geGUIBase();

	enum ESTYLE_GRADIENT
	{
		EGRADIENT_NONE,
		EGRADIENT_VERTICAL_UP,
		EGRADIENT_VERTICAL_DOWN,
		EGRADIENT_HORIZONTAL_RIGHT,
		EGRADIENT_HORIZONTAL_LEFT
	};

	geGUIBase(unsigned short uGUIID, const char* name);
	virtual ~geGUIBase();

	int getGUIID()	{	return m_uGUIID;	}

	void createBase(geGUIBase* parent);
	void setParent(geGUIBase* parent);

	void setColor(stVertexBuffer* vbuffer, float r, float g, float b, float a, ESTYLE_GRADIENT eGradientStyle=EGRADIENT_NONE, float gradientScale=0.5f);
	void setColor(stVertexBuffer* vbuffer, int index, float r, float g, float b, float a);

	bool isPointInsideWindow(float x, float y);
	bool isPointInsideClientArea(float x, float y);

	void setPos(const geVector2f& pos);
	void setPos(float x, float y);
	geVector2f& getPos()	{	return m_cPos;	}
	geVector2f& getSize()	{	return m_cSize;	}

	void setSize(const geVector2f& size);
	void setSize(float cx, float cy);

	virtual void draw()=0;
	virtual int getTopMarginOffsetHeight()	{	return 0;	}
	virtual int getTitleOffsetHeight()		{	return 0;	}

	const char* getName()			{	return m_szName;		}
	void setName(const char* name)	{	STRCPY(m_szName, name);	onSetName();	}

	//mouse events
	bool MouseLButtonDown(float x, float y, int nFlag);
	void MouseLButtonUp(float x, float y, int nFlag);

	bool MouseRButtonDown(float x, float y, int nFlag);
	void MouseRButtonUp(float x, float y, int nFlag);

	bool MouseMove(float x, float y, int flag);
	void MouseWheel(int zDelta, int x, int y, int flag);

	void DragEnter(int x, int y);
	void DragDrop(int x, int y, MDataObject* dropObject);
	void DragLeave();

	bool KeyDown(int charValue, int flag);
	bool KeyUp(int charValue, int flag);

	void CancelEngagedControls();

	geGUIBase* getParent()	{	return m_pParentPtr;	}
	//void setParent(geGUIBase* parent)	{	m_pParentPtr=parent;	}
	void appendChildControl(geGUIBase* child);
	std::vector<geGUIBase*>* getChildControls()	{	return &m_vControls;	}
	void hoverControl();
	void unHoverControl();

	void mouseEnteredClientArea();
	void mouseExitClientArea();

	bool isSizable()			{	return m_bSizable;	}
	void setSizable(bool flag)	{	m_bSizable=flag;	}
	virtual geVector2f getAbsolutePositionOnScreen();

	void focusLost();
	void resizeComplete();

	void setUserData(void* userdata)	{	m_pUserData=userdata;	}
	void* getUserData()					{	return m_pUserData;		}

	void setGUIObserver(MGUIObserver* observer)		{	m_pGUIObserver=observer;	}
	MGUIObserver* getGUIObserver()					{	return m_pGUIObserver;		}

	bool isNodeExistsInTree(geGUIBase* node);

protected:

	virtual void onCreate();

	virtual void onPosition(float x, float y, int flag);
	virtual void onSize(float cx, float cy, int flag);
	virtual void onResizeComplete();

	virtual bool onMouseLButtonDown(float x, float y, int nFlag);
	virtual bool onMouseLButtonUp(float x, float y, int nFlag);
	virtual bool onMouseRButtonDown(float x, float y, int nFlag);
	virtual void onMouseRButtonUp(float x, float y, int nFlag);

	virtual bool onMouseMove(float x, float y, int flag);
	virtual void onMouseWheel(int zDelta, int x, int y, int flag);

	virtual void onHoverControl(int x, int y);
	virtual void onUnHoverControl(int x, int y);

	virtual void onMouseEnterClientArea();
	virtual void onMouseExitClientArea();

	virtual void onCancelEngagedControls();

	virtual void onFocusLost();

	virtual void onAppendChild(geGUIBase* child);

	virtual void onDragEnter(int x, int y);
	virtual void onDragDrop(int x, int y, MDataObject* dropObject);
	virtual void onDragLeave();

	virtual bool onKeyDown(int charValue, int flag);
	virtual bool onKeyUp(int charValue, int flag);

	virtual void onSetName();

	void drawRect(stVertexBuffer* vbuffer);
	void drawRect(stVertexBuffer* vbuffer, float* textureCoord, unsigned int texID);
	void drawLine(float* line, float r, float g, float b, float a, int count=4, bool bLoop=true);
	void drawTriangle(float* buffer, float r, float g, float b, float a, int count);

	bool isMouseEntered()			{	return m_bMouseEntered;	}
	void setMouseEntered(bool flag)	{	m_bMouseEntered=flag;	}

	bool isMouseBoundCheckEnabled()		{	return m_bMouseBoundCheckEnabled;	}
	void setMouseBoundCheck(bool flag)	{	m_bMouseBoundCheckEnabled=flag;		}

	unsigned short m_uGUIID;
	char m_szName[64];

	geVector2f m_cPos;
	geVector2f m_cSize;

	bool m_bMouseEntered;
	bool m_bSizable;
	bool m_bMouseBoundCheckEnabled;

	geGUIBase* m_pParentPtr;
	std::vector<geGUIBase*> m_vControls;
	geGUIBase* m_pSelectedControlPtr;

	void* m_pUserData;
	MGUIObserver* m_pGUIObserver	;
};

class MGUIObserver
{
public:
	virtual void onButtonClicked(geGUIBase* btn)
	{
	}
	virtual void onSliderChange(geGUIBase* slider)
	{
	}

	virtual void onTextChange(geGUIBase* textbox)
	{
	}
};

#endif