#pragma once

#include "ofMain.h"

#include "ofxBlackmagicGrabber.h"
#include "ofxBlackmagicAudioReceiver.h"

#include "CircularBuffer.h"

#define WIDTH 720
#define HEIGHT 576

#define CBUFFER_SIZE 50000


#include <iostream>
#include <fstream>

class testApp : public ofBaseApp, public ofxBlackmagicAudioReceiver {
	public:
		void setup();
		void update();
		void draw();
		
		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y);
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);
  
  void audioRequested(float *output, int bufferSize, int nChannels);
  
  void audioReceived(void *data, int nSamples);
  
//  float audioBuffer[CBUFFER_SIZE];
  CircularBuffer<float> buffer;
  
  deque<float> data;
  ifstream inputf;

};




