//
//  Feed.h
//  mixer
//
//  Created by Will Gallia on 25/07/2012.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#pragma once

#include "ofMain.h"

class Feed {
public:
  Feed();
  ~Feed();
  
  void setup();
  void update();
  void draw();
  
  
  float background_hue;
  void setBackgroundHueFromPoint(int x, int y);
  
  void setBackgroundSource(ofBaseVideoDraws *bd) { background = bd; }
  
  
protected:
  ofVideoGrabber grabber;
  ofTexture gTexture;
  
  ofBaseVideoDraws *background;
  
  
  ofVec2f imagePos;
  
  ofShader chromaShader;
  ofShader backgroundShader;
  
  ofMesh mainContent;
  
};