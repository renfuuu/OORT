#include "MeshSlicer.h"

XML_Mesh::XML_Mesh()
{
	path = "";
	doc = new XMLDocument;
}

XML_Mesh::XML_Mesh(std::string filename)
{
	path = filename;
	doc = new XMLDocument;
}

XML_Mesh::XML_Mesh(std::vector<vec3f> v, std::vector<vec3i> f)
{
	for (std::vector<vec3f>::iterator i = v.begin(); i != v.end(); ++i)
	{
		verts.push_back(*i);
	}

	for (std::vector<vec3i>::iterator i = f.begin(); i != f.end(); ++i)
	{
		faces.push_back(*i);
	}
}


void XML_Mesh::loadFromXMLFile(std::string filename)
{
	std::cout << "Creating XML_Mesh class from .mesh.xml" << std::endl;
	std::cout << "Trying to load " << filename << std::endl;

	doc->LoadFile( filename.c_str() );
	if(doc->Error())
		doc->PrintError();
	else
		std::cout << "Loading complete" << std::endl;

	XMLNode* pVert = doc->FirstChildElement( "mesh" )->FirstChildElement("sharedgeometry");
  XMLNode* pFace = doc->FirstChildElement( "mesh" )->FirstChildElement("submeshes")->FirstChildElement("submesh")->FirstChildElement("faces");
  std::cout << "vertexcount" << pVert->ToElement()->IntAttribute("vertexcount") << std::endl;

  pVert = pVert->FirstChildElement("vertexbuffer");

 // XMLNode* xmlRoot = doc.FirstChildElement("mesh");
 // XMLNode* xmlVertexBuffer = xmlRoot->FirstChildElement("sharedgeometry")->FirstChildElement("vertexbuffer");
 // XMLNode* xmlFaceBuffer = xmlRoot->FirstChildElement("submeshes")->FirstChildElement("submesh")->FirstChildElement("faces");

 //  //PRINT ALL VERTS OF MESH

  int k = 0;

  for(XMLNode* node = pVert->FirstChildElement("vertex"); node != NULL; node=node->NextSibling())
  {
 		k++;
		float x = node->FirstChildElement("position")->FloatAttribute("x");
		float y = node->FirstChildElement("position")->FloatAttribute("y");
		float z = node->FirstChildElement("position")->FloatAttribute("z");

		float nx = node->FirstChildElement("normal")->FloatAttribute("x");
		float ny = node->FirstChildElement("normal")->FloatAttribute("y");
		float nz = node->FirstChildElement("normal")->FloatAttribute("z");

		float u = node->FirstChildElement("texcoord")->FloatAttribute("u");
		float v = node->FirstChildElement("texcoord")->FloatAttribute("v");

		this->verts.push_back(vec3f(x,y,z));
		this->normals.push_back(vec3f(nx,ny,nz));
		this->texcoords.push_back(vec2f(u,v));
  }
	std::cout << "Loading verts complete "<<std::endl;


 //  //CREATE TRIANGLES OF MESH
	k = 0;
  for(XMLNode* node = pFace->FirstChildElement("face"); node != NULL; node=node->NextSibling())
  {
  	k++;
		int v3 = node->ToElement()->IntAttribute("v3");
		int v2 = node->ToElement()->IntAttribute("v2");
		int v1 = node->ToElement()->IntAttribute("v1");

		this->faces.push_back(vec3i(v1,v2,v3));
  }

  	std::cout << "Loading indices complete "<<k << std::endl;

}

