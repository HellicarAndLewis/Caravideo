//
//  Feed.cpp
//  mixer
//
//  Created by Will Gallia on 25/07/2012.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#include "Feed.h"
#include "Constants.h"

Feed::Feed():
background_hue(0),
background(NULL),
imagePos(0, 0) {
  
}

Feed::~Feed() {
  
  if (background != NULL) {
    delete background;
  }
}

void Feed::setup() {

  grabber.initGrabber(VIDEO_WIDTH, VIDEO_HEIGHT);

  //we need a separate grabber texture, because we want an alpha channel
  //grabber.setPixelFormat() doesn't work properly
  gTexture.allocate(VIDEO_WIDTH, VIDEO_HEIGHT, GL_RGBA);

  
  chromaShader.load("chromakey");
  backgroundShader.load("background");
  
  
  
  mainContent.setMode(OF_PRIMITIVE_TRIANGLES);
  
  vector<ofVec3f> vertices;
  vertices.push_back(ofVec3f(0, 0, 0));
  vertices.push_back(ofVec3f(VIDEO_WIDTH, 0, 0));
  vertices.push_back(ofVec3f(VIDEO_WIDTH, VIDEO_HEIGHT, 0));
  vertices.push_back(ofVec3f(VIDEO_WIDTH, VIDEO_HEIGHT, 0));
  vertices.push_back(ofVec3f(0,VIDEO_HEIGHT, 0));
  vertices.push_back(ofVec3f(0, 0, 0));
  mainContent.addVertices(vertices);
  
  mainContent.enableTextures();
  vector<ofVec2f> texcoords;
  texcoords.push_back(ofVec2f(0, 0));
  texcoords.push_back(ofVec2f(VIDEO_WIDTH, 0));
  texcoords.push_back(ofVec2f(VIDEO_WIDTH, VIDEO_HEIGHT));
  texcoords.push_back(ofVec2f(VIDEO_WIDTH, VIDEO_HEIGHT));
  texcoords.push_back(ofVec2f(0, VIDEO_HEIGHT));
  texcoords.push_back(ofVec2f(0, 0));
  mainContent.addTexCoords(texcoords);
  
  
  
  
}

void Feed::update() {
  
  grabber.update();
  
  if (grabber.isFrameNew()) {
    gTexture = grabber.getTextureReference();
  }
}

void Feed::draw() {
  

  
  
  if (background != NULL) {
    if (ofVideoPlayer *bplayer = dynamic_cast<ofVideoPlayer*>(background)) {

      bplayer->getTextureReference().bind();
      backgroundShader.begin();
      backgroundShader.setUniformTexture("tex", bplayer->getTextureReference(), 0);
      backgroundShader.setUniform2f("inputSize", VIDEO_WIDTH, VIDEO_HEIGHT);
      backgroundShader.setUniform1f("scale_factor", 5);
      
      mainContent.draw();
      
      backgroundShader.end();
      bplayer->getTextureReference().unbind();
      
    }
    else if (ofVideoGrabber *bgrabber = dynamic_cast<ofVideoGrabber*>(background)) {
      cout << "grabber" << endl;
    }
  }
  
  
  ofEnableAlphaBlending();
  
  gTexture.bind();
  chromaShader.begin();
  chromaShader.setUniformTexture("tex0", gTexture, 0);
  chromaShader.setUniform1f("background_hue", background_hue);
  
  mainContent.draw();
  
  chromaShader.end();
  gTexture.unbind();
  
  ofDisableAlphaBlending();
  
  
//  grabber.getTextureReference().draw(0, 0);
}

void Feed::setBackgroundHueFromPoint(int x , int y) {
  
  //make sure we're in bounds
  if (x > imagePos.x + grabber.width || y > imagePos.y + grabber.height ||
      x < 0 || y < 0) {
    return;
  }
  
  ofPixels pix = grabber.getPixelsRef();
  ofColor col = pix.getColor(x - imagePos.x, y - imagePos.y);
  
  //normalise the hue, as floating point values are used in shader
  background_hue = col.getHue() / 255.0; 
}