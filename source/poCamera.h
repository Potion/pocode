/*
 *	Copyright 2012 Potion Design. All rights reserved.
 *	This file is part of pocode.
 *
 *	pocode is free software: you can redistribute it and/or modify
 *	it under the terms of the GNU Lesser General Public License as 
 *	published by the Free Software Foundation, either version 3 of 
 *	the License, or (at your option) any later version.
 *
 *	pocode is distributed in the hope that it will be useful,
 *	but WITHOUT ANY WARRANTY; without even the implied warranty of
 *	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *	GNU Lesser General Public License for more details.
 *
 *	You should have received a copy of the GNU Lesser General Public 
 *	License along with pocode.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

//
//  Camera.h
//  pocode
//
//  Created by Jared Schiffman on 5/20/11.
//  Copyright 2011 Potion Design. All rights reserved.
//

#pragma once


#include "poObject.h"
#include "poColor.h"

// CLASS NOTES
//
// Camera dervies from ObjectModifier. It is the parent class of Camera2D,
// CameraOrtho and poPerspectiveCamera.
//
// Camera is responsible for clearing the background on every frame and for
// the color of the background.
//
// The virtual setProjection() method is the method responsible for setting
// the OpenGL projection matrix.
//

namespace po {
    class Camera : public ObjectModifier {
    public:
        Camera();
        Camera(poColor clear);
        virtual ObjectModifier *copy();
        
        // BACKGROUND CLEARING and BACKGROUND COLOR
        bool                clearsBackground() const;
        Camera*           setClearsBackground(bool b);
        poColor             getBackgroundColor() const;
        Camera*           setBackgroundColor(poColor color);
        Camera*           setBackgroundColor(float r, float g, float b, float a=1.f);
        
        // RESET MODEL VIEW
        // Setting this to true means the model view matrix will be reset by the camera.
        // Setting this to false means the model view matrix will be inherited.
        bool                resetsModelview() const;
        Camera*           setResetsModelview(bool b);
        
        // FIXED VIEW SIZE
        bool                isFixedSize() const;
        Camera*           setFixedSize(bool b, poPoint p=poPoint());
        
        static CameraType   getCurrentCameraType() { return currentCameraType; };
        
    protected:
        void				clone(Camera *cam);
        
        // SETUP and SETDOWN
        // The camera projection matrix is setup (pushed) by doSetUp 
        // and setdown (popped) by doSetDown.
        virtual void        doSetUp( Object* obj );
        virtual void        doSetDown( Object* obj );

        virtual void        setProjection();
        virtual void        setModelview();
        virtual void        saveAndUpdateGLSettings();
        virtual void        restoreGLSettings();
        
        poPoint             fixedSize;
        bool                reset;

        static CameraType   currentCameraType;
        
    private:
        bool                clearsBG;
        poColor             backgroundColor;
        bool                isSizeFixed;
    };

    // CLASS NOTES
    //
    // Camera2D implements a simple two-dimensional camera. All objects viewed by this camera
    // should have Z values equal to 0.

    class Camera2D : public Camera {
    public:
        Camera2D();
        Camera2D(poColor clear);
        virtual ObjectModifier *copy();

    protected:
        void				clone(Camera2D *cam);
        virtual void        setProjection();
    };

    // CLASS NOTES
    //
    // poOrthoCamera implements an orthographic camera. This is a 3D camera in which
    // all perspective lines are parallel, as in SimCity.
    //
    // By default the "near" and "far" clipping planes are set to -1 and 1. These can
    // be set to greater values in the expanded constructor or via the set() method.
    //

    class poOrthoCamera : public Camera {
    public:
        poOrthoCamera();
        poOrthoCamera(float w, float h, float n, float f);
        poOrthoCamera(float x1, float y1, float x2, float y2, float n, float f);
        virtual ObjectModifier *copy();

        void                set(poRect r, float n=-1, float f=1);
        void                set(float x1, float y1, float x2, float y2, float n, float f);
        poRect              get() const;
        
    protected:
        void				clone(poOrthoCamera *cam);
        virtual void        setProjection();

    private:
        float               x1, y1, x2, y2, nearClip, farClip;
    };

    // CLASS NOTES
    //
    // poPerspectiveCamera implements a traditional 3D camera. The camera's view is defined
    // two points: the cameraPosition and the lookAtPosition. By default, these are
    // both set to (0,0,0). At least one of these MUST be set in order for the camera
    // to see anything. Assuming the objects being viewed are situated near the origin
    // of the space, the camera may be set up as follows:
    //
    //      Cam = new poPerspectiveCamera(90, 1, 1000);
    //      Cam->cameraPosition( poPoint(0,0,-100 ) );
    //      Cam->lookAtPosition( poPoint(0,0,0 ) );
    //      addModifier( Cam );
    //
    // Notice that the Z value of the cameraPosition is set to -100. This effectively pulls
    // camera back from the scene so it can see the objects at the origin.

    class poPerspectiveCamera : public Camera {
    public:
        poPerspectiveCamera(float fov, float near, float far);
        virtual ObjectModifier* copy();

        // CAMERA POSITION and LOOKAT POSITION
        poPoint                 cameraPosition() const;
        poPerspectiveCamera*    cameraPosition(poPoint p);
        
        poPoint                 lookAtPosition() const;
        poPerspectiveCamera*    lookAtPosition(poPoint p);
        
        // This method positions the 3D camera so that at Z=0, there is a 1:1 dimension match
        // to the XY coordinate system of a 2D camera. The only difference is that the origin
        // is at the center of the screen, instead of the upper left corner. The method is
        // dependendent on the field of view (fov), and always positions the camera along
        // the Z axis.
        void                    setupFor2DOnZPlane();
        
        bool                    doDepthTesting;
        bool                    doBackfaceCulling;
        
    protected:
        void					clone(poPerspectiveCamera *cam);
        
        virtual void            doSetUp(Object* obj);
        virtual void            setProjection();
        virtual void            setModelview();
        virtual void            saveAndUpdateGLSettings();
        
    private:
        float                   fov, nearClip, farClip;
        poPoint                 lookAtPos, cameraPos;
    };
}