/*
 *  poVisionSystem.cpp
 *  poCameraApp
 *
 *  Created by Young on 2/23/09.
 *  Copyright 2009 __MyCompanyName__. All rights reserved.
 *
 */

#include "poVisionSystem.h"


// ======================== poVisionSystem ======================== //

bool poVisionSystem::overrideCheckSum = false;

poVisionSystem::poVisionSystem( int vWidth, int vHeight, int camID )
{	
	printf("========== making poVisionSystem ============\n");
	this->camID = camID;
	rawBuffer = new poVisionImageBuffer( vWidth, vHeight );
	rawBuffer->FLIP_VIDEO_VERT = false;
	rawBuffer->FLIP_VIDEO_HORZ = false;
	
	maskBuffer = new poVisionImageBuffer( vWidth, vHeight );
	maskBuffer->setAllPixels(0);
	maskBuffer->fillRect( 10,10, 320-10, 240-10, 255 );]

	readConfigFile();

	previousChecksum = 0;
	
	Ref[0][0] = new poPoint(275, 164);
	Ref[0][1] = new poPoint(56, 161);
	Ref[1][0] = new poPoint(272, 7);
	Ref[1][1] = new poPoint(67, 8);

	DrawX = DrawY = 0.0;
	DrawMode = 0;
	DrawColor = 0;
	DrawWidth = 1;
	
	printf("========== finished poVisionSystem ============\n");
}

void poVisionSystem::readConfigFile()
{
	int ref_x = 0;
	int ref_y = 0;

	char filename [ 50 ];
	sprintf( filename, "local/config_cam%d.txt", camID );
	FILE* F = fopen(filename,"r");
	if ( F != NULL )
	{
		fscanf( F, "threshold %d\n",  &threshold );
		fscanf( F, "ref_x %d\n",  &ref_x );
		fscanf( F, "ref_y %d\n",  &ref_y );
		fscanf( F, "gridBoxSize %d\n",  &gridBoxSize );
		fscanf( F, "objectMinBoxSize  %d\n",  &objectMinBoxSize  );
		fscanf( F, "accumUpdateSpeed %f\n",  &accumUpdateSpeed );
		fscanf( F, "accumRejectionRange %d\n",  &accumRejectionRange );

		printf( "cam%d threshold %d\n",  camID, threshold );
		printf( "cam%d ref_x %d\n",  camID, ref_x );
		printf( "cam%d ref_y %d\n",  camID, ref_y );
		printf( "cam%d gridBoxSize %d\n",  camID, gridBoxSize );
		printf( "cam%d objectMinBoxSize %d\n",  camID, objectMinBoxSize );
		printf( "cam%d accumUpdateSpeed %f\n",  camID, accumUpdateSpeed );
		printf( "cam%d accumRejectionRange %d\n",  camID, accumRejectionRange );

		refPoint.set( ref_x, ref_y, 0 );

		fclose(F);
	}
	else
	{
		printf("ERROR: couldn't open %s file to read\n", filename);
		exit(0);
	}
}

void poVisionSystem::WriteConfigFile()
{
	int ref_x = 0;
	int ref_y = 0;
	
	char filename [ 50 ];
	sprintf( filename, "config_cam%d.txt", camID );
	FILE* F = fopen(filename,"w");
	if ( F != NULL )
	{
		fprintf( F, "threshold %d\n",  threshold );
		fprintf( F, "ref_x %d\n",  ref_x );
		fprintf( F, "ref_y %d\n",  ref_y );
		fprintf( F, "gridBoxSize %d\n",  gridBoxSize );
		fprintf( F, "objectMinBoxSize  %d\n",  objectMinBoxSize  );
		fprintf( F, "accumUpdateSpeed %f\n",  accumUpdateSpeed );
		fprintf( F, "accumRejectionRange %d\n",  accumRejectionRange );
		fprintf( F, "UsingThreshMap %d\n",  usingMAP  );
		fprintf( F, "thresholdMIN %d\n",  thresholdMIN );
		fprintf( F, "thresholdMAX %d\n",  thresholdMAX );
		fclose(F);
	}
	else
		printf("ERROR: couldn't open %s file to write\n", filename);
}


void	poVisionSystem::process( unsigned char* buffer )
{
	// just copy buffer here
	rawBuffer->copy( buffer );
	
}


