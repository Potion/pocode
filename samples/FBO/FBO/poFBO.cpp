#include "poFBO.h"
#include "Helpers.h"
#include "poObject.h"

poFBO::poFBO(uint w, uint h)
:	uid(0)
,	tex(NULL)
{
	resize(w,h);
}

poFBO::~poFBO() {
	if(uid > 0) {
		glDeleteFramebuffers(1, &uid);
		delete tex;
	}
}

void poFBO::resize(uint w, uint h) {
	if(uid > 0 && !(tex->width() == w && tex->height() == h)) {
		glDeleteFramebuffers(1, &uid);
		delete tex;
	}
	
	// create the texture
	tex = new poTexture(GL_RGBA,w,h,w*4,NULL);
	
	// create the framebuffer
	glGenFramebuffers(1, &uid);
	glBindFramebuffer(GL_FRAMEBUFFER, uid);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, tex->uid(), 0);
	// make sure its ok
	GLenum err = glCheckFramebufferStatus(GL_FRAMEBUFFER);
	if(GL_FRAMEBUFFER_COMPLETE != err) {
		log("poFBO: error when creating framebuffer (%d)", err);
		// something was wrong, so clean up
		glDeleteFramebuffers(1, &uid);
		uid = 0;
		// and kill the texture too
		delete tex;
		tex = NULL;
	}
	// unbind so we don't get any surpises
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void poFBO::doSetUp(poObject *obj) {
	glBindFramebuffer(GL_FRAMEBUFFER, uid);
	for(int i=0; i<obj->numModifiers(); i++) {
		poObjectModifier *mod = obj->getModifier(i);
		if(mod != this)
			mod->setUp(obj);
	}
}

void poFBO::doSetDown(poObject *obj) {
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

bool poFBO::isValid() const {
	return uid != 0;
}

poTexture *poFBO::texture() const {
	return tex;
}
