#pragma once

#include "../util/geDefines.h"
#include "../renderer/rendererGL10.h"
#ifdef _WIN32
    #include <Windows.h>
    #include "../../resource.h"
#endif
#include "../gui/geLayout.h"
#include "../gui/geLayoutManager.h"
#include <string>

#ifdef _WIN32
	#include <SDL.h>
#elif defined(__APPLE__)
	#include <SDL2/SDL.h>
#else
	#error SDL not included
#endif

class geSecondryView
{
public:
	virtual ~geSecondryView();

	void createRenderer(SDL_Window* window);
	rendererGL10* getRenderer()     {	return secondryRenderer;	}

	void showView(int extraWindowFlags=0);
	void drawView();
	void sizeView(float cx, float cy);
	void destroyView();

	void setSize(geVector2f& sz);
	void setPos(geVector2i& pos);

    //mouse events
	bool mouseLButtonDown(float x, float y, int nFlag);
	void mouseLButtonUp(float x, float y, int nFlag);
	bool mouseRButtonDown(float x, float y, int nFlag);
	void mouseRButtonUp(float x, float y, int nFlag);
	bool mouseMove(float x, float y, int flag);
	void mouseWheel(int zDelta, int x, int y, int flag);

    void doCommand(int cmd);

    SDL_Window* getSecondryWindow() {   return sdlSecondryWindow;   }
        
protected:
    geSecondryView(const std::string& name, geFontManager* fontmanager, rendererGL10* mainRenderer);
    
	virtual void onCreate(float cx, float cy);
	virtual void onSize(float cx, float cy);
	virtual void onDraw();
	virtual void onDestroy();

	virtual bool onMouseLButtonDown(float x, float y, int nFlag);
	virtual void onMouseLButtonUp(float x, float y, int nFlag);
	virtual bool onMouseRButtonDown(float x, float y, int nFlag);
	virtual void onMouseRButtonUp(float x, float y, int nFlag);
	virtual bool onMouseMove(float x, float y, int flag);
	virtual void onMouseWheel(int zDelta, int x, int y, int flag);

    virtual void onDoCommand(int cmd);

    SDL_Window* createSecondryWindow();
    void processEvent(SDL_Window * window, SDL_Event& e, void* userData);
    
    static int secondryThread( void *ptr );
    
    int extraWindowFlags;
    SDL_Window* sdlSecondryWindow;
    rendererGL10* primaryRenderer;
    rendererGL10* secondryRenderer;
    geVector2f previousScale;
    std::string m_szName;
    geLayoutManager* layoutManager;
    
    geVector2i m_cPos;
    geVector2f m_cSize;
    geFontManager* fontManager;
};