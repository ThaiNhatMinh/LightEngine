#include "Matrix.h"
#include <string.h>
#include "CMath.h"
#include <iomanip>
Matrix4::Matrix4()
{
	// Nothing here
	Identity();
}

// 0	1	2	3
// 4	5	6	7
// 8	9	10	11
// 12	13	14	15

Matrix4::Matrix4(const Vector3& v1,const Vector3& v2,const Vector3& v3)
{
	mV[0] = v1.x;
	mV[1] = v2.x;
	mV[2] = v3.x;
	mV[3] = mV[7] = mV[11] = mV[12] = mV[13] = mV[14] = 0;
	mV[4] = v1.y;
	mV[5] = v2.y;
	mV[6] = v3.y;
	mV[8] = v1.z;
	mV[9] = v2.z;
	mV[10] = v3.z;
	mV[15] = 1.0f;
}

Matrix4::Matrix4(const Vector3& v1,const Vector3& v2,const Vector3& v3,const Vector3& v4)
{
	mV[0] = v1.x;
	mV[4] = v1.y;
	mV[8] = v1.z;

	mV[1] = v2.x;
	mV[5] = v2.y;
	mV[9] = v2.z;

	mV[2] = v3.x;
	mV[6] = v3.y;
	mV[10] = v3.z;

	mV[12] = v4.x;
	mV[13] = v4.y;
	mV[14] = v4.z;
	
	mV[3] =	0;
	mV[7] = 0;
	mV[11] = 0;
	mV[15] = 1.0f;

}
Matrix4::Matrix4( float m11, float m12, float m13, float m14,
			 float m21, float m22, float m23, float m24,
			 float m31, float m32, float m33, float m34,
			 float m41, float m42, float m43, float m44)
{
	mV[0] = m11;
	mV[1] = m12;
	mV[2] = m13;
	mV[3] = m14;
	mV[4] = m21;
	mV[5] = m22;
	mV[6] = m23;
	mV[7] = m24;
	mV[8] = m31;
	mV[9] = m32;
	mV[10] = m33;
	mV[11] = m34;
	mV[12] = m41;
	mV[13] = m42;
	mV[14] = m43;
	mV[15] = m44;
}
Matrix4::Matrix4(const Matrix4& M)
{
	mV[0] = M.mV[0];
    mV[1] = M.mV[1];
    mV[2] = M.mV[2];
    mV[3] = M.mV[3];
    mV[4] = M.mV[4];
    mV[5] = M.mV[5];
    mV[6] = M.mV[6];
    mV[7] = M.mV[7];
    mV[8] = M.mV[8];
    mV[9] = M.mV[9];
    mV[10] = M.mV[10];
    mV[11] = M.mV[11];
    mV[12] = M.mV[12];
    mV[13] = M.mV[13];
    mV[14] = M.mV[14];
    mV[15] = M.mV[15];
}

Matrix4::Matrix4(bool Zero)
{
	if(!Zero) Identity();
	else
		for(int i=0;i<16;i++) mV[i] = 0;
}

Matrix4::Matrix4(const float* Data)
{
	memcpy( mV, Data, 4 * 4 * sizeof( float ) );
}


Matrix4::~Matrix4()
{
}

// 0	1	2	3
// 4	5	6	7
// 8	9	10	11
// 12	13	14	15

void Matrix4::Identity()
{
	 mV[0] = 1.0f;
    mV[1] = 0.0f;
    mV[2] = 0.0f;
    mV[3] = 0.0f;
    mV[4] = 0.0f;
    mV[5] = 1.0f;
    mV[6] = 0.0f;
    mV[7] = 0.0f;
    mV[8] = 0.0f;
    mV[9] = 0.0f;
    mV[10] = 1.0f;
    mV[11] = 0.0f;
    mV[12] = 0.0f;
    mV[13] = 0.0f;
    mV[14] = 0.0f;
    mV[15] = 1.0f;
}

