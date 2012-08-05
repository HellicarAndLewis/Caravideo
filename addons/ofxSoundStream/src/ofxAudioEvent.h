/*
 *  ofxAudioEvent.h
 *  audioMultiInputExample
 *
 *  Created by Eduard Prats Molner on 7/3/10.
 *
 */

#ifndef OFX_AUDIO_EVENT_H_
#define OFX_AUDIO_EVENT_H_

#include "ofMain.h"
#include <map>



class ofxAudioEventArgs: public ofAudioEventArgs{
public:
	int 	deviceID;
	string	deviceName;
};


#endif