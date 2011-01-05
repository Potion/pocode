#include "poPDFShape.h"
#include "poMatrix33.h"

#ifndef WIN32

// push transformation matrix
void op_q(CGPDFScannerRef scanner, void* info);
void op_Q(CGPDFScannerRef scanner, void* info);
void op_cm(CGPDFScannerRef scanner, void* info);
void op_k(CGPDFScannerRef scanner, void* info);
void op_K(CGPDFScannerRef scanner, void* info);
void op_g(CGPDFScannerRef scanner, void* info);
void op_G(CGPDFScannerRef scanner, void* info);
void op_rg(CGPDFScannerRef scanner, void* info);
void op_RG(CGPDFScannerRef scanner, void* info);
void op_w(CGPDFScannerRef scanner, void* info);
void op_W(CGPDFScannerRef scanner, void* info);
void op_n(CGPDFScannerRef scanner, void* info);
void op_m(CGPDFScannerRef scanner, void* info);
void op_l(CGPDFScannerRef scanner, void* info);
void op_c(CGPDFScannerRef scanner, void* info);
void op_v(CGPDFScannerRef scanner, void* info);
void op_y(CGPDFScannerRef scanner, void* info);
void op_h(CGPDFScannerRef scanner, void* info);
void op_re(CGPDFScannerRef scanner, void* info);
void op_f(CGPDFScannerRef scanner, void* info);
void op_s(CGPDFScannerRef scanner, void* info);
void op_S(CGPDFScannerRef scanner, void* info);
void op_BDC(CGPDFScannerRef scanner, void* info);
void op_EMC(CGPDFScannerRef scanner, void* info);

struct PDFParser
{
	PDFParser(poPDFShape* shape) 
	:	shape(shape), document(NULL), curveInterpStep(shape->curveInterpLimit),
		strokeWidth(0), strokeColor(1,1,1), fillColor(.5,.5,.5)
	{}

	void openPDF(const char* filename) 
	{
		CFStringRef path;
		CFURLRef url;
	 
		path = CFStringCreateWithCString (NULL, filename, kCFStringEncodingUTF8);
		url = CFURLCreateWithFileSystemPath (NULL, path, kCFURLPOSIXPathStyle, 0);
		CFRelease (path);

		document = CGPDFDocumentCreateWithURL (url);
		if(document == NULL) {
			printf("ERROR: openPDF: can't open document %s\n", filename);
			exit(-1);
		}
		CFRelease(url);

		int count = CGPDFDocumentGetNumberOfPages (document);
		if (count == 0) {
			printf("'%s' needs at least one page!", filename);
			return;
		}
	}

	void parsePDF() 
	{
		CGPDFOperatorTableRef table = CGPDFOperatorTableCreate();
		CGPDFOperatorTableSetCallback (table, "q", &op_q);
		CGPDFOperatorTableSetCallback (table, "Q", &op_Q);
		CGPDFOperatorTableSetCallback (table, "cm", &op_cm);

		CGPDFOperatorTableSetCallback (table, "k", &op_k);
		CGPDFOperatorTableSetCallback (table, "K", &op_K);
		CGPDFOperatorTableSetCallback (table, "g", &op_g);
		CGPDFOperatorTableSetCallback (table, "G", &op_G);
		CGPDFOperatorTableSetCallback (table, "rg", &op_rg);
		CGPDFOperatorTableSetCallback (table, "RG", &op_RG);
		CGPDFOperatorTableSetCallback (table, "w", &op_w);

		CGPDFOperatorTableSetCallback (table, "W", &op_W);
		CGPDFOperatorTableSetCallback (table, "n", &op_n);

		CGPDFOperatorTableSetCallback (table, "m", &op_m);
		CGPDFOperatorTableSetCallback (table, "l", &op_l);
		CGPDFOperatorTableSetCallback (table, "c", &op_c);
		CGPDFOperatorTableSetCallback (table, "v", &op_v);
		CGPDFOperatorTableSetCallback (table, "y", &op_y);
		CGPDFOperatorTableSetCallback (table, "h", &op_h);
		CGPDFOperatorTableSetCallback (table, "re", &op_re);

		CGPDFOperatorTableSetCallback (table, "f", &op_f);
		CGPDFOperatorTableSetCallback (table, "s", &op_s);
		CGPDFOperatorTableSetCallback (table, "S", &op_S);
		
		CGPDFOperatorTableSetCallback (table, "BDC", &op_BDC);
		CGPDFOperatorTableSetCallback (table, "EMC", &op_EMC);

		CGPDFPageRef page;
		CGPDFScannerRef scanner;
		CGPDFContentStreamRef contentStream;
		
		int numPages = CGPDFDocumentGetNumberOfPages(document);
		for(int i=1; i<=numPages; i++) 
		{
			page = CGPDFDocumentGetPage(document, i);
			contentStream = CGPDFContentStreamCreateWithPage(page);
			scanner = CGPDFScannerCreate(contentStream, table, this);
			CGPDFScannerScan(scanner);

			CGPDFPageRelease(page);
			CGPDFScannerRelease(scanner);
			CGPDFContentStreamRelease(contentStream);
		}
	}
	
