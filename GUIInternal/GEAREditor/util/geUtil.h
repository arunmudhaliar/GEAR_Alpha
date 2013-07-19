#ifndef GEUTIL_H
#define GEUTIL_H

#include <string.h>

class geUtil
{
public:
	static float getFloat(const char* str)
	{
		float value=0.0f;
		if(str==NULL)
			return 0.0f;
		if(strlen(str)==0)
			return 0.0f;

		if(EOF==sscanf(str, "%f", &value))
		{
			return 0.0f;
		}

		return value;
	}
};

#endif