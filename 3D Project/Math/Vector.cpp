#include "..\pch.h"


/*
-
-
-		Vector 4 part
-
-
*/ 


void Vector4::zero()
{
	x = y = z = w = 0.0f;
}

bool Vector4::operator==(const Vector4& v) const
{
	return x==v.x && y==v.y && z==v.z && w==v.w;
}

bool Vector4::operator!=(const Vector4& v) const
{
	return !(*this==v);
}

bool Vector4::operator>(const Vector4& v) const
{
	return x>v.x && y>v.y && z>v.z && w>v.w;
}

bool Vector4::operator>=(const Vector4& v) const
{
	return x>=v.x && y>=v.y && z>=v.z && w >=v.w;
}

bool Vector4::operator<(const Vector4& v) const
{
	return x<v.x && y<v.y && z<v.z&& w <v.w;
}

bool Vector4::operator<=(const Vector4& v) const 
{
	return x<=v.x && y<=v.y && z<=v.z&& w <=v.w;
}

Vector4& Vector4::operator=(const Vector4& v)
{
	x = v.x;
	y = v.y;
	z = v.z;
	w = v.w;
	return *this;
}

Vector4 Vector4::operator+(const Vector4& v) const
{
	return Vector4(x + v.x, y + v.y, z + v.z,w+v.w);
}

Vector4 Vector4::operator-(const Vector4& v) const
{
	return Vector4(x - v.x, y - v.y, z-v.z,w-v.w);
}
Vector4 Vector4::operator-() const
{
	return Vector4(-x, -y, -z,-w);
}
Vector4 Vector4::operator*(float Scalar) const
{
	return Vector4(x*Scalar, y*Scalar, z*Scalar,w*Scalar);
}

Vector4 Vector4::operator/(float Scalar) const
{
	float oneOverScalar = 1.0f/Scalar;
	return Vector4(x*oneOverScalar, y*oneOverScalar, z*oneOverScalar,w*oneOverScalar);
}

Vector4& Vector4::operator+=(const Vector4& v)
{
	x +=v.x;
	y +=v.y;
	z +=v.z;
	w +=v.w;
	return *this;
}

Vector4& Vector4::operator-=(const Vector4& v)
{
	x -=v.x;
	y -=v.y;
	z -=v.z;
	w -=v.w;
	return *this;
}

Vector4& Vector4::operator*=(float Scalar)
{
	x *= Scalar;
	y *= Scalar;
	z *= Scalar;
	w *= Scalar;
	return *this;
}

Vector4& Vector4::operator/=(float Scalar)
{
	float oneOverScalar = 1.0f/Scalar;
	x *=oneOverScalar;
	y *=oneOverScalar;
	z *=oneOverScalar;
	w *=oneOverScalar;
	return *this;
}

float Vector4::Length() const
{
	return sqrtf(x*x + y*y + z*z + w*w);
}

float Vector4::SquareLength() const
{
	return x*x + y*y +z*z + w*w;
}

float Vector4::Dot(const Vector4& v) const
{
	return (x*v.x + y*v.y + z*v.z + w*v.w);
}

float Vector4::Normalize()
{
	float S = x*x + y*y +z*z + w*w;
	if(S > 0.0f)
	{
		float oneover =  1.0f/sqrtf(S);
		x *= oneover;
		y *= oneover;
		z *= oneover;
		w *= oneover;
		return S* oneover;
	}
	return 0.0f;
}



std::ostream&  operator<<(std::ostream& os,const Vector4& v )
{
	os<< '<' << v.x << ',' << v.y << ',' << v.z << ',' << v.w << '>';
	return os;
}

std::ostream&  operator<<(const Vector4& v ,std::ostream& os)
{
	return os;
}


/*
-
-
-		Vector3 part
-
-
*/ 

Vector3::Vector3()
{

}


Vector3::~Vector3()
{
}


void Vector3::zero()
{
	x = y = z = 0.0f;
}

bool Vector3::operator==(const Vector3& v) const
{
	return x==v.x && y==v.y && z==v.z;
}

bool Vector3::operator!=(const Vector3& v) const
{
	return !(*this==v);
}

bool Vector3::operator>(const Vector3& v) const
{
	return x>v.x && y>v.y && z>v.z;
}

bool Vector3::operator>=(const Vector3& v) const
{
	return x>=v.x && y>=v.y && z>=v.z;
}

bool Vector3::operator<(const Vector3& v) const
{
	return x<v.x && y<v.y && z<v.z;
}

bool Vector3::operator<=(const Vector3& v) const 
{
	return x<=v.x && y<=v.y && z<=v.z;
}

Vector3& Vector3::operator=(const Vector3& v)
{
	x = v.x;
	y = v.y;
	z = v.z;
	return *this;
}

