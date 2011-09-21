//
//  Shader.vsh
//  potionCode
//
//  Created by Joshua Fisher on 9/21/11.
//  Copyright 2011 Potion Design. All rights reserved.
//

attribute vec4 position;
attribute vec4 color;

varying vec4 colorVarying;

uniform float translate;

void main()
{
    gl_Position = position;
    gl_Position.y += sin(translate) / 2.0;

    colorVarying = color;
}
