//
//  ClockMode.h
//  AIGA_Dots
//
//  Created by Brett Renfer on 4/16/14.
//  Copyright (c) 2014 __MyCompanyName__. All rights reserved.
//

#pragma once

#include "ofMain.h"
#include "Clock.h"
#include "ofxSpacebrew.h"
#include "ofxXmlSettings.h"

enum AnimationMode {
    MODE_NORMAL = 0,
    MODE_WAVE,
    MODE_RANDOM,
    MODE_EXPLODE,
    MODE_ORDERED,
    MODE_SHADES,
    MODE_NOISE,
    MODE_SIN,
    MODE_YEARS,
    MODE_ARROW
};

enum State {
    STATE_INTERACTIVE = 0,
    STATE_TRANSITION,
    STATE_LETTERS
};

struct Timer {
    int delay = 1000;
    int lastTriggered = 0;
    bool bLoop = true;
    bool bTriggered = false;
};

class ClockMode
{
public:
    
    ClockMode();
    
    void setup( Spacebrew::Connection & sb, Clocks & c, string path = "settings/mode-settings.xml" );
    void setup( Spacebrew::Connection * sb, Clocks * c, string path = "settings/mode-settings.xml" );
    void update();
    
    bool isInteractive();
    int  getCurrentName();
    
protected:
    
    void resetAnimation( AnimationMode mode );
    
    bool bChangeModes;
    State mode;
    int interactiveDuration;
    int nameDuration;
    int lastChanged;
    int currentName;
    int numNames;
    
    // animation vars
    AnimationMode animationMode, transitionMode;
    int aniLastChanged, aniDuration;
    
    // letters + names
    int letterIndex;
    int letterRate;
    int letterLastChanged;
    
    vector<string>      names;
    vector<ofColor>     colors;
    vector<Timer>       delays;
    
    int arrowStartMinute, arrowStopMinute;
    
    // transition vars
    ofColor nextColor, nextArmColor;
    State nextState;
    
    Clocks * clocks;
    Spacebrew::Connection * spacebrew;
    
};

