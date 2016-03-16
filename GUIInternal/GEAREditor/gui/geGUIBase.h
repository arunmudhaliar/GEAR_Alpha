#pragma once

#include "../renderer/rendererGL10.h"
#include "../util/geDefines.h"
#include "../util/geVector2.h"
#include <vector>
#include "../util/geUtil.h"

//#if !defined(__APPLE__) //disable Drag-Drop
    #include "../OSSpecific/MDropData.h"
//#endif
#include "../util/geFontManager.h"


#pragma mark Defines for mouse events in MACOS
#ifdef __APPLE__
    #define MK_LBUTTON  (1<<0)
    #define MK_RBUTTON  (1<<1)
    #define MK_CONTROL  (1<<2)
    #define MK_MBUTTON  (1<<3)
    #define MK_SHIFT    (1<<4)
    #define ERROR_ENVVAR_NOT_FOUND  0
#endif

#pragma mark Defines for GEGUI_* control IDs

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
#define GEGUI_GRAPHCONTROL		GEGUI_BASE+20

#pragma mark stVertexBuffer
struct stVertexBuffer
{
    stVertexBuffer();
    void setRect(const gxRectf& rect, const vector4f& color);
    void setRect(const gxRectf& rect, const vector4f& upperColor, const vector4f& lowerColor);
    void setColor(int index, float r, float g, float b, float a);
    void updateRect(float x, float y, float cx, float cy);
    void draw(float* textureCoord, unsigned int texID);
    void reset();
    const gxRectf& getRect()    {   return rect;    }
private:
    gxRectf rect;
	float vertexArray[8];		//4*2
	float vertexColorArray[16];	//rgba 4*4
};

#pragma mark geGUIBase
class MGUIObserver;
class geGUIBase
{
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

	int getGUIID()	{	return guiID;	}

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

    bool MouseMButtonDown(float x, float y, int nFlag);
    void MouseMButtonUp(float x, float y, int nFlag);

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

	geGUIBase* getParent()	{	return parent;	}
	//void setParent(geGUIBase* parent)	{	this->parent=parent;	}
	void appendChildControl(geGUIBase* child);
    void removeChildControl(geGUIBase* child);
    
	std::vector<geGUIBase*>* getChildControls()	{	return &childControlList;	}
	void hoverControl();
	void unHoverControl();

	void mouseEnteredClientArea();
	void mouseExitClientArea();

	bool isSizable()			{	return is_Sizable;	}
	void setSizable(bool flag)	{	is_Sizable=flag;	}
	virtual geVector2f getAbsolutePositionOnScreen();
	geVector2f getPositionOnScreen();

	void focusLost();
	void resizeComplete();

	void setUserData(void* userdata)	{	this->userData=userdata;	}
	void* getUserData()					{	return userData;		}

	void setGUIObserver(MGUIObserver* observer)		{	guiObserver=observer;	}
	MGUIObserver* getGUIObserver()					{	return guiObserver;		}

	bool isNodeExistsInTree(geGUIBase* node);

	//layout hack
	void setActiveWindowPtrOnlyForLayout(geGUIBase* wnd)	{	activeWindowPtrOnlyForLayout = wnd;	}
	geGUIBase* getActiveWindowPtrOnlyForLayout()			{	return activeWindowPtrOnlyForLayout;	}
	//
	stVertexBuffer* getVertexBuffer()	{	return &vertexBufferClientArea;	}
	void setClientAreaPrimaryActiveForeColor(float r, float g, float b, float a=1.0f);
	void setClientAreaSecondryActiveForeColor(float r, float g, float b, float a=1.0f);

	void notifyParent(int msg);

    geFontManager* getFontManager()             {   return fontManagerGUI;      }
    rendererGL10* getRenderer()                 {   return rendererGUI;         }
    virtual void setRenderer(rendererGL10* renderer, bool recursive=false);
    
    bool isMouseBoundCheckEnabled()             {	return is_MouseBoundCheckEnabled;	}
    void setMouseBoundCheck(bool flag)          {	is_MouseBoundCheckEnabled=flag;		}
    
protected:
	virtual void onCreate(float cx, float cy);

	virtual void onPosition(float x, float y, int flag);
	virtual void onSize(float cx, float cy, int flag);
	virtual void onResizeComplete();

	virtual bool onMouseLButtonDown(float x, float y, int nFlag);
	virtual bool onMouseLButtonUp(float x, float y, int nFlag);
	virtual bool onMouseRButtonDown(float x, float y, int nFlag);
	virtual void onMouseRButtonUp(float x, float y, int nFlag);
    virtual bool onMouseMButtonDown(float x, float y, int nFlag);
    virtual void onMouseMButtonUp(float x, float y, int nFlag);

	virtual bool onMouseMove(float x, float y, int flag);
	virtual void onMouseWheel(int zDelta, int x, int y, int flag);

	virtual void onHoverControl(int x, int y);
	virtual void onUnHoverControl(int x, int y);

	virtual void onMouseEnterClientArea();
	virtual void onMouseExitClientArea();

	virtual void onCancelEngagedControls();

	virtual void onFocusLost();

	virtual void onAppendChild(geGUIBase* child);
    virtual void onRemoveChild(geGUIBase* child);

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

	bool isMouseEntered()			{	return is_MouseEntered;	}
	void setMouseEntered(bool flag)	{	is_MouseEntered=flag;	}
    
    void doDragDropSynchronous(MDropData* dropData);
    geGUIBase* getMainWindow()  {   return mainWindow;  }
    
	unsigned short guiID;
	char m_szName[256];

	geVector2f m_cPos;
	geVector2f m_cSize;

	bool is_MouseEntered;
	bool is_Sizable;
	bool is_MouseBoundCheckEnabled;

	geGUIBase* parent;
	std::vector<geGUIBase*> childControlList;
	geGUIBase* selectedControl;

	void* userData;
	MGUIObserver* guiObserver;
	rendererGL10* rendererGUI;

	//hack for layout
	geGUIBase* activeWindowPtrOnlyForLayout;	//valid only if object is a layout
	//

	float clientAreaPrimaryActiveForeColor[4];
	float clientAreaSecondryActiveForeColor[4];
	
	stVertexBuffer vertexBufferClientArea;
    geFontManager* fontManagerGUI;
    geGUIBase* mainWindow;
    
private:
    void applyClipIfIamOnMainWindow();
};

#pragma mark MGUIObserver
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