void Matrix4::ZeroTranslate()
{
	mV[12] = mV[13] = mV[14] = 0.0f;
}

float Matrix4::operator()(int Col, int Row) const
{
	return mV[Col + Row*4];
}

float& Matrix4::operator()(int Row, int Col) 
{
	return mV[Col + Row*4];
}

Vector3 Matrix4::operator()(int axis) const
{
	return Vector3(mV[0 + axis],mV[4 + axis], mV[8+axis]);
}

void Matrix4::operator()(int axis, Vector3 v)
{
	mV[axis + 0] = v.x;
	mV[axis + 4] = v.y;
	mV[axis + 8] = v.z;
}



float Matrix4::operator[](int id) const
{
	return mV[id];
}

float& Matrix4::operator[](int id) 
{
	return mV[id];
}


Matrix4& Matrix4::operator=(const Matrix4& M)
{
	if(this == &M)
		return *this;
	mV[0] = M.mV[0];
    mV[1] = M.mV[1];
    mV[2] = M.mV[2];
    mV[3] = M.mV[3];
    mV[4] = M.mV[4];
    mV[5] = M.mV[5];
    mV[6] = M.mV[6];
    mV[7] = M.mV[7];
    mV[8] = M.mV[8];
    mV[9] = M.mV[9];
    mV[10] = M.mV[10];
    mV[11] = M.mV[11];
    mV[12] = M.mV[12];
    mV[13] = M.mV[13];
    mV[14] = M.mV[14];
    mV[15] = M.mV[15];
	return *this;
}

bool Matrix4::operator==(const Matrix4& M) const
{
	for(int i=0;i<16;i++)
		if(M.mV[i] != mV[i]) return false;
	return true;
}

bool Matrix4::operator!=(const Matrix4& M) const 
{
	for(int i=0;i<16;i++)
		if(M.mV[i] == mV[i]) return false;
	return true;
}

Matrix4 Matrix4::operator+(const Matrix4& M) const
{
	return Matrix4( mV[0] + M.mV[0],mV[1] + M.mV[1],mV[2] + M.mV[2],mV[3] + M.mV[3],
					mV[4] + M.mV[4],mV[5] + M.mV[5],mV[6] + M.mV[6],mV[7] + M.mV[7],
					mV[8] + M.mV[8],mV[9] + M.mV[9],mV[10] + M.mV[10],mV[11] + M.mV[11],
					mV[12] + M.mV[12],mV[13] + M.mV[13],mV[14] + M.mV[14],mV[15] + M.mV[15]);
}

Matrix4 Matrix4::operator-(const Matrix4& M) const
{
	return Matrix4( mV[0] - M.mV[0],mV[1] - M.mV[1],mV[2] - M.mV[2],mV[3] - M.mV[3],
					mV[4] - M.mV[4],mV[5] - M.mV[5],mV[6] - M.mV[6],mV[7] - M.mV[7],
					mV[8] - M.mV[8],mV[9] - M.mV[9],mV[10] - M.mV[10],mV[11] - M.mV[11],
					mV[12] - M.mV[12],mV[13] - M.mV[13],mV[14] - M.mV[14],mV[15] - M.mV[15]);
}
// Matrix present



