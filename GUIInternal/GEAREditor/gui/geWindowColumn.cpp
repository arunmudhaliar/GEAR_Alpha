#include "geWindowColumn.h"
#ifndef GEAR2D
#include "../EditorApp.h"
#else
#include "../../../../GEAR2D_Alpha/GEAR2D/app/Editor2DApp.h"
#endif

stWindowColumnRow::stWindowColumnRow(const char* name, float ypos)
{
	cName.assign(name);
	height=0.0f;
	yposition=ypos;
}

stWindowColumnRow::~stWindowColumnRow()
{
	cControls.clear();
	cName.clear();
}

void stWindowColumnRow::addControl(geGUIBase* cntrl, float override_height)
{
	cControls.push_back(cntrl);
	if(override_height>0.0f)
	{
		if(height<override_height)
			height=override_height;
	}
	else
	{
		if(height<cntrl->getSize().y)
			height=cntrl->getSize().y;
	}
}

void stWindowColumnRow::setHeight(float h)
{
	height=h;
}

float stWindowColumnRow::getHeight()
{
	return height;
}

void stWindowColumnRow::setName(const char* name)
{
	cName.assign(name);
}

//=====================================================================
geWindowColumn::geWindowColumn(geFontManager* fontmanager):
geGUIBase(GEGUI_WINDOW_COLUMN, "window column", fontmanager)
{
	m_cszColumnRatio=0.5f;
}

geWindowColumn::~geWindowColumn()
{
	for(std::vector<stWindowColumnRow*>::iterator it = m_vRow.begin(); it != m_vRow.end(); ++it)
	{
		stWindowColumnRow* obj = *it;
		GE_DELETE(obj);
	}
	m_vRow.clear();
}

void geWindowColumn::create(rendererGL10* renderer, geGUIBase* parent, int y, float minimum_coloumn_width, float minimum_space_between_controls, float ratio)
{
	createBase(renderer, parent);
	//m_pRenderer=renderer;
	m_fMinimumColumnWidth=minimum_coloumn_width;
	m_fMinimumSpaceBetweenControls=minimum_space_between_controls;

	setPos(0, y);
	setSize(parent->getSize().x, 1);
	setSizable(true);
	setMouseBoundCheck(false);

	m_cszColumnRatio=ratio;
}

stWindowColumnRow* geWindowColumn::addRow(const char* name)
{
	float ypos=m_cPos.y;
	for(std::vector<stWindowColumnRow*>::iterator it = m_vRow.begin(); it != m_vRow.end(); ++it)
	{
		stWindowColumnRow* obj = *it;
		ypos+=obj->getHeight();
	}

	stWindowColumnRow* newrow = new stWindowColumnRow(name, ypos);
	m_vRow.push_back(newrow);

	return newrow;
}

void geWindowColumn::addControl(stWindowColumnRow* row, geGUIBase* cntrl, float override_height)
{
	float start_xpos=m_cPos.x + m_cSize.x*m_cszColumnRatio;
	std::vector<geGUIBase*>* controllist=row->getControlList();
	for(std::vector<geGUIBase*>::iterator it = controllist->begin(); it != controllist->end(); ++it)
	{
		geGUIBase* obj = *it;
		start_xpos+=(m_fMinimumSpaceBetweenControls + obj->getSize().x);
	}

	//float control_y=cntrl->getPos().y;
	cntrl->setPos(start_xpos, row->getYPoistion());
	row->addControl(cntrl, override_height);
}

void geWindowColumn::draw()
{
	for(std::vector<stWindowColumnRow*>::iterator it = m_vRow.begin(); it != m_vRow.end(); ++it)
	{
		stWindowColumnRow* row = *it;
		
		geFontManager::g_pFontArial10_80Ptr->drawString(row->getName(), 10.0f, row->getYPoistion()/*+row->getHeight()*/+geFontManager::g_pFontArial10_80Ptr->getLineHeight()*0.5f, m_cSize.x);
	}
}

void geWindowColumn::onPosition(float x, float y, int flag)
{
}

void geWindowColumn::onSize(float cx, float cy, int flag)
{
	float calc_cx=cx;
	if(m_fMinimumColumnWidth>cx)
	{
		calc_cx=m_fMinimumColumnWidth;
	}
	for(std::vector<stWindowColumnRow*>::iterator it = m_vRow.begin(); it != m_vRow.end(); ++it)
	{
		stWindowColumnRow* row = *it;
		float start_xpos=m_cPos.x + calc_cx*m_cszColumnRatio;
		std::vector<geGUIBase*>* controllist=row->getControlList();
		for(std::vector<geGUIBase*>::iterator it = controllist->begin(); it != controllist->end(); ++it)
		{
			geGUIBase* cntrl = *it;
			cntrl->setPos(start_xpos, row->getYPoistion());
			start_xpos+=(m_fMinimumSpaceBetweenControls + cntrl->getSize().x);
		}

		//ypos+=row->getHeight();
	}


	geGUIBase::onSize(cx, cy, flag);
}