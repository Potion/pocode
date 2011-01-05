#include "poCommon.h"

#ifndef __poVariableRegistry_h__
#define __poVariableRegistry_h__

class poXMLnode;

class poVariable {
public:

	poVariable(const std::string& n, const std::string& t, void* a);
	virtual ~poVariable() {};
	
	virtual poXMLnode*	write();
	virtual void		read(poXMLnode* node);
	
	std::string name;
	std::string type;
	void* addr;
};


typedef std::vector<poVariable*> poVariableVector;

class poFontInfo;

class poVariableRegistry {
public:
	
	~poVariableRegistry();
	
	void add(const std::string& name, const std::string& type, void* addr);
	
	void add(const std::string& name, bool* addr) { add(name, "bool", addr); }
	void add(const std::string& name, int* addr) { add(name, "int", addr); }
	void add(const std::string& name, float* addr) { add(name, "float", addr); }
	void add(const std::string& name, std::string* addr) { add(name, "string", addr); }
	void add(const std::string& name, poColor* addr) { add(name, "poColor", addr); }
	void add(const std::string& name, poPoint* addr) { add(name, "poPoint", addr); }
	void add(const std::string& name, poFontInfo* addr) { add(name, "poFontInfo", addr); }

	void addSeparator(const std::string& title) { add(title, "---", (void*)NULL); };
	
	poXMLnode*	write(const std::string& name);
	void		read(poXMLnode* node);
	
	int			numVariables() { return registry.size(); };
	poVariable*	getVariable( int N ) { return registry[N]; };

//private:
	poVariableVector registry;
	
	static bool	blockAllRegistryAdditions;
};



#endif