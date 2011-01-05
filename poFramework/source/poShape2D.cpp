#include "poShape2D.h"
#include "poTexture.h"


#ifdef _WIN32
#define GL_VOID_CALL (void(__stdcall*)())
#else
#define CALLBACK
#ifdef i386
#define GL_VOID_CALL (GLvoid (*)())
#else
#define GL_VOID_CALL (GLvoid (*)(...)) 
#endif
#endif

void tessellate(poPointVector& points, const poPointVector& textureCoords);
void CALLBACK combineCallback(double coords[5], float* vertexData[4], float weight[4], float** dataOut);

poShape2D::poShape2D()
{
	enableFill = true;
	fillColor.set(.5f,.5f,.5f,1.f);
	strokeColor.set(0,0,0,1);
	strokeWidth = 1;
	useThickStrokeMethod = false;
	usePointInside3D = false;
	includeChildrenInBounds = true;
	
	fillDrawStyle = GL_POLYGON;
	strokeDrawStyle = GL_LINE_LOOP;
	strokePlacement = PO_STROKE_CENTER;

	hOrigin = PO_ALIGN_LEFT;
	vOrigin = PO_ALIGN_BOTTOM;

	callListID = -1;
	
	registry.addSeparator( getClassName() );
	registry.add("enableFill", &enableFill );
	registry.add("fillColor", &fillColor );
	registry.add("strokeColor", &strokeColor );
	registry.add("strokeWidth", &strokeWidth );
	registry.add("fillDrawStyle", (int*) &fillDrawStyle );
	registry.add("strokeDrawStyle",(int*)  &strokeDrawStyle );
	registry.add("strokePlacement", (int*)  &strokePlacement );
	registry.add("hOrigin", (int*)&hOrigin);
	registry.add("vOrigin", (int*)&vOrigin);
}

poShape2D::~poShape2D()
{
	pointList.clear();
	pointColorList.clear();
	texCoordList.clear();
	
	if ( callListID != -1 )
		glDeleteLists( callListID, 1 );
}


poObject* poShape2D::duplicate( bool copyChildren, poObject* copyIntoObject )
{
	// either make new poShape2D or accept external one
	poShape2D* obj = (poShape2D*) copyIntoObject;
	if ( obj==NULL )
		obj = new poShape2D();
	
	// set poShape2D specific properties
	obj->enableFill = enableFill;
	obj->fillColor = fillColor;
	obj->strokeColor = strokeColor;
	obj->strokeWidth = strokeWidth;
	obj->useThickStrokeMethod = useThickStrokeMethod;
	obj->usePointInside3D = usePointInside3D;
	
	obj->fillDrawStyle = fillDrawStyle;
	obj->strokeDrawStyle = strokeDrawStyle;
	obj->strokePlacement = strokePlacement;
	
	obj->callListID = -1;
	
	obj->pointList = pointList;
	obj->pointColorList = pointColorList;
	obj->texCoordList = texCoordList;
	
	// set poObject proeperties
	poObject::duplicate( copyChildren, obj );
	return obj;
}

void poShape2D::init()
{
	poObject::init();
	
	// doTessellation();	// in general, we shouldn't tesselate
}

void poShape2D::draw()
{	
	glColor4f(fillColor.R, fillColor.G, fillColor.B, fillColor.A*masterAlphaControl);
	if(callListID != -1) {
		glCallList(callListID);
	}
	else if(!texCoordList.empty() && !pointColorList.empty())
		drawFillWithColorPointsAndTexture();
	else if(!texCoordList.empty())
		drawFillWithTexture();
	else if(!pointColorList.empty() && pointColorList.size() == pointList.size())
		drawFillWithColorPoints();
	else if(enableFill)
		drawFillStandard();

	if(strokeWidth > 0)
		drawStroke();
		
	if ( poObject::doDebugDraw )
	{
		debugDraw();
		poObject::debugDraw();
	}
	
#ifdef WIN32
	glDisable( GL_TEXTURE_2D );
#else
	glDisable( GL_TEXTURE_RECTANGLE_EXT );
#endif
}

