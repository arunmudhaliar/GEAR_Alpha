#include "geGUIManager.h"

CGETextureManager geGUIManager::g_cTextureManager;

geGUIManager::geGUIManager(geFontManager* fontmanager)
{
    m_pLayoutManager = new geLayoutManager(fontmanager);
}

geGUIManager::~geGUIManager()
{
	reset();
    GX_DELETE(m_pLayoutManager);
}

void geGUIManager::reset()
{
	for(std::vector<geWindow*>::iterator it = m_vWindowObjects.begin(); it != m_vWindowObjects.end(); ++it)
	{
		geWindow* obj = *it;
		GE_DELETE(obj);
	}
	m_vWindowObjects.clear();
}

void geGUIManager::init(rendererGL10* renderer)
{
	m_pLayoutManager->create(renderer, 0, 0, 1184, 567);
}

void geGUIManager::appendWindow(geWindow* window)
{
	m_vWindowObjects.push_back(window);
}

void geGUIManager::size(int cx, int cy)
{
	m_pLayoutManager->setSize(cx, cy);
}

void geGUIManager::update(float dt)
{
	for(std::vector<geWindow*>::iterator it = m_vWindowObjects.begin(); it != m_vWindowObjects.end(); ++it)
	{
		geWindow* obj = *it;
		obj->update(dt);
	}
}

void geGUIManager::draw()
{
	m_pLayoutManager->draw();
}

void geGUIManager::MouseLButtonDown(float x, float y, int nFlag)
{
	m_pLayoutManager->MouseLButtonDown(x, y, nFlag);
}

void geGUIManager::MouseLButtonUp(float x, float y, int nFlag)
{
	m_pLayoutManager->MouseLButtonUp(x, y, nFlag);
}

void geGUIManager::MouseMove(float x, float y, int flag)
{
	m_pLayoutManager->MouseMove(x, y, flag);
}

void geGUIManager::MouseWheel(int zDelta, int x, int y, int flag)
{
	m_pLayoutManager->MouseWheel(zDelta, x, y, flag);
}

void geGUIManager::MouseRButtonDown(float x, float y, int nFlag)
{
	m_pLayoutManager->MouseRButtonDown(x, y, nFlag);
}

void geGUIManager::MouseRButtonUp(float x, float y, int nFlag)
{
	m_pLayoutManager->MouseRButtonUp(x, y, nFlag);
}

//#if !defined(__APPLE__) //disable Drag-Drop
void geGUIManager::DragEnter(int x, int y)
{
	m_pLayoutManager->DragEnter(x, y);
}

void geGUIManager::DragDrop(int x, int y, MDropData* dropObject)
{
	m_pLayoutManager->DragDrop(x, y, dropObject);
}

void geGUIManager::DragLeave()
{
	m_pLayoutManager->DragLeave();
}
//#endif

bool geGUIManager::KeyDown(int charValue, int flag)
{
	return m_pLayoutManager->KeyDown(charValue, flag);
}

bool geGUIManager::KeyUp(int charValue, int flag)
{
	return m_pLayoutManager->KeyUp(charValue, flag);
}

void geGUIManager::DoCommand(int cmd)
{
	m_pLayoutManager->DoCommand(cmd);

}