void XML_Mesh::toFile(std::string filename)
{
	//update vertex count
	//write relevent data to file
	XMLDocument temp;
	temp.LoadFile("../Assets/Asteroid/Stone_temp.mesh.xml");


  if(temp.Error())
		temp.PrintError();
	else
		std::cout << "Loading complete" << std::endl;


  int vsize = verts.size();
	temp.FirstChildElement("mesh")->FirstChildElement("sharedgeometry")->SetAttribute("vertexcount", vsize);
	
	std::cout << "Set Number of Verts " << vsize << std::endl;

	XMLNode* vertbuffer = temp.FirstChildElement("mesh")->FirstChildElement("sharedgeometry")->FirstChildElement("vertexbuffer");
	for (int i = 0; i < verts.size(); ++i)
	{
		XMLElement* vertex = temp.NewElement("vertex");

		XMLElement* position = temp.NewElement("position");
		vertex->InsertEndChild(position);
		XMLElement* normal = temp.NewElement("normal");
		vertex->InsertEndChild(normal);
		XMLElement* texcoord = temp.NewElement("texcoord");
		vertex->InsertEndChild(texcoord);

		position->SetAttribute("x", verts[i].x);
		position->SetAttribute("y", verts[i].y);
		position->SetAttribute("z", verts[i].z);
		if(i < normals.size())
		{
			normal->SetAttribute("x", normals[i].x);
			normal->SetAttribute("y", normals[i].y);
			normal->SetAttribute("z", normals[i].z);
		}
		else
		{
			normal->SetAttribute("x", 0.0f);
			normal->SetAttribute("y", 0.0f);
			normal->SetAttribute("z", 0.0f);
		}


		if(i < texcoords.size())
		{
			texcoord->SetAttribute("u", texcoords[i].u);
			texcoord->SetAttribute("v", texcoords[i].v);
		}
		else
		{
			texcoord->SetAttribute("u", 0.1f);
			texcoord->SetAttribute("v", 0.2f);
		}
    // vertex->InsertEndChild(vertex);	


		vertbuffer->InsertEndChild(vertex);		
  }

  std::cout << "Writing Verts Complete" << std::endl;


  int fsize = faces.size();
	XMLNode* facebuffer = temp.FirstChildElement("mesh")->FirstChildElement("submeshes")->FirstChildElement("submesh")->FirstChildElement("faces");
	facebuffer->ToElement()->SetAttribute("count", fsize);
	std::cout << "Set Number of Faces " << fsize <<  std::endl;


	for (int i = 0; i < faces.size(); ++i)
	{
		XMLElement* face = temp.NewElement("face");

		face->SetAttribute("v1", faces[i].x);
		face->SetAttribute("v2", faces[i].y);
		face->SetAttribute("v3", faces[i].z);
    // face->InsertEndChild(face);
		facebuffer->InsertEndChild(face);		
	}

  std::cout << "Writing Triangles Complete" << std::endl;

	std::ostringstream oss;
	std::string path;
	std::string xmlName;
	std::string meshName;

	path = "../Assets/meshgen/";
	
	xmlName = path + filename +".xml";
	meshName = path + filename;

	temp.SaveFile(xmlName.c_str());

	std::cout << "saved xml mesh file as " << xmlName;

	std::ostringstream cmd;
	cmd << "OgreXMLConverter " << xmlName <<" " << meshName;
	system("pwd");
	system(cmd.str().c_str());

	std::cout << "Called following cmd :: " << cmd.str();

}



MeshSlicer::MeshSlicer(Ogre::SceneNode* node)
{
	mSceneNode = node;

	system("rm ../Assets/meshgen/* -rf");
}

void MeshSlicer::loadMesh(XML_Mesh* mesh)
{
	mHost = mesh;
}

MeshSlicer::~MeshSlicer()
{

}

