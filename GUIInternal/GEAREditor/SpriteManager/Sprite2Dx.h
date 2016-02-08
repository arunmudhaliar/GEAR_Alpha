#pragma once

#include "Sprite.h"

class Sprite2Dx : public Sprite
{
public:	
	Sprite2Dx();
	Sprite2Dx(Sprite2Dx& sprite2dx);

	~Sprite2Dx();
	
	virtual void draw(/*const matrix4x4f& parentTM, */geVector2f* pos=NULL);
#ifndef GEAR2D
	virtual void draw(gxHWShader* shader);
#endif

    virtual void onSpritePreRender(){};
    virtual void onSpritePostRender(){};

    void drawVerextBufferOnly(geVector2f* pos=NULL);
};

