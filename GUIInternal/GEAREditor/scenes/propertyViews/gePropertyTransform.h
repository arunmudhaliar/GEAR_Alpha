#ifndef GEPROPERTYTRANSFORM_H
#define GEPROPERTYTRANSFORM_H

#include "../../gui/geTreeView.h"
#include "../../gui/geButton.h"
#include "../../gui/gePushButton.h"
#include "../../gui/geTextBox.h"
#include "../../../../GEAREngine/src/core/vector4.h"
#include "../../../../GEAREngine/src/core/quaternion.h"
#include "../../util/pxMath.h"
#include "../../gui/geWindowColumn.h"

class gePropertyTransform : public geTreeNode, public MGUIObserver
{
public:

	geTextBox* m_pszTextBoxTranslation[3];
	geTextBox* m_pszTextBoxRotation[3];
	geTextBox* m_pszTextBoxScale[3];
	object3d* m_pObject3dPtr;

	gePropertyTransform(rendererGL10* renderer, geGUIBase* parent, const char* name, Sprite2Dx* sprite, geFontManager* fontManager);
	virtual ~gePropertyTransform();

	virtual void drawNode();
	virtual void onTVSelectionChange(geTreeNode* tvnode, geTreeView* treeview);
	void onTextChange(geGUIBase* textbox);

	void populatePropertyOfTransform(object3d* obj);
	void updatePropertyView();

	bool decompose(vector3f& scale, Quaternion& quat, vector3f& translate , const float* m)
	{
		vector3f vec;

		// Compute the scaling part.
		double scaleX = pxMath::SQRT( m[ 0 ] * m[ 0 ] + m[ 1 ] * m[ 1 ] + m[ 2 ] * m[ 2 ] );
		double scaleY = pxMath::SQRT( m[ 4 ] * m[ 4 ] + m[ 5 ] * m[ 5 ] + m[ 6 ] * m[ 6 ] );
		double scaleZ = pxMath::SQRT( m[ 8 ] * m[ 8 ] + m[ 9 ] * m[ 9 ] + m[ 10 ] * m[ 10 ] );

		// Compute the translation part. 
		translate.x = m[ 12 ];
		translate.y = m[ 13 ];
		translate.z = m[ 14 ];

		// Let's calculate the rotation now 
		if( ( scaleX == 0.0 ) || ( scaleY == 0.0 ) || ( scaleZ == 0.0 ) )
		{
			return false;
		}

		float mat[16];// = new Matrix44();
		mat[ 0 ] = m[ 0 ] / scaleX;
		mat[ 1 ] = m[ 1 ] / scaleX;
		mat[ 2 ] = m[ 2 ] / scaleX;
		mat[ 4 ] = m[ 4 ] / scaleY;
		mat[ 5 ] = m[ 5 ] / scaleY;
		mat[ 6 ] = m[ 6 ] / scaleY;
		mat[ 8 ] = m[ 8 ] / scaleZ;
		mat[ 9 ] = m[ 9 ] / scaleZ;
		mat[ 10 ] = m[ 10 ] / scaleZ;

		scale.set( (float)scaleX, (float)scaleY, (float)scaleZ );

		quat = Quaternion::fromRotationMatrix( mat );

		quat.normalize();
		//quat = quat.inverse();

		return true;
	}

	void getRotation(float& Yaw, float& Pitch, float& Roll, const float* m) const
	{
		/*
			union {
		struct 
		{
			float        _11, _12, _13, _14;
			float        _21, _22, _23, _24;
			float        _31, _32, _33, _34;
			float        _41, _42, _43, _44;
		};
		float m[4][4];
		float m2[16];
	};
	*/
		if (m[0] == 1.0f)
		{
			Yaw = atan2f(m[2], m[11]);
			Pitch = 0;
			Roll = 0;

		}else if (m[0] == -1.0f)
		{
			Yaw = atan2f(m[2], m[11]);
			Pitch = 0;
			Roll = 0;
		}else 
		{

			Yaw = atan2(-m[8],m[0]);
			Pitch = asin(m[4]);
			Roll = atan2(-m[6],m[5]);
		}
	}
};

#endif