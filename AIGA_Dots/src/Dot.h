//
//  Dot.h
//  AIGA_Dots
//
//  Created by Brett Renfer on 3/5/14.
//
//

#pragma once

#include "ofMain.h"
#include "Constants.h"

class Dot : public ofVec3f {
public:
    
    // build
    void setup();
    void loadFont( ofTrueTypeFont & font );
    
    void draw( bool bRender = true);
    void clear();
    
    void mousePressed( ofVec2f m );
    void mouseReleased( ofVec2f m );
    
    void setText( string letter );
    string getText();
    
    int state;
    int r, g, b;
    
    ofFloatColor floatColor;
    int width;
    
    Mode mode, lastMode;
    
protected:
    
    ofTrueTypeFont * drawFont;
    string text;
    
private:
    float speed;
    float ran;
    int lastState;
    float stateMult;
    bool bOver;
    
    ofMesh mesh;
    
};