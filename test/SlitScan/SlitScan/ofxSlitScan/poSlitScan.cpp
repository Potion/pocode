/*
 *  poSlitScan.cpp
 *  PoCode
 *
 *  Created by James George on 12/7/09.
 *  Copyright 2009 http://jamesgeorge.org. All rights reserved.
 *
 *	Modified for PoCode by Jonathan Bobrow on 12/18/2011
 *	Copyright 2011 http://potiondesign.com. All rights reserved.
 *
 */

#include "poSlitScan.h"

//converts from an index (0, capacity) to the appropriate fraem in the rolling buffer
static inline int frame_index(int framepointer, int index, int capacity)
{ return (framepointer + index) % capacity; }

poSlitScan::poSlitScan()
{
	buffersAllocated = false;
}

void poSlitScan::setup(int w, int h, int _capacity)
{
	setup(w,h,_capacity, PO_IMAGE_COLOR);
}

void poSlitScan::setup(int w, int h, int _capacity, uint _type)
{
	numChannels = _type;
	switch (numChannels) {
		case PO_IMAGE_GRAYSCALE:{
			bytesPerPixel = 1;
		}break;
		case PO_IMAGE_COLOR:{
			bytesPerPixel = 3;
		}break;
		case PO_IMAGE_COLOR_ALPHA:{
			bytesPerPixel = 4;
		}break;
		default:{
//			ofLog(OF_LOG_ERROR, "poSlitScan Error -- Invalid image type");
			return;
		}break;
	}
	
	//clean up if reallocating
	if(buffersAllocated){
		free(delayMapPixels);
		for(int i = 0; i < capacity; i++){
			free(buffer[i]);
		}
		free(buffer);
		buffersAllocated = false;
	}
	
	width = w;
	height = h;
	capacity = _capacity;
	framepointer = 0;
	blend = false;
	timeDelay = 0;
	timeWidth = capacity;
	bytesPerFrame = width*height*bytesPerPixel;
	delayMapPixels = (float*)calloc(w*h, sizeof(float));
	buffer = (unsigned char**)calloc(capacity, sizeof(unsigned char*));
	for(int i = 0; i < capacity; i++){
		buffer[i] = (unsigned char*)calloc(bytesPerFrame, sizeof(unsigned char));
	}
	outputImage = poImage(w, h, numChannels, NULL);
	delayMapImage = poImage(w, h, PO_IMAGE_GRAYSCALE, NULL);
	buffersAllocated = true;
	outputIsDirty = true;
	delayMapIsDirty = true;
}

bool poSlitScan::isSetup()
{
	return buffersAllocated;
}

void poSlitScan::setCapacity(int _capacity)
{
	if(_capacity <= 0){
		_capacity = 1;
	}
	
	if(_capacity == capacity){
		return;
	}
	
	//the new capacity is bigger
	if (capacity < _capacity) {
		buffer = (unsigned char**)realloc(buffer, _capacity*sizeof(unsigned char*));
		for(int i = capacity; i < _capacity; i++){
			buffer[i] = (unsigned char*)calloc(bytesPerFrame, sizeof(unsigned char));
		}
	}
	//the new capacity is smaller
	else {
		for( int i = _capacity; i < capacity; i++){
			free(buffer[i]);
		}
		buffer = (unsigned char**)realloc(buffer, _capacity*sizeof(unsigned char*));
		framepointer %= _capacity;
	}
	capacity = _capacity;
	outputIsDirty = true;
}

void poSlitScan::setDelayMap(poVisionImageBuffer* pix, uint type)
{
	switch (type) {
		case PO_IMAGE_COLOR:{
			for(int i = 0; i < width*height; i++){
				//RGB 0 - 255 ==> YUV 0.0 - 1.0
				delayMapPixels[i] = (0.299*pix->getPixel(i*3) + 0.587*pix->getPixel(i*3+1) + 0.114*pix->getPixel(i*3+2)) / 255.0; 
			}				
		}break;
			
		case PO_IMAGE_COLOR_ALPHA:{
			for(int i = 0; i < width*height; i++){
				//RGBA 0 - 255 ==> YUV 0.0 - 1.0
				delayMapPixels[i] = (0.299*pix->getPixel(i*4) + 0.587*pix->getPixel(i*4+1) + 0.114*pix->getPixel(i*4+2)) / 255.0;
			}				
		}break;
			
		case PO_IMAGE_GRAYSCALE:{
			for(int i = 0; i < width*height; i++){
				delayMapPixels[i] = pix->getPixel(i) / 255.0;
			}	
		}break;
			
		default:{
//			ofLog(OF_LOG_ERROR, "poSlitScan -- unsupported image map type");
		}break;
	}
	
	delayMapIsDirty = true;
	outputIsDirty = true; 
}

void poSlitScan::setDelayMap(float* mappix)
{
	//assumed monochrome float image
	for(int i = 0; i < width*height; i++){
		delayMapPixels[i] = mappix[i];
	}
	delayMapIsDirty = true;
	outputIsDirty = true; 
}

void poSlitScan::setDelayMap(poImage map)
{
	if(map.getWidth() != width || map.getHeight() != height){
//		ofLog(OF_LOG_ERROR, "poSlitScan Error -- Map dimensions do not match image dimensions. given %fx%f, need %dx%d\n", map.getWidth(), map.getHeight(), width, height);
		return;
	}
	setDelayMap(map.getPixels(), map.getChannels());
}

