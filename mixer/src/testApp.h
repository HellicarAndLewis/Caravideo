#pragma once

#include "ofMain.h"
//#include "Feed.h"
#include "Mixer.h"

class testApp : public ofBaseApp{
public:
	void setup();
	void update();
	void draw();
	
	void audioReceived(float *input, int bufferSize, int nChannels);
	void keyPressed(int key);
	
	Mixer mixer;
		
	float getCurrentVolume() { return currentVolume; }
	
private:
	ofSoundStream soundStream;
	float currentVolume;
};
