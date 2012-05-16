//Copyright (c) 2012 Sekhar Bhattacharya
//Licensed under the MIT license
//See license.txt

#ifndef __SHADER__
#define __SHADER__

#include <GL/glew.h>
#include <SFML/Window.hpp>
#include <string>
#include <fstream>
#include <vector>

//!@brief Creates a shader program
struct Shader {

	std::vector<GLuint> shaderObjects; //!<A list of handles to shader objects
	GLuint program; //!<A handle to the shader program
	std::string errString; //!<Stores the error message

	//!@brief Loads multiple shaders from the specified file, compiles and
	//!links them
	//!
	//!The file must wrap shader code with "#define SHADERTPE #ifdef SHADERTYPE
	//!#endif" statements, where SHADERTYPE is __VERTEX/__FRAGMENT/__GEOMETRY
	//!@param [in] filename - The name of the shader file
	//!@return True if file was opened successfully, false otherwise
	GLboolean Open(const std::string &filename);

	//!@brief Loads multiple shaders from the specified files, compiles 
	//!and links them
	//!
	//!At the least a vertex and fragment shader must be specified. Geometry 
	//!shaders are optional. Each file holds code for one type of shader.
	//!@param [in] vertfile - Filename for the vertex shader
	//!@param [in] fragfile - Filename for the fragment shader.
	//!@param [in] geofile - Filename for the geometry shader. Default is 
	//!empty string.
	//!@return True if files was opened successfully, false otherwise 
	GLboolean Open(const std::string &vertfile, const std::string &fragfile,
			const std::string &geofile=std::string());
	
	//!@brief Loads a shader file containing multiple shaders
	//!
	//!Use this to load shader code from files with multiple shaders.
	//!The files must wrap shader code with "#define SHADERTYPE 
	//!#ifdef SHADETRTYPE #endif" statements, where SHADERTYPE is 
	//!__VERTEX/__FRAGMENT/__GEOMETRY, for this method to work.
	//!@param [in] file - Reference to the shader file
	//!@param [in] shaderType - The type of shader to load from the file
	//!either vertex, geometry, or fragment
	//!@return A string with the contents of the specified shader from the file
	const std::string LoadShaderFile(std::ifstream &file, GLenum shaderType);

	//!@brief Loads a shader file containing a single shader
	//!@param [in] file - Reference to the shader file
	//!@return A string containing the contents of the shader file
	const std::string LoadShaderFile(std::ifstream &file);
	
	//!@brief Creates a shader object
	//!@param [in] shaderStr - String containing the shader code
	//!@param [in] shaderType - The type of shader to create 
	//!(vertex/fragment/geo)
	//!@return The handle to the shader object
	GLuint CompileShaderObject(const std::string &shaderStr,
			GLenum shaderType);

	//!@brief Creates a shader program with the shader objects
	//!
	//!CompileShader must have been called at least once for this method to 
	//!have any effect
	//!@return The handle to the shader program
	GLuint LinkShaderObjects();

	//!@brief Deletes the list of shader objects
	GLvoid DeleteShaderObjects();

	//!@brief Deletes the shader program and all shader objects
	GLvoid Close();

	//!@brief Calls Close()
	~Shader();
};

#endif //__SHADER__
