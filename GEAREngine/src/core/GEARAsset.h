#pragma once

#include "basicIncludes.h"
#include <string>

class DECLSPEC GEARAsset
{
public:
	GEARAsset();
	virtual ~GEARAsset();

	void setAssetFileCRC(unsigned int crc, std::string filename);
	unsigned int getAssetFileCRC();
	std::string getAssetFileName();

protected:
	unsigned int assetFileCRC;
	std::string assetFileName;
};