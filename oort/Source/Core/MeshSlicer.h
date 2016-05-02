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
  
  vec3f(float a = 0.0f)
  {
    x = a;
    y = a;
    z = a;
  }

  ~vec3f(){}
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
	XML_Mesh* host;

public:



	MeshSlicer(XML_Mesh* mesh, Ogre::SceneNode* node);
	~MeshSlicer();

	void sliceByPlane(std::vector<XML_Mesh*> meshHalves, Ogre::Vector3 planepoint, Ogre::Vector3 planenormal);

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

// int ClipFacet(std::vector<vec3f> inTri, std::vector<vec3f> outTri1, std::vector<vec3f> outTri2, vec3f n, vec3f p0){
//    double A,B,C,D;
//    double l;
//    double side[3];
//    vec3f q;

//    /*
//       Determine the equation of the plane as
//       Ax + By + Cz + D = 0
//    */
//    l = sqrt(n.x*n.x + n.y*n.y + n.z*n.z);
//    A = n.x / l;
//    B = n.y / l;
//    C = n.z / l;
//    D = -(n.x*p0.x + n.y*p0.y + n.z*p0.z);

//    /*
//       Evaluate the equation of the plane for each vertex
//       If side < 0 then it is on the side to be retained
//       else it is to be clippped
//    */
//    side[0] = A*inTri[0].x + B*inTri[0].y + C*inTri[0].z + D;
//    side[1] = A*inTri[1].x + B*inTri[1].y + C*inTri[1].z + D;
//    side[2] = A*inTri[2].x + B*inTri[2].y + C*inTri[2].z + D;


//    //CASE 0
//    /* Are all the vertices are on the clipped side */
//    if (side[0] >= 0 && side[1] >= 0 && side[2] >= 0)
//    {
//       outTri2.push_back(inTri);
//       return(0);
//    }

//    //CASE 1
//    /* Are all the vertices on the not-clipped side */
//    if (side[0] <= 0 && side[1] <= 0 && side[2] <= 0)
//    {
//       outTri1.push_back(inTri);
//       return(3);
//    }
//    //CASE 2
//    /* Is p0 the only point on the clipped side */
//    if (side[0] > 0 && side[1] < 0 && side[2] < 0) {

//       q.x = inTri[0].x - side[0] * (inTri[2].x - inTri[0].x) / (side[2] - side[0]);
//       q.y = inTri[0].y - side[0] * (inTri[2].y - inTri[0].y) / (side[2] - side[0]);
//       q.z = inTri[0].z - side[0] * (inTri[2].z - inTri[0].z) / (side[2] - side[0]);
//       vec3f p4 = q;
//       q.x = inTri[0].x - side[0] * (inTri[1].x - inTri[0].x) / (side[1] - side[0]);
//       q.y = inTri[0].y - side[0] * (inTri[1].y - inTri[0].y) / (side[1] - side[0]);
//       q.z = inTri[0].z - side[0] * (inTri[1].z - inTri[0].z) / (side[1] - side[0]);
//       vec3f p0 = q;

//       outTri1.push_back(vec3f(p0, ))



