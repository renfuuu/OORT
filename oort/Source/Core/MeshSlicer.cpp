#include "MeshSlicer.h"


XML_Mesh::XML_Mesh(std::string filename)
{
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

void XML_Mesh::run()
{

    // Structure of the XML file:
    // - Element "PLAY"      the root Element, which is the 
    //                       FirstChildElement of the Document
    // - - Element "TITLE"   child of the root PLAY Element
    // - - - Text            child of the TITLE Element

    // Navigate to the title, using the convenience function,
    // with a dangerous lack of error checking.
	XMLNode* pVert = doc.FirstChild( "mesh" )->FirstChild("sharedgeometry");
    XMLNode* pFace = doc.FirstChild( "mesh" )->FirstChild("submeshes");
    std::cout << pVert->ToElement()->IntAttribute("vertexcount");

    for(XMLNode* node = pVert->FirstChildElement("vertex"); node != node->LastSibling(); node = node->NextSibling())
    {
    	std::cout << node->FirstChildElement("position")
    }

    system("OgreXMLConverter");

}

