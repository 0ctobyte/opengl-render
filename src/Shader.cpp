//Copyright (c) 2012 Sekhar Bhattacharya
//Licensed under the MIT license
//See license.txt

#include <fstream>
#include <sstream>

#include <Shader.h>

using namespace std;

GLboolean Shader::Open(const string &filename)
{
	//Deallocate previously used resources, if any exist
	Close();

	//Open the file. Check if file exists and can be opened
	ifstream file(filename.c_str());
	if(!file.is_open() || !file.good()) {file.close(); return(false);}

	//Load shader code from the file
	string vertex = LoadShaderFile(file, GL_VERTEX_SHADER);
	string geometry = LoadShaderFile(file, GL_GEOMETRY_SHADER);
	string fragment = LoadShaderFile(file, GL_FRAGMENT_SHADER);

	//Close that shit!
	file.close();

	//Compile each of the shaders, if they exist
	if(!vertex.empty())
	{
		if(CompileShaderObject(vertex, GL_VERTEX_SHADER) == 0) 
			return(false);
	}
	if(!geometry.empty())
	{
		if(CompileShaderObject(geometry, GL_GEOMETRY_SHADER) == 0) 
			return(false);
	}
	if(!fragment.empty())
	{
		if(CompileShaderObject(fragment, GL_FRAGMENT_SHADER) == 0) 
			return(false);
	}

	//Link the shaders to create the program. Delete the shader objects
	//as they're useless after linking.
	if(LinkShaderObjects() == 0) return(false);
	DeleteShaderObjects();
	
	return(true);
}

GLboolean Shader::Open(const string &vertfile, const string &fragfile,
		const string &geofile)
{
	//Deallocate previously used resources, if any exist
	Close();

	//Open the files and load the shader code from each of them, if they exist
	string vertex, geometry, fragment;
	if(!vertfile.empty())
	{
		ifstream file(vertfile.c_str());
		if(!file.is_open() || !file.good()) {file.close(); return(false);}
		vertex = LoadShaderFile(file);
		file.close();
	}
	if(!fragfile.empty())
	{
		ifstream file(fragfile.c_str());
		if(!file.is_open() || !file.good()) {file.close(); return(false);}
		fragment = LoadShaderFile(file);
		file.close();
	}
	if(!geofile.empty())
	{
		ifstream file(geofile.c_str());
		if(!file.is_open() || !file.good()) {file.close(); return(false);}
		geometry = LoadShaderFile(file);
		file.close();
	}

	//Compile each of the shaders, if they exist
	if(!vertex.empty())
	{
		if(CompileShaderObject(vertex, GL_VERTEX_SHADER) == 0) 
			return(false);
	}
	if(!geometry.empty())
	{
		if(CompileShaderObject(geometry, GL_GEOMETRY_SHADER) == 0) 
			return(false);
	}
	if(!fragment.empty())
	{
		if(CompileShaderObject(fragment, GL_FRAGMENT_SHADER) == 0) 
			return(false);
	}
	
	//Link the shaders to create the program. Delete the shader objects
	//as they're useless after linking.
	if(LinkShaderObjects() == 0) return(false);
	DeleteShaderObjects();

	return(true);
}

const string Shader::LoadShaderFile(ifstream &file, GLenum shaderType)
{
	ostringstream s(ostringstream::out);
	
	//The type of shader to load from the file
	string shaderTypeStr;
	switch(shaderType)
	{
		case GL_VERTEX_SHADER:
		{
			shaderTypeStr = string("__VERTEX"); break;
		}
		case GL_GEOMETRY_SHADER:
		{
			shaderTypeStr = string("__GEOMETRY"); break;
		}
		case GL_FRAGMENT_SHADER:
		{
			shaderTypeStr = string("__FRAGMENT"); break;
		}
	}

	//Loop through each line in the file
	//Check for #define SHADERTYPE, then start reading until
	//#endif is reached
	while(file.good())
	{
		string buf;
		getline(file, buf);

		if(buf.rfind("#define") != string::npos && 
				buf.rfind(shaderTypeStr) != string::npos)
		{
			string str = buf;
			s << str << "\n";
			do {
				getline(file, str);
				s << str << "\n";
			} while(str.rfind("#endif") == string::npos);
			break;
		}
	}

	//Reset the file pointer to the beginning and clear any flags
	file.clear(ifstream::goodbit);
	file.seekg(0);
	
	//Return the string containing the shader code
	return(s.str());
}