Vector3 Vector3::operator+(const Vector3& v) const
{
	return Vector3(x + v.x, y + v.y, z + v.z);
}

Vector3 Vector3::operator-(const Vector3& v) const
{
	return Vector3(x - v.x, y - v.y, z-v.z);
}
Vector3 Vector3::operator-() const
{
	return Vector3(-x, -y, -z);
}
Vector3 Vector3::operator*(float Scalar) const
{
	return Vector3(x*Scalar, y*Scalar, z*Scalar);
}

Vector3 Vector3::operator/(float Scalar) const
{
	float oneOverScalar = 1.0f/Scalar;
	return Vector3(x*oneOverScalar, y*oneOverScalar, z*oneOverScalar);
}

Vector3 Vector3::operator/(const Vector3 & v) const
{
	Vector3 result = Vector3(0.0f);
	if (v.x != 0) result.x = x / v.x;
	if (v.y != 0) result.y = y / v.y;
	if (v.z != 0) result.z = z / v.z;
	return result;
}

Vector3 Vector3::operator*(const Vector3 & v) const
{
	return Vector3(x*v.x,y*v.y,z*v.z);
}

Vector3& Vector3::operator+=(const Vector3& v)
{
	x +=v.x;
	y +=v.y;
	z +=v.z;
	return *this;
}

Vector3& Vector3::operator-=(const Vector3& v)
{
	x -=v.x;
	y -=v.y;
	z -=v.z;
	return *this;
}

Vector3& Vector3::operator*=(float Scalar)
{
	x *= Scalar;
	y *= Scalar;
	z *= Scalar;
	return *this;
}

Vector3& Vector3::operator/=(float Scalar)
{
	float oneOverScalar = 1.0f/Scalar;
	x *=oneOverScalar;
	y *=oneOverScalar;
	z *=oneOverScalar;
	return *this;
}

float & Vector3::operator[](int index)
{
	// TODO:
	return (&x)[index];
}

float Vector3::operator[](int index) const
{
	return (&x)[index];
}

float Vector3::Length() const
{
	return sqrtf(x*x + y*y + z*z);
}

float Vector3::SquareLength() const
{
	return x*x + y*y +z*z;
}

float Vector3::Dot(const Vector3& v) const
{
	return (x*v.x + y*v.y + z*v.z);
}

float Vector3::Normalize()
{
	float S = x*x + y*y +z*z;
	if(S > 0.0f)
	{
		float oneover =  1.0f/sqrtf(S);
		x *= oneover;
		y *= oneover;
		z *= oneover;
		return S* oneover;
	}
	return 0.0f;
}

Vector3 Vector3::Cross(const Vector3& v) const
{
	return Vector3(
		y*v.z - z*v.y,
		z*v.x - x*v.z,
		x*v.y - y*v.x);
}

Vector3 Vector3::Abs()
{
	return Vector3(fabs(x),fabs(y),fabs(z));
}


std::ostream&  operator<<(std::ostream& os,const Vector3& v )
{
	os<< '<' << std::setiosflags(std::ios::showpoint)<<std::setprecision(10)<< v.x << ',' <<std::setprecision(10)<< v.y << ','<<std::setprecision(10) << v.z << '>';
	return os;
}

std::ostream&  operator<<(const Vector3& v ,std::ostream& os)
{
	return os;
}


/*
-
-
-		Vector3i part
-
-
*/

Vector3i::Vector3i()
{

}


Vector3i::~Vector3i()
{
}


void Vector3i::zero()
{
	x = y = z = 0;
}

bool Vector3i::operator==(const Vector3i& v) const
{
	return x == v.x && y == v.y && z == v.z;
}

bool Vector3i::operator!=(const Vector3i& v) const
{
	return !(*this == v);
}

bool Vector3i::operator>(const Vector3i& v) const
{
	return x>v.x && y>v.y && z>v.z;
}

bool Vector3i::operator>=(const Vector3i& v) const
{
	return x >= v.x && y >= v.y && z >= v.z;
}

bool Vector3i::operator<(const Vector3i& v) const
{
	return x<v.x && y<v.y && z<v.z;
}

bool Vector3i::operator<=(const Vector3i& v) const
{
	return x <= v.x && y <= v.y && z <= v.z;
}

Vector3i& Vector3i::operator=(const Vector3i& v)
{
	x = v.x;
	y = v.y;
	z = v.z;
	return *this;
}

Vector3i Vector3i::operator+(const Vector3i& v) const
{
	return Vector3i(x + v.x, y + v.y, z + v.z);
}

Vector3i Vector3i::operator-(const Vector3i& v) const
{
	return Vector3i(x - v.x, y - v.y, z - v.z);
}
Vector3i Vector3i::operator-() const
{
	return Vector3i(-x, -y, -z);
}
Vector3i Vector3i::operator*(int Scalar) const
{
	return Vector3i(x*Scalar, y*Scalar, z*Scalar);
}

