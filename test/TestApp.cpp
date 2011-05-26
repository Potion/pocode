#include "TestApp.h"
#include "poSimpleDrawing.h"
#include "poShape2D.h"
#include "poImage.h"
#include "poTextBox.h"
#include "poShapeBasics2D.h"

#define TEST_OBJ_ID 1000

poObject *createObjectForID(uint uid) {
	if( uid==TEST_OBJ_ID )
        return new TestObj();
	return NULL;
}

void setupApplication() {
	applicationCreateWindow(TEST_OBJ_ID, WINDOW_TYPE_NORMAL, "MainWindow 1", 100, 100, 1024, 768);
}

void cleanupApplication() {
}



TestObj::TestObj() {
    poCamera2D* cam = new poCamera2D(poColor::black);
    this->addModifier( cam );
}

void TestObj::update()
{
    
}

void TestObj::preDraw()
{

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