//       return(4);
//    }
//    //CASE 3
//    /* Is p1 the only point on the clipped side */
//    if (side[1] > 0 && side[0] < 0 && side[2] < 0) {
//       inTri[3] = inTri[2];
//       q.x = inTri[1].x - side[1] * (inTri[2].x - inTri[1].x) / (side[2] - side[1]);
//       q.y = inTri[1].y - side[1] * (inTri[2].y - inTri[1].y) / (side[2] - side[1]);
//       q.z = inTri[1].z - side[1] * (inTri[2].z - inTri[1].z) / (side[2] - side[1]);
//       inTri[2] = q;
//       q.x = inTri[1].x - side[1] * (inTri[0].x - inTri[1].x) / (side[0] - side[1]);
//       q.y = inTri[1].y - side[1] * (inTri[0].y - inTri[1].y) / (side[0] - side[1]);
//       q.z = inTri[1].z - side[1] * (inTri[0].z - inTri[1].z) / (side[0] - side[1]);
//       inTri[1] = q;
//       return(4);
//    }
//    //CASE 4
//    /* Is p2 the only point on the clipped side */
//    if (side[2] > 0 && side[0] < 0 && side[1] < 0) {
//       q.x = inTri[2].x - side[2] * (inTri[0].x - inTri[2].x) / (side[0] - side[2]);
//       q.y = inTri[2].y - side[2] * (inTri[0].y - inTri[2].y) / (side[0] - side[2]);
//       q.z = inTri[2].z - side[2] * (inTri[0].z - inTri[2].z) / (side[0] - side[2]);
//       inTri[3] = q;
//       q.x = inTri[2].x - side[2] * (inTri[1].x - inTri[2].x) / (side[1] - side[2]);
//       q.y = inTri[2].y - side[2] * (inTri[1].y - inTri[2].y) / (side[1] - side[2]);
//       q.z = inTri[2].z - side[2] * (inTri[1].z - inTri[2].z) / (side[1] - side[2]);
//       inTri[2] = q;
//       return(4);
//    }
//    //CASE 5
//    /* Is p0 the only point on the not-clipped side */
//    if (side[0] < 0 && side[1] > 0 && side[2] > 0) {
//       q.x = inTri[0].x - side[0] * (inTri[1].x - inTri[0].x) / (side[1] - side[0]);
//       q.y = inTri[0].y - side[0] * (inTri[1].y - inTri[0].y) / (side[1] - side[0]);
//       q.z = inTri[0].z - side[0] * (inTri[1].z - inTri[0].z) / (side[1] - side[0]);
//       inTri[1] = q;
//       q.x = inTri[0].x - side[0] * (inTri[2].x - inTri[0].x) / (side[2] - side[0]);
//       q.y = inTri[0].y - side[0] * (inTri[2].y - inTri[0].y) / (side[2] - side[0]);
//       q.z = inTri[0].z - side[0] * (inTri[2].z - inTri[0].z) / (side[2] - side[0]);
//       inTri[2] = q;
//       return(3);
//    }
//    //CASE 6 
//    /* Is p1 the only point on the not-clipped side */
//    if (side[1] < 0 && side[0] > 0 && side[2] > 0) {
//       q.x = inTri[1].x - side[1] * (inTri[0].x - inTri[1].x) / (side[0] - side[1]);
//       q.y = inTri[1].y - side[1] * (inTri[0].y - inTri[1].y) / (side[0] - side[1]);
//       q.z = inTri[1].z - side[1] * (inTri[0].z - inTri[1].z) / (side[0] - side[1]);
//       inTri[0] = q;
//       q.x = inTri[1].x - side[1] * (inTri[2].x - inTri[1].x) / (side[2] - side[1]);
//       q.y = inTri[1].y - side[1] * (inTri[2].y - inTri[1].y) / (side[2] - side[1]);
//       q.z = inTri[1].z - side[1] * (inTri[2].z - inTri[1].z) / (side[2] - side[1]);
//       inTri[2] = q;
//       return(3);
//    }
//    //CASE 7
//    /* Is p2 the only point on the not-clipped side */
//    if (side[2] < 0 && side[0] > 0 && side[1] > 0) {
//       q.x = inTri[2].x - side[2] * (inTri[1].x - inTri[2].x) / (side[1] - side[2]);
//       q.y = inTri[2].y - side[2] * (inTri[1].y - inTri[2].y) / (side[1] - side[2]);
//       q.z = inTri[2].z - side[2] * (inTri[1].z - inTri[2].z) / (side[1] - side[2]);
//       inTri[1] = q;
//       q.x = inTri[2].x - side[2] * (inTri[0].x - inTri[2].x) / (side[0] - side[2]);
//       q.y = inTri[2].y - side[2] * (inTri[0].y - inTri[2].y) / (side[0] - side[2]);
//       q.z = inTri[2].z - side[2] * (inTri[0].z - inTri[2].z) / (side[0] - side[2]);
//       inTri[0] = q;
//       return(3);
//    }
//    //NULL CASE
//    /* Shouldn't get here */
//    return(-1);
// }