/*void	poVisionSystem::sendDataToCameraConnection( int cameraID )
{
	if ( cameraID < 0 || cameraID >= 4)
	{
		printf("ERROR: poVisionSystem::sendDataToCameraConnection cameraID is %d\n", cameraID );
		return;
	}

	//stuff the shapelist into a datum object (a datum is a collection of shapes from one camera)
	datum shapeData = datum();
	int finalShapeCount = 0;
	for( int i=0; i<gridAlgorithm->shapeCount && i<OBJECTS_TO_SEND; i++ )
	{
		if ( gridAlgorithm->shapeList[ i ].boxCount <= objectMinBoxSize )
			continue;

		PackedComponent packt = PackedComponent();
		
		packt.tipX = gridAlgorithm->shapeList[ i ].closestPoint.x;		//gridAlgorithm->shapeList[ i ].midPoint.x;		// THE CHOSEN POINT
		packt.tipY = gridAlgorithm->shapeList[ i ].closestPoint.y;		//gridAlgorithm->shapeList[ i ].midPoint.y;

		packt.closestX = gridAlgorithm->shapeList[ i ].closestPoint.x;
		packt.closestY = gridAlgorithm->shapeList[ i ].closestPoint.y;

		packt.furthestX = gridAlgorithm->shapeList[ i ].furthestPoint.x;
		packt.furthestY = gridAlgorithm->shapeList[ i ].furthestPoint.y;

		packt.centroidX = gridAlgorithm->shapeList[ i ].midPoint.x;
		packt.centroidY = gridAlgorithm->shapeList[ i ].midPoint.y;

		packt.leftX = gridAlgorithm->shapeList[ i ].leftPoint.x;
		packt.leftY = gridAlgorithm->shapeList[ i ].leftPoint.y;

		packt.rightX = gridAlgorithm->shapeList[ i ].rightPoint.x;
		packt.rightY = gridAlgorithm->shapeList[ i ].rightPoint.y;

		packt.topX = gridAlgorithm->shapeList[ i ].topPoint.x;
		packt.topY = gridAlgorithm->shapeList[ i ].topPoint.y;

		packt.bottomX = gridAlgorithm->shapeList[ i ].bottomPoint.x;
		packt.bottomY = gridAlgorithm->shapeList[ i ].bottomPoint.y;

		shapeData.objects[ finalShapeCount ] = packt;
		finalShapeCount++;
	}
	shapeData.numObjects = finalShapeCount; //gridAlgorithm->shapeCount; //num of objects

	//set the assigned camera
	//poCameraConnection* camCon = poCameraConnection::getInstance();
	//camCon->cameraData->cameraSet[cameraID] = shapeData;
}*/


/*void	poVisionSystem::sendShapesToCameraConnection( int cameraID )
{
	if ( cameraID < 0 || cameraID >= 4)
	{
		printf("ERROR: poVisionSystem::sendDataToCameraConnection cameraID is %d\n", cameraID );
		return;
	}

	poCameraConnection* camCon = poCameraConnection::getInstance();
	poCameraShapeSet* shapeSet = &camCon->fourCameraShapeSet->cameraShapeSets[cameraID];

	shapeSet->numShapes = 0;
	for( int i=0; i<gridAlgorithm->shapeCount && i<MAX_SHAPES_PER_CAMERA; i++ )
	{
		if ( gridAlgorithm->shapeList[ i ].boxCount <= objectMinBoxSize )
			continue;

		gridShape* theGridShape = &gridAlgorithm->shapeList[i ];
		poCameraShape* camShape = &shapeSet->shapes[ shapeSet->numShapes ];

		camShape->shapeID = theGridShape->trackedShapeID;
		camShape->age = theGridShape->trackedAge;
		camShape->immediateVelocity.set( theGridShape->immediateVelocity );
		camShape->averageVelocity.set( theGridShape->averageVelocity );

		camShape->gridBoxCount = theGridShape->boxCount;

		camShape->midPoint.set( theGridShape->midPoint );
		camShape->brightestPoint.set( theGridShape->brightestPoint );
		camShape->bottomPoint.set( theGridShape->bottomPoint );
		camShape->topPoint.set( theGridShape->topPoint );
		camShape->leftPoint.set( theGridShape->leftPoint );
		camShape->rightPoint.set( theGridShape->rightPoint );
		camShape->closestPoint.set( theGridShape->closestPoint );
		camShape->furthestPoint.set( theGridShape->furthestPoint );
		camShape->majorAxisAngle = theGridShape->majorAxisAngle;
		camShape->tipAxisAngle = theGridShape->tipAxisAngle;

		for( int c=0; c<NUM_CAMERA_SHAPE_CONTOUR_POINTS; c++ )
		{
			camShape->contourPoints[c].set( theGridShape->contour[c] );
		}
		
		shapeSet->numShapes++;
	}
}*/


