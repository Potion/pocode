#include "FBOApp.h"

#include "Helpers.h"
#include "poCamera.h"
#include "poWindow.h"
#include "poApplication.h"
#include "poShapeBasics2D.h"

static FBOGenerator *generator = NULL;
static FBORenderer *renderer = NULL;

poObject *createObjectForID(uint uid) {
	if(uid == 0) {
		return new FBOGenerator();
	}
	
	return new FBORenderer();
}

void setupApplication() {
	setCurrentPath("../..");
	// first make the window that will draw the scene into the fbo
	generator = static_cast<FBOGenerator*>(applicationCreateWindow(0, WINDOW_TYPE_NORMAL, "FBOGenerator", 0, 0, 600, 600)->rootObject());
	// then a window to show just the texture
	renderer = static_cast<FBORenderer*>(applicationCreateWindow(1, WINDOW_TYPE_NORMAL, "FBORenderer", 600, 0, 600, 600)->rootObject());
	renderer->replaceTexture(generator->fbo->texture());
}

void cleanupApplication() {
}

FBOGenerator::FBOGenerator() {
	// holds the scene that the fbo is capturing
	poObject *holder = new poObject();
	addChild(holder);
	
	// fix the size of the viewport
	poCamera *cam = new poCamera2D();
	holder->addModifier(cam);
	
	// make the fbo modifier and attach it
	fbo = new poFBO(getWindowWidth(), getWindowHeight());
	holder->addModifier(fbo);
	
	holder->addChild(new poRectShape(getWindowWidth(), getWindowHeight()));
	
	// make the scene to render into the fbo (1 rotating shape)
	poShape2D *obj = new poRectShape(200,200);
	obj->placeTexture(poTexture("ike&tina.jpeg"));
	holder->addChild(obj);
	// setup the object's properties and animation
	obj->position(getWindowWidth()/2.f, getWindowHeight()/2.f)
		.alignment(PO_ALIGN_CENTER_CENTER)
		.rotation_tween
			.set(360)
			.setTweenFunction(linearFunc)
			.setDuration(5)
			.setRepeat(PO_TWEEN_REPEAT_REGULAR)
			.start();
	obj->addEvent(PO_MOUSE_PRESS_EVENT, this);
	
	// now we're done with the fbo scene so set up the view of the fbo in this window
	addModifier(new poOrthoCamera(-1,1,1,-1,-1,1));
	// attach the fbo to a rect so we can see it
	obj = new poRectShape(2,2);
	obj->alignment(PO_ALIGN_CENTER_CENTER).position(0,0);
	obj->placeTexture(fbo->texture(), PO_TEX_FIT_EXACT);
	addChild(obj);
	
	// and toggle the fbo for testing
	addEvent(PO_KEY_DOWN_EVENT, this);
	addEvent(PO_WINDOW_RESIZED_EVENT, this);
}

void FBOGenerator::eventHandler(poEvent* e) {
	// if the enter key was pressed, toggle the fbo
	if(e->type == PO_KEY_DOWN_EVENT && e->keyCode == PO_RETURN_KEY) {
		poObjectModifier *mod = getChild(0)->getModifier(1);
		mod->enabled(!mod->enabled());
		// hide the fbo view if we turn the fbo off
		getChild(1)->visible(mod->enabled());
	}
	if(e->type == PO_MOUSE_PRESS_EVENT && e->source == getChild(0)->getChild(1)) {
		getChildAs<poShape2D>(getChild(0),0)->fillColor(randomColor());
	}
}

FBORenderer::FBORenderer()
:	next(NULL)
{
	addModifier(new poOrthoCamera(-1,1,1,-1,-1,1));
	// make a rect to display the texture
	poShape2D *obj = new poRectShape(2,2);
	obj->alignment(PO_ALIGN_CENTER_CENTER).position(0,0);
	addChild(obj);
}

void FBORenderer::update() {
	if(next) {
		poShape2D *obj = getChildAs<poShape2D>(this, 0);
		obj->placeTexture(next, PO_TEX_FIT_EXACT);
		delete next;
        next = NULL;
	}
}

void FBORenderer::replaceTexture(poTexture *tex) {
	next = tex->copy();
}



