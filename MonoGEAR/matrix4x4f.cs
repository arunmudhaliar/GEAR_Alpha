using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace MonoGEAR
{
    public class matrix4x4f
    {
        float[] m = new float[16];

        matrix4x4f()
        {
            identity();
        }

        matrix4x4f(float v)
        {
            for (int x = 0; x < 16; x++)
            {
                m[x] = v;
            }
            matrixChanged();
        }

        matrix4x4f(matrix4x4f matrix)
        {
            copy(matrix);
        }

        void matrixChanged()
        {
        }

        //float operator[](int i)			{	return(i<16? m[i]:0.0f);	}

        //void operator=(const matrix4x4f& matrix)
        //{
        //    copy(matrix);
        //}

        void copy(matrix4x4f matrix)
        {
            for (int x = 0; x < 16; x++)
            {
                m[x] = matrix.m[x];
            }
            matrixChanged();
        }

        void identity()
        {
            Array.Clear(m, 0, m.Length);
            m[0] = 1.0f;
            m[5] = m[0];
            m[10] = m[0];
            m[15] = m[0];
            matrixChanged();
        }

        void setScale(float sx, float sy, float sz)
        {
            m[0] = sx;
            m[5] = sy;
            m[10] = sz;
            matrixChanged();
        }
        void setScale(vector3 scale)
        {
            setScale(scale.x, scale.y, scale.z);
        }

        float[] getMatrix() { return m; }

        public static matrix4x4f operator *(matrix4x4f a, matrix4x4f b)
        {
            matrix4x4f dest = new matrix4x4f();

            dest.m[0] = a.m[0] * b.m[0] + a.m[4] * b.m[1] + a.m[8] * b.m[2] + a.m[12] * b.m[3];
            dest.m[1] = a.m[1] * b.m[0] + a.m[5] * b.m[1] + a.m[9] * b.m[2] + a.m[13] * b.m[3];
            dest.m[2] = a.m[2] * b.m[0] + a.m[6] * b.m[1] + a.m[10] * b.m[2] + a.m[14] * b.m[3];
            dest.m[3] = a.m[3] * b.m[0] + a.m[7] * b.m[1] + a.m[11] * b.m[2] + a.m[15] * b.m[3];
            dest.m[4] = a.m[0] * b.m[4] + a.m[4] * b.m[5] + a.m[8] * b.m[6] + a.m[12] * b.m[7];
            dest.m[5] = a.m[1] * b.m[4] + a.m[5] * b.m[5] + a.m[9] * b.m[6] + a.m[13] * b.m[7];
            dest.m[6] = a.m[2] * b.m[4] + a.m[6] * b.m[5] + a.m[10] * b.m[6] + a.m[14] * b.m[7];
            dest.m[7] = a.m[3] * b.m[4] + a.m[7] * b.m[5] + a.m[11] * b.m[6] + a.m[15] * b.m[7];
            dest.m[8] = a.m[0] * b.m[8] + a.m[4] * b.m[9] + a.m[8] * b.m[10] + a.m[12] * b.m[11];
            dest.m[9] = a.m[1] * b.m[8] + a.m[5] * b.m[9] + a.m[9] * b.m[10] + a.m[13] * b.m[11];
            dest.m[10] = a.m[2] * b.m[8] + a.m[6] * b.m[9] + a.m[10] * b.m[10] + a.m[14] * b.m[11];
            dest.m[11] = a.m[3] * b.m[8] + a.m[7] * b.m[9] + a.m[11] * b.m[10] + a.m[15] * b.m[11];
            dest.m[12] = a.m[0] * b.m[12] + a.m[4] * b.m[13] + a.m[8] * b.m[14] + a.m[12] * b.m[15];
            dest.m[13] = a.m[1] * b.m[12] + a.m[5] * b.m[13] + a.m[9] * b.m[14] + a.m[13] * b.m[15];
            dest.m[14] = a.m[2] * b.m[12] + a.m[6] * b.m[13] + a.m[10] * b.m[14] + a.m[14] * b.m[15];
            dest.m[15] = a.m[3] * b.m[12] + a.m[7] * b.m[13] + a.m[11] * b.m[14] + a.m[15] * b.m[15];

            return dest;
        }


        public static vector3 operator *(matrix4x4f mat, vector3 vec)
        {
            vector3 res = new vector3();
            res.x = mat.m[0] * vec.x + mat.m[4] * vec.y + mat.m[8] * vec.z + mat.m[12];
            res.y = mat.m[1] * vec.x + mat.m[5] * vec.y + mat.m[9] * vec.z + mat.m[13];
            res.z = mat.m[2] * vec.x + mat.m[6] * vec.y + mat.m[10] * vec.z + mat.m[14];
            return res;
        }

        //vector2f operator*(const vector2f& vec) const
        //{
        //    vector2f res;
        //    res.x = m[0]* vec.x + m[4]* vec.y + m[12];
        //    res.y = m[1]* vec.x + m[5]* vec.y + m[13];
        //    return res;
        //}

        public static vector3 operator %(matrix4x4f mat, vector3 vec)
        {
            vector3 res = new vector3();
            res.x = mat.m[0] * vec.x + mat.m[4] * vec.y + mat.m[8] * vec.z;
            res.y = mat.m[1] * vec.x + mat.m[5] * vec.y + mat.m[9] * vec.z;
            res.z = mat.m[2] * vec.x + mat.m[6] * vec.y + mat.m[10] * vec.z;
            return res;
        }

        //vector2f operator%(const vector2f& vec) const
        //{
        //    vector2f res;
        //    res.x = m[0]* vec.x + m[4]* vec.y;
        //    res.y = m[1]* vec.x + m[5]* vec.y;
        //    return res;
        //}

        //void inverse()
        //{
        //    this = getInverse();
        //    matrixChanged();
        //}

        matrix4x4f getInverse()
        {
            float[] mat, dst;
            matrix4x4f res = new matrix4x4f(0);
            dst = res.m;
            mat = m;

            /************************************************************
             *
             * input:
             * mat - pointer to array of 16 no's (source matrix)
             * output:
             * dst - pointer to array of 16 no's (invert matrix)
             *
             *************************************************************/
            int i;
            float[] tmp = new float[12];	/* temp array for pairs */
            float[] src = new float[16];	/* array of transpose source matrix */
            float det;		/* determinant */

            /* transpose matrix */
            for (i = 0; i < 4; i++)
            {
                src[i] = mat[i * 4];
                src[i + 4] = mat[i * 4 + 1];
                src[i + 8] = mat[i * 4 + 2];
                src[i + 12] = mat[i * 4 + 3];
            }

            /* calculate pairs for first 8 elements (cofactors) */
            tmp[0] = src[10] * src[15];
            tmp[1] = src[11] * src[14];
            tmp[2] = src[9] * src[15];
            tmp[3] = src[11] * src[13];
            tmp[4] = src[9] * src[14];
            tmp[5] = src[10] * src[13];
            tmp[6] = src[8] * src[15];
            tmp[7] = src[11] * src[12];
            tmp[8] = src[8] * src[14];
            tmp[9] = src[10] * src[12];
            tmp[10] = src[8] * src[13];
            tmp[11] = src[9] * src[12];

            /* calculate first 8 elements (cofactors) */
            dst[0] = tmp[0] * src[5] + tmp[3] * src[6] + tmp[4] * src[7];
            dst[0] -= tmp[1] * src[5] + tmp[2] * src[6] + tmp[5] * src[7];
            dst[1] = tmp[1] * src[4] + tmp[6] * src[6] + tmp[9] * src[7];
            dst[1] -= tmp[0] * src[4] + tmp[7] * src[6] + tmp[8] * src[7];
            dst[2] = tmp[2] * src[4] + tmp[7] * src[5] + tmp[10] * src[7];
            dst[2] -= tmp[3] * src[4] + tmp[6] * src[5] + tmp[11] * src[7];
            dst[3] = tmp[5] * src[4] + tmp[8] * src[5] + tmp[11] * src[6];
            dst[3] -= tmp[4] * src[4] + tmp[9] * src[5] + tmp[10] * src[6];
            dst[4] = tmp[1] * src[1] + tmp[2] * src[2] + tmp[5] * src[3];
            dst[4] -= tmp[0] * src[1] + tmp[3] * src[2] + tmp[4] * src[3];
            dst[5] = tmp[0] * src[0] + tmp[7] * src[2] + tmp[8] * src[3];
            dst[5] -= tmp[1] * src[0] + tmp[6] * src[2] + tmp[9] * src[3];
            dst[6] = tmp[3] * src[0] + tmp[6] * src[1] + tmp[11] * src[3];
            dst[6] -= tmp[2] * src[0] + tmp[7] * src[1] + tmp[10] * src[3];
            dst[7] = tmp[4] * src[0] + tmp[9] * src[1] + tmp[10] * src[2];
            dst[7] -= tmp[5] * src[0] + tmp[8] * src[1] + tmp[11] * src[2];

            /* calculate pairs for second 8 elements (cofactors) */
            tmp[0] = src[2] * src[7];
            tmp[1] = src[3] * src[6];
            tmp[2] = src[1] * src[7];
            tmp[3] = src[3] * src[5];
            tmp[4] = src[1] * src[6];
            tmp[5] = src[2] * src[5];
            tmp[6] = src[0] * src[7];
            tmp[7] = src[3] * src[4];
            tmp[8] = src[0] * src[6];
            tmp[9] = src[2] * src[4];
            tmp[10] = src[0] * src[5];
            tmp[11] = src[1] * src[4];

            /* calculate second 8 elements (cofactors) */
            dst[8] = tmp[0] * src[13] + tmp[3] * src[14] + tmp[4] * src[15];
            dst[8] -= tmp[1] * src[13] + tmp[2] * src[14] + tmp[5] * src[15];
            dst[9] = tmp[1] * src[12] + tmp[6] * src[14] + tmp[9] * src[15];
            dst[9] -= tmp[0] * src[12] + tmp[7] * src[14] + tmp[8] * src[15];
            dst[10] = tmp[2] * src[12] + tmp[7] * src[13] + tmp[10] * src[15];
            dst[10] -= tmp[3] * src[12] + tmp[6] * src[13] + tmp[11] * src[15];
            dst[11] = tmp[5] * src[12] + tmp[8] * src[13] + tmp[11] * src[14];
            dst[11] -= tmp[4] * src[12] + tmp[9] * src[13] + tmp[10] * src[14];
            dst[12] = tmp[2] * src[10] + tmp[5] * src[11] + tmp[1] * src[9];
            dst[12] -= tmp[4] * src[11] + tmp[0] * src[9] + tmp[3] * src[10];
            dst[13] = tmp[8] * src[11] + tmp[0] * src[8] + tmp[7] * src[10];
            dst[13] -= tmp[6] * src[10] + tmp[9] * src[11] + tmp[1] * src[8];
            dst[14] = tmp[6] * src[9] + tmp[11] * src[11] + tmp[3] * src[8];
            dst[14] -= tmp[10] * src[11] + tmp[2] * src[8] + tmp[7] * src[9];
            dst[15] = tmp[10] * src[10] + tmp[4] * src[8] + tmp[9] * src[9];
            dst[15] -= tmp[8] * src[9] + tmp[11] * src[10] + tmp[5] * src[8];

            /* calculate determinant */
            det = src[0] * dst[0] + src[1] * dst[1] + src[2] * dst[2] + src[3] * dst[3];

            if (det == 0.0f) return null; //singular matrix

            /* calculate matrix inverse */
            det = 1.0f / det;

            for (i = 0; i < 16; i++)
                dst[i] = dst[i] * det;

            return res;
        }

        void setPerspective(float fovy, float aspect, float zNear, float zFar)
        {
            float xmin, xmax, ymin, ymax;

            ymax = zNear * (float)Math.Tan(fovy * gxMath.M_PI / 360.0f);
            ymin = -ymax;
            xmin = ymin * aspect;
            xmax = ymax * aspect;

            setFrustumf2(xmin, xmax, ymin, ymax, zNear, zFar);
        }

        void setFrustumf2(float left, float right, float bottom, float top, float znear, float zfar)
        {
            float temp, temp2, temp3, temp4;
            temp = 2.0f * znear;
            temp2 = right - left;
            temp3 = top - bottom;
            temp4 = zfar - znear;
            m[0] = temp / temp2;
            m[1] = 0.0f;
            m[2] = 0.0f;
            m[3] = 0.0f;
            m[4] = 0.0f;
            m[5] = temp / temp3;
            m[6] = 0.0f;
            m[7] = 0.0f;
            m[8] = (right + left) / temp2;
            m[9] = (top + bottom) / temp3;
            m[10] = (-zfar - znear) / temp4;
            m[11] = -1.0f;
            m[12] = 0.0f;
            m[13] = 0.0f;
            m[14] = (-temp * zfar) / temp4;
            m[15] = 0.0f;

            matrixChanged();
        }

        void setOrtho(float left, float right, float bottom, float top, float nearVal, float farVal)
        {
            identity();

            float right_left = right - left;
            float top_bottom = top - bottom;
            float far_near = farVal - nearVal;

            float right_plus_left = right + left;
            float top_plus_bottom = top + bottom;
            float far_plus_near = farVal + nearVal;

            setScale(2.0f / right_left, 2.0f / top_bottom, -2.0f / far_near);
            setPosition(-(right_plus_left / right_left), -(top_plus_bottom / top_bottom), -(far_plus_near / far_near));
        }

        public void setRotationMatrix(float angle, bool X, bool Y, bool Z)
	    {
		    float sn,cs;
		    sn = gxMath.SINF(angle);
		    cs = gxMath.COSF(angle);
		
		    identity();
		
		    float[] _m=m;
		    if(X)
		    {
			    _m[5]	= cs;
			    _m[6]	= sn;
			    _m[9]	= -sn;
			    _m[10]	= cs;
		    }
		
		    if(Y)
		    {
			    _m[0]	= cs;
			    _m[2]	= -sn;
			    _m[8]	= sn;
			    _m[10]	= cs;
		    }
		
		    if(Z)
		    {
			    _m[0]	= cs;
			    _m[1]	= sn;
			    _m[4]	= -sn;
			    _m[5]	= cs;
		    }

		    matrixChanged();
	    }

        void setRotationMatrix(float angle, vector3 axis)
	    {
		    float cs,sn;
		    cs=gxMath.COSF(angle);
		    sn=gxMath.SINF(angle);
		
		    float x,y,z,one_cs;
		    vector3 axisN=new vector3(axis);
		    axisN.normalize();
		
		    one_cs = 1.0f - cs;
		    x=axisN.x;
		    y=axisN.y;
		    z=axisN.z;
		
		    float one_csx=one_cs*x;
		    float one_csy=one_cs*y;
		    float xx=x*x;
		    float yy=y*y;
		    float zz=z*z;
		
		    m[0]=1.0f + (one_cs*(xx-1.0f));
		    m[1]=(z*sn) + (one_csx*y);
		    m[2]=(-y*sn) + (one_csx*z);
		    m[3]=0.0f;
		
		    m[4]=(-z*sn) + (one_csx*y);
		    m[5]=1.0f+(one_cs*(yy-1.0f));
		    m[6]=(x*sn) + (one_csy*z);
		    m[7]=0.0f;
		
		    m[8]=(y*sn) + (one_csx*z);
		    m[9]=(-x*sn) + (one_csy*z);
		    m[10]=1.0f + (one_cs*(zz-1.0f));
		    m[11]=0.0f;
		
		    m[12]=0.0f;
		    m[13]=0.0f;
		    m[14]=0.0f;
		    m[15]=1.0f;

		    matrixChanged();
	    }

        //void setRotationMatrix(float angle, float* axis)
        //{
        //    setRotationMatrix(angle, vector3f(axis[0], axis[1], axis[2]));
        //}

        //void setDirection(vector3f* dir, vector3f* up=NULL)
        //{
        //    vector3f aDir, aLeft, aUp;

        //    if(!dir)
        //    {	return;		}
        //    else
        //    {
        //        aDir = *dir;
        //        aDir.normalize();
        //    }

        //    if(!up)
        //    {		
        //        aUp.x = 0.0f;
        //        aUp.y = 0.0f;
        //        aUp.z = 1.0f;
        //    }
        //    else
        //    {
        //        aUp = *up;
        //        aUp.normalize();
        //    }

        //    aLeft = aDir.cross(aUp);
        //    aLeft.normalize();

        //    if(up)
        //    {
        //        aUp	= aLeft.cross(aDir);
        //        aUp.normalize();
        //    }

        //    m[0]=aLeft.x; m[1]=aLeft.y;  m[2]=aLeft.z;
        //    m[4]=aDir.x; m[5]=aDir.y;  m[6]=aDir.z;
        //    m[8]=aUp.x; m[9]=aUp.y;  m[10]=aUp.z;

        //    matrixChanged();
        //}

        vector3 getXAxis() { return new vector3(m[0], m[1], m[2]); }
        vector3 getYAxis() { return new vector3(m[4], m[5], m[6]); }
        vector3 getZAxis() { return new vector3(m[8], m[9], m[10]); }
        vector3 getPosition() { return new vector3(m[12], m[13], m[14]); }
        vector3 getScale() { return new vector3(m[0], m[5], m[10]); }

        void setXAxis(vector3 axis)
        {
            m[0] = axis.x; m[1] = axis.y; m[2] = axis.z;
            matrixChanged();
        }

        void setYAxis(vector3 axis)
        {
            m[4] = axis.x; m[5] = axis.y; m[6] = axis.z;
            matrixChanged();
        }

        void setZAxis(vector3 axis)
        {
            m[8] = axis.x; m[9] = axis.y; m[10] = axis.z;
            matrixChanged();
        }

        void setPosition(float x, float y, float z)
        {
            m[12] = x; m[13] = y; m[14] = z;
            matrixChanged();
        }

        void setPosition(vector3 pos) { setPosition(pos.x, pos.y, pos.z); }
    }
}