void	poVisionSystem::resetBuffers()
{
}

bool poVisionSystem::getBox(int i, int j)
{
	return gridAlgorithm->theGrid[i][j].isFull;
}

int poVisionSystem::get_videoWidth()
	{ return gridAlgorithm->videoWidth; }

int poVisionSystem::get_videoHeight()
	{ return gridAlgorithm->videoHeight; }

int poVisionSystem::get_boxWidth()
	{ return gridAlgorithm->boxWidth; }

int poVisionSystem::get_boxHeight()
	{ return gridAlgorithm->boxHeight; }

int poVisionSystem::get_gridWidth()
	{ return gridAlgorithm->gridWidth; }
int poVisionSystem::get_gridHeight()
	{ return gridAlgorithm->gridHeight; }


// ======================== poVisionSubstractionSystem ======================== //

poVisionSubstractionSystem::poVisionSubstractionSystem( int vWidth, int vHeight, int camID ) : poVisionSystem( vWidth, vHeight, camID )
{
	printf("========== making poVisionSubstractionSystem ============\n");

	maskProc = new poVisionAndProc( vWidth, vHeight );

	// we blur the raw image for both systems and use an accumulation buffer
	blurProc = new poVisionBlurProc( vWidth, vHeight );
	blurProc->blurRepeat = 1;
	accumProc = new poVisionAccumulateProc( vWidth, vHeight );
	accumProc->rejectionRange = accumRejectionRange;
	accumProc->updateSpeed = accumUpdateSpeed;
	
	// this is the grid algorithm
	gridAlgorithm = new poVisionGridAlgorithm( vWidth, vHeight, gridBoxSize );
	gridAlgorithm->referencePoint = refPoint;

	// mask
	char maskFileName [ 50 ];
	sprintf( maskFileName, "local/masks/mask_cam%d.png", camID );
	maskBuffer = new poVisionImageBuffer( rawBuffer->width, rawBuffer->height, maskFileName );
	// threshold Map
	char mapFileName [ 128 ];
	sprintf( mapFileName, "local/masks/thresholdmap_cam%d.png", camID );
	threshMapBuffer = new poVisionImageBuffer( rawBuffer->width, rawBuffer->height, mapFileName );
	threshMapBufferACTIVE = new poVisionImageBuffer( rawBuffer->width, rawBuffer->height, mapFileName );

	// this is a separate system just for background subtraction againt accumulation buffer
	diffProc = new poVisionDifererenceProc( vWidth, vHeight );
	threshProc = new poVisionThresholdProc( vWidth, vHeight );
	threshProc->threshold = threshold;
	
	//make the threshMapProc
	//thresholdMIN + thresholdMAX set in parent class;
	threshMapProc = new poVisionThresholdMAPProc(vWidth, vHeight);
	threshMapProc->MapBufferREFERENCE = threshMapBuffer;
	threshMapProc->MapBufferACTIVE = threshMapBufferACTIVE;
	threshMapProc->setMINMAX(thresholdMIN, thresholdMAX);
	threshMapProc->setMap();
	
	//Choose which threshold Method being used...
	//usingMAP set in parent class;
	if(usingMAP == true)
		threshMETHOD = threshMapProc;
	else
		threshMETHOD = threshProc;
	
	//make drawing labels
	/* (label[0] = new poTextBox("masked frame", 200, 100);
	label[1] = new poTextBox("raw frame", 200, 100);
	label[2] = new poTextBox("threshold frame", 200, 100);
	label[3] = new poTextBox("grid + shapes frame", 200, 100);*/
	//for(int i=0; i<4; i++)
	//	label[i]->makeStaticTexture();
	
	//Make Variables text boxes
	/*for(int i=0; i<6; i++)
	{
		variables[i] = new poTextBox("...", 100, 20);
		variables[i]->doLayout();
		variables[i]->enableFill = true;
		variables[i]->fillColor = poColor::grey;
	}*/
	
	//Make Labels
	MAPvariableLabels[0] = new std::string("TYPE: ");
	MAPvariableLabels[1] = new std::string("threshMIN: ");
	MAPvariableLabels[2] = new std::string("threshMAX: ");
	MAPvariableLabels[3] = new std::string("AccumSpeed: ");
	MAPvariableLabels[4] = new std::string("AccumRej: ");
	MAPvariableLabels[5] = new std::string("MinBoxObj: ");
	
	NONMAPvariableLabels[0] = new std::string("TYPE: ");
	NONMAPvariableLabels[1] = new std::string("threshold: ");
	NONMAPvariableLabels[2] = new std::string("AccumSpeed: ");
	NONMAPvariableLabels[3] = new std::string("AccumReject: ");
	NONMAPvariableLabels[4] = new std::string("MinBoxObj: ");
	NONMAPvariableLabels[5] = new std::string(" ");
	
	//Make fixed Variables
	std::stringstream out; std::string s;
	out << "camID: " << camID << ", gridBoxSize: " << gridBoxSize;
	s += out.str();
	//FixedVariables = new poTextBox( s.c_str(), 500, 20);
	//FixedVariables->makeStaticTexture();
	
	//Set EditMode
	EditMode = 0;
	//start up text display by calling handleEditKey with -1
	handleEditKey(-1);

	maskPTS = new poShape2D();
	maskPTS->enableFill = false;
	maskPTS->strokeWidth = 1;
	maskPTS->strokeColor = poColor::blue;

	OuterMask = new poRectShape(320+2,240+2);
	OuterMask->enableFill = false;
	OuterMask->strokeWidth = 1;
	OuterMask->strokeColor = poColor::green;

	Instructions = new poTextBox("< > switch among settings, + - to change them\nd toggle stencil vs. pencil on mask\n[ ] to change pencil size w to toggle color\nx clears stencil, a fills stencil in\nq saves mask, s saves settings", 500, 500);
	//Instructions->makeStaticTexture();
}

