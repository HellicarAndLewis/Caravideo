//
//  BaseHasPanel.cpp
//  mixer
//
//  Created by Will Gallia on 26/07/2012.
//  Copyright (c) 2012. All rights reserved.
//

#include "BaseHasPanel.h"
#include "Constants.h"
#include "Feed.h"
#include "testApp.h"
#include "Mixer.h"

BaseHasPanel::BaseHasPanel(string title) {
	ofRegisterMouseEvents(this);
	panel = new buttons::Buttons(title, 250);
}

BaseHasPanel::~BaseHasPanel() {
	cout << "Destructed" << endl;
	delete panel;
}

void BaseHasPanel::operator()(unsigned int bID) {
	
	switch (bID) {
		case LOAD_BACKGROUND_BUTTON:
		{
			ofFileDialogResult result = ofSystemLoadDialog();
			ofVideoPlayer *player = new ofVideoPlayer();
			player->loadMovie(result.filePath);
			player->play();
			((Feed*) this)->setBackgroundSource(player);
			break;
		}	
		
		case LOAD_OTHER_SOURCE_AS_BACKGROUND_BUTTON:
		{	
			
			int nfeeds = ((testApp*) ofGetAppPtr())->mixer.getNumFeeds();
			cout << "nfeeds = " << nfeeds << endl;
			for (int i = 0; i < nfeeds; i++) {
				Feed *feed = ((testApp*) ofGetAppPtr())->mixer.getFeed(i);
				if (this != feed) {
					cout << "YO!" << endl;
					
					((Feed*) this)->setBackgroundSource(feed->getGrabberPointer(), true);
				}
			}
			
			break;
		}
			
	}
}



void BaseHasPanel::mouseMoved(ofMouseEventArgs &args){
	panel->onMouseMoved(args.x, args.y);
}


void BaseHasPanel::mouseDragged(ofMouseEventArgs &args){
	panel->onMouseMoved(args.x, args.y);
}

void BaseHasPanel::mousePressed(ofMouseEventArgs &args){
	panel->onMouseDown(args.x, args.y);
}

void BaseHasPanel::mouseReleased(ofMouseEventArgs &args){
	panel->onMouseUp(args.x, args.y);
}