Matrix4 Matrix4::operator*(const Matrix4& M) const
{
	Matrix4 C;
	/*for(int Row = 0; Row<4 ; Row++ )
		for(int Col = 0; Col<4 ; Col++)
			C.mV[Row*4 + Col] = mV[Row*4 + 0] * M.mV[0*4 + Col] +
								mV[Row*4 + 1] * M.mV[1*4 + Col] + 
								mV[Row*4 + 2] * M.mV[2*4 + Col] + 
								mV[Row*4 + 3] * M.mV[3*4 + Col];
	*/

	// We need do it by myself, more faster
	// 0	1	2	3			0	1	2	3
	// 4	5	6	7	 x		4	5	6	7
	// 8	9	10	11			8	9	10	11
	// 12	13	14	15			12	13	14	15
    C.mV[0*4+0] = mV[0*4+0]*M.mV[0*4+0] + mV[0*4+1]*M.mV[1*4+0] + mV[0*4+2]*M.mV[2*4+0] + mV[0*4+3]*M.mV[3*4+0];
    C.mV[1*4+0] = mV[1*4+0]*M.mV[0*4+0] + mV[1*4+1]*M.mV[1*4+0] + mV[1*4+2]*M.mV[2*4+0] + mV[1*4+3]*M.mV[3*4+0];
    C.mV[2*4+0] = mV[2*4+0]*M.mV[0*4+0] + mV[2*4+1]*M.mV[1*4+0] + mV[2*4+2]*M.mV[2*4+0] + mV[2*4+3]*M.mV[3*4+0];
    C.mV[3*4+0] = mV[3*4+0]*M.mV[0*4+0] + mV[3*4+1]*M.mV[1*4+0] + mV[3*4+2]*M.mV[2*4+0] + mV[3*4+3]*M.mV[3*4+0];

    C.mV[0*4+1] = mV[0*4+0]*M.mV[0*4+1] + mV[0*4+1]*M.mV[1*4+1] + mV[0*4+2]*M.mV[2*4+1] + mV[0*4+3]*M.mV[3*4+1];
    C.mV[1*4+1] = mV[1*4+0]*M.mV[0*4+1] + mV[1*4+1]*M.mV[1*4+1] + mV[1*4+2]*M.mV[2*4+1] + mV[1*4+3]*M.mV[3*4+1];
    C.mV[2*4+1] = mV[2*4+0]*M.mV[0*4+1] + mV[2*4+1]*M.mV[1*4+1] + mV[2*4+2]*M.mV[2*4+1] + mV[2*4+3]*M.mV[3*4+1];
    C.mV[3*4+1] = mV[3*4+0]*M.mV[0*4+1] + mV[3*4+1]*M.mV[1*4+1] + mV[3*4+2]*M.mV[2*4+1] + mV[3*4+3]*M.mV[3*4+1];

    C.mV[0*4+2] = mV[0*4+0]*M.mV[0*4+2] + mV[0*4+1]*M.mV[1*4+2] + mV[0*4+2]*M.mV[2*4+2] + mV[0*4+3]*M.mV[3*4+2];
    C.mV[1*4+2] = mV[1*4+0]*M.mV[0*4+2] + mV[1*4+1]*M.mV[1*4+2] + mV[1*4+2]*M.mV[2*4+2] + mV[1*4+3]*M.mV[3*4+2];
    C.mV[2*4+2] = mV[2*4+0]*M.mV[0*4+2] + mV[2*4+1]*M.mV[1*4+2] + mV[2*4+2]*M.mV[2*4+2] + mV[2*4+3]*M.mV[3*4+2];
    C.mV[3*4+2] = mV[3*4+0]*M.mV[0*4+2] + mV[3*4+1]*M.mV[1*4+2] + mV[3*4+2]*M.mV[2*4+2] + mV[3*4+3]*M.mV[3*4+2];

    C.mV[0*4+3] = mV[0*4+0]*M.mV[0*4+3] + mV[0*4+1]*M.mV[1*4+3] + mV[0*4+2]*M.mV[2*4+3] + mV[0*4+3]*M.mV[3*4+3];
    C.mV[1*4+3] = mV[1*4+0]*M.mV[0*4+3] + mV[1*4+1]*M.mV[1*4+3] + mV[1*4+2]*M.mV[2*4+3] + mV[1*4+3]*M.mV[3*4+3];
    C.mV[2*4+3] = mV[2*4+0]*M.mV[0*4+3] + mV[2*4+1]*M.mV[1*4+3] + mV[2*4+2]*M.mV[2*4+3] + mV[2*4+3]*M.mV[3*4+3];
    C.mV[3*4+3] = mV[3*4+0]*M.mV[0*4+3] + mV[3*4+1]*M.mV[1*4+3] + mV[3*4+2]*M.mV[2*4+3] + mV[3*4+3]*M.mV[3*4+3];
	return C;
}