void poVisionSubstractionSystem::editMask(float x, float y,  bool click, bool dragging)
{
	if(DrawMode == 0 && dragging == false && click == true)
		addMaskPoint(x, y);
	if(DrawMode == 1)
		drawOnMask(x, y, dragging);
}

void poVisionSubstractionSystem::addMaskPoint(float x, float y)
{
	//check if this is over the actual image buffer
	if(OuterMask->pointInside(x, y))
		maskPTS->addPoint(x, y);
}

void poVisionSubstractionSystem::drawOnMask(float x, float y, bool dragging)
{
	if(OuterMask->pointInside(x, y))
	{
		//find position within mask reference:
			int Xoff = (int) DrawX;
			int Yoff = (int) DrawY;
			int mx = x - Xoff;
			int my = y - Yoff;
		//set number of pixels drawn from mouse position
		//0 is 1 pixel drawn, 1 is 3 pixels drawn etc.
			int drawsize = DrawWidth;
		//calculate and bound box coordinates
			int x0 = mx-drawsize;
			if(x0 < 0) x0 = 0;
			if(x0 > 319) x0 = 319;
			int x1 = mx+drawsize;
			if(x1 < 1) x1 = 0;
			if(x1 > 319) x1 = 319;
			int y0 = my-drawsize;
			if(y0 < 0) y0 = 0;
			if(y0 > 239) y0 = 239;
			int y1 = my+drawsize;
			if(y1 < 1) y1 = 0;
			if(y1 > 239) y1 = 239;
		//draw rect
			maskBuffer->fillRect( x0, y0, x1, y1, DrawColor );
	}
}

void poVisionSubstractionSystem::deleteRecentMaskPoint()
{

}

void poVisionSubstractionSystem::GenerateMask()
{
	int Xoff = (int) DrawX;
	int Yoff = (int) DrawY;

	for(int x=0; x<320; x++)
		for(int y=0; y<240; y++)
		{
			if(maskPTS->pointInside((float) x+Xoff, (float) y+Yoff) )
				maskBuffer->setPixel(x, 239-y, 255);
			else
				maskBuffer->setPixel(x, 239-y, 0);
		}
}

