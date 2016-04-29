#pragma once

#include "GameObject.h"
#include "tinyxml2.h"
#include <string>
#include <cstdlib>

using namespace tinyxml2;

struct vec3f
{
	float x;
	float y;
	float z;
};

struct vec3i
{
	int x;
	int y;
	int z;
};

struct XML_Mesh
{

	XMLDocument doc;

	std::vector<vec3f> verts;
 	std::vector<vec3i> faces;
 	std::string path;

 	XML_Mesh();
 	XML_Mesh(std::string m);

 	~XML_Mesh();
	void run();
};


class MeshSlicer{

public:

	MeshSlicer();
	~MeshSlicer();

};
