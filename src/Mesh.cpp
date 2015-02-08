//Copyright (c) 2012 Sekhar Bhattacharya
//Licensed under the MIT license
//See license.txt

#include <sstream>
#include <fstream>
#include <iomanip>

#include <Mesh.h>

using namespace std;

GLboolean Material::Open(const string &filename, const string &matname)
{
	//Open the file and make sure it is good
	ifstream file(filename.c_str(), ifstream::in);
	if(!file.is_open() || !file.good()) return(false);

	//Loop through the file, one line at a time
	while(file.good())
	{
		//Get the next line
		string buf;
		getline(file, buf);
		
		//Load only the data from the specified material
		//When we are done loading we break out of the 
		//top level loop. Make sure the line specifies 
		//a new material by the tag <newmtl> and is not
		//just a comment!
		if(buf[0] == 'n' && buf.find(matname) != string::npos)
		{
			//If we are done reading in the material definition
			//this will be set to true so we could break out of this loop
			GLboolean finished = false;

			while(file.good())
			{
				//Get the next line
				getline(file, buf);

				//Break out of the loop when we are done
				if(finished) break;

				switch(buf[0])
				{
					//If N is found that means it is either a specular exponent
					//or index of refraction.
					case 'N':
					{
						istringstream s(buf.substr(3), istringstream::in);
						if(buf[1] == 's')
							s >> ns;
						else if(buf[1] == 'i')
							s >> ni;
						break;
					}

					//If the first char is K, it specifies either a specular
					//ambient or diffuse color
					//Blender doesn't output a alpha component for the colors
					//Set that shit to 1 (fully opaque)
					case 'K':
					{
						istringstream s(buf.substr(3), istringstream::in);
						if(buf[1] == 'a')
							s >> ka[0] >> ka[1] >> ka[2];
						else if(buf[1] == 'd')
							s >> kd[0] >> kd[1] >> kd[2];
						else
							s >> ks[0] >> ks[1] >> ks[2];
						break;
					}
	
					//If the first char is d, it specifies the transparency
					case 'd':
					{
						istringstream s(buf.substr(2), istringstream::in);
						s >> d;
						break;
					}

					//If the first char is i, it specifies the illumination
					//model
					case 'i':
					{
						istringstream s(buf.substr(6), istringstream::in);
						s >> illum;
						break;
					}

					//If we find none of the above then we know we are done
					//reading the file defintion so set finished to true
					default:
					{
						finished = true;
						break;
					}
				}
			}
	
			//Break out of the top level loop
			break;
		}
	}
	
	//Close that shit!
	file.close();

	return(true);
}

const string Material::ToString() const
{
	ostringstream s;
	s<<"MATERIAL DATA"<<endl<<setfill('-')<<setw(14)<<'-'<<setfill(' ')<<endl;

	s<<left;
	s<<setw(22)<<"Specular Exponent"<<setw(8)<<"(ns)"<<": "<<ns<<endl;
	s<<setw(22)<<"Index of Refraction"<<setw(8)<<"(ni)"<<": "<<ni<<endl; 

	s<<setw(22)<<"Ambience"<<setw(8)<<"(ka)"<<": "<<"["<<setw(10)<<ka[0];
	s<<", "<<setw(10)<<ka[1]<<", "<<setw(10)<<ka[2]<<"]"<<endl;
	
	s<<setw(22)<<"Diffuse"<<setw(8)<<"(kd)"<<": "<<"["<<setw(10)<<kd[0];
	s<<", "<<setw(10)<<kd[1]<<", "<<setw(10)<<kd[2]<<"]"<<endl;
	
	s<<setw(22)<<"Specular"<<setw(8)<<"(ks)"<<": "<<"["<<setw(10)<<ks[0];
	s<<", "<<setw(10)<<ks[1]<<", "<<setw(10)<<ks[2]<<"]"<<endl;

	s<<setw(22)<<"Transparency"<<setw(8)<<"(d)"<<": "<<d<<endl; 

	s<<setw(22)<<"Illumination Model"<<setw(8)<<"(illum)"<<": "<<illum;
	
	return(s.str());
}

const string TriangleGroup::ToString() const
{
	ostringstream s(ostringstream::out);
	
	//Print out the # of indices for this group
	s<<"# OF INDICES FOR GROUP: "<<indices.size()<<endl;
	s<<setfill('-')<<setw(23)<<'-'<<setfill(' ')<<endl;
	
	//Here we print out all the indices belonging to this group, with 
	//three indices per line (3 indices define a face)
	GLuint count = 0;
	s<<"f";
	for(vector<GLuint>::const_iterator i=indices.begin();i<indices.end();i++)
	{
		if(count >= 3)
		{
			count = 0;
			s<<endl<<"f";
		}
		
		s<<setw(5)<<*i;
		count++;
	}

	//Now print out the material data for this group
	s<<endl<<endl<<mtl.ToString();

	return(s.str());
}

