#pragma once

#include "Pipe.h"
#include "ofMain.h"

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
  
  
  void audioReceived(float *input, int bufferSize, int nChannels);
  float *audio_buffer;
  
  
  ofVideoGrabber grabber;
  ofSoundStream stream;
  
  Pipe audio_pipe;
  Pipe video_pipe;
  Poco::Thread audio_thread;
  Poco::Thread video_thread;
};