	void makeFillShapeFromCurrentState() 
	{
		poShape2D* shp = new poShape2D();
		shp->fillColor = fillColor;
		shp->strokeWidth = 0;
		addPathToShape(shp);
		points.clear();
		shp->init();
		shp->doTessellation();
		shape->addChild(shp);
	}
	
	void makeStrokeShapeFromCurrentState()
	{
		poShape2D* shp = new poShape2D();
		shp->strokeWidth = strokeWidth;
		shp->strokeColor = strokeColor;
		shp->enableFill = false;
		addPathToShape(shp);
		points.clear();
		shape->addChild(shp);
	}
	
	void addPathToShape(poShape2D* shp)
	{
		poPointList::iterator iter = points.begin();
		while(iter != points.end())
		{
			poPoint p = transformation.transform(*iter);
			shp->addPoint(p);
			iter++;
		}
		points.clear();
	}

	poShape2D* shape;
	CGPDFDocumentRef document;
	
	poMatrix33List matrixStack;
	poMatrix33 transformation;
	poPointList points;
	
	poColor fillColor;
	poColor strokeColor;
	float strokeWidth;
	
	float curveInterpStep;
};


// push transformation matrix
void op_q(CGPDFScannerRef scanner, void* info)
{
	PDFParser* parser = (PDFParser*)info;
	parser->matrixStack.push_back(parser->transformation);
}

// pop transformation matrix
void op_Q(CGPDFScannerRef scanner, void* info)
{
	PDFParser* parser = (PDFParser*)info;
	parser->transformation = parser->matrixStack.back();
	parser->matrixStack.pop_back();
}

// set working transformation matrix
void op_cm(CGPDFScannerRef scanner, void* info)
{
	PDFParser* parser = (PDFParser*)info;
	
	float a, b, c, d, e, f;
	CGPDFScannerPopNumber(scanner, &f);
	CGPDFScannerPopNumber(scanner, &e);
	CGPDFScannerPopNumber(scanner, &d);
	CGPDFScannerPopNumber(scanner, &c);
	CGPDFScannerPopNumber(scanner, &b);
	CGPDFScannerPopNumber(scanner, &a);

	float mat[] = {a, b, e, c, d, f, 0, 0, 1};
	parser->transformation.multiply(mat);
}

// set fill color (CMYK)
void op_k(CGPDFScannerRef scanner, void* info)
{
	PDFParser* parser = (PDFParser*)info;

	float c, m, y, k;
	CGPDFScannerPopNumber(scanner, &k);
	CGPDFScannerPopNumber(scanner, &y);
	CGPDFScannerPopNumber(scanner, &m);
	CGPDFScannerPopNumber(scanner, &c);
	
	parser->fillColor.setCMYK(c, m, y, k);
}

// set stroke color (CMYK)
void op_K(CGPDFScannerRef scanner, void* info)
{
	PDFParser* parser = (PDFParser*)info;

	float c, m, y, k;
	CGPDFScannerPopNumber(scanner, &k);
	CGPDFScannerPopNumber(scanner, &y);
	CGPDFScannerPopNumber(scanner, &m);
	CGPDFScannerPopNumber(scanner, &c);
	
	parser->strokeColor.setCMYK(c, m, y, k);
}

// set fill color (Grey)
void op_g(CGPDFScannerRef scanner, void* info)
{
	PDFParser* parser = (PDFParser*)info;

	float g;
	CGPDFScannerPopNumber(scanner, &g);
	
	parser->fillColor.set(g,g,g);
}