Matrix4 Matrix4::operator*(float Scalar) const
{
	return Matrix4( mV[0]*Scalar,	mV[1]*Scalar,	mV[2]*Scalar,	mV[3]*Scalar,
					mV[4]*Scalar,	mV[5]*Scalar,	mV[6]*Scalar,	mV[7]*Scalar,
					mV[8]*Scalar,	mV[9]*Scalar,	mV[10]*Scalar,	mV[11]*Scalar,
					mV[12]*Scalar,	mV[13]*Scalar,	mV[14]*Scalar,	mV[15]*Scalar);
}

//				0	1	2	3		
//	x y z w		4	5	6	7		
//				8	9	10	11		
//				12	13	14	15		

Vector3 Matrix4::operator*(const Vector3& v)const
{
	return Vector3(		v.x * mV[0] + v.y*mV[4] + v.z*mV[8] + mV[12],
						v.x * mV[1] + v.y*mV[5] + v.z*mV[9] + mV[13],
						v.x * mV[2] + v.y*mV[6] + v.z*mV[10] + mV[14]);
}

Vector4 Matrix4::operator*(const Vector4& v)const
{
	return Vector4(		v.x * mV[0] + v.y*mV[4] + v.z*mV[8] + v.w*mV[12],
						v.x * mV[1] + v.y*mV[5] + v.z*mV[9] + v.w*mV[13],
						v.x * mV[2] + v.y*mV[6] + v.z*mV[10] + v.w*mV[14],
						v.x * mV[3] + v.y*mV[7] + v.z*mV[11] + v.w*mV[15]);
}

Matrix4 Matrix4::operator/(float Scalar) const
{
	float oneoverScalar = 1.0f/Scalar;
	return Matrix4( mV[0]*oneoverScalar,	mV[1]*oneoverScalar,	mV[2]*oneoverScalar,	mV[3]*oneoverScalar,
					mV[4]*oneoverScalar,	mV[5]*oneoverScalar,	mV[6]*oneoverScalar,	mV[7]*oneoverScalar,
					mV[8]*oneoverScalar,	mV[9]*oneoverScalar,	mV[10]*oneoverScalar,	mV[11]*oneoverScalar,
					mV[12]*oneoverScalar,	mV[13]*oneoverScalar,	mV[14]*oneoverScalar,	mV[15]*oneoverScalar);
}

Matrix4 Matrix4::operator-() const
{
	return Matrix4( -mV[0],		-mV[1],		-mV[2],		-mV[3],
					-mV[4],		-mV[5],		-mV[6],		-mV[7],
					-mV[8],		-mV[9],		-mV[10],	-mV[11],
					-mV[12],	-mV[13],	-mV[14],	-mV[15]);
}

Matrix4& Matrix4::operator+=(const Matrix4& M)
{
	mV[0] += M.mV[0];
    mV[1] += M.mV[1];
    mV[2] += M.mV[2];
    mV[3] += M.mV[3];
    mV[4] += M.mV[4];
    mV[5] += M.mV[5];
    mV[6] += M.mV[6];
    mV[7] += M.mV[7];
    mV[8] += M.mV[8];
    mV[9] += M.mV[9];
    mV[10] += M.mV[10];
    mV[11] += M.mV[11];
    mV[12] += M.mV[12];
    mV[13] += M.mV[13];
    mV[14] += M.mV[14];
    mV[15] += M.mV[15];
	return *this;
}

Matrix4& Matrix4::operator-=(const Matrix4& M)
{
	mV[0] -= M.mV[0];
    mV[1] -= M.mV[1];
    mV[2] -= M.mV[2];
    mV[3] -= M.mV[3];
    mV[4] -= M.mV[4];
    mV[5] -= M.mV[5];
    mV[6] -= M.mV[6];
    mV[7] -= M.mV[7];
    mV[8] -= M.mV[8];
    mV[9] -= M.mV[9];
    mV[10] -= M.mV[10];
    mV[11] -= M.mV[11];
    mV[12] -= M.mV[12];
    mV[13] -= M.mV[13];
    mV[14] -= M.mV[14];
    mV[15] -= M.mV[15];
	return *this;
}

