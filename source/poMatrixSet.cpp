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

#include "poMatrixSet.h"
#include "poOpenGLState.h"
#include "poApplication.h"

#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace po {
    MatrixSet::MatrixSet() 
    :   camType(po::CAMERA_2D)
    {}

    void MatrixSet::capture() {
        projection = po::projection();
        modelview = po::modelview();
        viewport = po::viewport();
    }
    
    
    //------------------------------------------------------------------------
    Point MatrixSet::globalToLocal(Point pt) const {
        glm::vec3 p(pt.x, pt.y, pt.z);
        glm::vec4 vp(viewport.x, viewport.y, viewport.width, viewport.height);
        glm::vec3 r = glm::unProject(p, modelview, projection, vp);
        return Point(r.x, r.y, r.z);
    }
    
    
    //------------------------------------------------------------------------
    Point MatrixSet::localToGlobal(Point pt) const {
        glm::vec3 p(pt.x, pt.y, pt.z);
        glm::vec4 vp(viewport.x, viewport.y, viewport.width, viewport.height);
        glm::vec3 r = glm::project(p, modelview, projection, vp);
        return Point(r.x, r.y, r.z);
    }
    
    
    //------------------------------------------------------------------------
    Point MatrixSet::localToGlobal2(Point pt) const {
        return glm::value_ptr(glm::inverse(modelview) * glm::vec4(pt.x,pt.y,pt.z,1.f));
    }
} /*End po Namespace */