// set stroke color (Grey)
void op_G(CGPDFScannerRef scanner, void* info)
{
	PDFParser* parser = (PDFParser*)info;

	float g;
	CGPDFScannerPopNumber(scanner, &g);
	
	parser->strokeColor.set(g,g,g);
}

// set fill color (RGB)
void op_rg(CGPDFScannerRef scanner, void* info)
{
	PDFParser* parser = (PDFParser*)info;

	float r, g, b;
	CGPDFScannerPopNumber(scanner, &b);
	CGPDFScannerPopNumber(scanner, &g);
	CGPDFScannerPopNumber(scanner, &r);
	
	//parser->fillColor.set255(r,g,b);
	parser->fillColor.set(r,g,b);

}

// set stroke color (RGB)
void op_RG(CGPDFScannerRef scanner, void* info)
{
	PDFParser* parser = (PDFParser*)info;

	float r, g, b;
	CGPDFScannerPopNumber(scanner, &b);
	CGPDFScannerPopNumber(scanner, &g);
	CGPDFScannerPopNumber(scanner, &r);
	

	parser->strokeColor.set(r,g,b);
}

void op_w(CGPDFScannerRef scanner, void* info)
{
	PDFParser* parser = (PDFParser*)info;
	
	float lw;
	CGPDFScannerPopNumber(scanner, &lw);
	
	parser->strokeWidth = lw;
}

// set clipping path
void op_W(CGPDFScannerRef scanner, void* info)
{
}

// close shape without filling or stroking
void op_n(CGPDFScannerRef scanner, void* info)
{
	PDFParser* parser = (PDFParser*)info;
	parser->points.clear();
}

// move to command
void op_m(CGPDFScannerRef scanner, void* info)
{
	PDFParser* parser = (PDFParser*)info;
	
	poPoint p;
	CGPDFScannerPopNumber(scanner, &p.y);
	CGPDFScannerPopNumber(scanner, &p.x);

	p.mark = 100;
	parser->points.push_back(p);
}

void op_l(CGPDFScannerRef scanner, void* info)
{
	PDFParser* parser = (PDFParser*)info;
	
	poPoint p;
	CGPDFScannerPopNumber(scanner, &p.y);
	CGPDFScannerPopNumber(scanner, &p.x);
	
	parser->points.push_back(p);
}

void op_c(CGPDFScannerRef scanner, void* info)
{
	PDFParser* parser = (PDFParser*)info;

	float x1, y1, x2, y2, x3, y3;
	CGPDFScannerPopNumber(scanner, &y3);
	CGPDFScannerPopNumber(scanner, &x3);
	CGPDFScannerPopNumber(scanner, &y2);
	CGPDFScannerPopNumber(scanner, &x2);
	CGPDFScannerPopNumber(scanner, &y1);
	CGPDFScannerPopNumber(scanner, &x1);

	poPoint p;
	poPoint c = parser->points.back();
	float dt = parser->curveInterpStep;
	for(float t=dt; t<1.0f; t+=dt)
	{
		float invt = 1 - t;
		p.x = invt*invt*invt*c.x + 3*t*invt*invt*x1 + 3*t*t*invt*x2 + t*t*t*x3;
		p.y = invt*invt*invt*c.y + 3*t*invt*invt*y1 + 3*t*t*invt*y2 + t*t*t*y3;
		
		parser->points.push_back(p);
	}
}

void op_v(CGPDFScannerRef scanner, void* info)
{
	PDFParser* parser = (PDFParser*)info;

	float x2, y2, x3, y3;
	CGPDFScannerPopNumber(scanner, &y3);
	CGPDFScannerPopNumber(scanner, &x3);
	CGPDFScannerPopNumber(scanner, &y2);
	CGPDFScannerPopNumber(scanner, &x2);
	
	poPoint p;
	poPoint c = parser->points.back();
	float dt = parser->curveInterpStep;
	float x1 = c.x;
	float y1 = c.y;
	for(float t=0; t<1.0f; t+=dt)
	{
		float invt = 1 - t;
		p.x = invt*invt*invt*c.x + 3*t*invt*invt*x1 + 3*t*t*invt*x2 + t*t*t*x3;
		p.y = invt*invt*invt*c.y + 3*t*invt*invt*y1 + 3*t*t*invt*y2 + t*t*t*y3;
		
		parser->points.push_back(p);
	}
}

