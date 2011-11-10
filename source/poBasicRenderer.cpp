#include "poBasicRenderer.h"
#include "poOpenGLState.h"
#include "poHelpers.h"

const char * colored_shader = 
"	[[uniforms]]							\n"
"	uniform mat4 mvp;						\n"
"	uniform vec4 color;						\n"

"	[[varyings]]							\n"

"	[[vertex]]								\n"
"	attribute vec4 position;				\n"

"	void main() {							\n"
"		gl_Position = mvp * position;		\n"
"	}										\n"

"	[[fragment]]							\n"
"	void main() {							\n"
"		gl_FragColor = color;				\n"
"	}										\n";

const char * geom_3D_shader = 
"	[[uniforms]]							\n"
"	uniform mat4 modelView;                 \n"
"   uniform mat4 projection;                \n"
"	uniform vec4 color;						\n"
"   uniform vec3 lightPos;                  \n"

"	[[varyings]]							\n"
"   varying vec3 normalVar;                 \n"
"   varying vec3 lightDir;                  \n"

"	[[vertex]]								\n"
"	attribute vec4 position;				\n"
"   attribute vec3 normal;                  \n"

"	void main() {							\n"
"       normalVar = (modelView * vec4(normal,0.0)).xyz; \n"
"       vec4 pos = modelView * position;    \n"
"       lightDir = lightPos - pos.xyz;  \n"
"		gl_Position = projection * pos;     \n"
"	}										\n"

"	[[fragment]]							\n"
"	void main() {							\n"
"       float NdotL = max(dot(normalVar,normalize(lightDir)),0.0); \n"
"		gl_FragColor = color * NdotL;       \n"
"	}										\n";

const char * textured_shader = 
"	[[uniforms]]							\n"
"	uniform mat4 mvp;						\n"
"	uniform vec4 color;						\n"
"	uniform sampler2D tex;					\n"
"	uniform int isAlphaMask;				\n"

"	[[varyings]]							\n"
"	varying vec2 texCoord;					\n"

"	[[vertex]]								\n"
"	attribute vec4 position;				\n"
"	attribute vec2 textureCoordinates;		\n"

"	void main() {							\n"
"		texCoord = textureCoordinates;		\n"
"		gl_Position = mvp * position;		\n"
"	}										\n"

"	[[fragment]]							\n"
"	void main() {							\n"
"		vec4 texColor = texture2D(tex, texCoord);\n"
"		gl_FragColor = texColor * color;	\n"
		
"		if(bool(isAlphaMask)) {				\n"
"			gl_FragColor = vec4(1.0,1.0,1.0,texColor.a) * color;\n"
"		}									\n"
"	}										\n";

static boost::thread_specific_ptr<poBasicRenderer> instance;

poBasicRenderer *poBasicRenderer::get() {
#if defined(POTION_IOS)
	static poBasicRenderer *instance = NULL;
	if(!instance)
		instance = new poBasicRenderer();
	return instance;
#else
	if(!instance.get())
		instance.reset(new poBasicRenderer());
	return instance.get();
#endif
}

poBasicRenderer::poBasicRenderer() {
	rebuild();
}

void poBasicRenderer::setFromState() {
	poOpenGLState *ogl = poOpenGLState::get();
	
	// but texturing isn't on by default
	if(ogl->texture.boundID > 0) {
		if(active->getUid() != texturedShader.getUid()) {
			glUseProgram(texturedShader.getUid());
			active = &texturedShader;
		}
		glUniform1i(texturedShader.uniformLocation("tex"), 0);
		glUniform1i(texturedShader.uniformLocation("isAlphaMask"), ogl->texture.isMask);
	}
	else if(active->getUid() != colorShader.getUid()) {
		glUseProgram(colorShader.getUid());
		active = &colorShader;
	}
	
	glUniformMatrix4fv(active->uniformLocation("mvp"), 1, GL_FALSE, glm::value_ptr(ogl->matrix.transformation()));
	glUniform4fv(active->uniformLocation("color"), 1, &ogl->color.R);
}

void poBasicRenderer::setFor3D() {
	poOpenGLState *ogl = poOpenGLState::get();
    
    glUseProgram(geom3DShader.getUid());
    active = &geom3DShader;
    
    glUniform4fv(active->uniformLocation("color"), 1, &ogl->color.R);
    glUniformMatrix4fv(active->uniformLocation("modelView"), 1, GL_FALSE, glm::value_ptr(ogl->matrix.getModelview()));
    glUniformMatrix4fv(active->uniformLocation("projection"), 1, GL_FALSE, glm::value_ptr(ogl->matrix.getProjection()));
    
    glm::vec4 lightPos(0,1000,0,0);
    glUniform3fv(active->uniformLocation("lightPos"), 1, glm::value_ptr(ogl->matrix.getCameraMatrix() * lightPos));
}

void poBasicRenderer::rebuild() {
	glUseProgram(0);
	
	std::istringstream color(colored_shader);
	colorShader.loadSource(color);
	if(!colorShader.compile())
		throw std::runtime_error("color shader didn't compile");
	glBindAttribLocation(colorShader.getUid(), 0, "position");
	if(!colorShader.link())
		throw std::runtime_error("color shader didn't link");
	
	std::istringstream texture(textured_shader);
	texturedShader.loadSource(texture);
	if(!texturedShader.compile())
		throw std::runtime_error("texture shader didn't compile");
	glBindAttribLocation(texturedShader.getUid(), 0, "position");
	glBindAttribLocation(texturedShader.getUid(), 1, "textureCoordinates");
	if(!texturedShader.link())
		throw std::runtime_error("texture shader didn't link");
    
    std::istringstream geom(geom_3D_shader);
    geom3DShader.loadSource(geom);
    if(!geom3DShader.compile())
        throw std::runtime_error("3D geometry shader didn't compile");
    glBindAttribLocation(geom3DShader.getUid(), 0, "position");
    glBindAttribLocation(geom3DShader.getUid(), 1, "normal");
    if(!geom3DShader.link())
        throw std::runtime_error("3D geometry shader didn't link");
	
	active = &colorShader;
	glUseProgram(active->getUid());
}