void poVisionSubstractionSystem::saveMask()
{
	//get mask size
	/*	int IMAGE_W = maskBuffer->width;
		int IMAGE_H = maskBuffer->height;
	//create new image with 3 char per pixel and set all black
		unsigned char* image = (unsigned char*) malloc( IMAGE_W*IMAGE_H*3 );
		memset( &image[0], 0, IMAGE_W*IMAGE_H*3);
	//go through maskBuffer and set pixels in image
		for(int x=0; x<IMAGE_W; x++)
			for(int y=0; y<IMAGE_H; y++)
				memset( &image[(x+y*IMAGE_W)*3], maskBuffer->getPixel(x,y), 3);
	//Create FreeImage Buffer
		FIBITMAP* memJpegInBuff; 
		memJpegInBuff = FreeImage_Allocate(IMAGE_W, IMAGE_H, 24);
		long *bits = (long*)FreeImage_GetBits(memJpegInBuff);
	//Copy image in FreeImage Buffer
		memcpy(bits, (unsigned char*)image, IMAGE_W*IMAGE_H*3);
	//Create File name:
		char maskFileName [ 50 ];
		sprintf( maskFileName, "masks/mask_cam%d.png", camID );
		//maskBuffer = new poVisionImageBuffer( rawBuffer->width, rawBuffer->height, maskFileName );
	//save png via freeimage
		if( FreeImage_Save(FIF_PNG, memJpegInBuff, maskFileName , 0))
			printf("PNG SAVED\n"); 
		else
			printf("------------->PNG FAILED TO SAVE\n");*/
}




void poVisionSubstractionSystem::clearMaskPoints()
{
	//there is no explicit clear points in poShape2D, so just kill and recreate
	delete maskPTS;
	maskPTS = new poShape2D();
	maskPTS->enableFill = false;
	maskPTS->strokeWidth = 1;
	maskPTS->strokeColor = poColor::red;
}

void poVisionSubstractionSystem::update()
{
	// confirm that frame is new and do processing
	int newChecksum = rawBuffer->calculateChecksum();
	if ( newChecksum != previousChecksum || overrideCheckSum )
	{
		previousChecksum = newChecksum;

		// blur the image and mask it first
		blurProc->process( rawBuffer );
		maskProc->process( blurProc->outputBuffer, maskBuffer );

		// accumulate into background buffer
		accumProc->processProcOutput( maskProc );

		// subtract background buffer from foreground
		diffProc->processProcOutput( maskProc, accumProc );
		
		//threshold and push into grid algorithm
		threshMETHOD->processProcOutput( diffProc);
		gridAlgorithm->process( threshMETHOD->outputBuffer, rawBuffer );
	}
}

void poVisionSubstractionSystem::draw()//;int x, int y )
{	
	int x = (int) DrawX;
	int y = (int) DrawY;
	

	//draw frame
	glColor3f( 1,1,1 );
	glRectf(x-1,y-1,x+1280,y+241);
	
	float drawScale = 1.0;
	//draw mask:
	maskProc->outputBuffer->draw( x,y, drawScale );
	//maskBuffer->drawBlacks( x, y, poColor::red );
	
	//draw raw incoming frame:
	rawBuffer->draw( x+320+1,y,drawScale);
	
	//draw threshold output and points
	threshMETHOD->outputBuffer->draw( x+640+2,y, drawScale );
	//gridAlgorithm->draw( x+640+2,y, false );
	
	//draw grid on top of raw frame
	rawBuffer->draw( x+960+3,y,drawScale );
	gridAlgorithm->drawSHAPES( x+960+3,y, true, objectMinBoxSize);
}

