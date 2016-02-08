#pragma once

#include "geGUIBase.h"

struct stWindowColumnRow
{
	stWindowColumnRow(const char* name, float ypos);
	~stWindowColumnRow();
	void addControl(geGUIBase* cntrl, float override_height);
	void setHeight(float h);
	float getHeight();
	float getYPoistion()	{	return yposition;	}
	std::vector<geGUIBase*>* getControlList()	{	return &cControls;	}
	const char* getName()	{	return cName.c_str();	}
	void setName(const char* name);
private:
	std::string cName;
	std::vector<geGUIBase*> cControls;
	float yposition;
	float height;
};

class geWindowColumn : public geGUIBase
{
public:
	geWindowColumn(geFontManager* fontmanager);
	virtual ~geWindowColumn();

	void create(rendererGL10* renderer, geGUIBase* parent, int y, float minimum_coloumn_width, float minimum_space_between_controls, float ratio=0.5f);
	stWindowColumnRow* addRow(const char* name);
	void addControl(stWindowColumnRow* row, geGUIBase* cntrl, float override_height=0.0f);

	std::vector<stWindowColumnRow*>* getRowList()	{ return &m_vRow; }

	virtual void draw();
protected:
	virtual void onPosition(float x, float y, int flag);
	virtual void onSize(float cx, float cy, int flag);

private:
	std::vector<stWindowColumnRow*> m_vRow;
	float m_cszColumnRatio;
	float m_fHeight;
	float m_fMinimumColumnWidth;
	float m_fMinimumSpaceBetweenControls;
};