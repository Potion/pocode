#ifndef __poPDFShape_h__
#define __poPDFShape_h__

#include "poShape2D.h"

class poPDFShape : public poShape2D 
{
public:
							poPDFShape();
							poPDFShape( char* theFileName );
	virtual					~poPDFShape();
	
	virtual poObject*		duplicate( bool copyChildren=false, poObject* copyIntoObject=NULL );
	virtual const char*		getClassName() { return "poPDFShape"; }

	virtual void			init();
	void					readFromFile();
	
	void					setFilename(const char* fn);
	const char*				getFilename() const { return filename.c_str(); }
	
	float curveInterpLimit;

protected:
	std::string filename;
};


#endif