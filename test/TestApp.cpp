#include "TestApp.h"
#include "poSimpleDrawing.h"
#include "poShape2D.h"
#include "poImage.h"
#include "poTextBox.h"
#include "poShapeBasics2D.h"

void setupApplication() {
	// make the application
	poApplication *app = poApplication::get();
	
	// make the window and add a root
	poWindow *win = new poWindow(WINDOW_TYPE_NORMAL, "MEH", 100, 100, 800, 600);
	win->setRootObject(new TestObj());
	// add the window
	app->addWindow(win);
}

void cleanupApplication() {
	// delete all the windows
	poApplication::get()->quit();
}

poTextBox *tb;

TestObj::TestObj() {
	/*tb = new poTextBox();
	tb->bounds.include(300,300);
	tb->setRegularFont(new poFont("Helvetica", 0, 30));
	tb->setItalicFont(new poFont("Helvetica", FONT_ITALIC, 30));
	tb->setBoldFont(new poFont("Helvetica", FONT_BOLD, 50));
	addChild(tb);*/
    
    poRectShape* R = new poRectShape( 200,100 );
    R->fillColor( poColor(1,1,1) );
    R->strokeWidth( 2 );
    R->strokeColor( poColor(1,0,1) );
    R->enableStroke( true );
    R->enableFill( false );
    R->position.set( 100,100,0 );
    R->generateStroke();

    addChild( R );
}

void TestObj::update() {
	glClearColor(0,0,0,1);
}

void TestObj::preDraw()
{
	glClear(GL_COLOR_BUFFER_BIT);
	
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0, getWindowWidth(), getWindowHeight(), 0, -1, 1);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

void TestObj::draw() {

	//tb->text("hello world <em>con brio</em> with <strong>some bold text</strong>. and some symbols: ®†¥");
	//tb->layout();
}

void TestObj::postDraw()
{
    
}

void TestObj::eventHandler(poEvent *event) {
}