const string Shader::LoadShaderFile(ifstream &file)
{
	ostringstream s(ostringstream::out);

	//Get each line and put it into the string stream
	while(file.good())
	{
		string buf;
		getline(file, buf);
		s << buf << "\n";
	}

	//Reset the file pointer to the beginning and clear any flags
	file.clear(ifstream::goodbit);
	file.seekg(0);

	//Return the string containing the shader code
	return(s.str());
}

GLuint Shader::CompileShaderObject(const string &shaderStr, GLenum shaderType)
{
	//Create a new shader object, load the string containing the code
	//and compile the shader
	GLuint shader = glCreateShader(shaderType);
	const GLchar *shaderCSTR = shaderStr.c_str();
	glShaderSource(shader, 1, &shaderCSTR, NULL);
	glCompileShader(shader);

	//Check if the compilation went okay
	GLint status;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &status);
	if(status == GL_FALSE)
	{
		//We need the length of the info log so we can allocate space for
		//the info log string
		GLint infoLogLength;
		glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &infoLogLength);

		//Get the info log string detailing exactly what went wrong
		GLchar infoLogStr[infoLogLength+1];
		glGetShaderInfoLog(shader, infoLogLength, NULL, infoLogStr);

		//A string of the shader type, needed for the output
		string shaderTypeStr;
		switch(shaderType)
		{
			case GL_VERTEX_SHADER:
			{
				shaderTypeStr = string("vertex"); break;
			}
			case GL_GEOMETRY_SHADER:
			{
				shaderTypeStr = string("geometry"); break;
			}
			case GL_FRAGMENT_SHADER:
			{
				shaderTypeStr = string("fragment"); break;
			}
		}

		//Now print the error message showing where the compilation went wrong 
		//and how
		ostringstream s(ostringstream::out);
		s << "Compile failure in " << shaderTypeStr << " shader:\n";
		s << infoLogStr << "\n" << endl;
		errString = s.str();

		//Since there was compilation failure, we need to cleanup. Delete the 
		//shader from the GPU and return.
		glDetachShader(program, shader);
		glDeleteShader(shader);
		DeleteShaderObjects();
		glDeleteProgram(program);
		return(0);
	}

	//If all goes well, we add the shader to the shader objects list and
	//return the handle to the shader
	shaderObjects.push_back(shader);
	return(shader);
}

GLuint Shader::LinkShaderObjects()
{
	//Create a shader program object
	GLuint program = glCreateProgram();

	//Attach each of the shader objects we compiled before to the program
	for(GLuint i = 0; i < shaderObjects.size(); i++)
		glAttachShader(program, shaderObjects[i]);

	//Now link the shader objects together to form the shader program
	glLinkProgram(program);

	//Check if the linking went okay
	GLint status;
	glGetProgramiv(program, GL_LINK_STATUS, &status);
	if(status == GL_FALSE)
	{
		//We need the info log length to allocate space for the info log string
		GLint infoLogLength;
		glGetProgramiv(program, GL_INFO_LOG_LENGTH, &infoLogLength);

		//Get the info log string
		GLchar infoLogStr[infoLogLength+1];
		glGetProgramInfoLog(program, infoLogLength, NULL, infoLogStr);

		//Print out the error message detailing what went wrong
		ostringstream s(ostringstream::out);
		s << "Linker failure: " << infoLogStr << "\n" << endl;
		errString = s.str();

		//Cleanup: delete the program from the GPU and return
		DeleteShaderObjects();
		glDeleteProgram(program);
		return(0);
	}

	//If all goes well, assign the handle to shaderProgram and return the
	//handle
	this->program = program;
	return(program);
}

GLvoid Shader::DeleteShaderObjects()
{
	//Loop through the shader objects and delete each of them
	//This can be done after calling LinkShaders; once the program
	//is created, the shader objects are not needed anymore
	for(GLuint i = 0; i < shaderObjects.size(); i++)
	{
		glDetachShader(program, shaderObjects[i]);
		glDeleteShader(shaderObjects[i]);
	}

	//Nice trick to empty the vector and free it from taking up memory
	//Vectors never shrink
	vector<GLuint>().swap(shaderObjects);
}

GLvoid Shader::Close()
{
	//Delete the shader objects and the program
	DeleteShaderObjects();
	glDeleteProgram(program);
}

Shader::~Shader()
{
	Close();
}