void poShape2D::drawFillStandard()
{
	glBegin(fillDrawStyle);
//	if(fillDrawStyle == GL_POLYGON && pointList.size() > 3)
//	{
//		poPoint normal = pointList[1].cross(pointList[3]).normalize();
//		normal.z *= -1;
//		glNormal3fv((GLfloat*)&normal);
//	}

	for(int i=0; i<pointList.size(); i++)
	{
		glVertex3fv((float*)&pointList[i]);
	}
	
	glEnd();
}

void poShape2D::drawFillWithTexture()
{
	glBegin(fillDrawStyle);
	for(int i=0; i<pointList.size(); i++) 
	{
		glTexCoord2fv( (float*) &texCoordList[i] );
		glVertex3fv((float*)&pointList[i]);
	}
	glEnd();
}

void poShape2D::drawFillWithColorPoints()
{
	glBegin(fillDrawStyle);
	for(int i=0; i<pointList.size(); i++) 
	{
		glColor4f(pointColorList[i].R, pointColorList[i].G, pointColorList[i].B, pointColorList[i].A*masterAlphaControl);
		glVertex3fv((float*)&pointList[i]);
	}
	glEnd();
}

void poShape2D::drawFillWithColorPointsAndTexture()
{
	glBegin(fillDrawStyle);
	for(int i=0; i<pointList.size(); i++) 
	{
		glTexCoord2fv( (float*) &texCoordList[i] );
		glColor4f(pointColorList[i].R, pointColorList[i].G, pointColorList[i].B, pointColorList[i].A*masterAlphaControl);
		glVertex3fv((float*)&pointList[i]);
	}
	glEnd();
}

void poShape2D::drawStroke()
{
#ifdef WIN32
	glDisable( GL_TEXTURE_2D );
#else
	glDisable( GL_TEXTURE_RECTANGLE_EXT );
#endif

	if(strokeWidth < 4 && useThickStrokeMethod==false)
	{
		glLineWidth(strokeWidth);
		glColor4f(strokeColor.R, strokeColor.G, strokeColor.B, strokeColor.A*masterAlphaControl);
		
		glBegin(strokeDrawStyle);
		for(int i=0; i<pointList.size(); i++) 
		{
			if(pointList[i].mark == 100)
			{
				glEnd();
				glBegin(strokeDrawStyle);
			}
			glVertex3fv((float*)&pointList[i]);
		}
		glEnd();
		glLineWidth(1);
	} else
	{
		// default to centering stroke
		float insideOffset = -strokeWidth / 2.f;
		float outsideOffset = strokeWidth / 2.f;
		if(strokePlacement == PO_STROKE_INSIDE)
		{
			insideOffset = -strokeWidth;
			outsideOffset = 0;
		} else if(strokePlacement == PO_STROKE_OUTSIDE)
		{
			insideOffset = 0;
			outsideOffset = strokeWidth;
		}

		glColor4f(strokeColor.R, strokeColor.G, strokeColor.B, strokeColor.A*masterAlphaControl);
		glBegin(GL_TRIANGLE_STRIP);
		int finish = pointList.size();
		if ( strokeDrawStyle == GL_LINE_LOOP )
			finish += 1;
		for(int i=0; i<finish; i++)
		{
			poPoint nextPoint = pointList[(i+1) % pointList.size()];
			poPoint thisPoint = pointList[i % pointList.size()];
			poPoint prevPoint = pointList[(i-1+pointList.size()) % pointList.size()];
			
			// get average of previous normal and next normal
			poPoint nextNormal = nextPoint - thisPoint;
			nextNormal = nextNormal.get2Dnormal();
			nextNormal.normalize();
			
			poPoint prevNormal = thisPoint - prevPoint;
			prevNormal = prevNormal.get2Dnormal();
			prevNormal.normalize();
			
			poPoint normal = (nextNormal + prevNormal) / 2.0;
			if ( strokeDrawStyle == GL_LINE_STRIP || strokeDrawStyle == GL_LINES )
			{
				if ( i==0 )
					normal = nextNormal;
				if ( i==finish-1 )
					normal = prevNormal;
			}
			normal.normalize();
			
			// compute inside and outside points
			poPoint inside = normal * insideOffset;
			poPoint outside = normal * outsideOffset;
			poPoint shift1 = thisPoint + inside;
			poPoint shift2 = thisPoint + outside;
			
			if(thisPoint.mark == 100) 
			{
				glEnd();
				glBegin(GL_TRIANGLE_STRIP);
			}

			glVertex3fv((float*)&shift1);
			glVertex3fv((float*)&shift2);
		}
		glEnd();
	}
}

