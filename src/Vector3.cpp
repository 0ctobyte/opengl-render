//Copyright (c) 2012 Sekhar Bhattacharya
//Licensed under the MIT license
//See license.txt

#include <cmath>
#include <sstream>
#include <iomanip>

#include <Vector3.h>

using namespace std;

Vector3::Vector3()
{
	x=0, y=0, z=0, w=1;
}

Vector3::Vector3(GLfloat x, GLfloat y, GLfloat z)
{
	this->x=x, this->y=y, this->z=z, this->w=1;
}

Vector3::Vector3(GLfloat x, GLfloat y, GLfloat z, GLfloat w)
{
	this->x=x, this->y=y, this->z=z, this->w=w;
}

Vector3::Vector3(const Vector3 &v)
{
	*this = v;
}

GLfloat Vector3::Distance(const Vector3 &v) const
{
	return((*this - v).Length());
}

Vector3 Vector3::Normalize() const
{
	GLfloat k = this->Length();
	return(Vector3(x/k, y/k, z/k));
}

GLfloat Vector3::Length() const
{
	return(sqrt(x*x+y*y+z*z));
}

GLfloat Vector3::Angle(const Vector3 &v) const
{
	return(acos(((*this)*v)/(this->Length()*v.Length())));
}

Vector3 Vector3::CrossProduct(const Vector3 &v) const
{
	return(Vector3(y*v.z-z*v.y, z*v.x-x*v.z, x*v.y-y*v.x));
}

Vector3 Vector3::operator+(const Vector3 &v) const
{
	return(Vector3(x+v.x, y+v.y, z+v.z));
}

Vector3 Vector3::operator-(const Vector3 &v) const
{
	return(Vector3(x-v.x, y-v.y, z-v.z));
}

GLfloat Vector3::operator*(const Vector3 &v) const
{
	return(x*v.x+y*v.y+z*v.z);
}

Vector3 Vector3::operator*(GLfloat k) const
{
	return(Vector3(x*k, y*k, z*k));
}

Vector3 Vector3::operator-() const
{
	return(Vector3(-x, -y, -z));
}

Vector3& Vector3::operator=(const Vector3 &v)
{
	x=v.x, y=v.y, z=v.z, w=v.w;
	return(*this);
}

GLboolean Vector3::operator==(const Vector3 &v) const
{
	return(x==v.x && y==v.y && z==v.z);
}

GLboolean Vector3::operator!=(const Vector3 &v) const
{
	return(x!=v.x || y!=v.y || z!=v.z);
}

Vector3& Vector3::operator+=(const Vector3 &v)
{
	x+=v.x, y+=v.y, z+=v.z;
	return(*this);
}

Vector3& Vector3::operator-=(const Vector3 &v)
{
	x-=v.x, y-=v.y, z-=v.z;
	return(*this);
}

Vector3& Vector3::operator*=(GLfloat k)
{
	x*=k, y*=k, z*=k;
	return(*this);
}

Vector3& Vector3::operator/=(GLfloat k)
{
	x/=k, y/=k, z/=k;
	return(*this);
}

const string Vector3::ToString() const
{
	ostringstream s(ostringstream::out);
	s<<left;
	s<<"["<<setw(10)<<x<<", "<<setw(10)<<y<<", "<<setw(10)<<z;
	s<<", "<<setw(10)<<w<<"]";
	return(s.str());
}
