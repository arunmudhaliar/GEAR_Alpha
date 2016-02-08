#pragma once

#include "object3d.h"
#include <vector>
#include <algorithm>

#define MAX_LAYER	32

class DECLSPEC Layer
{
private:
	Layer(){}
public:
	Layer(const char* name);
	~Layer();

	void setLayerName(const char* name);
	const char* getLayerName();
	std::vector<object3d*>* getLayerList();
	bool append(object3d* obj);
	bool remove(object3d* obj);

	void clear();

private:
	std::string m_cName;
	std::vector<object3d*> m_vList;
};

class DECLSPEC LayerManager
{
public:
	LayerManager();
	~LayerManager();

	void clearLayers();
	bool appendOnLayer(object3d* obj, int layer);
	bool removeFromLayer(object3d* obj, int layer);

	Layer* getLayer(int layer)	{	return m_pszLayers[layer];	}

	int getTotalLayerObject();

private:
	Layer* m_pszLayers[MAX_LAYER];
};