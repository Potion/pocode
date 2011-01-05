#include "poObject.h"
#include "poMath.h"

#ifndef _PO_APP_H_
#define _PO_APP_H_

class poApp : public poObject
{
public:
	poApp(int w, int h);
	virtual ~poApp() {}

	virtual void addChild(poObject* child);
	virtual void drawApp();
	
	virtual void initGL();
	virtual void setupLighting();
	
	//virtual void resetView();
	
	void	cleanCanvas();
	void	cleanCamera();
		
	virtual void mouseDown( float x, float y, int modifiers );
	virtual void mouseUp( float x, float y, int modifiers );
	virtual void mouseDrag( float x, float y, int modifiers );
	virtual void mouseMove( float x, float y, int modifiers);
	virtual void keyDown( char key, float x, float y, int modifiers );
	virtual void specialKeyDown( int key, unsigned int modifiers );
	virtual void resize(int newW, int newH);

	virtual void readApp( const char* fileName );
	virtual void writeApp( const char* fileName );
	virtual void registerClasses();
	
	virtual void messageHandler(const char* message, const poDictionary& data = poDictionary() );
	
	void		launchApp();
	
	void		capture( int x, int y, int w, int h );
	
	static			int appW, appH;
	poColor			clearColor;
	
	poObject*		rootNode;
	
	static poApp*	theApp;
};


class poAppFrame : public poObject
{
public:
	poAppFrame();
	poAppFrame( int W, int H );
	
	void	construct( int W, int H );
	
	virtual poObject*	duplicate( bool copyChildren=false, poObject* copyIntoObject=NULL );
	virtual const char*	getClassName() { return "poAppFrame"; };
		
	virtual void debugDraw();
	
	int		frameWidth, frameHeight;
};


#endif