TriangleGroup::~TriangleGroup()
{
	//Clear the indices vector and deallocate memory
	indices.clear(); vector<GLuint>().swap(indices);

	//Delete the index buffer object
	glDeleteBuffers(1, &ibo);
}

GLboolean Mesh::Open(const string &filename)
{
	//Deallocate previous mesh resources, if any exist
	Close();

	//We need at most two files, the OBJ and MTL files
	ifstream file;
	string mtlfilename;

	//Store any normals temporarily in this array, this will be added on to
	//the end of the vertex array at the end of this method.
	vector<Vector3> vn;

	//Open the OBJ file here, read only!
	file.open(filename.c_str(), ifstream::in);
	if(!file.is_open() || !file.good()) return(false);
	
	//If we encounter another group we have to set this variable
	//to true so we don't skip the group the next time around the loop
	GLboolean foundGroup = false;

	//Loop through the file, one line at a time
	//This is to store the line from the file
	string buf;
	while(file.good())
	{
		//Get the next line only if we are not on a line defining a group
		if(!foundGroup) getline(file, buf);
		else foundGroup = false;
		
		switch(buf[0])
		{
			//If we find a 'v' that means it's a description of either a
			//vertex, vertex texture or a vertex normal
			case 'v':
			{
				GLfloat x, y, z;
				//Add the texture coordinates to the vector
				if(buf[1] == 't')
				{
					istringstream s(buf.substr(3), istringstream::in);
					s >> x >> y >> z;
					vt.push_back(Vector3(x, y, z));
				}
				//Add the normal vertex to the vector
				else if(buf[1] == 'n')
				{
					istringstream s(buf.substr(3), istringstream::in);
					s >> x >> y >> z;
					vn.push_back(Vector3(x, y, z));
				}
				//Add the vertex to the vector
				else
				{
					istringstream s(buf.substr(2), istringstream::in);
					s >> x >> y >> z;
					v.push_back(Vector3(x, y, z));
				}
				break;
			}
			
			//If we find a 'm' that means a MTL file is specified
			case 'm':
			{
				//A material file is specified! Let's copy the file name
				mtlfilename = buf.substr(7);
				break;
			}

			//If 'g' is found it means a group is defined
			//Read all the faces in this group and store the indices
			case 'g':
			{
				//Store all the information in a temporary TriangleGroup struct
				//and then add the TriangleGroup to the TriangleGroup vector
				TriangleGroup temp;

				//Looping stops when a 'g' is read or when the end of file is
				//reached
				do {
					//Get the next line
					getline(file, buf);
					
					switch(buf[0])
					{
						//We found a face! Then load the indices
						case 'f':
						{
							//A wavefront OBJ is defined such that:
							//v/vt/vn
							//The first value is the vertex which is
							//always after a space. We only want the 
							//vertex index as the normal and texture
							//coordinates will be generated manually
							//to fit our needs
							size_t pos = buf.find_first_of(' ', 0);
							while(pos != string::npos)
							{
								//Add the index to the indices vector of the
								//group
								istringstream s(buf.substr(pos+1), 
										istringstream::in);
								GLfloat index;
								s >> index;
								temp.indices.push_back(index-1);
								pos = buf.find_first_of(' ', pos+1);
							}
							break;
						}

						//We found a material definition, load it up
						case 'u':
						{
							//The name of the material definition to load up
							//One MTL file may hold multiple definitions
							string matname = buf.substr(7);
							
							//We have to make sure that the MTL file exists
							//TODO: So multiple groups may use the same 
							//material. We could save memory by allowing groups
							//to reference the same material structure. How? 
							//Well maybe have a Material vector in the mesh 
							//class with each material in the vector having a
							//unique id, such as the material name, and then if
							//the material specified is already in the vector,
							//just make the group reference that! However, 
							//the material struct is pretty small, so maybe not
							// nescessary. 
							if(!mtlfilename.empty())
							{
								if(!temp.mtl.Open(mtlfilename, matname))
									return(false);
							}
							else {return(false);}
							break;
						}
					
						//Default does nothing
						default:
						{
							break;
						}
					}
				} while(file.good() && buf[0] != 'g');

				//Add the group to the group vector
				g.push_back(temp);
				
				//If we find another group, we must not skip it!
				if(buf[0] == 'g') foundGroup = true;
				
				break;
			}

			default:
			{
				break;
			}
		}	
	}

	//close that shit!
	file.close();

	//Get the number of vertices in the array, the number of normals should be
	//equal to this value
	numVerts = v.size();

	//Merge the vector array with the normal array, creating an interleaved
	//array

	return(true);
}	

