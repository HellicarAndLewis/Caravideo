#include "testApp.h"

#define N_CHANNELS 2

ofVideoPlayer *player;

void testApp::setup(){

	ofSetFrameRate(30);
	ofSetVerticalSync(true);

	ofBackground(20);

	soundStream.setup(this, N_CHANNELS, N_CHANNELS, 44100, 512, 4);
	currentVolume = 0;
  
  mixer.setup();
	mixer.addFeed(new Feed(2));
	mixer.addFeed(new Feed(3));
	
}


void testApp::update(){


	mixer.update();
}

void testApp::draw(){

	mixer.draw();
	
}

void testApp::audioReceived(float *input, int bufferSize, int nChannels) {
		
	float vol = 0.0;
	
	for (int i = 0; i < bufferSize; i++){
		float left 	= input[i*N_CHANNELS];
		float right	= input[i*N_CHANNELS+1];
		
		vol += left * left;
		vol += right * right;
	}
	
	//calculate root mean squared
	vol/= (bufferSize*2);
	vol = sqrt( vol );
	
	//do some smoothing.
	currentVolume *= 0.93;
	currentVolume += 0.07 * vol;
	
}

void testApp::keyPressed(int key) {
	
	switch (key) {
		case ' ':
			mixer.toggleGuis();
			break;
	}
}
