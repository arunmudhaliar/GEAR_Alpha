#include "gearSceneGraphEditor.h"
#include "../EditorApp.h"

gearSceneGraphEditor::gearSceneGraphEditor(geFontManager* fontmanager):
geWindow("Graph Editor", fontmanager)
{
    graphControlView = new geGraphControl(fontmanager);
}

gearSceneGraphEditor::~gearSceneGraphEditor()
{
    GE_DELETE(graphControlView);
}

void gearSceneGraphEditor::onCreate()
{
    graphControlView->create(rendererGUI, this, 0, 0, 10, 10, 1, 1000);
}

void gearSceneGraphEditor::onSize(float cx, float cy, int flag)
{
    geWindow::onSize(cx, cy, flag);
}

void gearSceneGraphEditor::onDraw()
{
#ifdef LOG_GLERROR
	int err=glGetError();
	if(err!=GL_NO_ERROR)
	{
		printf("glGetError 0x%x\n", err);
	}
#endif

    graphControlView->draw();
}