void poVisionSubstractionSystem::drawVariables(int Mx, int My)//int x, int y)
{
	int x = (int) DrawX;
	int y = (int) DrawY;
	float drawScale = 1.0;
	//draw Map buffer if it is being used
	if(usingMAP)
		threshMapBufferACTIVE->draw(x+0, y+320+2, drawScale );
	
	//set labels and draw them
	label[0]->position.set(x+0, y);
	label[1]->position.set(x+320, y);
	label[2]->position.set(x+640, y);
	label[3]->position.set(x+960, y);
	for(int i=0; i<4; i++)
		label[i]->drawTree();
	
	//set variables
	int max = (usingMAP) ? 6 : 5;
	for(int i=0; i<max; i++)
	{
		variables[i]->position.set(960+320+x+i*110+5, y+50);
		variables[i]->drawTree();		
	}
	
	//set static variables
	FixedVariables->position.set(960+320+x+5, y+25);
	FixedVariables->drawTree();

	OuterMask->position.set(x,y);
	OuterMask->drawTree();
	maskPTS->drawTree();

	Instructions->position.set(960+320+x+5, y+125);
	Instructions->drawTree();

	if(DrawMode == 1){
	if(DrawColor >0)
		glColor4f(1.0, 1.0, 1.0, 0.25);
	else
		glColor4f(1.0, 0.0, 0.0, 0.25);
	glRectf(Mx-DrawWidth, My-DrawWidth, Mx+DrawWidth, My+DrawWidth);}

}

void	poVisionSubstractionSystem::resetBuffers()
{
	accumProc->doFullCapture = true;	
}

//UPDATE VALUE FUNCTIONS: ///////////////////////////////////////////////

void poVisionSubstractionSystem::SetToUseThreshMap(bool yes){
	if(yes == true)
		{ threshMETHOD = threshMapProc; usingMAP=true;}
	else
		{ threshMETHOD = threshProc; usingMAP=false;}	}

void poVisionSubstractionSystem::SetThreshold(int newval)
{
	threshold = newval;
	threshProc->threshold = threshold;
}

void poVisionSubstractionSystem::SetMIN(int newMIN)
{
	thresholdMIN = newMIN;
	threshMapProc->setMINMAX(thresholdMIN, thresholdMAX);
	threshMapProc->setMap();
}

void poVisionSubstractionSystem::SetMAX(int newMAX)
{
	thresholdMAX = newMAX;
	threshMapProc->setMINMAX(thresholdMIN, thresholdMAX);
	threshMapProc->setMap();
}

void poVisionSubstractionSystem::SetMINMAX(int newMIN, int newMAX)
{
	thresholdMIN = newMIN; thresholdMAX = newMAX;
	threshMapProc->setMINMAX(thresholdMIN, thresholdMAX);
	threshMapProc->setMap();
}

void poVisionSubstractionSystem::SetAccumSpeed(float newval)
{
	accumUpdateSpeed = newval;
	accumProc->updateSpeed = accumUpdateSpeed;
}

void poVisionSubstractionSystem::SetAccumReject(int newval)
{
	accumRejectionRange = newval;
	accumProc->rejectionRange = accumRejectionRange;
}

