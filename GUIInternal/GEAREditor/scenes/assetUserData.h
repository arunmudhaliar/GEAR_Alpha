#ifndef ASSETUSERDATA_H
#define ASSETUSERDATA_H

class assetUserData
{
	assetUserData()
	{
	}
public:
	assetUserData(int typeID, const char* path, void* ptr)
	{
		type=typeID;
		STRCPY(assetAbsolutePath, path);
		assetObjectPtr=ptr;
	}

	const char* getAssetAbsolutePath()	{	return assetAbsolutePath;	}
	int getAssetType()					{	return type;				}
	void* getAssetObjectPtr()			{	return assetObjectPtr;		}
	void setAssetObjectPtr(void* ptr)	{	assetObjectPtr= ptr;		}

private:
	char assetAbsolutePath[1024];
	int type;
	void* assetObjectPtr;
};

#endif