#include "poVariableRegistry.h"
#include "poFont.h"
#include "poXMLparser.h"

// ====================== poVariable ====================== //

poVariable::poVariable(const std::string& n, const std::string& t, void* a)
: name(n), type(t), addr(a) 
{}

poXMLnode* poVariable::write()
{
	poXMLnode* node = new poXMLnode( name.c_str() );
	node->addAttribute("type", (char*)type.c_str());

	if(type == "---")
		node->setData( "-----------------------------" );
		
	else if(type == "bool")
		node->setData(*(bool*)addr ? "true" : "false");

	else if(type == "int")
		node->setData(*(int*)addr);
		
	else if(type == "float")
		node->setData(*(float*)addr);
		
	else if(type == "string") {
		node->setData(((std::string*)addr)->c_str());
	}
	
	else if(type == "poColor") 
	{
		poColor* color = (poColor*)addr;
		node->addChild(new poXMLnode("R", color->R));
		node->addChild(new poXMLnode("G", color->G));
		node->addChild(new poXMLnode("B", color->B));
		node->addChild(new poXMLnode("A", color->A));
	}
	
	else if(type == "poPoint")
	{
		poPoint* point = (poPoint*)addr;
		node->addChild(new poXMLnode("x", point->x));
		node->addChild(new poXMLnode("y", point->y));
		node->addChild(new poXMLnode("z", point->z));
	}
	
	else if(type == "poFontInfo")
	{
		poFontInfo* info = (poFontInfo*)addr;
		node->addChild(new poXMLnode("fontName", info->fontName));
		node->addChild(new poXMLnode("fontSize", info->fontSize));
		node->addChild(new poXMLnode("fontIndex", info->fontIndex));
		node->addChild(new poXMLnode("charSet", info->charSet));
	}
	
	else 
	{
		printf("WARNING: poVariable->write: unknown type (%s)\n", type.c_str());
		return NULL;
	}
	
	return node;
}

void poVariable::read(poXMLnode* node)
{
	using namespace std;
	type = node->getAttribute("type");
	
	if(type == "---")
		return;
		
	if(type == "bool")
		*(bool*)addr = string("true") == node->getDataAsString();
	
	else if(type == "int")
		*(int*)addr = node->getDataAsInt();
		
	else if(type == "float")
		*(float*)addr = node->getDataAsFloat();
		
	else if(type == "string")
	{
		const char* data = node->getDataAsString();
		*((std::string*)addr) = data;
	}
	
	else if(type == "poColor") 
	{
		poColor* color = (poColor*)addr;
		color->R = node->getNodeAsFloat("R");
		color->G = node->getNodeAsFloat("G");
		color->B = node->getNodeAsFloat("B");
		color->A = node->getNodeAsFloat("A");
	}
	
	else if(type == "poPoint")
	{
		poPoint* point = (poPoint*)addr;
		point->x = node->getNodeAsFloat("x");
		point->y = node->getNodeAsFloat("y");
		point->z = node->getNodeAsFloat("z");
	}
	
	else if(type == "poFontInfo")
	{
		poFontInfo* info = (poFontInfo*)addr;
		const char* fontName = node->getNodeAsString("fontName");
		info->fontName = (char*)realloc(info->fontName, strlen(fontName+1));
		strcpy(info->fontName, fontName);
		info->fontSize = node->getNodeAsInt("fontSize");
		info->fontIndex = node->getNodeAsInt("fontIndex");
		info->charSet = node->getNodeAsInt("charSet");
	}
}


// ====================== poVariableRegistry ====================== //

bool	poVariableRegistry::blockAllRegistryAdditions = false;

poVariableRegistry::~poVariableRegistry()
{
	for( int i=0; i<registry.size(); i++ )
		delete registry[i];
	registry.clear();
}

void poVariableRegistry::add(const std::string& name, const std::string& type, void* addr)
{
	if ( blockAllRegistryAdditions )
		return;
	
	registry.push_back( new poVariable(name, type, addr) );
}

poXMLnode* poVariableRegistry::write(const std::string& name)
{
	poXMLnode* node = new poXMLnode( name.c_str() );
		
	for( int i=0; i<registry.size(); i++ )
	{
		if ( registry[i]->addr == NULL || registry[i]->type=="---" )
			continue;
		node->addChild( registry[i]->write() );
	}

	return node;
}

void poVariableRegistry::read(poXMLnode* node)
{
	for( int i=0; i<registry.size(); i++ )
	{
		if ( registry[i]->addr == NULL || registry[i]->type=="---" )
			continue;
	
		poXMLnode* propNode = node->getNode( registry[i]->name.c_str() );
		if(propNode == NULL) 
			printf("ERROR: poVariableRegistry->read: expected data node '%s' not found\n", registry[i]->name.c_str() );
		else
			registry[i]->read(propNode);
	}
}


