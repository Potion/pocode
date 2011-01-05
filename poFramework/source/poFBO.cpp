#include "poFBO.h"
#include "poTexture.h"

poFBO::poFBO(float w, float h)
:	tex(NULL), width(w), height(h)
{
	glEnable(GL_FRAMEBUFFER_EXT);
	glGenFramebuffersEXT(1, (GLuint*)&fboID);
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, fboID);

	glEnable(GL_RENDERBUFFER_EXT);
	glGenRenderbuffersEXT(1, (GLuint*)&depthbufferID);
	glBindRenderbufferEXT(GL_RENDERBUFFER_EXT, depthbufferID);
	glRenderbufferStorageEXT(GL_RENDERBUFFER_EXT, GL_DEPTH_COMPONENT, width, height);
	glFramebufferRenderbufferEXT(GL_FRAMEBUFFER_EXT, GL_DEPTH_ATTACHMENT_EXT, GL_RENDERBUFFER_EXT, depthbufferID);
	
	
	tex = new poTexture();
	tex->width = width;
	tex->height = height;
	tex->components = 4;
	
	glGenTextures(1, (GLuint*)&tex->bindID);
	tex->format = poTexture::determineFormat(3);
	glBindTexture(GL_TEXTURE_RECTANGLE_EXT, tex->bindID);
	
	glTexParameteri(GL_TEXTURE_RECTANGLE_EXT,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
	glTexParameteri(GL_TEXTURE_RECTANGLE_EXT,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
	glTexImage2D(GL_TEXTURE_RECTANGLE_EXT, 0, GL_RGB,  width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
	glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT0_EXT, GL_TEXTURE_RECTANGLE_EXT, tex->bindID, 0);

	glBindTexture(GL_TEXTURE_RECTANGLE_EXT, 0);
	glBindRenderbufferEXT(GL_RENDERBUFFER_EXT, 0);
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);
	
	GLenum status = glCheckFramebufferStatusEXT(GL_FRAMEBUFFER_EXT);
	if(status != GL_FRAMEBUFFER_COMPLETE_EXT)
	{
		printf("bad fbo setup: %d\n", status);
	}
}

poFBO::~poFBO()
{
	glDeleteTextures(1, (GLuint*)&tex->bindID);
	glDeleteRenderbuffersEXT(1, (GLuint*)&depthbufferID);
	glDeleteFramebuffersEXT(1, (GLuint*)&fboID);
}

void poFBO::setup(poObject*)
{
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, fboID);
	glPushAttrib(GL_VIEWPORT_BIT);
	glViewport(0, 0, width, height);
	glPushMatrix();
	// texture gets rendered to y-reversed
	// and, this matrix has to be column-major
	/* // does not work in 3D - jared
	float mat[] = {	1,0,0,0,
					0,-1,0,0,
					0,0,1,0,
					0,height,0,1 };
	glMultMatrixf(mat);
	*/
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); 
}

void poFBO::setdown(poObject*)
{
	glPopMatrix();
	glPopAttrib();
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);
}
