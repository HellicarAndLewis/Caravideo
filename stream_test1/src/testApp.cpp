#include "testApp.h"

#define VIDEO_WIDTH 640
#define VIDEO_HEIGHT 480

#define BUFFER_SIZE 1024

testApp::~testApp() {
  delete [] audio_buffer;
}

//--------------------------------------------------------------
void testApp::setup(){

  grabber.setDeviceID(4);
  grabber.initGrabber(VIDEO_WIDTH, VIDEO_HEIGHT);
  
  stream.setup(this, 2, 2, 44100, BUFFER_SIZE, 4);
  
  audio_buffer = new float[BUFFER_SIZE*2];
  
  video_pipe.open("/Users/broadcast/Documents/AbandonNormalDevices/oF/of_0071_osx_release/apps/Caravideo/stream_data/video_pipe");
	video_thread.start(video_pipe);

	audio_pipe.open("/Users/broadcast/Documents/AbandonNormalDevices/oF/of_0071_osx_release/apps/Caravideo/stream_data/audio_pipe");
	audio_thread.start(audio_pipe);
}

//--------------------------------------------------------------
void testApp::update(){

  grabber.update();
  
  if (grabber.isFrameNew()) {
//    video_pipe.write((char*) grabber.getPixels(), VIDEO_WIDTH * VIDEO_HEIGHT * 3);
  }
}

//--------------------------------------------------------------
void testApp::draw(){

  ofSetHexColor(0xffffff);
  grabber.draw(0, 0);
  
  ofSetHexColor(0x0);
  ofPushMatrix();
  ofTranslate(0, 500);
  for (int i = 0; i < BUFFER_SIZE; i++) {
    ofCircle(i, audio_buffer[i*2] * 100, 1);
    ofCircle(i, audio_buffer[i*2+1] * 100 + 200, 1);
  }
  ofPopMatrix();
}


void testApp::audioReceived(float *input, int bufferSize, int nChannels) {
  
  //this isn't right!!! float* -> char*
  audio_pipe.write((char*) input, bufferSize * 2* 4);
  
  memcpy(audio_buffer, input, bufferSize * 2  * 4); // multiply by four because float is 32bit?
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