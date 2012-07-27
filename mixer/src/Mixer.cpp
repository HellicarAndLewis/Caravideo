//
//  Mixer.cpp
//  mixer
//
//  Created by Will Gallia on 26/07/2012.
//  Copyright (c) 2012 . All rights reserved.
//

#include "Mixer.h"

Mixer::Mixer():
BaseHasPanel("Mixer"),
BaseHasCanvas(),
feed_counter(0),
draw_guis(true) {

	for (int i = 0; i < NUM_FEEDS; i++) {
		feeds[i] = NULL;
	}
	
}

Mixer::~Mixer() {
	
	for (int i = 0; i < NUM_FEEDS; i++) {
		if (feeds[i] != NULL) {
			delete feeds[i];
		}
	}
}

ofImage img;

void Mixer::setup() {
	
	mixerShader.load("mixer");

	panel->setPosition(300, 300);
	
	cross_fader = 0;
	do_mix = true;
	panel->addFloat("crossfader", cross_fader).setMin(0.0).setMax(1.0);

	panel->addButton("do slide", SLIDE_BUTTON, this);
	
	do_slide = false;
	slide_amount = 0;
	
	panel->load();
	
	
	img.loadImage("image.jpg");
}

void Mixer::update() {
	
	for (int i = 0; i < NUM_FEEDS && feeds[i] != NULL; i++) {
		feeds[i]->update();
		feeds[i]->draw();
	}
	
	panel->update();
	
	if (do_slide) {
		slide_amount+=6;
		
		if (slide_amount > texture.getHeight()) {
			do_slide = false;
			slide_amount = 0;
			do_mix = tempDoMix;
//			cross_fader = cross_fader == 0.0 ? 1.0 : 0.0;
			Feed *temp = feeds[0];
			feeds[0] = feeds[1];
			feeds[1] = temp;
			
			ofLog(OF_LOG_NOTICE) << "slide transition finished" << endl;
		}
	}
}

void Mixer::draw() {
	
	texture.bind();
	
	mixerShader.begin();
	
	mixerShader.setUniformTexture("tex0", feeds[0]->getTextureReference(), 0);
	
	//use an image if we only have one feed
	if (feed_counter < 2) {
		mixerShader.setUniformTexture("tex1", img, 1);
	}
	else {
		mixerShader.setUniformTexture("tex1", feeds[1]->getTextureReference(), 1);
	}
	
	mixerShader.setUniform1f("cross_fader", cross_fader);
	mixerShader.setUniform1i("do_mix", do_mix);
	mixerShader.setUniform1f("slide_amount", slide_amount);
	mixerShader.setUniform1i("do_slide", do_slide);
	mixerShader.setUniform1i("slide_which", slide_which);
	
	canvas.draw();
	
	mixerShader.end();
	
	texture.unbind();
	
	
	if (draw_guis) {
		for (int i = 0; i < NUM_FEEDS && feeds[i] != NULL; i++) {
			feeds[i]->drawGui();
		}
		
		panel->draw();
	}
}


void Mixer::perfomTransition(transition_type type) {
	
	switch (type) {
		case SLIDE_TRANSITION:
		{
			if (cross_fader == 0.0 || cross_fader == 1.0) {
				tempDoMix = do_mix;
				do_mix = false;
				do_slide = true;
				slide_amount = 0;
				slide_which = (cross_fader == 0) ? true : false;
			}
			else {
				ofLog(OF_LOG_WARNING) << "can't slide transition when cross fader is not 1 or 0" << endl;
			}
			break;
		}	
		
		case FLIP_TRANSITION:
			
			break;
			
		case BLEND_TRANSITION:
			break;
	}
	
}


//there is no remove feed... once we add that's it!

void Mixer::addFeed(Feed *feed) {
	
	if (feed_counter < 0 || feed_counter >= NUM_FEEDS) {
		ofLog(OF_LOG_ERROR) << "Mixer: index out of bounds, not adding feed" << endl;
		return;
	}
	
	if (feeds[feed_counter] != NULL) {
		delete feeds[feed_counter];
		feeds[feed_counter] = NULL;
	}
	
	
	feeds[feed_counter] = feed;
	feeds[feed_counter]->setup();
	
	feed_counter++;
	
}

void Mixer::operator()(unsigned int bID) {
	
	switch(bID) {
		case SLIDE_BUTTON:
			perfomTransition(SLIDE_TRANSITION);
			break;
	}
}