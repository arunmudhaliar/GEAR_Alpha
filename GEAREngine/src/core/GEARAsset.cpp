#include "GEARAsset.h"

GEARAsset::GEARAsset()
{
	m_iAssetFileCRC=0;
	std::string m_cAssetFileName;
}

GEARAsset::~GEARAsset()
{
}

void GEARAsset::setAssetFileCRC(unsigned int crc, std::string filename)
{
	m_iAssetFileCRC=crc;
	m_cAssetFileName = filename;
}

unsigned int GEARAsset::getAssetFileCRC()
{
	return m_iAssetFileCRC;
}

std::string GEARAsset::getAssetFileName()
{
	return m_cAssetFileName;
}