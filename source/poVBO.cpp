//
//  poVBO.cpp
//  poFramework4
//
//  Created by Joshua Fisher on 6/26/11.
//  Copyright 2011 Potion Design. All rights reserved.
//

#include "poVBO.h"
#include "poPoint.h"
#include "poColor.h"

struct poVertex {
	poVertex(poPoint p) : p(p) {}
	
	poPoint p, tc, n;
	poColor c;
};

class poVBO {
public:
	poVBO() {
		glGenBuffers(2, uid);
	}
	
	~poVBO() {
		glDeleteBuffers(2, uid);
	}

	void pushVertsToCard(std::vector<poVertex> verts, GLenum usage) {
		glBindBuffer(GL_ARRAY_BUFFER, uid[0]);
		glBufferData(GL_ARRAY_BUFFER, sizeof(poVertex)*verts.size(), &verts[0], usage);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}
	
	void pushIdxToCard(std::vector<uint> idx, GLenum usage) {
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, uid[1]);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint)*idx.size(), &idx[0], usage);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}
	
	void map(GLenum usage) {
		glBindBuffer(GL_ARRAY_BUFFER, uid[0]);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, uid[1]);
		
		mapped_vert = glMapBuffer(GL_ARRAY_BUFFER, usage);
		mapped_idx = glMapBuffer(GL_ELEMENT_ARRAY_BUFFER, usage);
	}
	
	void unmap() {
		if(mapped_vert) {
			glUnmapBuffer(GL_ARRAY_BUFFER);
			glUnmapBuffer(GL_ELEMENT_ARRAY_BUFFER);
		}
		
		if(mapped_idx) {
			glBindBuffer(GL_ARRAY_BUFFER, 0);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
		}
	}
	
	void setup() {
		glBindBuffer(GL_ARRAY_BUFFER, uid[0]);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, uid[1]);
		
		glEnableClientState(GL_VERTEX_ARRAY);
		glVertexPointer(3, GL_FLOAT, sizeof(poVertex), 0);
		
		glEnableClientState(GL_TEXTURE_COORD_ARRAY);
		glTexCoordPointer(3, GL_FLOAT, sizeof(poVertex), sizeof(poPoint));
		
		glEnableClientState(GL_NORMAL_ARRAY);
		glNormalPointer(3, GL_FLOAT, sizeof(poVertex), sizeof(poPoint)*2);
		
		glEnableClientState(GL_COLOR_ARRAY);
		glColorPointer(3, GL_FLOAT, sizeof(poVertex), sizeof(poPoint)*3);
	}
	
	void setdown() {
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}
	
	poVertex &vertex(uint idx);

	// only valid 
	poVertex *mapped_vert;
	uint *mapped_idx;
	GLuint uid[2];
};