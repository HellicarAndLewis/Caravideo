/*
 *      ofxSoundStream.h
 *
 *      v 0.02 March 2010
 *      Arturo Castro & Eduard Prats Molner
 */

#ifndef _OFX_SOUND_STREAM
#define _OFX_SOUND_STREAM

#include <iostream>
#include "ofConstants.h"
#include "ofUtils.h"
#include "ofBaseApp.h"
#include "ofEvents.h"
#include "ofMath.h"
#include "RtAudio.h"
#include "ofxAudioEvent.h"

class ofxSoundStream{
public:
	ofxSoundStream();
	virtual ~ofxSoundStream();
	void setDeviceId(int deviceID);
	void setDeviceIdByName(string _deviceName);
	void setup(int nOutputChannels, int nInputChannels, ofBaseApp * OFSA = NULL);
	void setup(int nOutputChannels, int nInputChannels, unsigned int sampleRate, unsigned int bufferSize, unsigned int nBuffers);
	void setup(int nOutputChannels, int nInputChannels, ofBaseApp * OFSA, unsigned int sampleRate, unsigned int bufferSize, unsigned int nBuffers);
	void stop();
	void start();
	void close();
	void listDevices();

	int 	receiveAudioBuffer(void *outputBuffer, void *inputBuffer, int bufferSize);

	ofEvent<ofxAudioEventArgs> 	audioReceivedEvent;
	ofEvent<ofxAudioEventArgs> 	audioRequestedEvent;

protected:
	ofBaseApp 	* 		OFSAptr;
	RtAudio		*		audio;
	int 				nInputChannels;
	int 				nOutputChannels;
	ofxAudioEventArgs 	audioEventArgs;
	int					deviceID;
	string				deviceName;
	
	string getDeviceName();

};

#endif
