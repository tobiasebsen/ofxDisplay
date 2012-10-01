/*
 *  ofxDisplay.h
 *  displayExample
 *
 *  Created by Tobias Ebsen on 10/1/12.
 *  Copyright 2012 Tobias Ebsen. All rights reserved.
 *
 */

#pragma once

#include "ofPoint.h"
#include "ofRectangle.h"
#include "ofImage.h"

#ifdef TARGET_OSX
#include <Carbon/Carbon.h>
#endif

class ofxDisplay {
public:
	
#ifdef TARGET_OSX
	ofxDisplay(CGDirectDisplayID displayId);
#endif
	
	static ofxDisplay* getMainDisplay();
	static vector<ofxDisplay*> getActiveDisplays();
	
	// Get size in millimeters
	ofPoint getSize();
	// Get pixel bounds
	ofRectangle getBounds();
	// Is the display sleeping?
	bool isAsleep();
	
	// Grab entire screen
	void grabImage(ofImage& image);
	// Grab a specified rectagle of the screen
	void grabImage(ofImage& image, ofRectangle& rect);

private:
#ifdef TARGET_OSX
	CGDirectDisplayID displayId;
#endif
};