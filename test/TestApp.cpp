#include "TestApp.h"
#include "poSimpleDrawing.h"
#include "poShape2D.h"
#include "poImage.h"
#include "poTextBox.h"
#include "poShapeBasics2D.h"

#define TEST_OBJ_ID 1000

poObject *createObjectForID(uint uid) {
	switch(uid) {
		case TEST_OBJ_ID:
			return new TestObj();
			break;
	}	
	return NULL;
}

void setupApplication() {
	applicationCreateWindow(TEST_OBJ_ID, WINDOW_TYPE_NORMAL, "MainWindow 1", 100, 100, 1400, 1050);
}

void cleanupApplication() {
}



TestObj::TestObj() {
	/*tb = new poTextBox();
	tb->bounds.include(300,300);
	tb->setRegularFont(new poFont("Helvetica", 0, 30));
	tb->setItalicFont(new poFont("Helvetica", FONT_ITALIC, 30));
	tb->setBoldFont(new poFont("Helvetica", FONT_BOLD, 50));
	addChild(tb);*/
    
    poRectShape* R = new poRectShape( 200,100 );
    R->fillColor( poColor::white );
    R->strokeWidth( 3 ).strokeColor( 1,0,0 );
    R->position.set( 100,100,0 );
    R->setAlignment( PO_ALIGN_CENTER_CENTER );
    addChild( R );
    
    R->addEvent( PO_MOUSE_PRESS_EVENT, this, "action", poDictionary().setInt("thing",5).setInt("stuff",2) );
}

void TestObj::update()
{
    
}

void TestObj::preDraw()
{
	glClear(GL_COLOR_BUFFER_BIT);
	
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0, getWindowWidth(), getWindowHeight(), 0, -1, 1);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
    
    glClearColor(0,0,0,1);
	
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void TestObj::draw() {

	//tb->text("hello world <em>con brio</em> with <strong>some bold text</strong>. and some symbols: ®†¥");
	//tb->layout();
}

void TestObj::postDraw()
{
}    


void TestObj::eventHandler(poEvent *event) {
    
    if ( event->message == "action" )
    {
        printf("message: %s\n", event->message.c_str() );
        printf("thing: %d\n", event->dict.getInt("thing") );
        printf("stuff: %d\n", event->dict.getInt("stuff") );
        event->source->position = event->position;
        
        //printf("%f %f\n", event->local_position.x, event->local_position.y );
    }
}


