#include "gePropertyTransform.h"
#include "../../EditorApp.h"

gePropertyTransform::gePropertyTransform(rendererGL10* renderer, geGUIBase* parent, const char* name, Sprite2Dx* sprite, geFontManager* fontManager):
	geTreeNode(renderer, parent, name, sprite, fontManager, 10)
{
	m_pObject3dPtr=NULL;
	setSize(m_cSize.x, 85.0f);

	//geButton* btn=new geButton();
	//btn->create(this, "button1", 40, 10);

	//gePushButton* pbtn = new gePushButton("");
	//pbtn->create(this, "", 10, 10);

	m_pszTextBoxTranslation[0] = new geTextBox("X", fontManager);
	m_pszTextBoxTranslation[0]->create(renderer, this, "0.0", 30, 10, 60, 16);
	m_pszTextBoxTranslation[0]->setGUIObserver(this);
	m_pszTextBoxTranslation[1] = new geTextBox("Y", fontManager);
	m_pszTextBoxTranslation[1]->create(renderer, this, "0.0", 105, 10, 60, 16);
	m_pszTextBoxTranslation[1]->setGUIObserver(this);
	m_pszTextBoxTranslation[2] = new geTextBox("Z", fontManager);
	m_pszTextBoxTranslation[2]->create(renderer, this, "0.0", 180, 10, 60, 16);
	m_pszTextBoxTranslation[2]->setGUIObserver(this);

	m_pszTextBoxRotation[0] = new geTextBox("X", fontManager);
	m_pszTextBoxRotation[0]->create(renderer, this, "0.0", 30, 30, 60, 16);
	m_pszTextBoxRotation[0]->setGUIObserver(this);
	m_pszTextBoxRotation[1] = new geTextBox("Y", fontManager);
	m_pszTextBoxRotation[1]->create(renderer, this, "0.0", 105, 30, 60, 16);
	m_pszTextBoxRotation[1]->setGUIObserver(this);
	m_pszTextBoxRotation[2] = new geTextBox("Z", fontManager);
	m_pszTextBoxRotation[2]->create(renderer, this, "0.0", 180, 30, 60, 16);
	m_pszTextBoxRotation[2]->setGUIObserver(this);

	m_pszTextBoxScale[0] = new geTextBox("X", fontManager);
	m_pszTextBoxScale[0]->create(renderer, this, "0.0", 30, 50, 60, 16);
	m_pszTextBoxScale[0]->setGUIObserver(this);
	m_pszTextBoxScale[1] = new geTextBox("Y", fontManager);
	m_pszTextBoxScale[1]->create(renderer, this, "0.0", 105, 50, 60, 16);
	m_pszTextBoxScale[1]->setGUIObserver(this);
	m_pszTextBoxScale[2] = new geTextBox("Z", fontManager);
	m_pszTextBoxScale[2]->create(renderer, this, "0.0", 180, 50, 60, 16);
	m_pszTextBoxScale[2]->setGUIObserver(this);


	//window column
	geWindowColumn* pWindowColumn = new geWindowColumn(fontManager);
	pWindowColumn->create(rendererGUI, this, 10, 350.0f, 10.0f, 0.07f);
	stWindowColumnRow* row = pWindowColumn->addRow("T");
	pWindowColumn->addControl(row, m_pszTextBoxTranslation[0], 18.0f);
	pWindowColumn->addControl(row, m_pszTextBoxTranslation[1], 18.0f);
	pWindowColumn->addControl(row, m_pszTextBoxTranslation[2], 18.0f);

	row = pWindowColumn->addRow("R");
	pWindowColumn->addControl(row, m_pszTextBoxRotation[0], 18.0f);
	pWindowColumn->addControl(row, m_pszTextBoxRotation[1], 18.0f);
	pWindowColumn->addControl(row, m_pszTextBoxRotation[2], 18.0f);

	row = pWindowColumn->addRow("S");
	pWindowColumn->addControl(row, m_pszTextBoxScale[0], 18.0f);
	pWindowColumn->addControl(row, m_pszTextBoxScale[1], 18.0f);
	pWindowColumn->addControl(row, m_pszTextBoxScale[2], 18.0f);
	//

	setNodeColor(0.21f, 0.21f, 0.21f);
	setNodeSelectionColor(0.21f, 0.21f, 0.21f);
	setClientAreaPrimaryActiveForeColor(0.21f, 0.21f, 0.21f, 1.0f);
	applyPrimaryColorToVBClientArea();
}

