//Copyright (c) 2012 Sekhar Bhattacharya
//Licensed under the MIT license
//See license.txt

#ifndef __MATRIX4__
#define __MATRIX4__

#include <GL/glew.h>
#include <SFML/Window.hpp>
#include <string>

#include <Vector3.h>

//!@brief A standard 4x4 homogeneous matrix
struct Matrix4 {

	//@!brief The representation of the matrix
	//!
	//!The fourth column usually contains 0, 0, 0, 1 (unless a translation is
	//!specified).
	//!This matrix can be used to perform affine transformations (rotation, 
	//!scaling, translation)
	//!@note OpenGl prefers column major matrices.
	GLfloat mat[16]; //!<The matrix storing 4 rows and 4 columns

	//!@brief Constructs a new identity matrix
	Matrix4();

	//!@brief Sets the current matrix to an identity matrix
	//!
	//!An identity matrix does nothing. It is equivalent to multiplying by 1
	//!This method essentially erases the current matrix
	//!@return A reference to the identity matrix
	Matrix4& LoadIdentity();

	//!@brief Multiplies the matrix with a perspective projection matrix
	//!@param [in] left - Left vertical clipping plane
	//!@param [in] right - Right vertical clipping plane
	//!@param [in] bottom - Bottom horizontal clipping plane
	//!@param [in] top - Top horizontal clipping plane
	//!@param [in] znear - Near depth clipping plane (always positive)
	//!@param [in] zfar - Far depth clipping plane (always positive)
	//!@return A reference to the matrix producing the specified perspective 
	//!projection
	Matrix4& Frustum(GLfloat left, GLfloat right, GLfloat bottom,
			GLfloat top, GLfloat znear, GLfloat zfar);

	//!@brief Multiplies the matrix with a perspective projection matrix
	//!@param [in] fovy - Field of view angle, in degrees, in the y direction
	//!@param [in] faspect - Aspect ratio, which determines field of view 
	//!in x direction. Aspect ratio is width/height.
	//!@param [in] znear - Near depth clipping plane (always positive)
	//!@param [in] zfar - Far depth clipping plane (always positive)
	//!@return A reference to the matrix producing the specified perspective
	//!projection
	Matrix4& Perspective(GLfloat fovy, GLfloat faspect, GLfloat znear,
			GLfloat zfar);

	//!@brief Multiplies the matrix with an orthographic projection matrix
	//!@param [in] left - Left vertical clipping plane
	//!@param [in] right - Right vertical clipping plane
	//!@param [in] bottom - Bottom horizontal clipping plane
	//!@param [in] top - Top horizontal clipping plane
	//!@param [in] znear - Near depth clipping plane (-ve if behind viewer)
	//!@param [in] zfar - Far depth clipping plane (-ve if behind viewer)
	//!@return A reference to the matrix producing the specified orthographic 
	//!projection
	Matrix4& Orthographic(GLfloat left, GLfloat right, GLfloat bottom, 
			GLfloat top, GLfloat znear, GLfloat zfar);

	//!@brief Multiplies the matrix with a translation matrix created from the
	//!specified coordinates
	//!@param [in] x - The x coordinate of the translation vector
	//!@param [in] y - The y coordinate of the translation vector
	//!@param [in] z - The z coordinate of the translation vector
	//!@return A reference to the matrix producing the specified translation
	Matrix4& Translate(GLfloat x, GLfloat y, GLfloat z);

	//!@brief Mutiplies the matrix with a translation matrix created using the
	//!specified Vector3 object
	//!@param [in] v - The translation vector
	//!@return A reference to the matrix producing the specified translation
	Matrix4& Translate(const Vector3 &v);
	
	//!@brief Returns a matrix with the translation portion zeroed out
	//!@return A matrix equivalent to this matrix but without translation
	Matrix4 Untranslate() const;

	//!@brief calculates the transpose of the matrix
	//!
	//!A transposed matrix is a matrix where each column of the
	//!original matrix becomes the row for the transposed matrix.
	//!@return The transposed matrix
	Matrix4 Transpose() const;
	
	//!@brief Calculates the inverse of the matrix. This is useful for using
	//!the matrix as a camera/view transformation.
	//!@return The inverted matrix
	//!@note This does not affect the current matrix's data, instead it returns
	//!a new one with the inverted data.
	Matrix4 Inverse() const;

	//!@brief Calculates the cofactor of the element in the matrix at the 
	//!intersection of the specified column and row.
	//!
	//!The cofactor, of a square matrix, at a given column and row, is the
	//!signed determinant of the corresponding minor. The minor for a 4x4
	//!matrix is the 3x3 matrix resulting from the deletion of the specified
	//!column and row.
	//!@param [in] column - The column where the element is located
	//!@param [in] row - The row where the element is located
	//!@return The cofactor
	GLfloat Cofactor(GLuint column, GLuint row) const;

	//!@brief Calculates the determinant of the matrix. Needed to check if the
	//!matrix can be inverted.
	//!
	//!Only square matrices can have a determinant. If the determinant is 0,
	//!then the matrix contains a projection. If it is negative, the matrix
	//!contains a reflection. Also, a matrix is only invertible if the 
	//!magnitude of the matrix is greater than 0. If it is equal to 0, then it
	//!is non-invertible.
	//!@return The determinant of the matrix
	GLfloat Determinant() const;
	
	//!@brief Multiplies the current matrix with the vector
	//!@param [in] v - The vector to multiply with
	//!@return The transformed vector
	Vector3 operator*(const Vector3 &v) const;

	//!@brief Multiplies the current matrix with the specified matrix
	//!@param [in] m - The matrix to multiply with
	//!@return The new matrix
	Matrix4 operator*(const Matrix4 &m) const;

	//!@brief Assigns a matrix to this matrix
	//!@param [in] m - A reference to the matrix to copy
	//!@return A reference to the new matrix
	Matrix4& operator=(const Matrix4 &m);

	//!@brief Multiplies the two matrices and assigns the resultant matrix 
	//!to itself
	//!@param [in] m - A reference to the matrix to be multiplied with
	//!@return A reference to the resultant matrix
	Matrix4& operator*=(const Matrix4 &m);

	//!@brief Describes the matrix
	//!@return A string containing the contents of the matrix
	const std::string ToString() const;
};

#endif // __MATRIX4__
