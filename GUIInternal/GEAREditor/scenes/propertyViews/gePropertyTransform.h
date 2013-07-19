#ifndef GEPROPERTYTRANSFORM_H
#define GEPROPERTYTRANSFORM_H

#include "../../gui/geTreeView.h"
#include "../../gui/geButton.h"
#include "../../gui/gePushButton.h"
#include "../../gui/geTextBox.h"

class gePropertyTransform : public geTreeNode, public MGUIObserver
{
public:

	geTextBox* m_pszTextBoxTranslation[3];
	geTextBox* m_pszTextBoxRotation[3];
	geTextBox* m_pszTextBoxScale[3];
	object3d* m_pObject3dPtr;

	gePropertyTransform(geGUIBase* parent, const char* name, Sprite2Dx* sprite):
	  geTreeNode(parent, name, sprite, 10)
	{
		m_pObject3dPtr=NULL;
		setSize(m_cSize.x, 100.0f);

		//geButton* btn=new geButton();
		//btn->create(this, "button1", 40, 10);

		//gePushButton* pbtn = new gePushButton("");
		//pbtn->create(this, "", 10, 10);

		m_pszTextBoxTranslation[0] = new geTextBox("X");
		m_pszTextBoxTranslation[0]->create(this, "0.0", 30, 10, 60, 16);
		m_pszTextBoxTranslation[0]->setGUIObserver(this);
		m_pszTextBoxTranslation[1] = new geTextBox("Y");
		m_pszTextBoxTranslation[1]->create(this, "0.0", 105, 10, 60, 16);
		m_pszTextBoxTranslation[1]->setGUIObserver(this);
		m_pszTextBoxTranslation[2] = new geTextBox("Z");
		m_pszTextBoxTranslation[2]->create(this, "0.0", 180, 10, 60, 16);
		m_pszTextBoxTranslation[2]->setGUIObserver(this);

		m_pszTextBoxRotation[0] = new geTextBox("X");
		m_pszTextBoxRotation[0]->create(this, "0.0", 30, 30, 60, 16);
		m_pszTextBoxRotation[0]->setGUIObserver(this);
		m_pszTextBoxRotation[1] = new geTextBox("Y");
		m_pszTextBoxRotation[1]->create(this, "0.0", 105, 30, 60, 16);
		m_pszTextBoxRotation[1]->setGUIObserver(this);
		m_pszTextBoxRotation[2] = new geTextBox("Z");
		m_pszTextBoxRotation[2]->create(this, "0.0", 180, 30, 60, 16);
		m_pszTextBoxRotation[2]->setGUIObserver(this);

		m_pszTextBoxScale[0] = new geTextBox("X");
		m_pszTextBoxScale[0]->create(this, "0.0", 30, 50, 60, 16);
		m_pszTextBoxScale[0]->setGUIObserver(this);
		m_pszTextBoxScale[1] = new geTextBox("Y");
		m_pszTextBoxScale[1]->create(this, "0.0", 105, 50, 60, 16);
		m_pszTextBoxScale[1]->setGUIObserver(this);
		m_pszTextBoxScale[2] = new geTextBox("Z");
		m_pszTextBoxScale[2]->create(this, "0.0", 180, 50, 60, 16);
		m_pszTextBoxScale[2]->setGUIObserver(this);


		setNodeColor(0.21f, 0.21f, 0.21f);
		setNodeSelectionColor(0.21f, 0.21f, 0.21f);
		setColor(&m_cVBClientArea, 0.21f, 0.21f, 0.21f, 1.0f);
	}

	virtual ~gePropertyTransform()
	{
	}

	//virtual void draw();
	virtual void drawNode()
	{
		drawRect(&m_cVBClientArea);

		geGUIManager::g_pFontArial12Ptr->drawString(m_szName, 35, geGUIManager::g_pFontArial12Ptr->getLineHeight(), m_cSize.x);

		if(m_vControls.size() && m_bHaveAtleastOneTreeNodeChild)
		{
			if(m_bNodeOpen)
				drawTriangle(&m_cVBLayoutToggleButtonLine[3*2], 0.3f, 0.3f, 0.3f, 1.0f, 3);
			else
				drawTriangle(&m_cVBLayoutToggleButtonLine[0], 0.3f, 0.3f, 0.3f, 1.0f, 3);
		}

		for(std::vector<geGUIBase*>::iterator it = m_vControls.begin(); it != m_vControls.end(); ++it)
		{
			geGUIBase* tvnode = *it;
			tvnode->draw();
		}

		if(m_pSprite)
			m_pSprite->draw();
	}

	virtual void onTVSelectionChange(geTreeNode* tvnode, geTreeView* treeview)
	{

	}

	void populatePropertyOfTransform(object3d* obj)
	{
		m_pObject3dPtr=obj;
		char buffer[9];
		sprintf(buffer, "%6.2f", obj->getMatrix()[12]);
		m_pszTextBoxTranslation[0]->setName(buffer);
		sprintf(buffer, "%6.2f", obj->getMatrix()[13]);
		m_pszTextBoxTranslation[1]->setName(buffer);
		sprintf(buffer, "%6.2f", obj->getMatrix()[14]);
		m_pszTextBoxTranslation[2]->setName(buffer);

		sprintf(buffer, "%6.2f", obj->getMatrix()[0]);
		m_pszTextBoxScale[0]->setName(buffer);
		sprintf(buffer, "%6.2f", obj->getMatrix()[5]);
		m_pszTextBoxScale[1]->setName(buffer);
		sprintf(buffer, "%6.2f", obj->getMatrix()[10]);
		m_pszTextBoxScale[2]->setName(buffer);

		float Yaw;
		float Pitch;
		float Roll;

		getRotation(Yaw, Pitch, Roll, obj->getMatrix());

		sprintf(buffer, "%6.2f", Pitch);
		m_pszTextBoxRotation[0]->setName(buffer);
		sprintf(buffer, "%6.2f", Roll);
		m_pszTextBoxRotation[1]->setName(buffer);
		sprintf(buffer, "%6.2f", Yaw);
		m_pszTextBoxRotation[2]->setName(buffer);
	}

	void onTextChange(geGUIBase* textbox)
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

	void getRotation(float& Yaw, float& Pitch, float& Roll, const float* m) const
    {
		/*
		   union {
        struct 
        {
            float        _11, _12, _13, _14;
            float        _21, _22, _23, _24;
            float        _31, _32, _33, _34;
            float        _41, _42, _43, _44;
        };
        float m[4][4];
        float m2[16];
    };
	*/
        if (m[0] == 1.0f)
        {
            Yaw = atan2f(m[2], m[11]);
            Pitch = 0;
            Roll = 0;

        }else if (m[0] == -1.0f)
        {
            Yaw = atan2f(m[2], m[11]);
            Pitch = 0;
            Roll = 0;
        }else 
        {

            Yaw = atan2(-m[8],m[0]);
            Pitch = asin(m[4]);
            Roll = atan2(-m[6],m[5]);
        }
    }
};

#endif