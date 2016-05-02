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
		XMLElement* textcoord = temp.NewElement("texcoord");
		vertex->InsertEndChild(textcoord);

		position->SetAttribute("x", verts[i].x);
		position->SetAttribute("y", verts[i].y);
		position->SetAttribute("z", verts[i].z);

		normal->SetAttribute("x", normals[i].x);
		normal->SetAttribute("y", normals[i].y);
		normal->SetAttribute("z", normals[i].z);

		textcoord->SetAttribute("u", texcoords[i].u);
		textcoord->SetAttribute("v", texcoords[i].v);
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
	oss << "../Assets/meshgen/" <<filename;
	temp.SaveFile(oss.str().c_str());

	std::cout << "saved xml mesh file as " << oss.str();

	std::ostringstream cmd;
	cmd << "OgreXMLConverter " << oss.str();
	system("pwd");
	system(cmd.str().c_str());

	std::cout << "Called following cmd :: " << cmd.str();
}



MeshSlicer::MeshSlicer(XML_Mesh* mesh, Ogre::SceneNode* node)
{

}

MeshSlicer::~MeshSlicer()
{

}

void MeshSlicer::sliceByPlane(std::vector<XML_Mesh*> meshes, Ogre::Vector3 pp, Ogre::Vector3 pn)
{

}


