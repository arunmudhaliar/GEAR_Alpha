#ifndef GETEXTUREMANAGER_H_
#define GETEXTUREMANAGER_H_

#include "../renderer/glincludes.h"

#include <string.h>
#include <vector>
//#include "types.h"
#include "steTexturePacket.h"

class CGETextureManager
{
public:
	CGETextureManager();
	~CGETextureManager();

	steTexturePacket*	LoadTexture(const char* aFileName, const char* aOpFileName=NULL);
	void	Reset();	
    void    ReLoad();   //reload all textures from physical memory
    
    unsigned int getTotalTextureMemory()    {   return m_iTotalTextureMemory;   }
    
private:
	std::vector<steTexturePacket*>	iTexturePacket;
    unsigned int m_iTotalTextureMemory;
};

#endif