void poVisionSubstractionSystem::handleEditKey(int key)
{
	int La = 44; //<
	int Ra = 46; //>
	int Ua = 61; //+
	int Da = 45; //-
	
	if(key == 's')
		WriteConfigFile();
	if(key == 'q')
		saveMask();
	if (key == 'a')
		GenerateMask();
	if (key == 'd')
		DrawMode = (DrawMode+1)%2;
	if (key == 'w')
		DrawColor += 255; if(DrawColor > 255) DrawColor = 0;
	if (key == '[')
		DrawWidth -= 1; if(DrawWidth < 0) DrawWidth = 0;
	if (key == ']')
		DrawWidth += 1;
	else//catch other keys
	{
		//check if EditMode = 0 and a up/down key pressed, we switch threshold type
		if(EditMode == 0 && ( key==Ua || key==Da))
		{
			SetToUseThreshMap(! usingMAP);
			EditMode = 1;
		}
		else
		{
			if(usingMAP == true)
			{
				
				
				//Deal with switching Edit Mode
				if(key == Ra) //right pointing arrow key
					{EditMode = (EditMode+1)%6;}
				if(key == La) //left pointing arrow key
					{EditMode = EditMode-1; EditMode = (EditMode < 0) ? 5 : EditMode; }
				
				//Deal with making variable changes
				if(EditMode == 1) //change thresholdMIN
					{if(key == Ua) SetMIN(thresholdMIN+1);
					else if(key == Da) SetMIN(thresholdMIN-1);}
				if(EditMode == Ua) //change thresholdMAX
					{if(key == Da) SetMAX(thresholdMAX+1);
					else if(key == 1) SetMAX(thresholdMAX-1);}
				if(EditMode == 3) //change accumSpeed
					{if(key == Ua) SetAccumSpeed(accumUpdateSpeed + 0.001);
					else if(key == Da) SetAccumSpeed(accumUpdateSpeed - 0.001);}
				if(EditMode == 4) //change accumSpeed
					{if(key == Ua) SetAccumReject(accumRejectionRange + 1);
					else if(key == Da) SetAccumReject(accumRejectionRange - 1);}
				if(EditMode == 5) //change accumSpeed
					{if(key == Ua) objectMinBoxSize += 1;
					else if(key == Da) objectMinBoxSize -= 1;}
			}
			else
			{
				//Deal with switching Edit Mode
				if(key == Ra) //right pointing arrow key
					{EditMode = (EditMode+1)%5;}
				if(key == La) //left pointing arrow key
					{EditMode = EditMode-1; EditMode = (EditMode < 0) ? 4 : EditMode; }
				
				//Deal with making variable changes
				if(EditMode == 1) //change thresholdMIN
					{if(key == Ua) SetThreshold(threshold+1);
					else if(key == Da) SetThreshold(threshold-1);}
				if(EditMode == 2) //change accumSpeed
					{if(key == Ua) SetAccumSpeed(accumUpdateSpeed + 0.001);
					else if(key == Da) SetAccumSpeed(accumUpdateSpeed - 0.001);}
				if(EditMode == 3) //change accumSpeed
					{if(key == Ua) SetAccumReject(accumRejectionRange + 1);
					else if(key == Da) SetAccumReject(accumRejectionRange - 1);}
				if(EditMode == 4) //change objectMinBoxSize
					{if(key == Ua) objectMinBoxSize += 1;
					else if(key == Da) objectMinBoxSize -= 1;}
			}
		}
		
		if(key == 'x')
			clearMaskPoints();
	}
	
	//Update Texts...
	int index = 0;
	if(usingMAP)
	{
		index = 0; updateText( variables[index], MAPvariableLabels[index], std::string("MAP"));
		index = 1; updateText( variables[index], MAPvariableLabels[index], thresholdMIN);
		index = 2; updateText( variables[index], MAPvariableLabels[index], thresholdMAX);
		index = 3; updateText( variables[index], MAPvariableLabels[index], accumUpdateSpeed);
		index = 4; updateText( variables[index], MAPvariableLabels[index], accumRejectionRange);
		index = 5; updateText( variables[index], MAPvariableLabels[index], objectMinBoxSize);
	}
	else
	{
		index = 0; updateText( variables[index], NONMAPvariableLabels[index], std::string("NON-MAP"));
		index = 1; updateText( variables[index], NONMAPvariableLabels[index], threshold);
		index = 2; updateText( variables[index], NONMAPvariableLabels[index], accumUpdateSpeed);
		index = 3; updateText( variables[index], NONMAPvariableLabels[index], accumRejectionRange);
		index = 4; updateText( variables[index], NONMAPvariableLabels[index], objectMinBoxSize);
		index = 5; updateText( variables[index], NONMAPvariableLabels[index], 0);//not used
	}
	
	// Color which one is on
	// reset all lables to grey
	for(int i=0; i<6; i++)
		variables[i]->fillColor = poColor::grey;
	//set current one to red:
	if(EditMode >= 0 && EditMode < 6)
		variables[EditMode]->fillColor = poColor::red;
}

void poVisionSubstractionSystem::updateText(poTextBox* TXT, std::string* label, float fval)
{
	std::stringstream out;
	std::string s;
	out << fval;
	s += *label;
	s += out.str();
	TXT->setText(s.c_str());
	TXT->doLayout();
}

void poVisionSubstractionSystem::updateText(poTextBox* TXT, std::string* label, int ival)
{
	std::stringstream out;
	std::string s;
	out << ival;
	s += *label;
	s += out.str();
	TXT->setText(s.c_str());
	TXT->doLayout();
}

void poVisionSubstractionSystem::updateText(poTextBox* TXT, std::string* label, std::string sval)
{
	std::string s;
	s += *label;
	s += sval;
	TXT->setText(s.c_str());
	TXT->doLayout();
}


// ======================== poVisionEdgeDetectionSystem ======================== //

