#include "gePropertyOpenOnEditor.h"
#include "../../EditorApp.h"

gePropertyOpenOnEditor::gePropertyOpenOnEditor(rendererGL10* renderer, geGUIBase* parent, const char* name, Sprite2Dx* sprite, geFontManager* fontmanager):
	geTreeNode(renderer, parent, name, sprite, fontmanager, 10)
{
	setSize(m_cSize.x, 70.0f);

	openOnEditorButton = new geButton("", fontmanager);
	openOnEditorButton->create(renderer, this, "Open in editor", 15, 20);
	openOnEditorButton->setGUIObserver(this);

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
	drawRect(&vertexBufferClientArea);

	for(std::vector<geGUIBase*>::iterator it = childControlList.begin(); it != childControlList.end(); ++it)
	{
		geGUIBase* tvnode = *it;
		tvnode->draw();
	}

	if(sprite)
		sprite->draw();
}

void gePropertyOpenOnEditor::onButtonClicked(geGUIBase* btn)
{
	if(btn==openOnEditorButton)
	{
		if(openOnEditorButton->isButtonPressed())
		{
			char buffer[2048];
			char responsebuffer[4096];
			//GetCurrentDirectory(sizeof(buffer), buffer);
			memset(buffer, 0, sizeof(buffer));
			memset(responsebuffer, 0, sizeof(responsebuffer));
#if GEAR_WINDOWS
#ifdef GEAR_DEBUG
			sprintf(buffer, "%s\\CSharpProjectMaker.exe gearProject %s %s\\Debug\\MonoGEAR.dll", EditorGEARApp::getAppDirectory().c_str(), EditorGEARApp::getProjectHomeDirectory(), EditorGEARApp::getAppDirectory().c_str());
#else
			sprintf(buffer, "%s\\CSharpProjectMaker.exe gearProject %s %s\\Release\\MonoGEAR.dll", EditorGEARApp::getAppDirectory().c_str(), EditorGEARApp::getProjectHomeDirectory(), EditorGEARApp::getAppDirectory().c_str());
#endif
#else
#ifdef GEAR_DEBUG
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
#if GEAR_WINDOWS
				sprintf(buffer, "start %s/gearProject.sln", EditorGEARApp::getProjectHomeDirectory());
#elif GEAR_APPLE
                //open -n /Applications/MonoDevelop.app
                sprintf(buffer, "open -n /Applications/Unity/MonoDevelop.app %s/gearProject.sln", EditorGEARApp::getProjectHomeDirectory());
#else
#error not implemented
#endif
				printf("\n================Opening Visual Studio===============\n");
				if(monoWrapper::exec_cmd(buffer, responsebuffer)==0)
				{
					printf("%s", responsebuffer);
					printf("\nSuccess\n");
				}
			}
			//openOnEditorButton->buttonNormal();
		}
	}
}