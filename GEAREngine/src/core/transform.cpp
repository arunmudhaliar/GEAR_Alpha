#include "transform.h"

extern "C" {
	extern DECLSPEC void transform_updatePositionf(transform* t, float aXdt, float aYdt, float aZdt)
	{
		t->updatePositionf(aXdt, aYdt, aZdt);
	}
	extern DECLSPEC void transform_updateLocalPositionf(transform* t, float aXdt, float aYdt, float aZdt)
	{
		t->updateLocalPositionf(aXdt, aYdt, aZdt);
	}
	extern DECLSPEC void transform_scaleX(transform* t, float scale)
	{
		t->scaleX(scale);
	}
	extern DECLSPEC void transform_scaleY(transform* t, float scale)
	{
		t->scaleY(scale);
	}
	extern DECLSPEC void transform_scaleZ(transform* t, float scale)
	{
		t->scaleZ(scale);
	}
	extern DECLSPEC void transform_rotateLocalXf(transform* t, float aAngleInDeg)
	{
		t->rotateLocalXf(aAngleInDeg);
	}
	extern DECLSPEC void transform_rotateLocalYf(transform* t, float aAngleInDeg)
	{
		t->rotateLocalYf(aAngleInDeg);
	}
	extern DECLSPEC void transform_rotateLocalZf(transform* t, float aAngleInDeg)
	{
		t->rotateLocalZf(aAngleInDeg);
	}

	extern DECLSPEC void transform_rotateWorldXf(transform* t, float aAngleInDeg)
	{
		t->rotateWorldXf(aAngleInDeg);
	}

	extern DECLSPEC void transform_rotateWorldYf(transform* t, float aAngleInDeg)
	{
		t->rotateWorldYf(aAngleInDeg);
	}

	extern DECLSPEC void transform_rotateWorldZf(transform* t, float aAngleInDeg)
	{
		t->rotateWorldZf(aAngleInDeg);
	}

	extern DECLSPEC void transform_setPosition(transform* t, float x, float y, float z)
	{
		t->setPosition(x, y, z);
	}
	extern DECLSPEC float transform_getX(transform* t)
	{
		return t->getMatrix()[12];
	}
	extern DECLSPEC float transform_getY(transform* t)
	{
		return t->getMatrix()[13];
	}
	extern DECLSPEC float transform_getZ(transform* t)
	{
		return t->getMatrix()[14];
	}
	extern DECLSPEC void transform_copy(transform* a, transform* b)
	{
		*a=*b;
	}
}