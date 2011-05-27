#include "TestApp.h"

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
    addModifier(new poCamera2D(poColor::black));
	
	poDictionary dict;
	dict.read("test.xml");
	dict.write("test2.xml");
}

void TestObj::update()
{
    
}

void TestObj::preDraw()
{

}

void TestObj::draw() {
}

void TestObj::postDraw()
{
}    


void TestObj::eventHandler(poEvent *event) {
}


