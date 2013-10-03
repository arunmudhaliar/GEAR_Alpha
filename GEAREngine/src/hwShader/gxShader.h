#ifndef GXSHADER_H
#define GXSHADER_H

class __declspec( dllexport ) gxShader
{
public:
	gxShader()
	{}
	virtual ~gxShader()
	{}
    
   	virtual int getAttribLoc(const char* uvar)
    {
        return -1;
    }
};

#endif