void MeshSlicer::sliceByPlane(std::vector<XML_Mesh*>& meshes, vec3f pp, vec3f pn)
{

	std::vector<Triangle> preserved;
	std::vector<Triangle> clipped;
	std::cout << "size of vertex buffer:  " << mHost->verts.size() << std::endl;
	int count = 0;
	for (std::vector<vec3i>::iterator i = mHost->faces.begin(); i != mHost->faces.end(); ++i)
	{
		std:: cout <<"start loop" <<std::endl;
		std::vector<vec3f> addedPoints;
		vec3f a, b, c;
		std::cout << "facet: " << count++ << std::endl;
		std::cout << "indicies : " << (*i).x <<" "<< (*i).y <<" "<< (*i).z <<std::endl;
		a = mHost->verts[(*i).x];
		b = mHost->verts[(*i).y];
		c = mHost->verts[(*i).z];

		Triangle tri(a, (*i).x, b, (*i).y, c, (*i).z);
		
		std:: cout <<"Entering clip" <<std::endl;
		int flag = ClipFacet(tri, &addedPoints, &preserved, &clipped, pp, pn);
		std:: cout <<"Exiting clip" <<std::endl;

		if(flag != 0 || flag != 1)
			std:: cout <<"SOMETHING HAPPENDED" <<std::endl;

		std::cout << flag << std::endl;
		if(!addedPoints.empty())
		{
			std:: cout <<"added points not empty" <<std::endl;

			for (std::vector<vec3f>::iterator i = addedPoints.begin(); i != addedPoints.end(); ++i)
			{
				std:: cout <<"adding point" <<std::endl;
				mHost->verts.push_back((*i));
				std:: cout <<"point added" <<std::endl;
			}
		}
		else
		{
			std:: cout <<"no points added" <<std::endl;
		}
		std:: cout <<"start loop" <<std::endl;
	}
	std::cout << "size of vertex buffer:  " << mHost->verts.size() << std::endl;

	std::cout << "size of clipped side:  " << clipped.size() << std::endl;
	std::cout << "size of preserved side:  " << preserved.size() << std::endl;

	std::vector<vec3i> faces1;
	std::vector<vec3i> faces2;
	for (std::vector<Triangle>::iterator i = clipped.begin(); i != clipped.end(); ++i)
	{
		faces1.push_back(vec3i(i->getIndex(0), i->getIndex(1), i->getIndex(2)));
	}

	for (std::vector<Triangle>::iterator i = preserved.begin(); i != preserved.end(); ++i)
	{
		faces2.push_back(vec3i(i->getIndex(0), i->getIndex(1), i->getIndex(2)));
	}

	XML_Mesh* m1 = new XML_Mesh(mHost->verts, faces1);
	XML_Mesh* m2 = new XML_Mesh(mHost->verts, faces2);
	meshes.push_back(m1);
	meshes.push_back(m2);
	std::cout << "mesh slicing finished" << std::endl;

}

/*-------------------------------------------------------------------------
   Clip a 3 vertex facet in place
   The 3 point facet is defined by vertices p[0],p[1],p[2], "p[3]"
      There must be a fourth point as a 4 point facet may result
   The normal to the plane is n
   A point on the plane is p0
   The side of the plane containing the normal is clipped away
   Return the number of vertices in the clipped polygon
*/

