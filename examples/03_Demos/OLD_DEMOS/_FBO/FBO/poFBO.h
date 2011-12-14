//
//  poFBO.h
//  FBO
//
//  Created by Steve Varga on 9/21/11.
//  Copyright 2011 Potion Design. All rights reserved.
//

#ifndef FBO_poFBO_h
#define FBO_poFBO_h

#include "poObject.h"
#include "poTexture.h"

class poFBO : public poObject {
public:
    poFBO(uint w, uint h);
    ~poFBO();
    
    GLuint uid;
    
    bool isValid() const;
    void doSetUp(poObject *obj);
    void doSetDown(poObject *obj);
    
    void resize(uint w, uint h);
    
    poTexture *texture() const;
private:
    poTexture *tex;
};


#endif
