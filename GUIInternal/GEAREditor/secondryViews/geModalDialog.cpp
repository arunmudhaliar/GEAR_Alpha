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

void geModalDialog::onCreate(float cx, float cy)
{
    m_pWindow->setRenderer(secondryRenderer);
	auto layout = layoutManager->getRootLayout()->createLeft(m_pWindow, 1.0f);
    layout->unPinLayout();
}

void geModalDialog::onSize(float cx, float cy)
{

}

void geModalDialog::onDraw()
{
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
