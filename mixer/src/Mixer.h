//
//  Mixer.h
//  mixer
//
//  Created by Will Gallia on 26/07/2012.
//  Copyright (c) 2012 . All rights reserved.
//

#pragma once

#include "Feed.h"
#include "BaseHasPanel.h"
#include "BaseHasCanvas.h"

#define NUM_FEEDS 2


class Mixer : public BaseHasPanel, BaseHasCanvas {
public:
  Mixer();
	~Mixer();
	
	void setup();
	void update();
	void draw();
	
	void addFeed(Feed *feed);
	
	bool draw_guis;
	
	int getNumFeeds() { return feed_counter; }
	Feed* getFeed(int i) { return feeds[i]; }
	
protected:
	Feed *feeds[NUM_FEEDS];
	int feed_counter;
	
	ofShader mixerShader;
	
	float cross_fader;
};
