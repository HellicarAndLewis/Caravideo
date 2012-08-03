#include "testApp.h"


const int N = 1;
ofxBlackmagicGrabber grabber[N];
ofSoundStream soundStream;

const int BUFFER_SIZE = 1024;


//--------------------------------------------------------------
void testApp::setup(){

  
  for (int i = 0 ; i < N; i++) {
    grabber[i].setDeviceID(i);
    grabber[i].setPixelFormat(OF_PIXELS_RGB);
    grabber[i].setVideoMode(bmdModePAL);
    grabber[i].setDeinterlace(true);
    grabber[i].setAudioReceiver(this);
    grabber[i].initGrabber(WIDTH, HEIGHT);
  }

  soundStream.setup(this, 2, 0, 44100, 1024, 4);

  
  buffer = CircularBuffer<float>(CBUFFER_SIZE);
  
  data.push_back(0.0);
  
//  for (int i = 0; i < 50; i++) {
//    buffer.write(i * 1.0);
//    cout << buffer.read() << endl;
//  }
//  exitApp();
  
//  inputf = ifstream("sounddata.txt");
  inputf.open("sounddata.txt");
}

//--------------------------------------------------------------
void testApp::update(){

  for (int i = 0; i < N; i++) {
    grabber[i].update();
    if (grabber[i].isFrameNew()) {
      //
    }
  }
}

//--------------------------------------------------------------
void testApp::draw(){

  
  ofTexture tex;
  tex.allocate(WIDTH, HEIGHT, GL_RGB);
  
  for (int i = 0; i < N; i++) {
    tex.loadData(grabber[i].getPixelsRef());
    
    tex.draw(WIDTH*i, 0);
  }
  
}

float theta = 0;

void testApp::audioRequested(float *output, int bufferSize, int nChannels) {
  
  cout << "audioRequested start" << endl;
  
  for (int i = 0; i < bufferSize; i++) {

    
//    if (data.size()> 2) {
//    float v = data.front();
//      data.pop_front();
//    output[i*nChannels] = v;
//      v = data.front();
//      data.pop_front();
//    output[i*nChannels+1] = v;
//
//    }
//    else {
//      printf("no size\n");
//    }
    
    if (inputf.good()) {
      string line;
      getline(inputf, line);
      float v = ofToFloat(line);
      output[i*nChannels] = v;
      getline(inputf, line);
      v = ofToFloat(line);
      output[i*nChannels+1] = v;
    }
    else {
      cout << "out of sound data" << endl;
      exitApp();
    }
    
  }
  
  cout << "audioRequested end" << endl;
}


void testApp::audioReceived(void *_data, int nSamples) {
  cout << "audioReceived start" << endl;
  int16_t *sidata = (int16_t*) _data;
  
  
  
  for (int i = 0; i < nSamples; i++) {
    data.push_back(sidata[i]/(32768.0));
//    data.push_back(0.0);
//    data.push_back(<#const value_type &__x#>)
  }
  return;
  
  for (int i = 0; i < nSamples; i++) {
    buffer.write(sidata[i]/65536.0);
  }
  cout << "audioReceived end" << endl;
}

//--------------------------------------------------------------
void testApp::keyPressed(int key){
  
//  for (int i = 0; i < data.size(); i++) {
//    cout << data[i] << endl;
//  }

  ofstream file;
  file.open("sounddata.txt");
  for (int i = 0; i < data.size(); i++) {
    file << data[i] << endl;
  }
  file.close();
  
  exitApp();
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