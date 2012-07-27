//
//  BaseHasPanel.cpp
//  mixer
//
//  Created by Will Gallia on 26/07/2012.
//  Copyright (c) 2012. All rights reserved.
//

#include "BaseHasPanel.h"


BaseHasPanel::BaseHasPanel(string title) {
	ofRegisterMouseEvents(this);
	panel = new buttons::Buttons(title, 250);
}

BaseHasPanel::~BaseHasPanel() {
	cout << "Destructed" << endl;
	delete panel;
}

void BaseHasPanel::operator()(unsigned int bID) {

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