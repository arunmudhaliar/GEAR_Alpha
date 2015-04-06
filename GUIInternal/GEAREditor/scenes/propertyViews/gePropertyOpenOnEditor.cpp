#include "gePropertyOpenOnEditor.h"
#include "../../EditorApp.h"

gePropertyOpenOnEditor::gePropertyOpenOnEditor(rendererGL10* renderer, geGUIBase* parent, const char* name, Sprite2Dx* sprite, geFontManager* fontmanager):
	geTreeNode(renderer, parent, name, sprite, fontmanager, 10)
{
	setSize(m_cSize.x, 70.0f);

	m_pButtonOpenOnEditor = new geButton("", fontmanager);
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
			char buffer[2048];
			char responsebuffer[4096];
			//GetCurrentDirectory(sizeof(buffer), buffer);
			memset(buffer, 0, sizeof(buffer));
			memset(responsebuffer, 0, sizeof(responsebuffer));
#if _WIN32
#ifdef _DEBUG
			sprintf(buffer, "%s\\CSharpProjectMaker.exe gearProject %s %s\\Debug\\MonoGEAR.dll", EditorGEARApp::getAppDirectory().c_str(), EditorGEARApp::getProjectHomeDirectory(), EditorGEARApp::getAppDirectory().c_str());
#else
			sprintf(buffer, "%s\\CSharpProjectMaker.exe gearProject %s %s\\Release\\MonoGEAR.dll", EditorGEARApp::getAppDirectory().c_str(), EditorGEARApp::getProjectHomeDirectory(), EditorGEARApp::getAppDirectory().c_str());
#endif
#else
#ifdef _DEBUG
            sprintf(buffer, "mono %s/CSharpProjectMaker.exe gearProject %s %s/MonoGEAR.dll", EditorGEARApp::getAppDirectory().c_str(), EditorGEARApp::getProjectHomeDirectory(), EditorGEARApp::getAppDirectory().c_str());
#else
            sprintf(buffer, "mono %s/CSharpProjectMaker.exe gearProject %s %s/MonoGEAR.dll", EditorGEARApp::getAppDirectory().c_str(), EditorGEARApp::getProjectHomeDirectory(), EditorGEARApp::getAppDirectory().c_str());
#endif
#endif
			printf("\n================Creating Visual Studio Project===============\n");
			if(monoWrapper::exec_cmd(buffer, responsebuffer)==0)
			{
				memset(buffer, 0, sizeof(buffer));
				memset(responsebuffer, 0, sizeof(responsebuffer));
#if _WIN32
				sprintf(buffer, "start %s/gearProject.sln", EditorGEARApp::getProjectHomeDirectory());
#elif __APPLE__
                //open -n /Applications/MonoDevelop.app
                sprintf(buffer, "open -n /Applications/Unity/MonoDevelop.app %s/gearProject.sln", EditorGEARApp::getProjectHomeDirectory());
#else
#error not implemented
#endif
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