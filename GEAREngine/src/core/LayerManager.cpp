#include "LayerManager.h"

Layer::Layer(const char* name)
{
	m_cName.assign(name);
}

Layer::~Layer()
{
	clear();
}

const char* Layer::getLayerName()
{
	return m_cName.c_str();
}

std::vector<object3d*>* Layer::getLayerList()
{
	return &m_vList;
}

bool Layer::append(object3d* obj)
{
	m_vList.push_back(obj);
	return true;
}

bool Layer::remove(object3d* obj)
{
	if(m_vList.size()==0)
		return false;
	m_vList.erase(std::remove(m_vList.begin(), m_vList.end(), obj), m_vList.end());
	return true;
}

void Layer::clear()
{
	m_vList.clear();
}

////////////////////////////////////////////////////////
////////////////////////////////////////////////////////
////////////////////////////////////////////////////////

LayerManager::LayerManager()
{
	//memset(m_pszLayers, 0, sizeof(Layer)*32);
	for(int x=0;x<MAX_LAYER;x++)
	{
		char buffer[32];
		sprintf(buffer, "Layer %d", x);
		m_pszLayers[x] = new Layer(buffer);
	}
}

LayerManager::~LayerManager()
{
	for(int x=0;x<MAX_LAYER;x++)
	{
		GX_DELETE(m_pszLayers[x]);
	}
}

void LayerManager::clearLayers()
{
	for(int x=0;x<MAX_LAYER;x++)
	{
		m_pszLayers[x]->clear();
	}
}

bool LayerManager::appendOnLayer(object3d* obj, int layer)
{
	if(layer>=MAX_LAYER)
		return false;
	if(obj==NULL)
		return false;

	return m_pszLayers[layer]->append(obj);
}

bool LayerManager::removeFromLayer(object3d* obj, int layer)
{
	if(layer>=MAX_LAYER)
		return false;
	if(obj==NULL)
		return false;

	return m_pszLayers[layer]->remove(obj);
}

int LayerManager::getTotalLayerObject()
{
	int nCount=0;
	for(int x=0;x<MAX_LAYER;x++)
	{
		nCount+=m_pszLayers[x]->getLayerList()->size();
	}

	return nCount;
}