#include "geModalDialog.h"
#include "../EditorApp.h"

geModalDialog::geModalDialog(geWindow* pWindow, geVector2i pos):
	geSecondryView(pWindow->getName(), pWindow->getFontManager(), pWindow->getRenderer())
{
    setPos(pos);
	setSize(pWindow->getSize());
	m_pWindow=pWindow;
}

geModalDialog::~geModalDialog()
{
}

void geModalDialog::onCreate()
{
    m_pWindow->setRenderer(secondryRenderer);
	auto layout = layoutManager->getRootLayout()->createAsParent(m_pWindow);
    layout->unPinLayout();
}

void geModalDialog::onSize(float cx, float cy)
{

}

void geModalDialog::onDraw()
{
    m_pWindow->draw();
}

void geModalDialog::onDestroy()
{
    layoutManager->getRootLayout()->removeWindow(m_pWindow);
    m_pWindow->setRenderer(primaryRenderer);
}

geModalDialog* geModalDialog::createModalDialog(geWindow* pWindow, geVector2i pos)
{
    geModalDialog* modalDiaog = new geModalDialog(pWindow, pos);
    modalDiaog->showView(SDL_WINDOW_RESIZABLE);
    return modalDiaog;
}
