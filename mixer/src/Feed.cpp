//
//  Feed.cpp
//  mixer
//
//  Created by Will Gallia on 25/07/2012.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#include "Feed.h"
#include "testApp.h"

Feed::Feed():
BaseHasPanel("Feed"),
BaseHasCanvas(),
background_hue(0),
background(NULL),
imagePos(0, 0) {
  
}

Feed::Feed(int ID):
BaseHasPanel("Feed " + ofToString(ID)),
BaseHasCanvas(),
background_hue(0),
background(NULL),
imagePos(0, 0) {
#ifndef USE_VIDEO
  grabber.setDeviceID(ID);
#endif
	panel->setPosition(ID*300, 0);
}

Feed::~Feed() {
  
  removeBackground();
}

void Feed::setup() {


  //we need a separate grabber texture, because we want an alpha channel
  //grabber.setPixelFormat() doesn't work properly
  gTexture.allocate(VIDEO_WIDTH, VIDEO_HEIGHT, GL_RGBA);

  
  chromaShader.load("main_feed");
  backgroundShader.load("background");
  
	
	//set up and clear fbo
	fbo.setUseTexture(true);
  fbo.allocate(VIDEO_WIDTH, VIDEO_HEIGHT, GL_RGBA);
	fbo.begin();
	ofClear(0, 0, 0, 1);
	fbo.end();

	panel->addBool("Set background hue", dummy_bool);
	panel->addButton("Load background video", LOAD_BACKGROUND_BUTTON, this);
	panel->addButton("Set background to other source", LOAD_OTHER_SOURCE_AS_BACKGROUND_BUTTON, this);
	
	number_of_tiles = 1;
	panel->addFloat("Number of Tiles", number_of_tiles).setMin(0.0).setMax(10.0);
	
	do_chroma = false;
	panel->addBool("Do chroma", do_chroma);
	
	do_bloom = false;
	panel->addBool("Use Bloom", do_bloom);
	bloom_amount = 1.0;
	panel->addFloat("Bloom amount", bloom_amount).setMin(0.0).setMax(4.0);
	bloom_mix = 0.5;
	panel->addFloat("Bloom mix", bloom_mix).setMin(0.0).setMax(1.0);
	
	do_chab = false;
	panel->addBool("Use chromatic aberration", do_chab);
	chab_amount = 0;
	panel->addFloat("Chromatic aberration amount", chab_amount).setMin(0.0).setMax(20.0);
	
	do_fisheye = false;
	panel->addBool("Use fisheye", do_fisheye);
	
	panel->setColor(0.6);
	panel->load();
	
#ifdef USE_VIDEO
	ofFileDialogResult result = ofSystemLoadDialog();
	
	ofVideoPlayer *player = new ofVideoPlayer();
	grabber.loadMovie(result.filePath);
	grabber.play();
#else	
  grabber.initGrabber(VIDEO_WIDTH, VIDEO_HEIGHT);
#endif	
	
	
	background = new ofVideoPlayer();
	((ofVideoPlayer*) background)->setUseTexture(true);
	((ofVideoPlayer*) background)->loadMovie("bieber.gif");
	((ofVideoPlayer*) background)->play();
	
	isUsingOtherFeedAsBackground = false;
}

void Feed::update() {
  
  grabber.update();
  
  if (grabber.isFrameNew()) {
    gTexture = grabber.getTextureReference();
  }
	
	if (background != NULL) {
		
		
    if (ofVideoPlayer *bplayer = dynamic_cast<ofVideoPlayer*>(background)) {
			bplayer->update();
    }
		
    else if (ofVideoGrabber *bgrabber = dynamic_cast<ofVideoGrabber*>(background)) {
      
			//don't update the background if it's the other feed as it's already being updated
			if (!isUsingOtherFeedAsBackground) {
				bgrabber->update();
			}
    }

  }

	
	panel->update();
	
}