void poShape2D::debugDraw()
{	
	poObject::debugDraw();
}

void poShape2D::addPoint(float x, float y)
{
	pointList.push_back(poPoint(x,y,0));
}

void poShape2D::addPoint(const poPoint& p)
{
	pointList.push_back(p);
}

void poShape2D::addPoint(const poPoint& p, const poColor& c)
{
	addPoint(p);
	pointColorList.push_back(c);
}

void poShape2D::shiftAllPoints(const poPoint& p)
{
	for(int i=0; i<pointList.size(); i++)
		pointList[i] += (poPoint&)p;
	calculateBounds();
}

void poShape2D::setOrigin(poHoriOrientation hor, poVertOrientation vor)
{
	hOrigin = hor;
	vOrigin = vor;
	
	updateOrigin();
}

void	poShape2D::updateOrigin()
{
	calculateBounds();
	
	float width = boundingBox.right - boundingBox.left;
	float height = boundingBox.top - boundingBox.bottom;
	
	poPoint shifter;
	if(hOrigin == PO_ALIGN_RIGHT) shifter.x = width;
	else if(hOrigin == PO_ALIGN_LEFT) shifter.x = 0;
	else if(hOrigin == PO_ALIGN_HORI_CENTER) shifter.x = width / 2;
	
	if(vOrigin == PO_ALIGN_TOP) shifter.y = height;
	else if(vOrigin == PO_ALIGN_BOTTOM) shifter.y = 0;
	else if(vOrigin == PO_ALIGN_VERT_CENTER) shifter.y = height / 2;
	
	shifter.x += boundingBox.left;
	shifter.y += boundingBox.bottom;
	
	shiftAllPoints(-shifter);
}

void poShape2D::calculateBounds()
{
	boundingBox.initFarBounds();
	
	for(int i=0; i<pointList.size(); i++)
	{
		if(pointList[i].x > boundingBox.right)
			boundingBox.right = pointList[i].x;
		if(pointList[i].x < boundingBox.left)
			boundingBox.left = pointList[i].x;
		if(pointList[i].y > boundingBox.top)
			boundingBox.top = pointList[i].y;
		if(pointList[i].y < boundingBox.bottom)
			boundingBox.bottom = pointList[i].y;
	}
	
	if ( includeChildrenInBounds )
		poObject::calculateRelativeBounds( this, boundingBox );
}


void poShape2D::calculateRelativeBounds( poObject* relativeObj, poBoundingBox& bbox )
{
	for(int i=0; i<pointList.size(); i++)
	{
		poPoint P = pointList[i];
		if ( getParent()==NULL )
			return;
		localToGlobal( P );
		relativeObj->globalToLocal( P );
		
		if( P.x > bbox.right )
			bbox.right = P.x;
		if( P.x < bbox.left )
			bbox.left = P.x;
		if( P.y > bbox.top )
			 bbox.top = P.y;
		if( P.y < bbox.bottom )
			bbox.bottom = P.y;
	}

	if ( includeChildrenInBounds )
		poObject::calculateRelativeBounds( relativeObj, bbox );
}


