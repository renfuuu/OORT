#include "MeshSlicer.h"


XML_Mesh::XML_Mesh(std::string filename)
{
	path = filename;
}

XML_Mesh::~XML_Mesh()
{

}





void XML_Mesh::loadFromXMLFile(std::string filename)
{
	std::cout << "Creating XML_Mesh class from .mesh.xml" << std::endl;
	doc.LoadFile( path.c_str() );
	if(doc.Error())
		doc.PrintError();
	else
		std::cout << "Loading complete" << std::endl;

	XMLNode* pVert = doc.FirstChildElement( "mesh" )->FirstChildElement("sharedgeometry");
  XMLNode* pFace = doc.FirstChildElement( "mesh" )->FirstChildElement("submeshes")->FirstChildElement("submesh")->FirstChildElement("faces");
  std::cout << "vertexcount" <<pVert->ToElement()->IntAttribute("vertexcount") << std::endl;

  this->xmlRoot = doc.FirstChildElement("mesh");
  this->xmlVertexBuffer = xmlRoot->FirstChildElement("sharedgeometry")->FirstChildElement("vertexbuffer");
  this->xmlFaceBuffer = xmlRoot->FirstChildElement("submeshes")->FirstChildElement("submesh")->FirstChildElement("faces");

  //PRINT ALL VERTS OF MESH
  for(XMLNode* node = pVert->FirstChildElement("vertex"); node != NULL; node=node->NextSibling())
  {
		float x = node->FirstChildElement("position")->FloatAttribute("x");
		float y = node->FirstChildElement("position")->FloatAttribute("y");
		float z = node->FirstChildElement("position")->FloatAttribute("z");
		this->verts.push_back(vec3f(x,y,z));

		float nx = node->FirstChildElement("normal")->FloatAttribute("x");
		float ny = node->FirstChildElement("normal")->FloatAttribute("y");
		float nz = node->FirstChildElement("normal")->FloatAttribute("z");
		this->verts.push_back(vec3f(x,y,z));

		float u = node->FirstChildElement("texcoord")->FloatAttribute("u");
		float v = node->FirstChildElement("texcoord")->FloatAttribute("v");

		this->verts.push_back(vec3f(x,y,z));
		this->normals.push_back(vec3f(nx,ny,nz));
		this->texcoord.push_back(vec2f(u,v));
		this->xmlVerts.push_back(*node);
  }
  //CREATE TRIANGLES OF MESH
  for(XMLNode* node = pFace->FirstChildElement("face"); node != NULL; node=node->NextSibling())
  {
		int v3 = node->ToElement()->IntAttribute("v3");
		int v2 = node->ToElement()->IntAttribute("v2");
		int v1 = node->ToElement()->IntAttribute("v1");

		this->faces.push_back(vec3i(v1,v2,v3));
		this->xmlFaces.push_back(*node);
  }
}

void XML_Mesh::toFile(std::string filename)
{
	//update vertex count
	//write relevent data to file
	XMLDocument* temp;
	temp->LoadFile("../Assets/Asteroid/Stone_template.mesh.xml");
	temp->FirstChildElement("mesh")->FirstChildElement("sharedgeometry")->SetAttribute("vertexcount", verts.size());
	XMLNode* vertbuffer = temp->FirstChildElement("mesh")->FirstChildElement("sharedgeometry")->FirstChildElement("vertexbuffer");
	for (int i = 0; i < verts.size(); ++i)
	{
		XMLElement* vertex = new XMLElement("vertex");
		vertbuffer->InsertEndChild(vertex)		

		XMLElement* position = new XMLElement("position");
		vertex.InsertEndChild(position);
		XMLElement* normal = new XMLElement("normal");
		vertex.InsertEndChild(normal);
		XMLElement* textcoord = new XMLElement("textcoord");
		vertex.InsertEndChild(textcoord);

		position.SetAttribute("x", verts[i].x);
		position.SetAttribute("y", verts[i].y);
		position.SetAttribute("z", verts[i].z);

		normal.SetAttribute("x", normals[i].x);
		normal.SetAttribute("y", normals[i].y);
		normal.SetAttribute("z", normals[i].z);

		textcoord.SetAttribute("u", texcoords[i].u);
		textcoord.SetAttribute("v", texcoords[i].v);
	}

	XMLNode* facebuffer = temp->FirstChildElement("submeshes")->FirstChildElement("submesh")->FirstChildElement("faces");
	facebuffer->SetAttribute("count", faces.size());
	for (int i = 0; i < faces.size(); ++i)
	{
		XMLElement* face = new XMLElement("face");

		face.SetAttribute("v1", faces[i].x);
		face.SetAttribute("v2", faces[i].y);
		face.SetAttribute("v3", faces[i].z);

		facebuffer->InsertEndChild(&vertex)		
	}
	std::ostringstream oss;
	oss << ".meshgen/" <<filename;
	temp.SaveFile(oss.str().c_str());

	std::ostringstream cmd;
	cmd << "OgreXMLConverter " << oss.str();
	system(cmd.str().c_str());

}



MeshSlicer::MeshSlicer(XML_Mesh* mesh, Ogre::SceneNode* node)
{

}

MeshSlicer::~MeshSlicer();

void MeshSlicer::sliceByPlane(std::vector<XML_Mesh*> meshes, Ogre::Vector3 pp, Ogre::Vector3 pn)
{

}


