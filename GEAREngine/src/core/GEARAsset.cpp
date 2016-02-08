#include "GEARAsset.h"

GEARAsset::GEARAsset()
{
	assetFileCRC=0;
	std::string assetFileName;
}

GEARAsset::~GEARAsset()
{
}

void GEARAsset::setAssetFileCRC(unsigned int crc, std::string filename)
{
	assetFileCRC=crc;
	assetFileName = filename;
}

unsigned int GEARAsset::getAssetFileCRC()
{
	return assetFileCRC;
}

std::string GEARAsset::getAssetFileName()
{
	return assetFileName;
}