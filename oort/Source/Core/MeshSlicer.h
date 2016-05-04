#pragma once

#include "GameObject.h"
#include <string>
#include <cstdlib>
#include <vector>

#include "tinyxml2.h"

using namespace tinyxml2;

struct vec2f
{
	float u;
	float v;
	vec2f(float u_, float v_):
		u(u_), 
		v(v_)
	{}

  ~vec2f(){}
  
  vec2f(float a=0.0f)
  {
    u=a;
    v=a;
  }
};

struct vec3f
{
	float x;
	float y;
	float z;
	vec3f(float x_, float y_, float z_):
		x(x_), 
		y(y_), 
		z(z_)
	{}

	vec3f(Ogre::Vector3 v)
	{
		x = v.x;
		y = v.y;
		z = v.z;
	}
  
  vec3f(float a = 0.0f)
  {
    x = a;
    y = a;
    z = a;
  }

  ~vec3f(){}
};

struct vec3d
{
	double x;
	double y;
	double z;
	vec3d(double x_, double y_, double z_):
		x(x_), 
		y(y_), 
		z(z_)
	{}
  
  vec3d(double a = 0.0f)
  {
    x = a;
    y = a;
    z = a;
  }

  ~vec3d(){}
};

struct vec3i
{
	int x;
	int y;
	int z;
	vec3i(int x_, int y_, int z_):
		x(x_), 
		y(y_), 
		z(z_)
	{}
  
  ~vec3i(){}
};

struct Triangle
{
	std::vector<vec3f> verts;
	std::vector<int> indices;
	Triangle(vec3f p0, int i0, vec3f p1, int i1, vec3f p2, int i2)
	{
		verts.push_back(p0);
		verts.push_back(p1);
		verts.push_back(p2);

		indices.push_back(i0);
		indices.push_back(i1); 
		indices.push_back(i2);


	}

  	vec3f& operator[](unsigned int i)
  	{
  		return verts[i];
  	}
	const vec3f& operator[](unsigned int i)const
	{ 
		return verts[i];
	}

	int getIndex(unsigned int i)
	{
		return indices[i];
	}

	void setIndex(unsigned int i, int val)
	{
		indices[i] = val;
	}

  ~Triangle(){}
};

struct XML_Mesh
{
	XMLDocument* doc;

	std::vector<vec3f> verts;
 	std::vector<vec3i> faces;
 	std::vector<vec3f> normals;
 	std::vector<vec2f> texcoords;

 	// XMLElement* xmlRoot;

 	// XMLElement* xmlVertexBuffer;
 	// XMLElement* xmlFaceBuffer;

 	// std::vector<XMLElement> xmlVerts;
 	// std::vector<XMLElement> xmlFaces;


 	std::string path;
  XML_Mesh();
 	XML_Mesh(std::string name);
 	XML_Mesh(std::vector<vec3f> v, std::vector<vec3i> i);

  ~XML_Mesh()
   {}
   
 	void toFile(std::string filename);
 	void loadFromXMLFile(std::string filename);
/*
	void addVertex(vec3f pos, vec3f norm, vec2f uv);
	bool addFace(int a, int b, int c);
*/

};


class MeshSlicer
{
	XML_Mesh* mHost;
	Ogre::SceneNode* mSceneNode;

public:



	MeshSlicer(Ogre::SceneNode* node);
	~MeshSlicer();

	void sliceByPlane(std::vector<XML_Mesh*>& meshHalves, vec3f planepoint, vec3f planenormal);
	void loadMesh(XML_Mesh* mesh);
	private:
	int ClipFacet(Triangle in, 
		std::vector<vec3f>* addedPoints, 
		std::vector<Triangle>* preserved, 
		std::vector<Triangle>* clipped, 
		vec3f n, 
		vec3f p0);

};


/*-------------------------------------------------------------------------
   Clip a 3 vertex facet in place
   The 3 point facet is defined by vertices t1[0],t1[1],t1[2], "t1[3]"
      There must be a fourth point as a 4 point facet may result
   The normal to the plane is n
   A point on the plane is p0
   The side of the plane containing the normal is clipped away
   Return the number of vertices in the clipped polygon
*/

