#ifndef GEARASSET_H
#define GEARASSET_H

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
	unsigned int m_iAssetFileCRC;
	std::string m_cAssetFileName;
};

#endif