bool poShape2D::pointInside(float x, float y)
{	
	if ( usePointInside3D )
		return pointInside3D(x,y);
	
	if( !visible || matrixSet.isZero() ) {
		return false;
	}
	
    poPoint P;
    P.set( x, y, 0 );
	globalToLocal(P.x, P.y, P.z);
	
	// test point inside for given drawstyle
    if ( (fillDrawStyle == GL_POLYGON || fillDrawStyle == GL_TRIANGLE_FAN) && pointList.size() >= 3 )
    {
        for( int i=1; i<pointList.size()-1; i++ )
            if ( pointInTriangle( P, pointList[0], pointList[i], pointList[i+1] ) )
				return true;
        if (fillDrawStyle == GL_TRIANGLE_FAN)
            if ( pointInTriangle( P, pointList[0], pointList[1], pointList.back() ))
				return true;
    }
    else if ( fillDrawStyle == GL_TRIANGLE_STRIP && pointList.size() >= 3 )
    {
        for( int i=0; i<pointList.size()-2; i++ )
            if ( pointInTriangle( P, pointList[i], pointList[i+1], pointList[i+2] ) )
                return true;
    }
    
	for( int i=0; i<numChildren(); i++ )
		if ( getChild(i)->pointInside(x, y) )
			return true;
    
    return false;
}


bool	poShape2D::pointInside3D(float x, float y)
{
	if( !visible || matrixSet.isZero() ) {
		return false;
	}
	
    poPoint P;
    P.set( x, y, 0 );
	
	// test point inside for given drawstyle
    if ( (fillDrawStyle == GL_POLYGON || fillDrawStyle == GL_TRIANGLE_FAN) && pointList.size() >= 3 )
    {
        for( int i=1; i<pointList.size()-1; i++ )
            if ( pointInTriangle3D( P, pointList[0], pointList[i], pointList[i+1] ) )
				return true;
        if (fillDrawStyle == GL_TRIANGLE_FAN)
            if ( pointInTriangle3D( P, pointList[0], pointList[1], pointList.back() ))
				return true;
    }
    else if ( fillDrawStyle == GL_TRIANGLE_STRIP && pointList.size() >= 3 )
    {
        for( int i=0; i<pointList.size()-2; i++ )
            if ( pointInTriangle3D( P, pointList[i], pointList[i+1], pointList[i+2] ) )
                return true;
    }
    
	for( int i=0; i<numChildren(); i++ )
		if ( getChild(i)->pointInside(x, y) )
			return true;
    
    return false;
}

bool poShape2D::pointInTriangle3D( poPoint P, poPoint A, poPoint B, poPoint C )
{
	localToGlobal( A );
	localToGlobal( B );
	localToGlobal( C );
	A.z = B.z = C.z = 0;
	if ( pointInTriangle( P, A, B, C ) )
		return true;
	return false;
}

void poShape2D::placeTexture(poTexture* tex, poTextureScaleOption scaleOption, poHoriOrientation halign, poVertOrientation valign)
{
	calculateBounds();
	poBoundingBox ownBounds = getBounds();
	
	float bbw = ownBounds.right - ownBounds.left;
	float bbh = ownBounds.top - ownBounds.bottom;
	float imgW = tex->width;
	float imgH = tex->height;
		
	if(scaleOption == PO_TEX_FIT_MIN || scaleOption == PO_TEX_FIT_MAX)
	{
		float aspectTex, aspectBounds;
		aspectTex = (float) tex->width / (float) tex->height;
		aspectBounds = bbw / bbh;
		
		if(aspectTex > aspectBounds)
			scaleOption = PO_TEX_FIT_H;
		else
			scaleOption = PO_TEX_FIT_V;
	}
	
	if(scaleOption == PO_TEX_FIT_H)
	{
		imgW = bbw;
		imgH = ((float)tex->height / (float)tex->width) * imgW;
	} else if(scaleOption == PO_TEX_FIT_V)
	{
		imgH = bbh;
		imgW = ((float)tex->width / (float)tex->height) * imgH;
	}
	
	texCoordList.clear();
	
	poPoint upperLeft(ownBounds.left, ownBounds.top, 0);
	for(int i=0; i<pointList.size(); i++)
	{
		poPoint diff = pointList[i] - upperLeft;
		diff.y *= -1;

		poPoint texPoint;
#ifdef WIN32
		texPoint.x = diff.x / imgW;
		texPoint.y = diff.y / imgH;
#endif
#ifndef WIN32
		texPoint.x = diff.x / imgW * (float) tex->width;
		texPoint.y = diff.y / imgH * (float) tex->height;
#endif	
		texCoordList.push_back(texPoint);
	}

	float diffX=0.f, diffY=0.f;
	
	if(halign == PO_ALIGN_LEFT) {}
	else if(halign == PO_ALIGN_HORI_CENTER)
		diffX = (bbw - imgW) / 2;
	else if(halign == PO_ALIGN_RIGHT)
		diffX = bbw - imgW;
	
	if(valign == PO_ALIGN_TOP) {}
	else if(valign == PO_ALIGN_VERT_CENTER)
		diffY = (bbh - imgH) / 2;
	else if(valign == PO_ALIGN_BOTTOM)
		diffY = bbh - imgH;
		
	transformTexture(diffX, diffY, 1.f, 0.f);
}

