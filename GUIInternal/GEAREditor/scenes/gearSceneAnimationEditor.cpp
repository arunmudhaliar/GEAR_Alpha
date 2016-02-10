#include "gearSceneAnimationEditor.h"
#include "../EditorApp.h"

gearSceneAnimationEditor::gearSceneAnimationEditor(geFontManager* fontmanager):
geWindow("Animation Editor", fontmanager)
{
    m_pAnimationsTreeView = new geTreeView(fontmanager);
}

gearSceneAnimationEditor::~gearSceneAnimationEditor()
{   
    GE_DELETE(m_pAnimationsTreeView);
}

void gearSceneAnimationEditor::onCreate()
{
	m_pAnimationsTreeView->create(rendererGUI, this, "propertyeditorTV", this);
	geTreeNode* rootNode=m_pAnimationsTreeView->getRoot();
}

void gearSceneAnimationEditor::onDraw()
{
#ifdef LOG_GLERROR
	int err=glGetError();
	if(err!=GL_NO_ERROR)
	{
		printf("glGetError 0x%x\n", err);
	}
#endif

	m_pAnimationsTreeView->draw();
}

void gearSceneAnimationEditor::onTVSelectionChange(geTreeNode* tvnode, geTreeView* treeview)
{
}
