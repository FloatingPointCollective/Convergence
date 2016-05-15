//
//  visualSystem.h
//  JRHMemorialWall
//
//  Created by Jack Kalish on 6/25/14.
//
//

#pragma once
#include "ofMain.h"
#include "ofxArtNet.h"


class displaySystem{
    public:
    
    displaySystem();
    void init(int w, int h, int numRods, int rodSpacing, int rodMargins);
    void draw(int x, int y);
    void updateDisplay(ofFbo * frame);
    void clear();
    
    int width, height, numRodsOuter, numRodsInner;
    
    private:
    ofFbo * _frame;
    ofFbo stripFBO, strip;
    ofxArtnet artnet;
    ofImage testImage, stripImage;
    
    vector <ofFbo*> stripFBOs;
    vector <ofImage*> stripImages;
    
    int _rodSpacing, _numRods, _rodMargins, frameCount;
    
    

};