poVisionEdgeDetectionSystem::poVisionEdgeDetectionSystem( int vWidth, int vHeight, int camID ) : poVisionSystem( vWidth, vHeight, camID )
{
	maskProc = new poVisionAndProc( vWidth, vHeight );

	// we blur the raw image for both systems and use an accumulation buffer
	blurProc = new poVisionBlurProc( vWidth, vHeight );
	blurProc->blurRepeat = 1;
	accumProc = new poVisionAccumulateProc( vWidth, vHeight );
	accumProc->rejectionRange = accumRejectionRange;
	accumProc->updateSpeed = accumUpdateSpeed;
	
	// this is the grid algorithm
	gridAlgorithm = new poVisionGridAlgorithm( vWidth, vHeight, gridBoxSize );
	gridAlgorithm->referencePoint = refPoint;

	// mask
	char maskFileName [ 50 ];
	sprintf( maskFileName, "masks/mask_cam%d.png", camID );
	maskBuffer = new poVisionImageBuffer( rawBuffer->width, rawBuffer->height, maskFileName );

	// this is a system that does edge detection and subracts out edges that are in the accumulataed image	
	edgeDetectProc1 = new poVisionEdgeDetectProc( vWidth, vHeight );
	edgeDetectProc1->edgeReach = 1;
	threshProc1 = new poVisionThresholdProc( vWidth, vHeight );
	threshProc1->threshold = threshold;
	dilateProc = new poVisionDilateProc( vWidth, vHeight );
	invertProc = new poVisionInvertProc( vWidth, vHeight );
	
	edgeDetectProc2 = new poVisionEdgeDetectProc( vWidth, vHeight );
	edgeDetectProc2->edgeReach = 1;
	threshProc2 = new poVisionThresholdProc( vWidth, vHeight );
	threshProc2->threshold = threshProc1->threshold+5;

	andProc = new poVisionAndProc( vWidth, vHeight );
	erodeProc = new poVisionErodeProc( vWidth, vHeight );
}

void poVisionEdgeDetectionSystem::update()
{
	// confirm that frame is new and do processing
	int newChecksum = rawBuffer->calculateChecksum();
	if ( newChecksum != previousChecksum )
	{
		previousChecksum = newChecksum;

		// blur the image and mask it first
		blurProc->process( rawBuffer );
		maskProc->process( blurProc->outputBuffer, maskBuffer );
	
		// accumulate into background buffer
		accumProc->processProcOutput( maskProc );
		
		// do edge detection on accumulated background buffer
		// dilate image to expand edges and invert it (for subtraction)
		edgeDetectProc1->processProcOutput( accumProc );
		threshProc1->processProcOutput( edgeDetectProc1 );
		dilateProc->processProcOutput( threshProc1 );
		invertProc->processProcOutput( dilateProc );

		// do edge detection on live buffer
		edgeDetectProc2->processProcOutput( maskProc );
		threshProc2->processProcOutput( edgeDetectProc2 );

		// subtract edge detected background from edge detected foreground
		andProc->processProcOutput( invertProc, threshProc2 );
		//erodeProc3->processProcOutput( andProc3 );
		
		// push into grid algorithm
		gridAlgorithm->process( andProc->outputBuffer, rawBuffer );
	}
}

void poVisionEdgeDetectionSystem::draw(int x, int y )
{
	rawBuffer->draw( x+0,y+100 );						// draw buffer

	threshProc1->outputBuffer->draw( x+320,y+100 );		// accum edge buffer
	threshProc2->outputBuffer->draw( x+640,y+100 );		// live edge buffer

	andProc->outputBuffer->draw( x+960,y+100 );		// subtract edge buffers
	//rawBuffer->draw( x+990,y+100 );						// raw buffer
	gridAlgorithm->draw( x+960,y+100, true ); 				// grid system

	/*rawBuffer->draw( 100,100 );
	blurProc0->outputBuffer->draw( 450,100 );
	
	accumProc1->outputBuffer->draw( 100,400 );
	edgeDetectProc1->outputBuffer->draw( 450,400 );
	invertProc1->outputBuffer->draw( 800,400 );

	edgeDetectProc2->outputBuffer->draw( 100,700 );
	threshProc2->outputBuffer->draw( 450,700 );
	
	andProc3->outputBuffer->draw( 800,700 );
	
	threshProc4->outputBuffer->draw( 1150,100 );
	
	rawBuffer->draw( 1150,700 );
	gridAlgorithm->draw( 1150,700, true );*/
}

void	poVisionEdgeDetectionSystem::resetBuffers()
{
	accumProc->doFullCapture = true;	
}