void op_y(CGPDFScannerRef scanner, void* info)
{
	PDFParser* parser = (PDFParser*)info;

	float x1, y1, x3, y3;
	CGPDFScannerPopNumber(scanner, &y3);
	CGPDFScannerPopNumber(scanner, &x3);
	CGPDFScannerPopNumber(scanner, &y1);
	CGPDFScannerPopNumber(scanner, &x1);

	float x2 = x3;
	float y2 = y3;

	poPoint p;
	poPoint c = parser->points.back();
	float dt = parser->curveInterpStep;
	for(float t=0; t<1.0f; t+=dt)
	{
		float invt = 1 - t;
		p.x = invt*invt*invt*c.x + 3*t*invt*invt*x1 + 3*t*t*invt*x2 + t*t*t*x3;
		p.y = invt*invt*invt*c.y + 3*t*invt*invt*y1 + 3*t*t*invt*y2 + t*t*t*y3;
		
		parser->points.push_back(p);
	}
}

// close the current path
void op_h(CGPDFScannerRef scanner, void* info)
{
//	PDFParser* parser = (PDFParser*)info;
//	poPoint p = parser->points.front();
//	parser->points.push_back(p);
}

// a closed rectangle
void op_re(CGPDFScannerRef scanner, void* info)
{
	PDFParser* parser = (PDFParser*)info;

	float x, y, w, h;
	CGPDFScannerPopNumber(scanner, &h);
	CGPDFScannerPopNumber(scanner, &w);
	CGPDFScannerPopNumber(scanner, &y);
	CGPDFScannerPopNumber(scanner, &x);
	
	poPoint p(x, y);
	p.mark = 100;
	
	parser->points.push_back(p);
	parser->points.push_back(poPoint(x+w, y));
	parser->points.push_back(poPoint(x+w, y+h));
	parser->points.push_back(poPoint(x, y+h));
}

// make a shape, fill it, add it to the display list
void op_f(CGPDFScannerRef scanner, void* info)
{
	PDFParser* parser = (PDFParser*)info;
	parser->makeFillShapeFromCurrentState();
}

// close path, make shape, stroke, add to display list
void op_s(CGPDFScannerRef scanner, void* info)
{
	PDFParser* parser = (PDFParser*)info;
	parser->points.push_back(parser->points.front());
	parser->makeStrokeShapeFromCurrentState();
}

// make shape, stroke, add to display list
void op_S(CGPDFScannerRef scanner, void* info)
{
	PDFParser* parser = (PDFParser*)info;
	parser->makeStrokeShapeFromCurrentState();
}

void op_BDC(CGPDFScannerRef scanner, void* info)
{
//	printf("BDC\n");
}

void op_EMC(CGPDFScannerRef scanner, void* info)
{
//	printf("EMC\n");
}


poPDFShape::poPDFShape()
:	filename(""), curveInterpLimit(0.05f)
{
	setName(getClassName());
	setFilename("");
	
	registry.addSeparator( getClassName() );
	registry.add("curveInterpLimit", &curveInterpLimit);
	registry.add("filename", &filename);
}

poPDFShape::poPDFShape( char* theFileName )
:	filename(""), curveInterpLimit(0.05f)
{
	setName(getClassName());
	setFilename(theFileName);
	
	registry.addSeparator( getClassName() );
	registry.add("curveInterpLimit", &curveInterpLimit);
	registry.add("filename", &filename);
	
	readFromFile();
}

poPDFShape::~poPDFShape()
{}
	
poObject* poPDFShape::duplicate( bool copyChildren, poObject* copyIntoObject )
{
	// either make new poShape2D or accept external one
	poPDFShape* obj = (poPDFShape*) copyIntoObject;
	if ( obj==NULL )
		obj = new poPDFShape();
	
	obj->curveInterpLimit = curveInterpLimit;
	obj->setFilename(getFilename());
	
	poShape2D::duplicate( copyChildren, obj );
	return obj;
}


void poPDFShape::init()
{
	poShape2D::init();
	if(filename != "")
		readFromFile();
}

void poPDFShape::setFilename(const char* fn)
{
	if(fn == NULL)
		return;
		
	filename = fn;
}

void poPDFShape::readFromFile()
{
	printf("filename %s\n", filename.c_str() );
	
	PDFParser parser(this);
	parser.openPDF(filename.c_str());
	parser.parsePDF();
}

#endif
