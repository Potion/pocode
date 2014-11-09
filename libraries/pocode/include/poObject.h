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

#pragma once

#include "poTween.h"
#include "poEventCenter.h"
#include "poDictionary.h"
#include "poMatrixSet.h"
#include "poEnums.h"
#include "poPoint.h"
#include "poColor.h"
#include "poRect.h"
#include "poObjectModifier.h"
#include "poXML.h"

namespace po {
    class Object;
    typedef std::vector<Object*> ObjectVec;

    class ObjectModifier;
    typedef std::vector<ObjectModifier*> ObjectModifierVec;

    class poWindow;

    // po::Object is the central object in the pocode library. All pocode graphic objects
    // (including Shape2D, RectShape and TextBox) are subclasses Object.
    // 
    // All Objects and Object subclasses:
    //
    // + Have position, scale, rotation, offset and tranparency (alpha) properties.
    // + Have a list of children that are also Objects (this forms the scene graph).
    // + Can register for and receive events, including mouse, keyboard and touch events.
    // + Have a set of tweens for basic animation (animation may also be done in the "update" method).
    // + Have a list of ObjectModifiers that can modify the object and/or OpenGl state.

    class Object {
        friend class poWindow;
    public:
        // OBJECT CREATION AND DESTRUCTION
        Object();
        Object(const std::string &name);
        Object(int width, int height, const std::string &name="");
        virtual ~Object();
        
        virtual Object*	copy();
        
        // Most Object subclasses should implement all four of these methods.
        // The only method you should call explictly is messageHandler. 
        // The other three are called automatically if the object is added to the scene graph. 
         
        /// WORK-HORSE METHODS
        // "draw" is only necessary if the class does its own OpenGL drawing, instead of using pocode shapes.
        virtual void        draw();
        virtual void		drawAfter();
        
        // Draw the bounds if drawBounds is set.
        virtual void        _drawBounds();
        
        // "update" is called once per frame. It should be used for implementing animation.
        virtual void        update();
        
        // "eventHandler" is called when the object receives an event. Events are registered using "addEvent".
        virtual void        eventHandler(Event *event);
        
        // "messageHander" is a general utility method used for passing messages between any two Objects.
        virtual void        messageHandler(const std::string &msg, const Dictionary& dict=Dictionary(), const Object* sender=NULL);

        //  All events are registed using "addEvent". See "poEnums.h" for a list of all eventTypes.
        // EVENTS
        
        void				addEvent(int eventType, Object *sink, std::string message="", const Dictionary& dict=Dictionary());
        void				removeAllEvents();
        void				removeAllEventsOfType(int eventType);

        // OBJECT DIMENSIONS
        // The bounds are relative local to the object's coordinate system. The frame is relative
        // to the parents coordinate system. 
        
        float				getWidth();
        float               getScaledWidth();
        float				getHeight();
        float               getScaledHeight();
        virtual Rect        getBounds();
        Rect				getFrame();
        Point               getTransformedPoint( Point P );

        // The scene graph is a tree structure composed of Objects and subclasses of Object.
        // A pocode app is itself a Object and is also the root of the tree.
        // The most recently added objects appear on top of previously added objects.
        
        // SCENE GRAPH COMPOSITION
        int                 getNumChildren() const;
        po::Object*			addChild(Object* obj);
        po::Object*			addChild(Object* obj, int idx); 
        po::Object*			addChildBefore(Object* obj, Object* before);
        po::Object*			addChildAfter(Object* obj, Object* after);
        int                 getChildIndex(Object* child);
        po::Object*         getChild(int at_idx);
        po::Object*         getChildWithUID(uint uid);
        po::Object*         getChild(const std::string &with_name);
        po::Object*         getLastChild();
        std::vector<Object*> getChildren(const std::string &with_name);
        bool                removeChild(Object* obj, bool and_delete=false);
        bool                removeChild(int at_idx, bool and_delete=false);
        void                removeAllChildren(bool and_delete=false);
        
