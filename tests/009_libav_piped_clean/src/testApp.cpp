#include "testApp.h"

testApp::~testApp() {
}

//--------------------------------------------------------------
void testApp::setup(){
	ofSetFrameRate(60);

	vid_grabber.initGrabber(320,240);
	vid_grabber.setVerbose(true);
	
#ifdef USE_VIDEO_PIPE
	video_pipe.open("/Users/diederickhuijbers/c++/__PLAYGROUND__/libav/_libav_testing/my_video_pipe");
	video_thread.start(video_pipe);
#endif

#ifdef USE_AUDIO_PIPE
	audio_pipe.open("/Users/diederickhuijbers/c++/__PLAYGROUND__/libav/_libav_testing/my_audio_pipe");
	audio_thread.start(audio_pipe);
#endif

}

//--------------------------------------------------------------
void testApp::update(){
	vid_grabber.update();
}

//--------------------------------------------------------------
void testApp::draw(){
	vid_grabber.draw(0,0);
	ofDrawBitmapString(ofToString(ofGetFrameRate()), 10, 20);
	img.grabScreen(0,0,320,240);

#ifdef USE_VIDEO_PIPE
	video_pipe.write((char*)img.getPixels(), vid_grabber.getWidth() * vid_grabber.getHeight() * 3);
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
