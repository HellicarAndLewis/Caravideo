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
	
	enum transition_type {
		SLIDE_TRANSITION,
		BLEND_TRANSITION,
		FLIP_TRANSITION
	};
	
	enum {
		SLIDE_BUTTON,
		FLIP_BUTTON,
		BLEND_BUTTON
	};
	
  Mixer();
	~Mixer();
	
	void setup();
	void update();
	void draw();
	
	void operator()(unsigned int bID);
	
	void perfomTransition(transition_type type);
	
	
	void addFeed(Feed *feed);
	int getNumFeeds() { return feed_counter; }
	Feed* getFeed(int i) { return feeds[i]; }
	
	void toggleGuis() { draw_guis = !draw_guis; }
	
protected:
	Feed *feeds[NUM_FEEDS];
	int feed_counter;
	
	ofShader mixerShader;
	
	bool do_mix;
	float cross_fader;
	
	bool do_slide, slide_which;
	float slide_amount;
	
	bool draw_guis;

	
	bool tempDoMix;
	float tempMixValue;
};
