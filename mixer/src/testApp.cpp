#include "testApp.h"

ofVideoPlayer *player;

void testApp::setup(){

//  feed.setBackroundSource(new ofVideoPlayer());
  feed.setup();
  
  player = new ofVideoPlayer();
  player->loadMovie("bieber.gif");
  player->play();
  feed.setBackgroundSource(player);
  
  
//  delete feed.background;
}


void testApp::update(){

  feed.update();
  
  player->update();
}


void testApp::draw(){

  feed.draw();
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

  feed.setBackgroundHueFromPoint(x, y);
}


void testApp::mouseReleased(int x, int y, int button){

}


void testApp::windowResized(int w, int h){

}


void testApp::gotMessage(ofMessage msg){

}


void testApp::dragEvent(ofDragInfo dragInfo){ 

}