#include "gePropertyTransform.h"
#include "../../EditorApp.h"

gePropertyTransform::gePropertyTransform(rendererGL10* renderer, geGUIBase* parent, const char* name, Sprite2Dx* sprite, geFontManager* fontManager):
	geTreeNode(renderer, parent, name, sprite, fontManager, 10)
{
	object3dPtr=NULL;
	setSize(m_cSize.x, 85.0f);

	//geButton* btn=new geButton();
	//btn->create(this, "button1", 40, 10);

	//gePushButton* pbtn = new gePushButton("");
	//pbtn->create(this, "", 10, 10);

	translationTextBox[0] = new geTextBox("X", fontManager);
	translationTextBox[0]->create(renderer, this, "0.0", 30, 10, 60, 16);
	translationTextBox[0]->setGUIObserver(this);
	translationTextBox[1] = new geTextBox("Y", fontManager);
	translationTextBox[1]->create(renderer, this, "0.0", 105, 10, 60, 16);
	translationTextBox[1]->setGUIObserver(this);
	translationTextBox[2] = new geTextBox("Z", fontManager);
	translationTextBox[2]->create(renderer, this, "0.0", 180, 10, 60, 16);
	translationTextBox[2]->setGUIObserver(this);

	rotationTextBox[0] = new geTextBox("X", fontManager);
	rotationTextBox[0]->create(renderer, this, "0.0", 30, 30, 60, 16);
	rotationTextBox[0]->setGUIObserver(this);
	rotationTextBox[1] = new geTextBox("Y", fontManager);
	rotationTextBox[1]->create(renderer, this, "0.0", 105, 30, 60, 16);
	rotationTextBox[1]->setGUIObserver(this);
	rotationTextBox[2] = new geTextBox("Z", fontManager);
	rotationTextBox[2]->create(renderer, this, "0.0", 180, 30, 60, 16);
	rotationTextBox[2]->setGUIObserver(this);

	scaleTextBox[0] = new geTextBox("X", fontManager);
	scaleTextBox[0]->create(renderer, this, "0.0", 30, 50, 60, 16);
	scaleTextBox[0]->setGUIObserver(this);
	scaleTextBox[1] = new geTextBox("Y", fontManager);
	scaleTextBox[1]->create(renderer, this, "0.0", 105, 50, 60, 16);
	scaleTextBox[1]->setGUIObserver(this);
	scaleTextBox[2] = new geTextBox("Z", fontManager);
	scaleTextBox[2]->create(renderer, this, "0.0", 180, 50, 60, 16);
	scaleTextBox[2]->setGUIObserver(this);


	//window column
	geWindowColumn* pWindowColumn = new geWindowColumn(fontManager);
	pWindowColumn->create(rendererGUI, this, 10, 350.0f, 10.0f, 0.07f);
	stWindowColumnRow* row = pWindowColumn->addRow("T");
	pWindowColumn->addControl(row, translationTextBox[0], 18.0f);
	pWindowColumn->addControl(row, translationTextBox[1], 18.0f);
	pWindowColumn->addControl(row, translationTextBox[2], 18.0f);

	row = pWindowColumn->addRow("R");
	pWindowColumn->addControl(row, rotationTextBox[0], 18.0f);
	pWindowColumn->addControl(row, rotationTextBox[1], 18.0f);
	pWindowColumn->addControl(row, rotationTextBox[2], 18.0f);

	row = pWindowColumn->addRow("S");
	pWindowColumn->addControl(row, scaleTextBox[0], 18.0f);
	pWindowColumn->addControl(row, scaleTextBox[1], 18.0f);
	pWindowColumn->addControl(row, scaleTextBox[2], 18.0f);
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
	object3dPtr=obj;

	updatePropertyView();
}

void gePropertyTransform::onTextChange(geGUIBase* textbox)
{
	geTextBox* currentTextBox=(geTextBox*)textbox;

	float value=geUtil::getFloat(currentTextBox->getName());
	//geTextBox* translationTextBox[3];
	//geTextBox* rotationTextBox[3];
	//geTextBox* scaleTextBox[3];
	const float* mat=object3dPtr->getMatrix();

	if(textbox==translationTextBox[0])
		object3dPtr->setPosition(value, mat[13], mat[14]);
	else if(textbox==translationTextBox[1])
		object3dPtr->setPosition(mat[12], value, mat[14]);
	else if(textbox==translationTextBox[2])
		object3dPtr->setPosition(mat[12], mat[13], value);

	else if(textbox==scaleTextBox[0])
		object3dPtr->setScale(value, mat[5], mat[10]);
	else if(textbox==scaleTextBox[1])
		object3dPtr->setScale(mat[0], value, mat[10]);
	else if(textbox==scaleTextBox[2])
		object3dPtr->setScale(mat[0], mat[5], value);

}

void gePropertyTransform::updatePropertyView()
{
	if(object3dPtr==NULL) return;

	char buffer[32];
	sprintf(buffer, "%12.4f", object3dPtr->getMatrix()[12]);
	translationTextBox[0]->setName(buffer);
	sprintf(buffer, "%12.4f", object3dPtr->getMatrix()[13]);
	translationTextBox[1]->setName(buffer);
	sprintf(buffer, "%12.4f", object3dPtr->getMatrix()[14]);
	translationTextBox[2]->setName(buffer);

	sprintf(buffer, "%12.4f", object3dPtr->getMatrix()[0]);
	scaleTextBox[0]->setName(buffer);
	sprintf(buffer, "%12.4f", object3dPtr->getMatrix()[5]);
	scaleTextBox[1]->setName(buffer);
	sprintf(buffer, "%12.4f", object3dPtr->getMatrix()[10]);
	scaleTextBox[2]->setName(buffer);

	float Yaw;
	float Pitch;
	float Roll;

	matrix4x4f tm((matrix4x4f)*object3dPtr);
	tm.noScale();

	Quaternion quat;
	quat=Quaternion::fromRotationMatrix(tm.getMatrix());
	quat.getEuler(Yaw, Pitch, Roll);

	//Quaternion aa= quat.getQuaternion(Yaw, Pitch, Roll);
	//getRotation(Yaw, Pitch, Roll, obj->getMatrix());

	sprintf(buffer, "%12.4f", Pitch);
	rotationTextBox[0]->setName(buffer);
	sprintf(buffer, "%12.4f", Yaw);
	rotationTextBox[1]->setName(buffer);
	sprintf(buffer, "%12.4f", Roll);
	rotationTextBox[2]->setName(buffer);
}