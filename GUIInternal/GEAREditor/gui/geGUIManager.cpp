#include "geGUIManager.h"

CGETextureManager geGUIManager::g_cTextureManager;

geGUIManager::geGUIManager(geFontManager* fontmanager)
{
    layoutManager = new geLayoutManager(fontmanager);
}

geGUIManager::~geGUIManager()
{
	reset();
    GX_DELETE(layoutManager);
}

void geGUIManager::reset()
{
	for(std::vector<geWindow*>::iterator it = windowObjectList.begin(); it != windowObjectList.end(); ++it)
	{
		geWindow* obj = *it;
		GE_DELETE(obj);
	}
	windowObjectList.clear();
}

void geGUIManager::init(rendererGL10* renderer)
{
	layoutManager->create(renderer, 0, 0, 1184, 567);
}

void geGUIManager::appendWindow(geWindow* window)
{
	windowObjectList.push_back(window);
}

void geGUIManager::size(int cx, int cy)
{
	layoutManager->setSize(cx, cy);
}

void geGUIManager::update(float dt)
{
	for(std::vector<geWindow*>::iterator it = windowObjectList.begin(); it != windowObjectList.end(); ++it)
	{
		geWindow* obj = *it;
		obj->update(dt);
	}
}

void geGUIManager::draw()
{
	layoutManager->draw();
}

void geGUIManager::MouseLButtonDown(float x, float y, int nFlag)
{
	layoutManager->MouseLButtonDown(x, y, nFlag);
}

void geGUIManager::MouseLButtonUp(float x, float y, int nFlag)
{
	layoutManager->MouseLButtonUp(x, y, nFlag);
}

void geGUIManager::MouseMove(float x, float y, int flag)
{
	layoutManager->MouseMove(x, y, flag);
}

void geGUIManager::MouseWheel(int zDelta, int x, int y, int flag)
{
	layoutManager->MouseWheel(zDelta, x, y, flag);
}

void geGUIManager::MouseRButtonDown(float x, float y, int nFlag)
{
	layoutManager->MouseRButtonDown(x, y, nFlag);
}

void geGUIManager::MouseRButtonUp(float x, float y, int nFlag)
{
	layoutManager->MouseRButtonUp(x, y, nFlag);
}

//#if !defined(__APPLE__) //disable Drag-Drop
void geGUIManager::DragEnter(int x, int y)
{
	layoutManager->DragEnter(x, y);
}

void geGUIManager::DragDrop(int x, int y, MDropData* dropObject)
{
	layoutManager->DragDrop(x, y, dropObject);
}

void geGUIManager::DragLeave()
{
	layoutManager->DragLeave();
}
//#endif

bool geGUIManager::KeyDown(int charValue, int flag)
{
	return layoutManager->KeyDown(charValue, flag);
}

bool geGUIManager::KeyUp(int charValue, int flag)
{
	return layoutManager->KeyUp(charValue, flag);
}

void geGUIManager::DoCommand(int cmd)
{
	layoutManager->DoCommand(cmd);

}