void poSlitScan::setBlending(bool _blend)
{
	blend = _blend;
	outputIsDirty = true;
}

void poSlitScan::toggleBlending()
{
	blend = !blend;
	outputIsDirty = true;
}

void poSlitScan::addImage(poVisionImageBuffer* image)
{
	
	//write the image into the buffer
	memcpy(buffer[framepointer], image, bytesPerFrame*sizeof(unsigned char));
	
	//increment the framepointer
	framepointer = ( (framepointer + 1) % capacity );	
	
	outputIsDirty = true;	
}

void poSlitScan::addImage(poImage image)
{
	if(image.getChannels() != numChannels){
//		ofLog(OF_LOG_ERROR, "poSlitScan -- adding image of the wrong type");
		return;
	}
	addImage( image.getPixels() );
}

poImage* poSlitScan::getOutputImage()
{
	if(outputIsDirty){
		//calculate the new distorted image
		poVisionImageBuffer* writebuffer = outputImage->getPixels();
		poVisionImageBuffer* outbuffer = writebuffer;
		
		int x, y, offset, lower_offset, upper_offset, pixelIndex;
		float precise, alpha, invalpha;	
		int mapMin = capacity - timeDelay - timeWidth;// (time_delay + time_width);
		int mapMax = capacity - 1 - timeDelay;// - time_delay;
		int bytesPerRow = width*bytesPerPixel;
		
		if(blend){
			for(y = 0; y < height; y++){
				for(x = 0; x < width; x++){		
					//find pixel point in local reference
					pixelIndex = bytesPerRow*y + x*bytesPerPixel;
					precise = poMapf( 0.0, 1.0, delayMapPixels[width*y+x], mapMin, mapMax);
					//cast it to an integer
					offset = int(precise);
					
					//calculate alpha
					alpha = precise - offset;
					invalpha = 1 - alpha;
					
					//convert to framepointer reference point
					lower_offset = frame_index(framepointer, offset, capacity);
					upper_offset = frame_index(framepointer, offset+1, capacity);
					
					//get buffers
					poVisionImageBuffer *a = buffer->getPixel(lower_offset) + pixelIndex;
					poVisionImageBuffer *b = buffer->getPixel(upper_offset) + pixelIndex;
					
					//interpolate and set values;
					for(int c = 0; c < bytesPerPixel; c++){
						*outbuffer++ = (a[c]*invalpha)+(b[c]*alpha);
					}
				}
			}
		}
		else{
			for(y = 0; y < height; y++){
				for(x = 0; x < width; x++){
					pixelIndex = bytesPerRow*y + x*bytesPerPixel;
					offset = frame_index(framepointer, poMapf( 0.0, 1.0, delayMapPixels[width*y+x], mapMin, mapMax), capacity);
					memcpy(outbuffer, buffer[offset]+pixelIndex, bytesPerPixel);	
					outbuffer += bytesPerPixel;
				}
			}
		}
		outputImage.setFromPixels(writebuffer, width, height, type);
		outputIsDirty = false;
	}

	return &outputImage;
}

poImage* poSlitScan::getDelayMap()
{
	if(delayMapIsDirty){
		poVisionImageBuffer* pix = delayMapImage.getPixels();
		for(int i = 0; i < width*height; i++){
			pix[i] = char(delayMapPixels[i]*255);
		}
		delayMapImage.setFromPixels(pix, width, height, OF_IMAGE_GRAYSCALE);
		delayMapIsDirty = false;
	}
	return &delayMapImage;
}

int poSlitScan::getWidth()
{
	return width;
}

int poSlitScan::getHeight()
{
	return height;
}

void poSlitScan::pixelsForFrame(int num, poVisionImageBuffer* outbuf)
{
	memcpy(outbuf, buffer[frame_index(framepointer, num, capacity)], bytesPerFrame*sizeof(poVisionImageBuffer));
}

void poSlitScan::setTimeDelayAndWidth(int _timeDelay, int _timeWidth)
{
	timeDelay = ofClamp(_timeDelay, 0, capacity-1);
	timeWidth = ofClamp(_timeWidth, 1, capacity);
	if(timeDelay + timeWidth > capacity){
		ofLog(OF_LOG_ERROR, "poSlitScan -- Invalid time delay and width specified, adds to %d with a capacity of %d", (timeDelay+timeWidth), capacity);
		timeDelay = 0;
		timeWidth = capacity;
	}
	outputIsDirty = true;	
}

void poSlitScan::setTimeDelay(int _timeDelay)
{
	timeDelay = ofClamp(_timeDelay, 0, capacity - timeWidth - 1);
	outputIsDirty = true;
}

void poSlitScan::setTimeWidth(int _timeWidth)
{
	timeWidth = ofClamp(_timeWidth, 1, capacity - timeDelay);
	outputIsDirty = true;
}

int poSlitScan::getCapacity()
{
	return capacity;
}

int poSlitScan::getTimeDelay()
{
	return timeDelay;
}

int poSlitScan::getTimeWidth()
{
	return timeWidth;
}

int poSlitScan::getType()
{
	return type;
}

bool poSlitScan::isBlending()
{	
	return blend;
}

