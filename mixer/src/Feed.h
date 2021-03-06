//
//  Feed.h
//  mixer
//
//  Created by Will Gallia on 25/07/2012.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#pragma once

#include "ofMain.h"
#include "BaseHasPanel.h"
#include "BaseHasCanvas.h"

#define USE_VIDEO

class Feed : public BaseHasPanel, BaseHasCanvas {
public:
  Feed();
	Feed(int ID);
  ~Feed();
  
  void setup();
  void update();
  void draw();
	void drawGui();
  
	void operator()(unsigned int bID);
	
	ofTexture& getTextureReference();
  
  float background_hue;
  void setBackgroundHueFromPoint(int x, int y);
  
  void setBackgroundSource(ofBaseVideoDraws *bd, bool isOtherFeed=false);
	void removeBackground();
	
	void mousePressed(ofMouseEventArgs &args);

#ifdef USE_VIDEO
	ofVideoPlayer* getGrabberPointer() { return &grabber; }
#else
	ofVideoGrabber* getGrabberPointer() { return &grabber; }
#endif
	
protected:
#ifdef USE_VIDEO
  ofVideoPlayer grabber;
#else
  ofVideoGrabber grabber;
#endif
	ofTexture gTexture;
  ofBaseVideoDraws *background;
  
  
  ofVec2f imagePos;
  
  ofShader chromaShader;
  ofShader backgroundShader;
    
	ofFbo fbo;
	
	
	float number_of_tiles;
	bool dummy_bool;
	
	bool do_chroma;
	
	bool do_bloom;
	float bloom_amount, bloom_mix;
	
	bool do_chab;
	float chab_amount;
	
	bool do_fisheye;
	

	
	bool isUsingOtherFeedAsBackground;

	
	enum {
		LOAD_BACKGROUND_BUTTON,
		LOAD_OTHER_SOURCE_AS_BACKGROUND_BUTTON
	};
};