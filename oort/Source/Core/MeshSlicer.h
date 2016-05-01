#pragma once

#include "GameObject.h"
#include "tinyxml2.h"
#include <string>
#include <cstdlib>

using namespace tinyxml2;

struct vec2f
{
	float u;
	float v;
	vec3f(float u_, float v_):
		u(u_), 
		v(v_)
	{}

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

};




struct XML_Mesh
{
	XMLDocument doc;

	std::vector<vec3f> verts;
 	std::vector<vec3i> faces;
 	std::vector<vec3f> normals;
 	std::vector<vec3f> texcoords;

 	XMLNode* xmlRoot;

 	XMLNode* xmlVertexBuffer;
 	XMLNode* xmlFaceBuffer;

 	std::vector<XMLNode> xmlVerts;
 	std::vector<XMLNode> xmlFaces;


 	std::string path;

 	XML_Mesh(std::string name);

 	~XML_Mesh();
 	void toFile(std::string filename);
 	void loadFromXMLFile();

	void addVertex(vec3f pos, vec3f norm, vec2f uv);
	bool addFace(int a, int b, int c);
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

int ClipFacet(std::vector<vec3f> t1, std::vector<vec3f> t2, vec3f n,vec3f p0)
{
   double A,B,C,D;
   double l;
   double side[3];
   vec3f q;

   /*
      Determine the equation of the plane as
      Ax + By + Cz + D = 0
   */
   l = sqrt(n.x*n.x + n.y*n.y + n.z*n.z);
   A = n.x / l;
   B = n.y / l;
   C = n.z / l;
   D = -(n.x*p0.x + n.y*p0.y + n.z*p0.z);

   /*
      Evaluate the equation of the plane for each vertex
      If side < 0 then it is on the side to be retained
      else it is to be clippped
   */
   side[0] = A*t1[0].x + B*t1[0].y + C*t1[0].z + D;
   side[1] = A*t1[1].x + B*t1[1].y + C*t1[1].z + D;
   side[2] = A*t1[2].x + B*t1[2].y + C*t1[2].z + D;

   /* Are all the vertices are on the clipped side */
   if (side[0] >= 0 && side[1] >= 0 && side[2] >= 0)
      return(0);

   /* Are all the vertices on the not-clipped side */
   if (side[0] <= 0 && side[1] <= 0 && side[2] <= 0)
      return(3);

   /* Is p0 the only point on the clipped side */
   if (side[0] > 0 && side[1] < 0 && side[2] < 0) {
      q.x = t1[0].x - side[0] * (t1[2].x - t1[0].x) / (side[2] - side[0]);
      q.y = t1[0].y - side[0] * (t1[2].y - t1[0].y) / (side[2] - side[0]);
      q.z = t1[0].z - side[0] * (t1[2].z - t1[0].z) / (side[2] - side[0]);
      t1[3] = q;
      q.x = t1[0].x - side[0] * (t1[1].x - t1[0].x) / (side[1] - side[0]);
      q.y = t1[0].y - side[0] * (t1[1].y - t1[0].y) / (side[1] - side[0]);
      q.z = t1[0].z - side[0] * (t1[1].z - t1[0].z) / (side[1] - side[0]);
      t1[0] = q;
      return(4);
   }

   /* Is p1 the only point on the clipped side */
   if (side[1] > 0 && side[0] < 0 && side[2] < 0) {
      t1[3] = t1[2];
      q.x = t1[1].x - side[1] * (t1[2].x - t1[1].x) / (side[2] - side[1]);
      q.y = t1[1].y - side[1] * (t1[2].y - t1[1].y) / (side[2] - side[1]);
      q.z = t1[1].z - side[1] * (t1[2].z - t1[1].z) / (side[2] - side[1]);
      t1[2] = q;
      q.x = t1[1].x - side[1] * (t1[0].x - t1[1].x) / (side[0] - side[1]);
      q.y = t1[1].y - side[1] * (t1[0].y - t1[1].y) / (side[0] - side[1]);
      q.z = t1[1].z - side[1] * (t1[0].z - t1[1].z) / (side[0] - side[1]);
      t1[1] = q;
      return(4);
   }

   /* Is p2 the only point on the clipped side */
   if (side[2] > 0 && side[0] < 0 && side[1] < 0) {
      q.x = t1[2].x - side[2] * (t1[0].x - t1[2].x) / (side[0] - side[2]);
      q.y = t1[2].y - side[2] * (t1[0].y - t1[2].y) / (side[0] - side[2]);
      q.z = t1[2].z - side[2] * (t1[0].z - t1[2].z) / (side[0] - side[2]);
      t1[3] = q;
      q.x = t1[2].x - side[2] * (t1[1].x - t1[2].x) / (side[1] - side[2]);
      q.y = t1[2].y - side[2] * (t1[1].y - t1[2].y) / (side[1] - side[2]);
      q.z = t1[2].z - side[2] * (t1[1].z - t1[2].z) / (side[1] - side[2]);
      t1[2] = q;
      return(4);
   }

   /* Is p0 the only point on the not-clipped side */
   if (side[0] < 0 && side[1] > 0 && side[2] > 0) {
      q.x = t1[0].x - side[0] * (t1[1].x - t1[0].x) / (side[1] - side[0]);
      q.y = t1[0].y - side[0] * (t1[1].y - t1[0].y) / (side[1] - side[0]);
      q.z = t1[0].z - side[0] * (t1[1].z - t1[0].z) / (side[1] - side[0]);
      t1[1] = q;
      q.x = t1[0].x - side[0] * (t1[2].x - t1[0].x) / (side[2] - side[0]);
      q.y = t1[0].y - side[0] * (t1[2].y - t1[0].y) / (side[2] - side[0]);
      q.z = t1[0].z - side[0] * (t1[2].z - t1[0].z) / (side[2] - side[0]);
      t1[2] = q;
      return(3);
   }

   /* Is p1 the only point on the not-clipped side */
   if (side[1] < 0 && side[0] > 0 && side[2] > 0) {
      q.x = t1[1].x - side[1] * (t1[0].x - t1[1].x) / (side[0] - side[1]);
      q.y = t1[1].y - side[1] * (t1[0].y - t1[1].y) / (side[0] - side[1]);
      q.z = t1[1].z - side[1] * (t1[0].z - t1[1].z) / (side[0] - side[1]);
      t1[0] = q;
      q.x = t1[1].x - side[1] * (t1[2].x - t1[1].x) / (side[2] - side[1]);
      q.y = t1[1].y - side[1] * (t1[2].y - t1[1].y) / (side[2] - side[1]);
      q.z = t1[1].z - side[1] * (t1[2].z - t1[1].z) / (side[2] - side[1]);
      t1[2] = q;
      return(3);
   }

   /* Is p2 the only point on the not-clipped side */
   if (side[2] < 0 && side[0] > 0 && side[1] > 0) {
      q.x = t1[2].x - side[2] * (t1[1].x - t1[2].x) / (side[1] - side[2]);
      q.y = t1[2].y - side[2] * (t1[1].y - t1[2].y) / (side[1] - side[2]);
      q.z = t1[2].z - side[2] * (t1[1].z - t1[2].z) / (side[1] - side[2]);
      t1[1] = q;
      q.x = t1[2].x - side[2] * (t1[0].x - t1[2].x) / (side[0] - side[2]);
      q.y = t1[2].y - side[2] * (t1[0].y - t1[2].y) / (side[0] - side[2]);
      q.z = t1[2].z - side[2] * (t1[0].z - t1[2].z) / (side[0] - side[2]);
      t1[0] = q;
      return(3);
   }

   /* Shouldn't get here */
   return(-1);
}
