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

	void getEuler(float& yaw, float& pitch, float& roll)
	{
		    //roll = RAD2DEG(atan2f(2.0f*y*w - 2.0f*x*z, 1.0f - 2.0f*y*y - 2.0f*z*z));
			//pitch = RAD2DEG(atan2f(2.0f*x*w - 2.0f*y*z, 1.0f - 2.0f*x*x - 2.0f*z*z));
			//yaw = RAD2DEG(asin(2.0f*x*y + 2.0f*z*w));
		float sqw = w * w;
		float sqx = x * x;
		float sqy = y * y;
		float sqz = z * z;
		float unit = sqx + sqy + sqz + sqw; // if normalised is one, otherwise is correction factor
		float test = x * w - y * z;
		vector3f v;

		if (test>0.4995f*unit)	 // singularity at north pole
		{
			v.y = 2.0 * atan2f(y, x);
			v.x = M_PI*0.5f;
			v.z = 0;
			vector3f euler_angle=normalizeAngles(v * M_180_PI);
			yaw=euler_angle.y;
			pitch=euler_angle.x;
			roll=euler_angle.z;
			return;
		}
		if (test<-0.4995f*unit) { // singularity at south pole
			v.y = -2.0 * atan2f(y, x);
			v.x = -M_PI*0.5f;
			v.z = 0;
			vector3f euler_angle=normalizeAngles(v * M_180_PI);
			yaw=euler_angle.y;
			pitch=euler_angle.x;
			roll=euler_angle.z;
			return;
		}
		Quaternion q(w, z, x, y);
		v.y = (float)atan2f(2.0f * q.x * q.w + 2.0f * q.y * q.z, 1.0f - 2.0f * (q.z * q.z + q.w * q.w));    // Yaw
		v.x = (float)asin(2.0f * (q.x * q.z - q.w * q.y));													// Pitch
		v.z = (float)atan2f(2.0f * q.x * q.y + 2.0f * q.z * q.w, 1.0f - 2.0f * (q.y * q.y + q.z * q.z));    // Roll
		vector3f euler_angle=normalizeAngles(v * M_180_PI);
		yaw=euler_angle.y;
		pitch=euler_angle.x;
		roll=euler_angle.z;
	}

	Quaternion getQuaternion(float yaw, float pitch, float roll)
	{
		//http://stackoverflow.com/questions/12088610/conversion-between-euler-quaternion-like-in-unity3d-engine
		yaw *= M_PI_180;
		pitch *= M_PI_180;
		roll *= M_PI_180;
		float rollOver2 = roll * 0.5f;
		float sinRollOver2 = (float)sin(rollOver2);
		float cosRollOver2 = (float)cos(rollOver2);
		float pitchOver2 = pitch * 0.5f;
		float sinPitchOver2 = (float)sin(pitchOver2);
		float cosPitchOver2 = (float)cos(pitchOver2);
		float yawOver2 = yaw * 0.5f;
		float sinYawOver2 = (float)sin(yawOver2);
		float cosYawOver2 = (float)cos(yawOver2);
		Quaternion result;
		result.w = cosYawOver2 * cosPitchOver2 * cosRollOver2 + sinYawOver2 * sinPitchOver2 * sinRollOver2;
		result.x = cosYawOver2 * sinPitchOver2 * cosRollOver2 + sinYawOver2 * cosPitchOver2 * sinRollOver2;
		result.y = sinYawOver2 * cosPitchOver2 * cosRollOver2 - cosYawOver2 * sinPitchOver2 * sinRollOver2;
		result.z = cosYawOver2 * cosPitchOver2 * sinRollOver2 - sinYawOver2 * sinPitchOver2 * cosRollOver2;

		return result;
	}

private:
	vector3f normalizeAngles(vector3f angles)
	{
		angles.x = normalizeAngle(angles.x);
		angles.y = normalizeAngle(angles.y);
		angles.z = normalizeAngle(angles.z);
		return angles;
	}

	float normalizeAngle(float angle)
	{
		while (angle>360)
			angle -= 360;
		while (angle<0)
			angle += 360;
		return angle;
	}

public:
	float x;
	float y;
	float z;
	float w;
};
#endif