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

//	panel->content_x = 300;
//	panel->position();
	panel->setPosition(300, 300);
	
	cross_fader = 0;
	panel->addFloat("crossfader", cross_fader).setMin(0.0).setMax(1.0);
	panel->addFloat("eras", cross_fader).setMin(0.0).setMax(1.0);

	panel->load();
	
	
	img.loadImage("image.jpg");
}

void Mixer::update() {
	
	for (int i = 0; i < NUM_FEEDS && feeds[i] != NULL; i++) {
		feeds[i]->update();
		feeds[i]->draw();
	}
	
	panel->update();
}

void Mixer::draw() {
	
//	for (int i = 0; i < NUM_FEEDS && feeds[i] != NULL; i++) {
//		
//		feeds[i]->getTextureReference().draw(i*640, 0);
//	}
	
	
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