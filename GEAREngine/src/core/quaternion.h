#ifndef QUATERNION_H
#define QUATERNION_H

#include "gxMath.h"
#include "vector3.h"

class Quaternion
{
public:
	Quaternion()
	{ 
		x=y=z=0.0f;
		w=1.0f;
	}

	Quaternion( float x, float y, float z, float w )
	{
		this->x = x;
		this->y = y;
		this->z = z;
		this->w = w;
	}

	Quaternion inverse()
	{
		return Quaternion( -x, -y, -z, w );
	}

	void multiply( Quaternion quat )
	{
		vector3f qa( x, y, z );
		vector3f qb( quat.x, quat.y, quat.z );

		vector3f va(qa.cross(qb));

		qa=qa*quat.w;
		qb=qb*w;
		va=va+qa;
		va=va+qb;

		x = va.x;
		y = va.y;
		z = va.z;

		normalize();
	}

	void normalize()
	{
		float len = magnitude();
		w /= len;
		x /= len;
		y /= len;
		z /= len;
	}

	float magnitude()
	{
		return gxMath::SQRT( x * x + y * y + z * z + w * w );
	}


	static Quaternion fromRotationMatrix( const float* m )
	{
		int i, maxi;
		float maxdiag, S, trace;
		
		Quaternion quat;
			
		trace = m[ 0 ] + m[ 5 ] + m[ 10 ] + 1.0f;
		if( trace > 0.0f )
		{
			S = 0.5f / gxMath::SQRT( trace );
			quat.x = ( m[ 6 ] - m[ 9 ] ) * S;
			quat.y = ( m[ 8 ] - m[ 2 ] ) * S;
			quat.z = ( m[ 1 ] - m[ 4 ] ) * S;
			quat.w = 0.25f / S;
			return quat;
		}
		maxi = 0;
		maxdiag = m[ 0 ];
		for( i = 1; i < 3; i++ )
		{
			if( m[ i*5 ] > maxdiag )
			{
				maxi = i;
				maxdiag = m[ i*5 ];
			}
		}
		switch( maxi )
		{
			case 0:
				S = 2.0f * gxMath::SQRT( 1.0f + m[ 0 ] - m[ 5 ] - m[ 10 ] );
				quat.x = 0.25f * S;
				quat.y = ( m[ 1 ] + m[ 4 ] ) / S;
				quat.z = ( m[ 2 ] + m[ 8 ] ) / S;
				quat.w = ( m[ 6 ] - m[ 9 ] ) / S;
				break;
			case 1:
				S = 2.0f * gxMath::SQRT( 1.0f + m[ 5 ] - m[ 0 ] - m[ 10 ] );
				quat.x = ( m[ 1 ] + m[ 4 ] ) / S;
				quat.y = 0.25f * S;
				quat.z = ( m[ 6 ] + m[ 9 ] ) / S;
				quat.w = ( m[ 8 ] - m[ 2 ] ) / S;
				break;
			case 2:
				S = 2.0f * gxMath::SQRT( 1.0f + m[ 10 ] - m[ 0 ] - m[ 5 ] );
				quat.x = ( m[ 2 ] + m[ 8 ] ) / S;
				quat.y = ( m[ 6 ] + m[ 9 ] ) / S;
				quat.z = 0.25f * S;
				quat.w = ( m[ 1 ] - m[ 4 ] ) / S;
				break;
		}
		return quat;
	}

	float x;
	float y;
	float z;
	float w;
};
#endif