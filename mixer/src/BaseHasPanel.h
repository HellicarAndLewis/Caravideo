//
//  BaseHasPanel.h
//
//  Created by Will Gallia on 26/07/2012.
//  Copyright (c) 2012. All rights reserved.
//

#pragma once

#include "ofMain.h"
#include "buttons/Buttons.h"

class BaseHasPanel {
public:
	BaseHasPanel();
	virtual ~BaseHasPanel();
	
	virtual void operator()(unsigned int bID);
	virtual void mouseMoved(ofMouseEventArgs &args);
	virtual void mouseDragged(ofMouseEventArgs &args);
	virtual void mousePressed(ofMouseEventArgs &args);
	virtual void mouseReleased(ofMouseEventArgs &args);
	
protected:
	buttons::Panel *panel;
	
};