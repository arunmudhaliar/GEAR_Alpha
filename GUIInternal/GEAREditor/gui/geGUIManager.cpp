#include "geGUIManager.h"

FontManager geGUIManager::g_cFontManager;
gxFont* geGUIManager::g_pFontArial12Ptr=NULL;
CGETextureManager geGUIManager::g_cTextureManager;

geGUIManager::geGUIManager()
{
}

geGUIManager::~geGUIManager()
{
	for(std::vector<geWindow*>::iterator it = m_vWindowObjects.begin(); it != m_vWindowObjects.end(); ++it)
	{
		geWindow* obj = *it;
		GE_DELETE(obj);
	}
	m_vWindowObjects.clear();
}

void geGUIManager::init()
{
	g_cFontManager.init(rendererBase::gl_fixed_pipeline);
	g_pFontArial12Ptr=g_cFontManager.loadFont("res//fonts//arial_iphone10_84.ecf");
	m_cLayoutManager.create(0, 0, 1184, 567);
}

void geGUIManager::appendWindow(geWindow* window)
{
	m_vWindowObjects.push_back(window);
}

void geGUIManager::size(int cx, int cy)
{
	m_cLayoutManager.setSize(cx, cy);
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
	m_cLayoutManager.draw();
}

void geGUIManager::MouseLButtonDown(float x, float y, int nFlag)
{
	m_cLayoutManager.MouseLButtonDown(x, y, nFlag);
}

void geGUIManager::MouseLButtonUp(float x, float y, int nFlag)
{
	m_cLayoutManager.MouseLButtonUp(x, y, nFlag);
}

void geGUIManager::MouseMove(float x, float y, int flag)
{
	m_cLayoutManager.MouseMove(x, y, flag);
}

void geGUIManager::MouseWheel(int zDelta, int x, int y, int flag)
{
	m_cLayoutManager.MouseWheel(zDelta, x, y, flag);
}

void geGUIManager::MouseRButtonDown(float x, float y, int nFlag)
{
	m_cLayoutManager.MouseRButtonDown(x, y, nFlag);
}

void geGUIManager::MouseRButtonUp(float x, float y, int nFlag)
{
	m_cLayoutManager.MouseRButtonUp(x, y, nFlag);
}

void geGUIManager::DragEnter(int x, int y)
{
	m_cLayoutManager.DragEnter(x, y);
}

void geGUIManager::DragDrop(int x, int y, MDataObject* dropObject)
{
	m_cLayoutManager.DragDrop(x, y, dropObject);
}

void geGUIManager::DragLeave()
{
	m_cLayoutManager.DragLeave();
}

bool geGUIManager::KeyDown(int charValue, int flag)
{
	return m_cLayoutManager.KeyDown(charValue, flag);
}

bool geGUIManager::KeyUp(int charValue, int flag)
{
	return m_cLayoutManager.KeyUp(charValue, flag);
}

void geGUIManager::DoCommand(int cmd)
{
	m_cLayoutManager.DoCommand(cmd);

}