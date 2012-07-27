#include "testApp.h"


ofVideoPlayer *player;

void testApp::setup(){

	ofSetFrameRate(30);
	ofSetVerticalSync(true);

	ofBackground(20);

  
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


void testApp::keyPressed(int key){
  
}


void testApp::keyReleased(int key){

}


void testApp::mouseMoved(int x, int y){

}


void testApp::mouseDragged(int x, int y, int button){

}


void testApp::mousePressed(int x, int y, int button){

}


void testApp::mouseReleased(int x, int y, int button){

}


void testApp::windowResized(int w, int h){

}


void testApp::gotMessage(ofMessage msg){

}


void testApp::dragEvent(ofDragInfo dragInfo){ 

}