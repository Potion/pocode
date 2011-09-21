//
//  Shader.fsh
//  potionCode
//
//  Created by Joshua Fisher on 9/21/11.
//  Copyright 2011 Potion Design. All rights reserved.
//

varying lowp vec4 colorVarying;

void main()
{
    gl_FragColor = colorVarying;
}
