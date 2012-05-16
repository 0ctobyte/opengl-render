//Copyright (c) 2012 Sekhar Bhattacharya
//Licensed under the MIT license
//See license.txt

#include <cstring>
#include <cmath>
#include <sstream>
#include <iomanip>

#include <Matrix4.h>

#define PI 3.14159265358979323846264338327950288419716939937510582097494459231f

using namespace std;

Matrix4::Matrix4()
{
	for(GLuint i=0; i<16; i++) mat[i]=((i%5==0) ? 1.0f : 0.0f);
}

Matrix4& Matrix4::LoadIdentity()
{
	for(GLuint i=0; i<16; i++) mat[i]=((i%5==0) ? 1.0f : 0.0f);
	return(*this);
}

Matrix4& Matrix4::Frustum(GLfloat left, GLfloat right, GLfloat bottom, 
		GLfloat top, GLfloat znear, GLfloat zfar)
{
	Matrix4 m;
	m.mat[0] = (2.0f*znear)/(right-left);
	m.mat[5] = (2.0f*znear)/(top-bottom);
	m.mat[8] = (right+left)/(right-left);
	m.mat[9] = (top+bottom)/(top-bottom);
	m.mat[10] = -((zfar+znear)/(zfar-znear));
	m.mat[11] = -1.0f;
	m.mat[14] = -((2.0f*zfar*znear)/(zfar-znear));
	m.mat[15] = 0.0f;
	(*this)*=m;
	return(*this);
}

Matrix4& Matrix4::Perspective(GLfloat fovy, GLfloat faspect, GLfloat znear,
		GLfloat zfar)
{
	Matrix4 m;
	GLfloat f = 1.0f/(tan((fovy*(PI/180.0f))/2.0f));
	m.mat[0]=f/faspect;
	m.mat[5]=f;
	m.mat[10]=(zfar+znear)/(znear-zfar);
	m.mat[11]=-1.0f;
	m.mat[14]=(2.0f*zfar*znear)/(znear-zfar);
	m.mat[15]=0.0f;
	(*this)*=m;
	return(*this);
}

Matrix4& Matrix4::Orthographic(GLfloat left, GLfloat right, GLfloat bottom,
		GLfloat top, GLfloat znear, GLfloat zfar)
{
	Matrix4 m;
	m.mat[0] = 2.0f/(right-left);
	m.mat[5] = 2.0f/(top-bottom);
	m.mat[10] = -2.0f/(zfar-znear);
	m.mat[12] = -((right+left)/(right-left));
	m.mat[13] = -((top+bottom)/(top-bottom));
	m.mat[14] = -((zfar+znear)/(zfar-znear));
	(*this)*=m;
	return(*this);
}

Matrix4& Matrix4::Translate(GLfloat x, GLfloat y, GLfloat z)
{
	Matrix4 m;
	m.mat[12] = x, m.mat[13] = y, m.mat[14] = z;
	(*this)*=m;
	return(*this);
}

Matrix4& Matrix4::Translate(const Vector3 &v)
{
	Matrix4 m;
	m.mat[12] = v.x, m.mat[13] = v.y, m.mat[14] = v.z;
	(*this)*=m;
	return(*this);
}

Matrix4 Matrix4::Untranslate() const
{
	Matrix4 m = (*this);
	m.mat[12] = 0.0f, m.mat[13] = 0.0f, m.mat[14] = 0.0f;
	return m;
}

Matrix4 Matrix4::Transpose() const
{
	Matrix4 m;
	for(GLuint c=0; c<4; c++)
		for(GLuint r=0; r<4; r++)
			m.mat[r*4+c]=mat[c*4+r];
	return(m);
}

Matrix4 Matrix4::Inverse() const
{
	Matrix4 m;
	GLfloat det = Determinant();
	if(fabs(det) < 0.000001f) return(m);
	for(GLuint c=0; c<4; c++)
		for(GLuint r=0; r<4; r++)
			m.mat[c*4+r]=Cofactor(c, r)/det;
	return(m.Transpose());
}

