#include "Helpers.h"
#include "poCamera.h"
#include "poTextBox.h"
#include "poApplication.h"
#include "TextCachingApp.h"
#include "poShapeBasics2D.h"
#include "poResourceLoader.h"

poObject *createObjectForID(uint uid) {
	return new TextCachingApp();
}

void setupApplication() {
	applicationCreateWindow(0, WINDOW_TYPE_NORMAL, "TextCaching", 100, 100, 600, 600);
}

void cleanupApplication() {
}

TextCachingApp::TextCachingApp() {
	addModifier(new poCamera2D(poColor::white));
	
	std::stringstream ss;
	for(int i=0; i<10; i++) 
		ss << "I will not, ever again. ";
	
	poShape2D *shp = addChild(new poRectShape(200,200));
	shp->fillColor = poColor::orange;
	shp->rotation_tween.set(360).setRepeat(PO_TWEEN_REPEAT_REGULAR).setDuration(10).setTweenFunction(linearFunc).start();
	shp->alignment(PO_ALIGN_CENTER_CENTER);
	shp->generateStroke(10);
	shp->strokeColor = poColor::red;
	shp->position = getWindowCenter();
	
	poTextBox *tb = addChild(new poTextBox(500,300));
	tb->font(getFont("Helvetica","Regular"));
	tb->position.set(50,200,0);
	tb->textColor = poColor::green;
	tb->text(ss.str());
	tb->textSize(20);
	tb->layout();
	
	tb = addChild((poTextBox*)tb->copy());
	tb->cacheToTexture(false);
	tb->layout();
	tb->position.y += tb->textBounds().height + 10;
}

TextCachingApp::~TextCachingApp() {}
void TextCachingApp::eventHandler(poEvent *event) {}
void TextCachingApp::messageHandler(const std::string &msg, const poDictionary& dict) {}
