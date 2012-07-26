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

class Feed : public BaseHasPanel, BaseHasCanvas {
public:
  Feed();
  ~Feed();
  
  void setup();
  void update();
  void draw();
	void drawGui();
  
	ofTexture& getTextureReference();
  
  float background_hue;
  void setBackgroundHueFromPoint(int x, int y);
  
  void setBackgroundSource(ofBaseVideoDraws *bd);

	void mousePressed(ofMouseEventArgs &args);
  
protected:
  ofVideoGrabber grabber;
  ofTexture gTexture;
  
  ofBaseVideoDraws *background;
  
  
  ofVec2f imagePos;
  
  ofShader chromaShader;
  ofShader backgroundShader;
    
	ofFbo fbo;
	
	
	float number_of_tiles;
	bool dummy_bool;

};