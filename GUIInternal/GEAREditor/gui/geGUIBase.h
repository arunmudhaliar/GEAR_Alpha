#pragma once

#include "../renderer/rendererGL10.h"
#include "../util/geDefines.h"
#include "../util/geVector2.h"
#include <vector>
#include "../util/geUtil.h"

//#if !defined(__APPLE__) //disable Drag-Drop
    #include "../OSSpecific/MDropData.h"
//#endif

#ifdef __APPLE__
    #define MK_LBUTTON  (1<<0)
    #define MK_RBUTTON  (1<<1)
    #define MK_CONTROL  (1<<2)
    #define MK_MBUTTON  (1<<3)
    #define MK_SHIFT    (1<<4)
    #define ERROR_ENVVAR_NOT_FOUND  0
#endif

#include "../util/geFontManager.h"

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
#define GEGUI_TOOLBAR_DROPMENU	GEGUI_BASE+15
#define GEGUI_SCROLLBAR			GEGUI_BASE+16
#define GEGUI_WINDOW_COLUMN		GEGUI_BASE+17
#define GEGUI_STATICTEXTBOX		GEGUI_BASE+18
#define GEGUI_SEPERATOR			GEGUI_BASE+19

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

	geGUIBase(unsigned short uGUIID, const char* name, geFontManager* fontManager);
	virtual ~geGUIBase();

	int getGUIID()	{	return m_uGUIID;	}

	void createBase(rendererGL10* renderer, geGUIBase* parent);
	void setParent(geGUIBase* parent);

	void setColor(stVertexBuffer* vbuffer, float r, float g, float b, float a, ESTYLE_GRADIENT eGradientStyle=EGRADIENT_NONE, float gradientScale=0.5f);
	void setColor(stVertexBuffer* vbuffer, int index, float r, float g, float b, float a);
	void applyPrimaryColorToVBClientArea(ESTYLE_GRADIENT eGradientStyle=EGRADIENT_NONE, float gradientScale=0.5f);
	void applySecondryColorToVBClientArea(ESTYLE_GRADIENT eGradientStyle=EGRADIENT_NONE, float gradientScale=0.5f);

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
	void setName(const char* name);

	//mouse events
	bool MouseLButtonDown(float x, float y, int nFlag);
	void MouseLButtonUp(float x, float y, int nFlag);

	bool MouseRButtonDown(float x, float y, int nFlag);
	void MouseRButtonUp(float x, float y, int nFlag);

	bool MouseMove(float x, float y, int flag);
	void MouseWheel(int zDelta, int x, int y, int flag);

//#if !defined(__APPLE__) //disable Drag-Drop
	void DragEnter(int x, int y);
	void DragDrop(int x, int y, MDropData* dropObject);
	void DragLeave();
//#endif
    
	bool KeyDown(int charValue, int flag);
	bool KeyUp(int charValue, int flag);

	void DoCommand(int cmd);

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
	geVector2f getPositionOnScreen();

	void focusLost();
	void resizeComplete();

	void setUserData(void* userdata)	{	m_pUserData=userdata;	}
	void* getUserData()					{	return m_pUserData;		}

	void setGUIObserver(MGUIObserver* observer)		{	m_pGUIObserver=observer;	}
	MGUIObserver* getGUIObserver()					{	return m_pGUIObserver;		}

	bool isNodeExistsInTree(geGUIBase* node);

	//layout hack
	void setActiveWindowPtrOnlyForLayout(geGUIBase* wnd)	{	m_pActiveWindowPtrOnlyForLayout = wnd;	}
	geGUIBase* getActiveWindowPtrOnlyForLayout()			{	return m_pActiveWindowPtrOnlyForLayout;	}
	//
	stVertexBuffer* getVertexBuffer()	{	return &m_cVBClientArea;	}
	void setClientAreaPrimaryActiveForeColor(float r, float g, float b, float a=1.0f);
	void setClientAreaSecondryActiveForeColor(float r, float g, float b, float a=1.0f);

	void notifyParent(int msg);

    geFontManager* getFontManager()     {   return m_pFontManagerPtr;   }
    
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

//#if !defined(__APPLE__) //disable Drag-Drop
	virtual void onDragEnter(int x, int y);
	virtual void onDragDrop(int x, int y, MDropData* dropObject);
	virtual void onDragLeave();
//#endif

	virtual bool onKeyDown(int charValue, int flag);
	virtual bool onKeyUp(int charValue, int flag);

	virtual void onSetName();

	virtual void onCommand(int cmd);

	virtual void onNotify(int msg);

	void drawRect(stVertexBuffer* vbuffer);
	void drawRect(stVertexBuffer* vbuffer, float* textureCoord, unsigned int texID);
	void drawLine(float* line, float r, float g, float b, float a, int count=4, bool bLoop=true);
	void drawTriangle(float* buffer, float r, float g, float b, float a, int count);

	bool isMouseEntered()			{	return m_bMouseEntered;	}
	void setMouseEntered(bool flag)	{	m_bMouseEntered=flag;	}

	bool isMouseBoundCheckEnabled()		{	return m_bMouseBoundCheckEnabled;	}
	void setMouseBoundCheck(bool flag)	{	m_bMouseBoundCheckEnabled=flag;		}

    void doDragDropSynchronous(MDropData* dropData);
    
	unsigned short m_uGUIID;
	char m_szName[256];

	geVector2f m_cPos;
	geVector2f m_cSize;

	bool m_bMouseEntered;
	bool m_bSizable;
	bool m_bMouseBoundCheckEnabled;

	geGUIBase* m_pParentPtr;
	std::vector<geGUIBase*> m_vControls;
	geGUIBase* m_pSelectedControlPtr;

	void* m_pUserData;
	MGUIObserver* m_pGUIObserver;
	rendererGL10* m_pRenderer;

	//hack for layout
	geGUIBase* m_pActiveWindowPtrOnlyForLayout;	//valid only if object is a layout
	//

	float m_fszClientAreaPrimaryActiveForeColor[4];
	float m_fszClientAreaSecondryActiveForeColor[4];
	
	stVertexBuffer m_cVBClientArea;
    geFontManager* m_pFontManagerPtr;
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

	virtual void onColorChange(geGUIBase* colorControl)
	{
	}
};