void Feed::draw() {
  
	fbo.begin();
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  
  if (background != NULL) {
		
		ofTexture btex;
		
    if (ofVideoPlayer *bplayer = dynamic_cast<ofVideoPlayer*>(background)) {
			btex = bplayer->getTextureReference();      
    }
		
    else if (ofVideoGrabber *bgrabber = dynamic_cast<ofVideoGrabber*>(background)) {
      btex = bgrabber->getTextureReference();
    }
		
		btex.bind();
		
		backgroundShader.begin();
		backgroundShader.setUniformTexture("tex", btex, 0);
		backgroundShader.setUniform2f("inputSize", VIDEO_WIDTH, VIDEO_HEIGHT);
		backgroundShader.setUniform1f("scale_factor", number_of_tiles);
		
		canvas.draw();
		
		backgroundShader.end();
		
		btex.unbind();
		
  }
  
  
  ofEnableAlphaBlending();
  
  chromaShader.begin();
	texture.bind();

  chromaShader.setUniformTexture("tex0", gTexture, 0);
  chromaShader.setUniform1f("background_hue", background_hue);
	chromaShader.setUniform1i("do_chroma", do_chroma);
	
	chromaShader.setUniform1i("do_bloom", do_bloom);
	chromaShader.setUniform1f("bloom_amount", bloom_amount);
  chromaShader.setUniform1f("bloom_mix", bloom_mix);
	
	chromaShader.setUniform1i("do_chab", do_chab);
	chromaShader.setUniform1f("chab_amount", chab_amount);
	
	chromaShader.setUniform1i("do_fisheye", do_fisheye);
	
	float scurv = ((testApp*) ofGetAppPtr())->getCurrentVolume();
	scurv = ofMap(scurv, 0, 1, 1, 10, true);
	chromaShader.setUniform1f("signcurvature", scurv);
	
  canvas.draw();
	texture.unbind();

  chromaShader.end();
  
  ofDisableAlphaBlending();
	
	fbo.end();
	
  ofSetWindowTitle(ofToString(ofGetFrameRate()));
	
	
	
}

void Feed::drawGui() {
	panel->draw();
}


ofTexture& Feed::getTextureReference() {
	return fbo.getTextureReference();
}

void Feed::setBackgroundSource(ofBaseVideoDraws *bd, bool isOtherFeed) {
	
	removeBackground();
	
	background = bd;
	isUsingOtherFeedAsBackground = isOtherFeed;
}

void Feed::removeBackground() {
	if (background != NULL && !isUsingOtherFeedAsBackground) {
		delete background;
		background = NULL;
	}
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



void Feed::mousePressed(ofMouseEventArgs &args) {
	bool &value = ((buttons::Toggle*) panel->getElement("Set background hue"))->value;
	
	if (value) {
		setBackgroundHueFromPoint(args.x, args.y);
		value = false;
		((buttons::Toggle*) panel->getElement("Set background hue"))->needsRedraw();
	}
	
	BaseHasPanel::mousePressed(args);
}

void Feed::operator()(unsigned int bID) {
	
	switch (bID) {
		case LOAD_BACKGROUND_BUTTON:
		{
			ofFileDialogResult result = ofSystemLoadDialog();
			ofVideoPlayer *player = new ofVideoPlayer();
			player->loadMovie(result.filePath);
			player->play();
			setBackgroundSource(player);
			break;
		}	
			
		case LOAD_OTHER_SOURCE_AS_BACKGROUND_BUTTON:
		{	
			
			int nfeeds = ((testApp*) ofGetAppPtr())->mixer.getNumFeeds();
			
			for (int i = 0; i < nfeeds; i++) {
				Feed *feed = ((testApp*) ofGetAppPtr())->mixer.getFeed(i);
				if (this != feed) {
					
					setBackgroundSource(feed->getGrabberPointer(), true);
				}
			}
			
			break;
		}
		
			
	}
}
