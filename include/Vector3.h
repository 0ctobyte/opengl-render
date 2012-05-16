//Copyright (c) 2012 Sekhar Bhattacharya
//Licensed under the MIT license
//See license.txt

#ifndef __VECTOR3__
#define __VECTOR3__

#include <GL/glew.h>
#include <SFML/Window.hpp>
#include <string>

//!@brief Defines a 3D vector.
//!
//!Create a 3D vector. Manipulate vectors using vector operations.
//!The w coordinate is here just for the sake of matrix multiplication.
//!Otherwise it has no effect on the way this class operates; this is still
//!a 3D vector class.
//!@note All overloaded assignment operators (i.e. =, +=, -=, ...) return
//!a reference to a vector object so that the operators can be chained
struct Vector3 {

	GLfloat x; //!<Represents the x component of the vector 
	GLfloat y; //!<Represents the y component of the vector
	GLfloat z; //!<Represents the z component of the vector
	GLfloat w; //!<The homogeneous coordinate

	//!@brief Create a default "zero" vector.
	Vector3();
	
	//!@brief Create a vector with the specified coordinates, with
	//!w=1
	//!@param [in] x - The x coordinate
	//!@param [in] y - The y coordinate
	//!@param [in] z - The z coordinate
	Vector3(GLfloat x, GLfloat y, GLfloat z);

	//!@brief Create a vector with the specified coordinates
	//!@param [in] x - The x coordinate
	//!@param [in] y - The y coordinate
	//!@param [in] z - The z coordinate
	//!@param [in] w - The homogenous coordinate.
	Vector3(GLfloat x, GLfloat y, GLfloat z, GLfloat w);

	//!@brief Create a vector with the same coordinates as the specified vector
	//!@param [in] v - A reference to another vector
	Vector3(const Vector3 &v);

	//!@brief Calculates the distance between two vectors.
	//!@param [in] v - A reference to the other vector
	//!@return The distance between the two vectors
	GLfloat Distance(const Vector3 &v) const;

	//!@brief Converts the vector to a unit vector.
	//
	//!A normalized vector has a length of exactly 1
	//!@return The normalized vector.
	//!@note This method does not modify the vectors data.
	//!It instead returns another vector.
	Vector3 Normalize() const;

	//!@brief Calculates the Magnitude/length of the vector.
	//!@return The magnitude/length of the vector.
	GLfloat Length() const;

	//!@brief Calculates the angle between two vectors using the dot product.
	//!@param [in] v - A reference to another vector
	//!@return The angle in radians.
	GLfloat Angle(const Vector3 &v) const;

	//!@brief Calculates the cross product between the two vectors.
	//!@param [in] v - A reference to the other vector
	//!@return The vector resulting from the cross product.
	Vector3 CrossProduct(const Vector3 &v) const;

	//!@brief Adds two vectors together.
	//!@param [in] v - A reference to the vector to be added too
	//!@return The resultant vector
	Vector3 operator+(const Vector3 &v) const;

	//!@brief Subtracts the specified vector from this vector
	//!@param [in] v - A reference to the subtrahend vector
	//!@return The resultant vector
	Vector3 operator-(const Vector3 &v) const;

	//!@brief Calculates the dot product between the two vectors
	//!@param [in] v - The vector to calculate the dot product with
	//!@return The dot product
	GLfloat operator*(const Vector3 &v) const;

	//!@brief Multiplies the current vector with the scalar
	//!@param [in] k - The scalar to multiply with
	//!@return The scaled vector
	Vector3 operator*(GLfloat k) const;

	//!@brief Divides the current vector with the scalar
	//!@param [in] k - The scalar to divide by
	//!@return The scaled vector
	Vector3 operator/(GLfloat k) const;

	//!@brief Negates the current vector
	//!@return The negated vector
	Vector3 operator-() const;

	//!@brief Assigns a vector to this vector. The w coordinate
	//!is copied.
	//!@param [in] v - A reference to the vector to copy
	//!@return A reference to the new vector
	Vector3& operator=(const Vector3 &v);

	//!@brief Checks if two vectors are equal, does not consider the w
	//!coordinate
	//!@param [in] v - A reference to the vector to compare with
	//!@return True if equal or false if not equal
	GLboolean operator==(const Vector3 &v) const;

	//!@brief Checks if two vectors are not equal, does not consider
	//!the w coordinate
	//!@param [in] v - A reference to the vector to compare with
	//!@return True if not equal or false if equal
	GLboolean operator!=(const Vector3 &v) const;

	//!@brief Adds the two vectors than assigns the resulting vector to itself
	//!@param [in] v - A reference to the vector to be added
	//!@return A reference to the resultant vector
	Vector3& operator+=(const Vector3 &v);

	//!@brief Subtracts a vector from this vector and assigns the result to 
	//!itself
	//!@param [in] v - A reference to the subtrahend vector
	//!@return A reference to the resultant vector
	Vector3& operator-=(const Vector3 &v);

	//!@brief Multiplies the vector with a scalar and assigns the result to 
	//!itself
	//!@param [in] k - The scalar to be multiplied with
	//!@return A reference to the scaled vector
	Vector3& operator*=(GLfloat k);

	//!@brief Divides the vector with a scalar and assigns the result to itself
	//!@param [in] k - The scalar to divide by
	//!@return A reference to the scaled vector
	Vector3& operator/=(GLfloat k);

	//!@brief Describes the vector
	//!@return A string containing the x, y, and z coordinates
	const std::string ToString() const; 
};

#endif // __VECTOR3__