Vector3i Vector3i::operator/(int Scalar) const
{
	float oneOverScalar = 1.0f / Scalar;
	return Vector3i(x*oneOverScalar, y*oneOverScalar, z*oneOverScalar);
}

Vector3i& Vector3i::operator+=(const Vector3i& v)
{
	x += v.x;
	y += v.y;
	z += v.z;
	return *this;
}

Vector3i& Vector3i::operator-=(const Vector3i& v)
{
	x -= v.x;
	y -= v.y;
	z -= v.z;
	return *this;
}

Vector3i& Vector3i::operator*=(int Scalar)
{
	x *= Scalar;
	y *= Scalar;
	z *= Scalar;
	return *this;
}

Vector3i& Vector3i::operator/=(int Scalar)
{
	float oneOverScalar = 1.0f / Scalar;
	x *= oneOverScalar;
	y *= oneOverScalar;
	z *= oneOverScalar;
	return *this;
}

int & Vector3i::operator[](int index)
{
	// TODO:
	return (&x)[index];
}

int Vector3i::operator[](int index) const
{
	return (&x)[index];
}

float Vector3i::Length() const
{
	return sqrtf(x*x + y*y + z*z);
}

int Vector3i::SquareLength() const
{
	return x*x + y*y + z*z;
}


int Vector3i::Dot(const Vector3i& v) const
{
	int result = x*v.x + y*v.y + z*v.z;
	return (result);
}



std::ostream&  operator<<(std::ostream& os, const Vector3i& v)
{
	os << '<' <<  v.x << ','<< v.y << ',' << v.z << '>';
	return os;
}

std::ostream&  operator<<(const Vector3i& v, std::ostream& os)
{
	return os;
}


/*
-
-
-		Vector 2 part
-
-
*/ 


void Vector2::zero()
{
	x = y = 0.0f;
}

bool Vector2::operator==(const Vector2& v) const
{
	return x==v.x && y==v.y;
}

bool Vector2::operator!=(const Vector2& v) const
{
	return !(*this==v);
}

bool Vector2::operator>(const Vector2& v) const
{
	return x>v.x && y>v.y;
}

bool Vector2::operator>=(const Vector2& v) const
{
	return x>=v.x && y>=v.y;
}

bool Vector2::operator<(const Vector2& v) const
{
	return x<v.x && y<v.y ;
}

bool Vector2::operator<=(const Vector2& v) const 
{
	return x<=v.x && y<=v.y;
}

Vector2& Vector2::operator=(const Vector2& v)
{
	x = v.x;
	y = v.y;
	return *this;
}

Vector2 Vector2::operator+(const Vector2& v) const
{
	return Vector2(x + v.x, y + v.y);
}

Vector2 Vector2::operator-(const Vector2& v) const
{
	return Vector2(x - v.x, y - v.y);
}
Vector2 Vector2::operator-() const
{
	return Vector2(-x, -y);
}
Vector2 Vector2::operator*(float Scalar) const
{
	return Vector2(x*Scalar, y*Scalar );
}

Vector2 Vector2::operator/(float Scalar) const
{
	float oneOverScalar = 1.0f/Scalar;
	return Vector2(x*oneOverScalar, y*oneOverScalar);
}

Vector2& Vector2::operator+=(const Vector2& v)
{
	x +=v.x;
	y +=v.y;
	return *this;
}

Vector2& Vector2::operator-=(const Vector2& v)
{
	x -=v.x;
	y -=v.y;
	return *this;
}

Vector2& Vector2::operator*=(float Scalar)
{
	x *= Scalar;
	y *= Scalar;
	return *this;
}

Vector2& Vector2::operator/=(float Scalar)
{
	float oneOverScalar = 1.0f/Scalar;
	x *=oneOverScalar;
	y *=oneOverScalar;
	return *this;
}

float Vector2::Length() const
{
	return sqrtf(x*x + y*y);
}

float Vector2::SquareLength() const
{
	return x*x + y*y;
}

float Vector2::Dot(const Vector2& v) const
{
	return (x*v.x + y*v.y);
}

float Vector2::Normalize()
{
	float S = x*x + y*y ;
	if(S > 0.0f)
	{
		float oneover =  1.0f/sqrtf(S);
		x *= oneover;
		y *= oneover;
		return S* oneover;
	}
	return 0.0f;
}



std::ostream&  operator<<(std::ostream& os,const Vector2& v )
{
	os<< '<' << v.x << ',' << v.y << ',' << '>';
	return os;
}

std::ostream&  operator<<(const Vector2& v ,std::ostream& os)
{
	return os;
}