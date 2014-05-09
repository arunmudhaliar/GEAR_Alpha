#include "gePropertyOpenOnEditor.h"
#include "../../EditorApp.h"

gePropertyOpenOnEditor::gePropertyOpenOnEditor(rendererGL10* renderer, geGUIBase* parent, const char* name, Sprite2Dx* sprite):
	geTreeNode(renderer, parent, name, sprite, 10)
{
	setSize(m_cSize.x, 70.0f);

	m_pButtonOpenOnEditor = new geButton("");
	m_pButtonOpenOnEditor->create(renderer, this, "Open in editor", 15, 20);
	m_pButtonOpenOnEditor->setGUIObserver(this);

	setNodeColor(0.21f, 0.21f, 0.21f);
	setNodeSelectionColor(0.21f, 0.21f, 0.21f);
	setClientAreaPrimaryActiveForeColor(0.21f, 0.21f, 0.21f, 1.0f);
	applyPrimaryColorToVBClientArea();
}

gePropertyOpenOnEditor::~gePropertyOpenOnEditor()
{
}

void gePropertyOpenOnEditor::drawNode()
{
	drawRect(&m_cVBClientArea);

	for(std::vector<geGUIBase*>::iterator it = m_vControls.begin(); it != m_vControls.end(); ++it)
	{
		geGUIBase* tvnode = *it;
		tvnode->draw();
	}

	if(m_pSprite)
		m_pSprite->draw();
}

void gePropertyOpenOnEditor::onButtonClicked(geGUIBase* btn)
{
	if(btn==m_pButtonOpenOnEditor)
	{
		if(m_pButtonOpenOnEditor->isButtonPressed())
		{
			char buffer[1024];
			char responsebuffer[4096];
			//GetCurrentDirectory(sizeof(buffer), buffer);
			memset(buffer, 0, sizeof(buffer));
			memset(responsebuffer, 0, sizeof(responsebuffer));
			sprintf(buffer, "CSharpProjectMaker.exe gearProject %s", EditorApp::getProjectHomeDirectory());
			printf("\n================Creating Visual Studio Project===============\n");
			if(monoWrapper::exec_cmd(buffer, responsebuffer)==0)
			{
				memset(buffer, 0, sizeof(buffer));
				memset(responsebuffer, 0, sizeof(responsebuffer));
				sprintf(buffer, "start %s/gearProject.sln", EditorApp::getProjectHomeDirectory());
				printf("\n================Opening Visual Studio===============\n");
				if(monoWrapper::exec_cmd(buffer, responsebuffer)==0)
				{
					printf(responsebuffer);
					printf("\nSuccess\n");
				}
			}
			//m_pButtonOpenOnEditor->buttonNormal();
		}
	}
}