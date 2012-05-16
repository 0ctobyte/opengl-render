//Copyright (c) 2012 Sekhar Bhattacharya
//Licensed under the MIT license
//See license.txt

#ifndef __MESH__
#define __MESH__

#include <GL/glew.h>
#include <SFML/Window.hpp>
#include <string>
#include <Vector3.h>

//!@brief Stores material information
//!
//!Most material files contain multiple material definitions,
//!matname specifies which definition to read.
//!ka and ks are probably the most important info.
struct Material {

	GLfloat ns;    //!<The specular exponent, or the shininess
	GLfloat ka[3]; //!<The ambient color (3 components, RGB)
	GLfloat kd[3]; //!<Diffuse color
	GLfloat ks[3]; //!<Specular color
	GLfloat d;     //!<Transparency/dissolve; alpha value for the color arrays
	GLuint illum;  //!<Illumination model (What is it's use?)
	GLfloat ni;    //!<Index of refraction (What is it's use?)

	//!@brief Fills the material structure using data from the specified 
	//!MTL file
	//!@param [in] filename - The MTL filename
	//!@param [in] matname - The name of the material to read data from
	//!@return True if the method succeeded or false if something fails
	GLboolean Open(const std::string &filename, const std::string &matname);

	//!@brief Returns a string containing all the data contained within this 
	//!struct
	//!@return A string containing the data within this struct
	const std::string ToString() const;
};

//!@brief Defines a collection of triangles/vertices which use the same
//!materials
struct TriangleGroup {

	std::vector<GLuint> indices; //!<Contains the indices into the vector array
	Material mtl; //!<The material data for this group
	GLuint ibo; //!<The identifier for the index buffer object

	//!@brief Returns a string containing the data contained within this struct
	//!@return A string containing the data within this struct
	const std::string ToString() const;

	//!@brief Deletes all vector containers and buffer objects
	~TriangleGroup();
};

//!@brief Holds all information to define a mesh, including materials,
//!normals and texture coordinates. 
//!
//!Stores info in vertex buffer objects and index buffer objects. The arrays
//!use a vector to keep them flexible and organized. A vector also guarantees
//!that the elements are stored contiguously in memory.
struct Mesh {

	std::vector<Vector3> v; //!<A vector containing both vertices and normals
	std::vector<Vector3> vt; //!<A vector of texture coords
	std::vector<TriangleGroup> g; //!<A vector of material groups
	GLuint vbo; //!<Handle for vertex/normal/texcoord interleaved VBO
	GLuint numVerts; //!<Number of just the vertices in the array

	//!@brief Opens the OBJ file and the corresponding MTL file
	//!
	//!Opens the OBJ file and, if it exists, the corresponding MTL file.
	//!Loads all the data from the file and fills the vertex, normal,
	//!texture and material group vector containers.
	//!@param [in] filename - The name of the OBJ file
	//!@return True if file(s) were opened successfully, False otherwise
	GLboolean Open(const std::string &filename);
	
	//!@brief Creates vertex/index buffer objects from the vertex/group 
	//!data
	//!
	//!Creates a VBO for the vertex/normal/texture coordinate arrays and a
	//!index buffer object for each group.
	GLvoid CreateBufferObjects();

	//!@brief Calculates the vertex normals
	//!
	//!Calculates the normals for each vertex. The vertex normals are the
	//!normalized weighted sum of the surface normals of the surrounding 
	//!triangles/faces. Larger surfaces will produce larger normals which
	//!influences the vertex normals towards that surface, this provides
	//!the weighting.
	GLvoid CalculateNormals();

	//!@brief Deletes all vector containers and buffer objects
	GLvoid Close();

	//!@brief Describes the mesh
	//!@return A string containing the # of vertices, groups, the list of
	//!vertices, and a list of faces with the corresponding indices
	const std::string ToString() const;

	//!@brief Calls Close()
	~Mesh();
	
	//!@brief Temporary to test rendering of mesh
	//!@note This is temporary!
	GLvoid Draw() const;
};

#endif // __MESH__
