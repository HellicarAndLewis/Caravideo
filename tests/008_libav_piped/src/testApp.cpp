#include "testApp.h"

testApp::~testApp() {
	raw.close();
}

//--------------------------------------------------------------
void testApp::setup(){
	ofSetFrameRate(60);

	sound_stream.listDevices();
	sound_stream.setDeviceID(0);

	vid_grabber.initGrabber(320,240);
	vid_grabber.setVerbose(true);
	start_time = 0;
	
#ifdef USE_VIDEO_PIPE
	video_pipe.open("/Users/diederickhuijbers/c++/__PLAYGROUND__/libav/_libav_testing/my_video_pipe");
	video_thread.start(video_pipe);
#endif

#ifdef USE_AUDIO_PIPE
	audio_pipe.open("/Users/diederickhuijbers/c++/__PLAYGROUND__/libav/_libav_testing/my_audio_pipe");
	audio_thread.start(audio_pipe);
	
	sin_wave = new float[44100 * 2];
	float inc = (TWO_PI * 400) / 44100;
	float a = 0.0f;
	for(int i = 0; i < 44100; ++i) {
		sin_wave[i*2] = sin(a) * 100;
		sin_wave[i*2+1] = sin(a) * 100;
		a += inc;
	}
	sound_stream.setup(this, 0, 2, 44100, 512, 4);
#endif
	
	// setup h264 encoder with raw output.
#ifdef USE_ENCODING
	encoder.setup(vid_grabber.getWidth(), vid_grabber.getHeight(), vid_grabber.getWidth(), vid_grabber.getHeight());
	encoder.encodeHeader();
	raw.open("output2.h264");
	raw.writeHeaders(encoder.getEncodedNalsPtr(), encoder.getNumBytesOfEncodedNals());
#endif

}

void testApp::audioIn(float* input, int bufferSize, int nChannels) {
#ifdef USE_AUDIO_PIPE
	audio_pipe.write((char*)input, bufferSize *  2 * sizeof(float));
	//audio_pipe.write((char*)sin_wave, 44100 *  2 * sizeof(float));
#endif
}

//--------------------------------------------------------------
void testApp::update(){
	vid_grabber.update();
}

//--------------------------------------------------------------
void testApp::draw(){
	// only stream for X-millis
	if(!start_time) {
		start_time = ofGetElapsedTimeMillis();
	}
	else {
		int now = ofGetElapsedTimeMillis();
		int d = now - start_time;
		if( (d/1000) >= 1000) {
			printf("stop\n");
			::exit(0);
		}
	}
	
	vid_grabber.draw(0,0);
	ofDrawBitmapString(ofToString(ofGetFrameRate()), 10, 20);
	img.grabScreen(0,0,320,240);

#ifdef USE_VIDEO_PIPE
	//av.addVideoFrame(img.getPixels());
	video_pipe.write((char*)img.getPixels(), vid_grabber.getWidth() * vid_grabber.getHeight() * 3);
#endif

#ifdef USE_ENCODING
	static int num = 0;
	encoder.encodeFrame(img.getPixels(), num);	
	raw.writeFrame(encoder.getEncodedNalsPtr(), encoder.getNumBytesOfEncodedNals());
	++num;
#endif
}

//--------------------------------------------------------------
void testApp::keyPressed(int key){

}

//--------------------------------------------------------------
void testApp::keyReleased(int key){

}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y){

}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void testApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void testApp::dragEvent(ofDragInfo dragInfo){ 

}