gePropertyTransform::~gePropertyTransform()
{
}

void gePropertyTransform::drawNode()
{
	drawRect(&vertexBufferClientArea);

	geFontManager::g_pFontArial10_84Ptr->drawString(m_szName, 35, geFontManager::g_pFontArial10_84Ptr->getLineHeight(), m_cSize.x);

	if(childControlList.size() && hasAtleastOneTreeNodeChild)
	{
		if(is_OpenNode)
			drawTriangle(&vertexBufferToggleButtonArray[3*2], 0.3f, 0.3f, 0.3f, 1.0f, 3);
		else
			drawTriangle(&vertexBufferToggleButtonArray[0], 0.3f, 0.3f, 0.3f, 1.0f, 3);
	}

	for(std::vector<geGUIBase*>::iterator it = childControlList.begin(); it != childControlList.end(); ++it)
	{
		geGUIBase* tvnode = *it;
		tvnode->draw();
	}

	if(sprite)
		sprite->draw();
}

void gePropertyTransform::onTVSelectionChange(geTreeNode* tvnode, geTreeView* treeview)
{

}

void gePropertyTransform::populatePropertyOfTransform(object3d* obj)
{
	m_pObject3dPtr=obj;

	updatePropertyView();
}

void gePropertyTransform::onTextChange(geGUIBase* textbox)
{
	geTextBox* currentTextBox=(geTextBox*)textbox;

	float value=geUtil::getFloat(currentTextBox->getName());
	//geTextBox* m_pszTextBoxTranslation[3];
	//geTextBox* m_pszTextBoxRotation[3];
	//geTextBox* m_pszTextBoxScale[3];
	const float* mat=m_pObject3dPtr->getMatrix();

	if(textbox==m_pszTextBoxTranslation[0])
		m_pObject3dPtr->setPosition(value, mat[13], mat[14]);
	else if(textbox==m_pszTextBoxTranslation[1])
		m_pObject3dPtr->setPosition(mat[12], value, mat[14]);
	else if(textbox==m_pszTextBoxTranslation[2])
		m_pObject3dPtr->setPosition(mat[12], mat[13], value);

	else if(textbox==m_pszTextBoxScale[0])
		m_pObject3dPtr->setScale(value, mat[5], mat[10]);
	else if(textbox==m_pszTextBoxScale[1])
		m_pObject3dPtr->setScale(mat[0], value, mat[10]);
	else if(textbox==m_pszTextBoxScale[2])
		m_pObject3dPtr->setScale(mat[0], mat[5], value);

}

void gePropertyTransform::updatePropertyView()
{
	if(m_pObject3dPtr==NULL) return;

	char buffer[32];
	sprintf(buffer, "%12.4f", m_pObject3dPtr->getMatrix()[12]);
	m_pszTextBoxTranslation[0]->setName(buffer);
	sprintf(buffer, "%12.4f", m_pObject3dPtr->getMatrix()[13]);
	m_pszTextBoxTranslation[1]->setName(buffer);
	sprintf(buffer, "%12.4f", m_pObject3dPtr->getMatrix()[14]);
	m_pszTextBoxTranslation[2]->setName(buffer);

	sprintf(buffer, "%12.4f", m_pObject3dPtr->getMatrix()[0]);
	m_pszTextBoxScale[0]->setName(buffer);
	sprintf(buffer, "%12.4f", m_pObject3dPtr->getMatrix()[5]);
	m_pszTextBoxScale[1]->setName(buffer);
	sprintf(buffer, "%12.4f", m_pObject3dPtr->getMatrix()[10]);
	m_pszTextBoxScale[2]->setName(buffer);

	float Yaw;
	float Pitch;
	float Roll;

	matrix4x4f tm((matrix4x4f)*m_pObject3dPtr);
	tm.noScale();

	Quaternion quat;
	quat=Quaternion::fromRotationMatrix(tm.getMatrix());
	quat.getEuler(Yaw, Pitch, Roll);

	//Quaternion aa= quat.getQuaternion(Yaw, Pitch, Roll);
	//getRotation(Yaw, Pitch, Roll, obj->getMatrix());

	sprintf(buffer, "%12.4f", Pitch);
	m_pszTextBoxRotation[0]->setName(buffer);
	sprintf(buffer, "%12.4f", Yaw);
	m_pszTextBoxRotation[1]->setName(buffer);
	sprintf(buffer, "%12.4f", Roll);
	m_pszTextBoxRotation[2]->setName(buffer);
}