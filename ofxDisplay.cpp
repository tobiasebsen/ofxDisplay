/*
 *  ofxDisplay.cpp
 *  displayExample
 *
 *  Created by Tobias Ebsen on 10/1/12.
 *  Copyright 2012 Tobias Ebsen. All rights reserved.
 *
 */

#include "ofxDisplay.h"

#ifdef TARGET_OSX
ofxDisplay::ofxDisplay(CGDirectDisplayID displayId) {
	this->displayId = displayId;
}
#endif

ofxDisplay* ofxDisplay::getMainDisplay() {
#ifdef TARGET_OSX
	CGDirectDisplayID displayId = CGMainDisplayID();
	return new ofxDisplay(displayId);
#endif
}

vector<ofxDisplay*> ofxDisplay::getActiveDisplays() {
	vector<ofxDisplay*> displays;
#ifdef TARGET_OSX
	uint32_t count = 0;
	CGGetActiveDisplayList(0, NULL, &count);
	CGDirectDisplayID* ids = (CGDirectDisplayID*)malloc(count * sizeof(CGDirectDisplayID));
	CGGetActiveDisplayList(count, ids, &count);
	for (int i=0; i<count; i++) {
		ofxDisplay* display = new ofxDisplay(ids[i]);
		displays.push_back(display);
	}
	free(ids);
#endif
	return displays;
}

ofPoint ofxDisplay::getSize() {
#ifdef TARGET_OSX
	CGSize size = CGDisplayScreenSize(displayId);
	return ofPoint(size.width, size.height);
#endif
}

ofRectangle ofxDisplay::getBounds() {
#ifdef TARGET_OSX
	CGRect bounds = CGDisplayBounds(displayId);
	return ofRectangle(bounds.origin.x, bounds.origin.y, bounds.size.width, bounds.size.height);
#endif
}

bool ofxDisplay::isAsleep() {
#ifdef TARGET_OSX
	return CGDisplayIsAsleep(displayId);
#endif
}

void ofxDisplay::grabImage(ofImage& image) {
	ofRectangle rect = getBounds();
	grabImage(image, rect);
}

void ofxDisplay::grabImage(ofImage& image, ofRectangle& rect) {
#ifdef TARGET_OSX
	CGRect r = {rect.x, rect.y, rect.width, rect.height};
	CGImageRef imgr = CGDisplayCreateImageForRect(displayId, r);
	int width = CGImageGetWidth(imgr);
	int height = CGImageGetHeight(imgr);
	int row = CGImageGetBytesPerRow(imgr);
	CGColorSpaceRef csr = CGImageGetColorSpace(imgr);
	
	CGDataProviderRef dpr = CGImageGetDataProvider(imgr);
	CFDataRef dr = CGDataProviderCopyData(dpr);
	const unsigned char* spixels = (const unsigned char*)CFDataGetBytePtr(dr);

	if (CGColorSpaceGetModel(csr) == kCGColorSpaceModelRGB) {
		CGBitmapInfo info = CGImageGetBitmapInfo(imgr);
		CGImageAlphaInfo alpha = CGImageGetAlphaInfo(imgr);
		
		if (!(info & kCGBitmapFloatComponents)) {

			if ((info & kCGBitmapByteOrderMask) == kCGBitmapByteOrder32Big && alpha & kCGImageAlphaLast) {
				image.setFromPixels(spixels, width, height, OF_IMAGE_COLOR_ALPHA);
			}
			if ((info & kCGBitmapByteOrderMask) == kCGBitmapByteOrder32Little && alpha & kCGImageAlphaLast) {
				unsigned char* dpixels = image.getPixels();
				int dwidth = image.getWidth() * 4;
				int dheight = image.getHeight();
				int srow = 0;
				int drow = 0;
				for (int y=0; y<dheight; y++) {
					for (int x=0; x<dwidth; x+=4) {
						dpixels[drow+x+0] = spixels[srow+x+2];
						dpixels[drow+x+1] = spixels[srow+x+1];
						dpixels[drow+x+2] = spixels[srow+x+0];
						dpixels[drow+x+3] = spixels[srow+x+3];
					}
					srow += row;
					drow += dwidth;
				}
				image.update();
			}
		}
		CFRelease(dr);
	}
	
	CGImageRelease(imgr);
#endif
}