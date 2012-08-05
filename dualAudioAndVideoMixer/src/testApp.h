#pragma once

#include "ofMain.h"
#include "ofxSoundStream.h"

class testApp : public ofBaseApp{
	
	public:
		
		void setup();
		void update();
		void draw();
		
		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);		
		
		ofVideoGrabber 		videoChannelA;
		ofVideoGrabber 		videoChannelB;
		int 				camWidth;
		int 				camHeight;
  
  //from ofxSoundStream
  
  void audioReceived 	(float * input, int bufferSize, int nChannels);
  void audioInputListener (ofxAudioEventArgs &args);
	
  ofxSoundStream c1;
  ofxSoundStream c2;
	
  float * left;
  float * right;
  
  float * left2;
  float * right2;
};
