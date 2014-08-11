/*
 *  ofxDisplay.h
 *  displayExample
 *
 *  Created by Tobias Ebsen on 10/1/12.
 *  Copyright 2012 Tobias Ebsen. All rights reserved.
 *
 */

#pragma once

#include "ofMain.h"

#ifdef TARGET_OSX
#include <Carbon/Carbon.h>
#endif

typedef struct _ofxDisplayArgs {
    uint32_t id;
    uint32_t flags;
} ofxDisplayArgs;

class ofxDisplay {
public:
	ofxDisplay(uint32_t displayId);
	
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
    
    static void registerCallbacks();
    static ofEvent<ofxDisplayArgs> displayEvents;
    
private:
	uint32_t displayId;

    static void displayCallBack(uint32_t display, uint32_t flags, void *userInfo);
};
