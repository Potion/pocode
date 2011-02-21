/*
 *  Copyright 2011 Potion Design. All rights reserved.
 */

#pragma once

#include <string>
#include <vector>
#include <list>
#include <stack>

#include <boost/any.hpp>
#include <boost/bind.hpp>
#include <boost/foreach.hpp>
#include <boost/function.hpp>
#include <boost/noncopyable.hpp>
#include <boost/tuple/tuple.hpp>
#include <boost/unordered_map.hpp>

#include <cinder/app/App.h>
#include <cinder/Color.h>
#include <cinder/gl/gl.h>
#include <cinder/gl/Vbo.h>
#include <cinder/gl/Fbo.h>
#include <cinder/gl/Texture.h>

#include "poMath.h"
#include "poPoint.h"
#include "poRect.h"
#include "poEvent.h"
#include "poDictionary.h"

#ifdef CINDER_MAC
#include <OpenGL/glu.h>
#endif