Matrix4& Matrix4::operator*=(float Scalar)
{
	mV[0] *= Scalar; 
    mV[1] *= Scalar;
    mV[2] *= Scalar;
    mV[3] *= Scalar;
    mV[4] *= Scalar;
    mV[5] *= Scalar;
    mV[6] *= Scalar;
    mV[7] *= Scalar;
    mV[8] *= Scalar;
    mV[9] *= Scalar;
    mV[10] *= Scalar;
    mV[11] *= Scalar;
    mV[12] *= Scalar;
    mV[13] *= Scalar;
    mV[14] *= Scalar;
    mV[15] *= Scalar; 
	return *this;
}

Matrix4& Matrix4::operator/=(float Scalar)
{
	float oneoverScalar = 1.0f/Scalar;
	mV[0] *= oneoverScalar; 
    mV[1] *= oneoverScalar;
    mV[2] *= oneoverScalar;
    mV[3] *= oneoverScalar;
    mV[4] *= oneoverScalar;
    mV[5] *= oneoverScalar;
    mV[6] *= oneoverScalar;
    mV[7] *= oneoverScalar;
    mV[8] *= oneoverScalar;
    mV[9] *= oneoverScalar;
    mV[10] *= oneoverScalar;
    mV[11] *= oneoverScalar;
    mV[12] *= oneoverScalar;
    mV[13] *= oneoverScalar;
    mV[14] *= oneoverScalar;
    mV[15] *= oneoverScalar; 
	return *this;
}


// Matrix present

// 0	1	2	3
// 4	5	6	7
// 8	9	10	11
// 12	13	14	15

Matrix4 Matrix4::Transpose() const
{

	return Matrix4(	mV[0], mV[4], mV[8], mV[12],
					mV[1], mV[5], mV[9], mV[13],
					mV[2], mV[6], mV[10], mV[14],
					mV[3], mV[7], mV[11], mV[15]);
}

const float* Matrix4::ToFloatPtr() const
{
	return mV;
}
float* Matrix4::ToFloatPtr()
{
	return mV;
}
std::ostream&  operator<<(std::ostream& os,const Matrix4& v )
{
	os<< '|' << v(0,0) << '\t' << v(0,1) << '\t' << v(0,2) << '\t' << v(0,3) <<  '|' << '\n';
	os<< '|' << v(1,0) << '\t' << v(1,1) << '\t' << v(1,2) << '\t' << v(1,3) <<  '|' << '\n';
	os<< '|' << v(2,0) << '\t' << v(2,1) << '\t' << v(2,2) << '\t' << v(2,3) <<  '|' << '\n';
	os<< '|' << v(3,0) << '\t' << v(3,1) << '\t' << v(3,2) << '\t' << v(3,3) <<  '|' << '\n';
	return os;
}


std::ostream&  operator<<(const Matrix4& v ,std::ostream& os)
{
	return os;
}

void Matrix4::Translate(const Vector3& v)
{
	
	mV[12]= v.x;
	mV[13]= v.y;
	mV[14]= v.z;
}

Vector3 Matrix4::GetTranslate2() const
{
	return Vector3(mV[3], mV[7], mV[11]);
}

void Matrix4::Translate2(const Vector3 & v)
{
	mV[3] = v.x;
	mV[7] = v.y;
	mV[11] = v.z;
}

Vector3 Matrix4::GetTranslate()const
{
	return Vector3(mV[12], mV[13], mV[14]);
}

void Matrix4::Translate(float x, float y, float z)
{
	
	mV[12]= x;
	mV[13]= y;
	mV[14]= z;
}

