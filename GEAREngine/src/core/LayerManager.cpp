#include "LayerManager.h"

Layer::Layer(const char* name)
{
	m_cName.assign(name);
}

Layer::~Layer()
{
	clear();
}

void Layer::setLayerName(const char* name)
{
	m_cName.assign(name);
}

const char* Layer::getLayerName()
{
	return m_cName.c_str();
}

std::vector<object3d*>* Layer::getLayerList()
{
	return &layerObjects;
}

bool Layer::append(object3d* obj)
{
	layerObjects.push_back(obj);
	return true;
}

bool Layer::remove(object3d* obj)
{
	if(layerObjects.size()==0)
		return false;
	layerObjects.erase(std::remove(layerObjects.begin(), layerObjects.end(), obj), layerObjects.end());
	return true;
}

void Layer::clear()
{
	layerObjects.clear();
}

////////////////////////////////////////////////////////
////////////////////////////////////////////////////////
////////////////////////////////////////////////////////

LayerManager::LayerManager()
{
	//memset(layerList, 0, sizeof(Layer)*32);
	for(int x=0;x<MAX_LAYER;x++)
	{
		char buffer[32];
		sprintf(buffer, "Layer %d", x);
		layerList[x] = new Layer(buffer);
	}
}

LayerManager::~LayerManager()
{
	for(int x=0;x<MAX_LAYER;x++)
	{
		GX_DELETE(layerList[x]);
	}
}

void LayerManager::clearLayers()
{
	for(int x=0;x<MAX_LAYER;x++)
	{
		layerList[x]->clear();
	}
}

bool LayerManager::appendOnLayer(object3d* obj, int layer)
{
	if(layer>=MAX_LAYER)
		return false;
	if(obj==NULL)
		return false;

	return layerList[layer]->append(obj);
}

bool LayerManager::removeFromLayer(object3d* obj, int layer)
{
	if(layer>=MAX_LAYER)
		return false;
	if(obj==NULL)
		return false;

	return layerList[layer]->remove(obj);
}

int LayerManager::getTotalLayerObject()
{
	int nCount=0;
	for(int x=0;x<MAX_LAYER;x++)
	{
		nCount+=layerList[x]->getLayerList()->size();
	}

	return nCount;
}