        // SCENE GRAPH ORDERING
        void                moveChildToFront(po::Object* child);
        void                moveChildToBack(po::Object* child);
        void                moveChildForward(po::Object* child);
        void                moveChildBackward(po::Object* child);
        
        
        // Object modifiers attach to a Object and may modify it's properties.
        // ObjectModifiers have two virtual methods, doSetUp and doSetDown
        // that are called, respectively, before and afer a Object is drawn.

        // OBJECT MODIFIER OPERATIONS
        po::ObjectModifier*	addModifier(po::ObjectModifier* mod);
        po::ObjectModifier*	getModifier(int idx);
        std::vector<po::ObjectModifier*> const &getModifiers();
        bool                removeModifier(int idx, bool and_delete=true);
        bool                removeModifier(po::ObjectModifier* mod, bool and_delete=true);
        void                removeAllModifiers(bool and_delete=true);
        size_t              getNumModifiers() const;
        
        // pointInside assumes points are in window-native coordinates (0,0 is in the upper left).
        // globalToLocal and localToGlobal are useful in hitTesting and inter-object coordination.
        
        // HIT TESTING & COORDINATE TRANSFORMATIONS
        void				applyTransformation();
        virtual bool        pointInside(Point point, bool localize=false);
        bool                pointInside(float x, float y, float z=0.f, bool localize=false);
        Point               objectToLocal(Object* obj, Point point) const;
        Point               globalToLocal(Point point) const;
        Point               localToGlobal(Point point) const;
        
        // OBJECT ALIGNMENT & BOUNDS CALCULATION
        Alignment           getAlignment() const;
        virtual Object&     setAlignment(Alignment align);
        
        // TWEEN MANAGEMENT
        virtual void        stopAllTweens(bool recurse=false);
        
        //  Parent object should pass the child's XMLNode node into the child.
        // OBJECT SERIALIZATION
        virtual void        read(XMLNode node);
        virtual void        write(XMLNode &node);
        
        
        // SCENE GRAPH TRAVERSAL (DO NOT CALL)
        virtual void        drawTree();
        virtual void        updateTree();
        
        // These properties may be set directly either inside or outside the class.
        // Changes to these properties require no additional computation.
        
        // OBJECT PROPERTIES (DIRECTLY ACCESSIBLE)
        std::string         name;
        float               alpha;
        Point               scale;
        Point               position;
        float               rotation;
        Point               rotationAxis;
        Point               offset;
        bool                visible;
        int                 drawBounds;
        MatrixOrder         matrixOrder;
        
        
        // All Objects have tween operators that may be enabled and disabled independently.
        // By default, all tweens are disabled. See Tween.h for more about tweens.
        
        // OBJECT TWEENS (DIRECTLY ACCESSIBLE)
        Tween<Point>        positionTween;
        Tween<Point>        scaleTween;
        Tween<Point>        offsetTween;
        Tween<float>		alphaTween;
        Tween<float>		rotationTween;

        
        po::Object*         getParent() const;
        uint                getUID() const;
        //!alpha with parent alpha pre-multiplied
        float               getAppliedAlpha() const;
        MatrixSet&          getMatrixSet();
        int                 getDrawOrder() const;
        
        //Recursively check parents for definitive visibility
        bool                isVisible(); 
        
        // MEMORY MANAGEMENT
        int                 getSizeInMemoryTree();
        virtual int         getSizeInMemory();
        static const int    INVALID_INDEX = -1;
        
        EventMemory       *eventMemory;
        int                 drawOrder;
        
    protected:
        // PROTECTED PROPERTIES
        // new tween types should be updated within updateAllTweens
        virtual void        updateAllTweens();
        float               trueAlpha;
        void				clone(po::Object* obj);
        
        
        MatrixSet         matrices;
        
    private:
        
        // PRIVATE PROPERTIES
        
        // "children" is a vector of Objects. It is the basis for the Object scene graph/tree structure.
        // Every Object that has been added as child has a parent. Be careful not to access the parent
        // in a Object constructor, since it will not have one yet.
        // CHILDREN, PARENT AND MODIFIERS
        
        po::ObjectVec         children;
        po::Object*           parent;
        po::ObjectModifierVec modifiers;
        
        uint                uid;
        Alignment         alignment;
    };
} /* End po namespace */
