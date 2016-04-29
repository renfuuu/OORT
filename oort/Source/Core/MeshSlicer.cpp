#include "MeshSlicer.h"


XML_Mesh::XML_Mesh(std::string filename)
{
	path = filename;
	doc.LoadFile( filename.c_str() );
	if(doc.Error())
		doc.PrintError();
	else
		std::cout << "Parsing " << filename << "complete" << std::endl;
}


XML_Mesh::XML_Mesh()
{

}
XML_Mesh::~XML_Mesh()
{

}

void XML_Mesh::load()
{

}

void XML_Mesh::toFile()
{
	//update vertex count
	//write relevent data to file

	xmlVertexBuffer->DeleteChildren();
	for (std::vector<XMLNode*>::iterator i = xmlVerts.begin(); i != xmlVerts.end(); ++i)
	{

		xmlVertexBuffer->InsertEndChild(*i);
	}

	xmlFaceBuffer->DeleteChildren();
	for (std::vector<XMLNode*>::iterator i = xmlFaces.begin(); i != xmlFaces.end(); ++i)
	{

		xmlFaceBuffer->InsertEndChild(*i);
	}
	doc.SaveFile("../Assets/Asteroid/gen/other.xml");
}

void XML_Mesh::run()
{

    // Structure of the XML file:
    // - Element "PLAY"      the root Element, which is the 
    //                       FirstChildElement of the Document
    // - - Element "TITLE"   child of the root PLAY Element
    // - - - Text            child of the TITLE Element

    // Navigate to the title, using the convenience function,
    // with a dangerous lack of error checking.
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
		this->xmlVerts.push_back(node);
    }
    //CREATE TRIANGLES OF MESH
    for(XMLNode* node = pFace->FirstChildElement("face"); node != NULL; node=node->NextSibling())
    {
		int v3 = node->ToElement()->IntAttribute("v3");
		int v2 = node->ToElement()->IntAttribute("v2");
		int v1 = node->ToElement()->IntAttribute("v1");

		this->faces.push_back(vec3i(v1,v2,v3));
		this->xmlFaces.push_back(node);
    }

    // system("OgreXMLConverter");

}

