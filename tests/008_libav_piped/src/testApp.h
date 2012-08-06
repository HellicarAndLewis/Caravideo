#pragma once

#include "ofMain.h"
#include "AVPipe.h"
#include "Pipe.h"
#include "H264Raw.h"
#include "H264Encoder.h"

//#define USE_ENCODING
//#define USE_AUDIO_PIPE
#define USE_VIDEO_PIPE

class testApp : public ofBaseApp{
	public:
		~testApp();
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
		void audioIn(float* input, int bufferSize, int nChannels);
		
		//AVPipe av;
		ofImage img;
		ofVideoGrabber vid_grabber;
		ofSoundStream sound_stream;
		int start_time;
		
		Pipe audio_pipe;
		Pipe video_pipe;
		Poco::Thread audio_thread;
		Poco::Thread video_thread;
		float* sin_wave;

		// h264 encoder
		H264Encoder encoder;
		H264Raw raw;
};
