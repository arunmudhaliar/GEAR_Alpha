#include "transform.h"

extern "C" {
	extern DllExport void transform_updatePositionf(transform* t, float aXdt, float aYdt, float aZdt)
	{
		t->updatePositionf(aXdt, aYdt, aZdt);
	}
	extern DllExport void transform_updateLocalPositionf(transform* t, float aXdt, float aYdt, float aZdt)
	{
		t->updateLocalPositionf(aXdt, aYdt, aZdt);
	}
	extern DllExport void transform_scaleX(transform* t, float scale)
	{
		t->scaleX(scale);
	}
	extern DllExport void transform_scaleY(transform* t, float scale)
	{
		t->scaleY(scale);
	}
	extern DllExport void transform_scaleZ(transform* t, float scale)
	{
		t->scaleZ(scale);
	}
	extern DllExport void transform_rotateLocalXf(transform* t, float aAngleInDeg)
	{
		t->rotateLocalXf(aAngleInDeg);
	}
	extern DllExport void transform_rotateLocalYf(transform* t, float aAngleInDeg)
	{
		t->rotateLocalYf(aAngleInDeg);
	}
	extern DllExport void transform_rotateLocalZf(transform* t, float aAngleInDeg)
	{
		t->rotateLocalZf(aAngleInDeg);
	}

	extern DllExport void transform_rotateWorldXf(transform* t, float aAngleInDeg)
	{
		t->rotateWorldXf(aAngleInDeg);
	}

	extern DllExport void transform_rotateWorldYf(transform* t, float aAngleInDeg)
	{
		t->rotateWorldYf(aAngleInDeg);
	}

	extern DllExport void transform_rotateWorldZf(transform* t, float aAngleInDeg)
	{
		t->rotateWorldZf(aAngleInDeg);
	}

	extern DllExport void transform_setPosition(transform* t, float x, float y, float z)
	{
		t->setPosition(x, y, z);
	}
	extern DllExport float transform_getX(transform* t)
	{
		return t->getMatrix()[12];
	}
	extern DllExport float transform_getY(transform* t)
	{
		return t->getMatrix()[13];
	}
	extern DllExport float transform_getZ(transform* t)
	{
		return t->getMatrix()[14];
	}
	extern DllExport void transform_copy(transform* a, transform* b)
	{
		*a=*b;
	}
}