Matrix4 Matrix4::Inverse()const
{
	double inv[16], det;

    inv[0] = mV[5]  * mV[10] * mV[15] - 
             mV[5]  * mV[11] * mV[14] - 
             mV[9]  * mV[6]  * mV[15] + 
             mV[9]  * mV[7]  * mV[14] +
             mV[13] * mV[6]  * mV[11] - 
             mV[13] * mV[7]  * mV[10];

    inv[4] = -mV[4]  * mV[10] * mV[15] + 
              mV[4]  * mV[11] * mV[14] + 
              mV[8]  * mV[6]  * mV[15] - 
              mV[8]  * mV[7]  * mV[14] - 
              mV[12] * mV[6]  * mV[11] + 
              mV[12] * mV[7]  * mV[10];

    inv[8] = mV[4]  * mV[9] * mV[15] - 
             mV[4]  * mV[11] * mV[13] - 
             mV[8]  * mV[5] * mV[15] + 
             mV[8]  * mV[7] * mV[13] + 
             mV[12] * mV[5] * mV[11] - 
             mV[12] * mV[7] * mV[9];

    inv[12] = -mV[4]  * mV[9] * mV[14] + 
               mV[4]  * mV[10] * mV[13] +
               mV[8]  * mV[5] * mV[14] - 
               mV[8]  * mV[6] * mV[13] - 
               mV[12] * mV[5] * mV[10] + 
               mV[12] * mV[6] * mV[9];

    inv[1] = -mV[1]  * mV[10] * mV[15] + 
              mV[1]  * mV[11] * mV[14] + 
              mV[9]  * mV[2] * mV[15] - 
              mV[9]  * mV[3] * mV[14] - 
              mV[13] * mV[2] * mV[11] + 
              mV[13] * mV[3] * mV[10];

    inv[5] = mV[0]  * mV[10] * mV[15] - 
             mV[0]  * mV[11] * mV[14] - 
             mV[8]  * mV[2] * mV[15] + 
             mV[8]  * mV[3] * mV[14] + 
             mV[12] * mV[2] * mV[11] - 
             mV[12] * mV[3] * mV[10];

    inv[9] = -mV[0]  * mV[9] * mV[15] + 
              mV[0]  * mV[11] * mV[13] + 
              mV[8]  * mV[1] * mV[15] - 
              mV[8]  * mV[3] * mV[13] - 
              mV[12] * mV[1] * mV[11] + 
              mV[12] * mV[3] * mV[9];

    inv[13] = mV[0]  * mV[9] * mV[14] - 
              mV[0]  * mV[10] * mV[13] - 
              mV[8]  * mV[1] * mV[14] + 
              mV[8]  * mV[2] * mV[13] + 
              mV[12] * mV[1] * mV[10] - 
              mV[12] * mV[2] * mV[9];

    inv[2] = mV[1]  * mV[6] * mV[15] - 
             mV[1]  * mV[7] * mV[14] - 
             mV[5]  * mV[2] * mV[15] + 
             mV[5]  * mV[3] * mV[14] + 
             mV[13] * mV[2] * mV[7] - 
             mV[13] * mV[3] * mV[6];

    inv[6] = -mV[0]  * mV[6] * mV[15] + 
              mV[0]  * mV[7] * mV[14] + 
              mV[4]  * mV[2] * mV[15] - 
              mV[4]  * mV[3] * mV[14] - 
              mV[12] * mV[2] * mV[7] + 
              mV[12] * mV[3] * mV[6];

    inv[10] = mV[0]  * mV[5] * mV[15] - 
              mV[0]  * mV[7] * mV[13] - 
              mV[4]  * mV[1] * mV[15] + 
              mV[4]  * mV[3] * mV[13] + 
              mV[12] * mV[1] * mV[7] - 
              mV[12] * mV[3] * mV[5];

    inv[14] = -mV[0]  * mV[5] * mV[14] + 
               mV[0]  * mV[6] * mV[13] + 
               mV[4]  * mV[1] * mV[14] - 
               mV[4]  * mV[2] * mV[13] - 
               mV[12] * mV[1] * mV[6] + 
               mV[12] * mV[2] * mV[5];

    inv[3] = -mV[1] * mV[6] * mV[11] + 
              mV[1] * mV[7] * mV[10] + 
              mV[5] * mV[2] * mV[11] - 
              mV[5] * mV[3] * mV[10] - 
              mV[9] * mV[2] * mV[7] + 
              mV[9] * mV[3] * mV[6];

    inv[7] = mV[0] * mV[6] * mV[11] - 
             mV[0] * mV[7] * mV[10] - 
             mV[4] * mV[2] * mV[11] + 
             mV[4] * mV[3] * mV[10] + 
             mV[8] * mV[2] * mV[7] - 
             mV[8] * mV[3] * mV[6];

    inv[11] = -mV[0] * mV[5] * mV[11] + 
               mV[0] * mV[7] * mV[9] + 
               mV[4] * mV[1] * mV[11] - 
               mV[4] * mV[3] * mV[9] - 
               mV[8] * mV[1] * mV[7] + 
               mV[8] * mV[3] * mV[5];

    inv[15] = mV[0] * mV[5] * mV[10] - 
              mV[0] * mV[6] * mV[9] - 
              mV[4] * mV[1] * mV[10] + 
              mV[4] * mV[2] * mV[9] + 
              mV[8] * mV[1] * mV[6] - 
              mV[8] * mV[2] * mV[5];

    det = mV[0] * inv[0] + mV[1] * inv[4] + mV[2] * inv[8] + mV[3] * inv[12];

    if (det == 0)
        return false;

    det = 1.0 / det;

   // for (i = 0; i < 16; i++)
   //     invOut[i] = inv[i] * det;

	inv[0] *= det; 
    inv[1] *= det;
    inv[2] *= det;
    inv[3] *= det;
    inv[4] *= det;
    inv[5] *= det;
    inv[6] *= det;
    inv[7] *= det;
    inv[8] *= det;
    inv[9] *= det;
    inv[10] *= det;
    inv[11] *= det;
    inv[12] *= det;
    inv[13] *= det;
    inv[14] *= det;
    inv[15] *= det; 

	return Matrix4(	inv[0],inv[1],inv[2],inv[3],
					inv[4],inv[5],inv[6],inv[7],
					inv[8],inv[9],inv[10],inv[11],
					inv[12],inv[13],inv[14],inv[15]);
	}