GLvoid Mesh::CreateBufferObjects()
{
	//Generate buffer ids
	glGenBuffers(1, &vbo);

	//Upload the vertex data from the vector container to the GPU
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, v.size()*sizeof(Vector3), &(v.front()),
		GL_STATIC_DRAW);

	//We may or may not have multiple groups. Loop through each group and
	//create a buffer ID for the group and upload the indices to the GPU
	for(vector<TriangleGroup>::iterator it=g.begin(); it<g.end(); it++)
	{
		glGenBuffers(1, &(it->ibo));
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, it->ibo);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, 
			it->indices.size()*sizeof(GLuint), &(it->indices.front()),
			GL_STATIC_DRAW);
	}
}

GLvoid Mesh::CalculateNormals()
{
	//Reserve memory, we already know the size of the vector (same number of
	//normals as vertices)
	v.reserve(v.size()+numVerts);
	
	//Reset all normals to zero vectors
	for(GLuint i = 0; i < numVerts; i++) v.push_back(Vector3());

	//Loop through each triangle group
	for(vector<TriangleGroup>::const_iterator grp=g.begin();grp<g.end();grp++)
	{
		//Loop every third index, a group of three indices is one face
		for(vector<GLuint>::const_iterator i=grp->indices.begin();
				i<grp->indices.end(); i+=3)
		{
			//Calculate the normal via cross product of (a-c)x(b-c)
			Vector3 normal=(v[*i]-v[*(i+2)]).CrossProduct(v[*(i+1)]-v[*(i+2)]);

			//Add the normal to the normal already contained within
			//the interleaved array
			v[*i+numVerts] += normal;
			v[*(i+1)+numVerts] += normal;
			v[*(i+2)+numVerts] += normal;
		}
	}

	//Now normalize each normal
	for(GLuint i = numVerts; i < v.size(); i++) v[i] = v[i].Normalize();
}	

GLvoid Mesh::Close()
{
	//Delete all previously used resources
	//Delete all the vector containers and deallocate the memory they hold
	v.clear(); vector<Vector3>().swap(v);
	vt.clear(); vector<Vector3>().swap(vt);
	
	//Delete the trianglegroup container. Remember calling clear will call
	//the destructors for each of the elements in the vector!
	g.clear(); vector<TriangleGroup>().swap(g);

	// Delete buffer objects
	glDeleteBuffers(1, &vbo);
}

const string Mesh::ToString() const
{
	ostringstream s(ostringstream::out);

	//Print the # of vertices and groups into the string stream
	s<<left<<setfill('/')<<setw(68)<<"/"<<setfill(' ')<<endl;
	s<<setw(15)<<"# OF VERTICES"<<": "<<v.size()<<endl;
	s<<setw(15)<<"# OF GROUPS"<<": "<<g.size();
	
	//Now Print out the list of vertices
	s<<endl<<setfill('=')<<setw(68)<<'='<<setfill(' ')<<endl;
	s<<"LIST OF VERTICES"<<endl<<setfill('-')<<setw(16)<<'-'<<setfill(' ');
	for(GLuint i = 0; i < numVerts; i++)
	{
		s << endl << v[i].ToString();
	}

	//Loop through each group and print the number of indices in each group
	//and the indices it self in groups of three
	for(vector<TriangleGroup>::const_iterator k=g.begin(); k<g.end(); k++)
	{
		s<<endl<<setfill('=')<<setw(68)<<'='<<setfill(' ')<<endl;
		s<<k->ToString();
	}
	s<<endl<<setfill('/')<<setw(68)<<"/"<<setfill(' ');

	return(s.str());
}

Mesh::~Mesh()
{
	Close();
}

//TEMPORARY!
GLvoid Mesh::Draw() const
{
	//Bind the vertex buffer and set the vertex pointer to the binded buffer
	//All the groups contain indices to only one set of vertices
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glVertexAttribPointer(0,4,GL_FLOAT,GL_FALSE,sizeof(Vector3),(GLvoid*)0);
	glVertexAttribPointer(1,4,GL_FLOAT,GL_FALSE,sizeof(Vector3),(GLvoid*)((uintptr_t)numVerts));
	
	//Loop through each group and bind the index buffer and set the index
	//pointer. Then draw the elements as triangles using the indices
	for(vector<TriangleGroup>::const_iterator it=g.begin(); it<g.end(); it++)
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, it->ibo);
		glDrawElements(GL_TRIANGLES, it->indices.size(), GL_UNSIGNED_INT,
			(GLvoid*)0);
	}

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
}