GLfloat Matrix4::Cofactor(GLuint column, GLuint row) const
{
	if((GLint)column<0 || column>3 || (GLint)row<0 || row>3) return(0.0f);
	GLfloat cofactor = 0.0f;
	for(GLuint c=0; c<4; c++)
	{
		if(c==column) continue;
		GLfloat pfactor=1, nfactor=1;
		for(GLint pcc=c, ncc=c, r=0; r<4; r++)
		{
			if(r==(GLint)row) continue;
			if(pcc==(GLint)column) pcc++;
			if(pcc>3) pcc=(((GLint)column==0) ? 1 : 0);
			if(ncc==(GLint)column) ncc--;
			if(ncc<0) ncc=(((GLint)column==3) ? 2 : 3);
			pfactor*=mat[pcc*4+r];
			nfactor*=mat[ncc*4+r];
			pcc++, ncc--;
		}
		cofactor+=pfactor-nfactor;
	}
	return(((row+column)%2==0) ? cofactor : -1*cofactor);
}

GLfloat Matrix4::Determinant() const
{
	//Basically, we have to take an arbitrary row (or column), in this case we
	//took the fourth row of the matrix, and multiply each element in that row
	//by it's signed cofactor, which is found by calculating the determinant of
	//the minor of that element, and then summing up all the values.
	return(mat[3]*Cofactor(0, 3)+mat[7]*Cofactor(1, 3)+mat[11]*Cofactor(2, 3)
			+mat[15]*Cofactor(3, 3));
}

Vector3 Matrix4::operator*(const Vector3 &v) const
{
	Vector3 u;
	u.x = mat[0]*v.x+mat[4]*v.y+mat[8]*v.z+mat[12]*v.w;
	u.y = mat[1]*v.x+mat[5]*v.y+mat[9]*v.z+mat[13]*v.w;
	u.z = mat[2]*v.x+mat[6]*v.y+mat[10]*v.z+mat[14]*v.w;
	u.w = mat[3]*v.x+mat[7]*v.y+mat[11]*v.z+mat[15]*v.w;
	return(u);
}

Matrix4 Matrix4::operator*(const Matrix4 &m) const
{
	Matrix4 a;
	for(GLuint i=0, offset=0, ii=0; i<16; i++, ii++)
	{
		a.mat[i]=0, offset=((i%4==0) ? i : offset), ii=((ii>3) ? 0 : ii);
		for(GLuint j=0, mult=0; j<4; mult++, j++)
		{
			a.mat[i]+=mat[ii+mult*4]*m.mat[offset+j];
		}
	}
	return(a);
}

Matrix4& Matrix4::operator=(const Matrix4 &m)
{
	memmove(mat, m.mat, 16*sizeof(GLfloat));
	return(*this);
}

Matrix4& Matrix4::operator*=(const Matrix4 &m)
{
	(*this) = (*this)*m;
	return(*this);
}

const string Matrix4::ToString() const
{
	ostringstream s(ostringstream::out);

	s<<left;
	s<<"| "<<setw(10)<<mat[0]<<setw(10)<<mat[4]<<setw(10);
	s<<mat[8]<<setw(10)<<mat[12]<<" |"<<endl; 

	s<<"| "<<setw(10)<<mat[1]<<setw(10)<<mat[5]<<setw(10);
   	s<<mat[9]<<setw(10)<<mat[13]<<" |"<<endl;

	s<<"| "<<setw(10)<<mat[2]<<setw(10)<<mat[6]<<setw(10);
	s<<mat[10]<<setw(10)<<mat[14]<<" |"<<endl; 

	s<<"| "<<setw(10)<<mat[3]<<setw(10)<<mat[7]<<setw(10);
	s<<mat[11]<<setw(10)<<mat[15]<<" |"; 

	return(s.str());
}