void poShape2D::transformTexture(float transX, float transY, float scale, float rotation)
{
	for(int i=0; i<texCoordList.size(); i++)
	{
		texCoordList[i].rotate2D(rotation);
		texCoordList[i].scale(scale);
		texCoordList[i].translate(transX, transY);
	}
}

void poShape2D::doTessellation()
{
	if(callListID != -1)
		printf("WARNING: poShape2D->doTessellation: over-writing call list\n");
		
	callListID = glGenLists(1);
	glNewList(callListID, GL_COMPILE);
	tessellate(pointList, texCoordList);
	glEndList();
}

void poShape2D::makeCallList()
{
	if(callListID != -1)
		printf("WARNING: poShape2D->makeCallList: over-writing call list\n");
	
	callListID = -1;
	int tmp = glGenLists(1);
	glNewList(tmp, GL_COMPILE);
	draw();
	glEndList();
	
	callListID = tmp;
}

void tessellate(poPointVector& points, const poPointVector& textureCoords)
{
	GLUtesselator* tobj = gluNewTess();
#ifdef _WIN32
	gluTessCallback(tobj, GLU_BEGIN, (void(CALLBACK*)()) glBegin);
	gluTessCallback(tobj, GLU_VERTEX, (void(CALLBACK*)()) glVertex3fv ); 
	gluTessCallback(tobj, GLU_END, (void(CALLBACK*)()) glEnd );
	gluTessCallback(tobj, GLU_TESS_COMBINE, (void(CALLBACK*)()) combineCallback);
#else
	gluTessCallback(tobj, GLU_BEGIN, (GLvoid(*)()) glBegin);
	gluTessCallback(tobj, GLU_VERTEX, (GLvoid(*)()) glVertex3fv ); 
	gluTessCallback(tobj, GLU_END, (GLvoid(*)()) glEnd );
	gluTessCallback(tobj, GLU_TESS_COMBINE, (GLvoid(*)()) combineCallback);
#endif
	
    gluTessBeginPolygon(tobj, NULL);
	gluTessBeginContour(tobj);

    GLdouble vertex[3];
    for( int i=0; i<points.size(); i++ )
    {
        if ( points[i].mark == 100 )
        {
           gluTessEndContour(tobj);
           gluTessBeginContour(tobj);
        }
        vertex[0] = points[i].x;
        vertex[1] = points[i].y;
        vertex[2] = points[i].z;
		points[i].w = 0;
        points[i].t = 0;
        if ( !textureCoords.empty() )
        {
            points[i].w = textureCoords[i].w;
            points[i].t = textureCoords[i].t;
        }
        gluTessVertex( tobj, vertex, (void*)(&points[i]) );
    }

	gluTessEndContour(tobj);
    gluTessEndPolygon(tobj);
	
	gluDeleteTess(tobj);
}

void CALLBACK combineCallback(double coords[5], float* vertexData[4], float weight[4], float** dataOut)
{
    GLfloat *vertex;
    
    vertex = (GLfloat *) malloc(6 * sizeof(float));
    vertex[0] = coords[0];
    vertex[1] = coords[1];
    vertex[2] = coords[2];
    vertex[3] = coords[3];
    vertex[4] = coords[4];
    
    *dataOut = (float*) vertex;
}