quat Matrix4::ToQuat() const
{
	quat		q;
	float		trace;
	float		s;
	float		t;
	int     	i;
	int			j;
	int			k;

	static int 	next[ 3 ] = { 1, 2, 0 };

	trace = mV[ 0 ] + mV[ 5 ] + mV[ 10 ];

	if ( trace > 0.0f ) {

		t = trace + 1.0f;
		s = Math::InvSqrt( t ) * 0.5f;

		q.w = s * t;
		q.x = ( mV[ 9 ] - mV[ 6 ] ) * s;
		q.y= ( mV[ 2 ] - mV[ 8 ] ) * s;
		q.z = ( mV[ 4 ] - mV[ 1 ] ) * s;

	} else {

		i = 0;
		if ( mV[ 5 ] > mV[ 0 ] ) {
			i = 1;
		}
		if ( mV[ 10 ] > mV[ i*4+i ] ) {
			i = 2;
		}
		j = next[ i ];
		k = next[ j ];

		t = ( mV[ i*4+i ] - ( mV[ j*4+ j ] + mV[ k*4+ k ] ) ) + 1.0f;
		s = Math::InvSqrt( t ) * 0.5f;

		q[i] = s * t;
		q[3] = ( mV[ k*4+ j ] - mV[ j *4+ k ] ) * s;
		q[j] = ( mV[ j *4+ i ] + mV[ i *4+ j ] ) * s;
		q[k] = ( mV[ k *4+ i ] + mV[ i *4+ k ] ) * s;
	}
	return q;
}