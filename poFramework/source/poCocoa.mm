//
//  poCocoa.mm
//  poFramework3
//
//  Created by Jared Schiffman on 3/28/08.
//  Copyright 2008 Potion Design LLC. All rights reserved.
//

#import <Cocoa/Cocoa.h>


bool    cocoaOpenFile( char* fileNameString )
{
    NSOpenPanel *panel = [NSOpenPanel openPanel];
    
    if ( [panel runModal] )
    {
        NSString *filename = [panel filename];
        strcpy( fileNameString, [filename cString] );
        return true;
    }
    return false;
}

bool    cocoaSaveFile( char* fileNameString )
{
    NSSavePanel *panel = [NSSavePanel savePanel];
    
    if ( [panel runModal] )
    {
        NSString *filename = [panel filename];
        strcpy( fileNameString, [filename cString] );
        return true;
    }
    return false;
}