int MeshSlicer::ClipFacet(Triangle in, std::vector<vec3f>* addedPoints, std::vector<Triangle>* preserved, std::vector<Triangle>* clipped, vec3f p0, vec3f n)
{

   double A,B,C,D;
   double l;
   double side[3];
   vec3f q;
   std::vector<vec3f> p(4);
   p.push_back(in[0]);
   p.push_back(in[1]);
   p.push_back(in[2]);

   int buffersize = mHost->verts.size();

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
   side[0] = A*in[0].x + B*in[0].y + C*in[0].z + D;
   side[1] = A*in[1].x + B*in[1].y + C*in[1].z + D;
   side[2] = A*in[2].x + B*in[2].y + C*in[2].z + D;
   /* Are all the vertices are on the clipped side */
   if (side[0] >= 0 && side[1] >= 0 && side[2] >= 0)
   {
   		clipped->push_back(in);

      return(0);
   }

   /* Are all the vertices on the not-clipped side */
   if (side[0] <= 0 && side[1] <= 0 && side[2] <= 0)
   {
   		preserved->push_back(in);

      return(1);
   }


   /* Is p0 the only point on the clipped side */
   if (side[0] > 0 && side[1] < 0 && side[2] < 0) {
   		vec3f p0 = p[0];
      q.x = p[0].x - side[0] * (p[2].x - p[0].x) / (side[2] - side[0]);
      q.y = p[0].y - side[0] * (p[2].y - p[0].y) / (side[2] - side[0]);
      q.z = p[0].z - side[0] * (p[2].z - p[0].z) / (side[2] - side[0]);
      p[3] = q;
      q.x = p[0].x - side[0] * (p[1].x - p[0].x) / (side[1] - side[0]);
      q.y = p[0].y - side[0] * (p[1].y - p[0].y) / (side[1] - side[0]);
      q.z = p[0].z - side[0] * (p[1].z - p[0].z) / (side[1] - side[0]);
      p[0] = q;

      addedPoints->push_back(p[0]);
      addedPoints->push_back(p[3]);

      preserved->push_back(Triangle(p[0], buffersize, p[1], in.getIndex(1), p[2], in.getIndex(2)));
      preserved->push_back(Triangle(p[1], in.getIndex(1), p[2], in.getIndex(2), p[3], buffersize+1));

      clipped->push_back(Triangle(p0, in.getIndex(0), p[0], buffersize, p[3], buffersize+1));
      return(2);
   }

   /* Is p1 the only point on the clipped side */
   if (side[1] > 0 && side[0] < 0 && side[2] < 0) {
   		vec3f p1 = p[1];
      p[3] = p[2];
      q.x = p[1].x - side[1] * (p[2].x - p[1].x) / (side[2] - side[1]);
      q.y = p[1].y - side[1] * (p[2].y - p[1].y) / (side[2] - side[1]);
      q.z = p[1].z - side[1] * (p[2].z - p[1].z) / (side[2] - side[1]);
      p[2] = q;
      q.x = p[1].x - side[1] * (p[0].x - p[1].x) / (side[0] - side[1]);
      q.y = p[1].y - side[1] * (p[0].y - p[1].y) / (side[0] - side[1]);
      q.z = p[1].z - side[1] * (p[0].z - p[1].z) / (side[0] - side[1]);
      p[1] = q;

      addedPoints->push_back(p[1]);
      addedPoints->push_back(p[2]);

      preserved->push_back(Triangle(p[0], in.getIndex(0), p[1], buffersize, p[2], buffersize+1));
      preserved->push_back(Triangle(p[2], buffersize + 1, p[3], in.getIndex(2), p[0], in.getIndex(0)));

      clipped->push_back(Triangle(p1, in.getIndex(1), p[1], buffersize, p[2], buffersize+1));
      return(3);
   }

   /* Is p2 the only point on the clipped side */
   if (side[2] > 0 && side[0] < 0 && side[1] < 0) {
      
   		vec3f p2 = p[2];

      q.x = p[2].x - side[2] * (p[0].x - p[2].x) / (side[0] - side[2]);
      q.y = p[2].y - side[2] * (p[0].y - p[2].y) / (side[0] - side[2]);
      q.z = p[2].z - side[2] * (p[0].z - p[2].z) / (side[0] - side[2]);
      p[3] = q;
      q.x = p[2].x - side[2] * (p[1].x - p[2].x) / (side[1] - side[2]);
      q.y = p[2].y - side[2] * (p[1].y - p[2].y) / (side[1] - side[2]);
      q.z = p[2].z - side[2] * (p[1].z - p[2].z) / (side[1] - side[2]);
      p[2] = q;

      addedPoints->push_back(p[2]);
      addedPoints->push_back(p[3]);

      preserved->push_back(Triangle(p[0], in.getIndex(0), p[1], in.getIndex(1), p[2], buffersize));
      preserved->push_back(Triangle(p[2], buffersize, p[3],buffersize+1,  p[0], in.getIndex(0)));

      clipped->push_back(Triangle(p2,in.getIndex(2), p[3], buffersize+1,  p[2], buffersize));
      return(4);
   }

   /* Is p0 the only point on the not-clipped side */
   if (side[0] < 0 && side[1] > 0 && side[2] > 0) {
      q.x = p[0].x - side[0] * (p[1].x - p[0].x) / (side[1] - side[0]);
      q.y = p[0].y - side[0] * (p[1].y - p[0].y) / (side[1] - side[0]);
      q.z = p[0].z - side[0] * (p[1].z - p[0].z) / (side[1] - side[0]);
      p[1] = q;
      q.x = p[0].x - side[0] * (p[2].x - p[0].x) / (side[2] - side[0]);
      q.y = p[0].y - side[0] * (p[2].y - p[0].y) / (side[2] - side[0]);
      q.z = p[0].z - side[0] * (p[2].z - p[0].z) / (side[2] - side[0]);
      p[2] = q;	

      addedPoints->push_back(p[1]);
      addedPoints->push_back(p[2]);

      preserved->push_back(Triangle(p[0], in.getIndex(0), p[1], buffersize, p[2], buffersize+1));

      clipped->push_back(Triangle(p[1], buffersize, in[1], in.getIndex(1), in[2], in.getIndex(2)));
      clipped->push_back(Triangle(in[2], in.getIndex(2), p[2], buffersize+1, p[1], buffersize));

      return(5);
   }

   /* Is p1 the only point on the not-clipped side */
   if (side[1] < 0 && side[0] > 0 && side[2] > 0) {
   		std::cout<<6<<std::endl;
      q.x = p[1].x - side[1] * (p[0].x - p[1].x) / (side[0] - side[1]);
      q.y = p[1].y - side[1] * (p[0].y - p[1].y) / (side[0] - side[1]);
      q.z = p[1].z - side[1] * (p[0].z - p[1].z) / (side[0] - side[1]);
      p[0] = q;
      q.x = p[1].x - side[1] * (p[2].x - p[1].x) / (side[2] - side[1]);
      q.y = p[1].y - side[1] * (p[2].y - p[1].y) / (side[2] - side[1]);
      q.z = p[1].z - side[1] * (p[2].z - p[1].z) / (side[2] - side[1]);
      p[2] = q;
   	  

      addedPoints->push_back(p[0]);
      addedPoints->push_back(p[2]);
      in.getIndex(1);


      preserved->push_back( Triangle(p[0], buffersize, p[1], in.getIndex(1), p[2], buffersize+1));

      clipped->push_back(Triangle(in[0], in.getIndex(0), p[0], buffersize, p[2], buffersize+1));
      clipped->push_back(Triangle(p[2], buffersize+1, in[2], in.getIndex(2), in[0], in.getIndex(0)));

      return(6);
   }

   /* Is p2 the only point on the not-clipped side */
   if (side[2] < 0 && side[0] > 0 && side[1] > 0) {
      q.x = p[2].x - side[2] * (p[1].x - p[2].x) / (side[1] - side[2]);
      q.y = p[2].y - side[2] * (p[1].y - p[2].y) / (side[1] - side[2]);
      q.z = p[2].z - side[2] * (p[1].z - p[2].z) / (side[1] - side[2]);
      p[1] = q;
      q.x = p[2].x - side[2] * (p[0].x - p[2].x) / (side[0] - side[2]);
      q.y = p[2].y - side[2] * (p[0].y - p[2].y) / (side[0] - side[2]);
      q.z = p[2].z - side[2] * (p[0].z - p[2].z) / (side[0] - side[2]);
      p[0] = q;

      addedPoints->push_back(p[0]);
      addedPoints->push_back(p[1]);

      preserved->push_back(Triangle(p[0], buffersize, p[1], buffersize+1, p[2], in.getIndex(2)));

      clipped->push_back(Triangle(in[0], in.getIndex(0), in[1], in.getIndex(1), p[1], buffersize+1));
      clipped->push_back(Triangle(p[1], buffersize+1, p[0], buffersize, in[0], in.getIndex(0)));
      return(7);
   }

   /* Shouldn't get here */
   return(-1);
}





