#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){
	
	camWidth 		= 320;	// try to grab at this size. 
	camHeight 		= 240;
  

    videoChannelA.listDevices();
  
//	videoChannelA.setVerbose(true);
//	videoChannelA.initGrabber(camWidth,camHeight);

//
//  videoChannelB.setVerbose(true);
//	videoChannelB.initGrabber(camWidth,camHeight);
  
  //audio
  
  left = new float[256];
	right = new float[256];
	
	left2 = new float[256];
	right2 = new float[256];
	
	c1.listDevices();
	c1.setDeviceId(2); //device = 2 (Pinnacle Systems GmbH: DVC100)
	c1.setup(0, 2, this, 44100, 256, 4);
  ofAddListener(c1.audioReceivedEvent, this, &testApp::audioInputListener);
  
	c2.setDeviceId(3); //device = 3 (Pinnacle Systems GmbH: DVC100)
	c2.setup(0, 2, this, 44100, 256, 4);
	ofAddListener(c2.audioReceivedEvent, this, &testApp::audioInputListener);
}


//--------------------------------------------------------------
void testApp::update(){
	
	ofBackground(100,100,100);
	
//	videoChannelA.grabFrame();
//  videoChannelB.grabFrame();
	

}

//--------------------------------------------------------------
void testApp::draw(){
	ofSetHexColor(0xffffff);
//	videoChannelA.draw(0,0);
//  videoChannelB.draw(videoChannelA.getWidth(), 0);

  
  //audio drawing
  
//  ofSetColor(255,0,0);
//	ofDrawBitmapString("Device 1:" , 100, 80);
//  
//	// draw the left:
//  
//	ofRect(100,100,256,200);
//  ofSetColor(0,0,255);
//  
//	for (int i = 0; i < 256; i++){
//		ofLine(100+i,200,100+i,200+left[i]*100.0f);
//	}
//  
//  ofSetColor(255,0,0);
//	// draw the right:
//	ofRect(600,100,256,200);
//  ofSetColor(0,0,255);
//	for (int i = 0; i < 256; i++){
//		ofLine(600+i,200,600+i,200+right[i]*100.0f);
//	}
//  
//	ofSetColor(255,0,0);
//	ofDrawBitmapString("Device 2:" , 100, 380);
//  
//	// draw the left:
//	ofRect(100,400,256,200);
//  ofSetColor(0,0,255);
//	for (int i = 0; i < 256; i++){
//		ofLine(100+i,500,100+i,500+left2[i]*100.0f);
//	}
//  
//  ofSetColor(255,0,0);
//	// draw the right:
//	ofRect(600,400,256,200);
//  ofSetColor(0,0,255);
//	for (int i = 0; i < 256; i++){
//		ofLine(600+i,500,600+i,500+right2[i]*100.0f);
//	}
}


//--------------------------------------------------------------
void testApp::keyPressed  (int key){ 
	
	// in fullscreen mode, on a pc at least, the 
	// first time video settings the come up
	// they come up *under* the fullscreen window
	// use alt-tab to navigate to the settings
	// window. we are working on a fix for this...
	
	if (key == 'A' || key == 'a'){
		videoChannelA.videoSettings();
	}
  
	if (key == 'B' || key == 'b'){
		videoChannelB.videoSettings();
	}
	
	
}

//--------------------------------------------------------------
void testApp::keyReleased(int key){ 
	
}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y ){
	
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

static float adder = 0;
//--------------------------------------------------------------
void testApp::audioReceived(float * input, int bufferSize, int nChannels){
	/*// samples are "interleaved"
   for (int i = 0; i < bufferSize; i++){
   left[i] = input[i*2];
   right[i] = input[i*2+1];
   }
   bufferCounter++;*/
}

//--------------------------------------------------------------
void testApp::audioInputListener(ofxAudioEventArgs &args){
	// samples are "interleaved"
	if(args.deviceID == 0){
		for (int i = 0; i < args.bufferSize; i++){
			left2[i] = args.buffer[i*2];
			right2[i] = args.buffer[i*2+1];
		}
	}
	else{
		for (int i = 0; i < args.bufferSize; i++){
			left[i] = args.buffer[i*2];
			right[i] = args.buffer[i*2+1];
		}
	}
	//cout << "Audio Input event received from device " << args.deviceID << " - " << args.deviceName << endl;
}
