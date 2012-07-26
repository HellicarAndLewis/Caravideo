//
//  BaseHasCanvas.h
//  mixer
//
//  Created by Will Gallia on 26/07/2012.
//  Copyright (c) 2012. All rights reserved.
//

#pragma once

#include "ofMain.h"
#include "Constants.h"

class BaseHasCanvas {
public:
	
	BaseHasCanvas() {
		canvas.setMode(OF_PRIMITIVE_TRIANGLES);
		
		vector<ofVec3f> vertices;
		vertices.push_back(ofVec3f(0, 0, 0));
		vertices.push_back(ofVec3f(VIDEO_WIDTH, 0, 0));
		vertices.push_back(ofVec3f(VIDEO_WIDTH, VIDEO_HEIGHT, 0));
		vertices.push_back(ofVec3f(VIDEO_WIDTH, VIDEO_HEIGHT, 0));
		vertices.push_back(ofVec3f(0,VIDEO_HEIGHT, 0));
		vertices.push_back(ofVec3f(0, 0, 0));
		canvas.addVertices(vertices);
		
		canvas.enableTextures();
		vector<ofVec2f> texcoords;
		texcoords.push_back(ofVec2f(0, 0));
		texcoords.push_back(ofVec2f(VIDEO_WIDTH, 0));
		texcoords.push_back(ofVec2f(VIDEO_WIDTH, VIDEO_HEIGHT));
		texcoords.push_back(ofVec2f(VIDEO_WIDTH, VIDEO_HEIGHT));
		texcoords.push_back(ofVec2f(0, VIDEO_HEIGHT));
		texcoords.push_back(ofVec2f(0, 0));
		canvas.addTexCoords(texcoords);
		
		texture.allocate(VIDEO_WIDTH, VIDEO_HEIGHT, GL_RGBA);
	}
	
	ofMesh canvas;
	ofTexture texture;
};
