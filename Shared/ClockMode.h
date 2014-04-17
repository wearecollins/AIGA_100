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
#ifdef USE_SPACEBREW
#include "ofxSpacebrew.h"
#endif
#include "ofxXmlSettings.h"

enum AnimationMode {
    MODE_NORMAL = 0,
    MODE_WAVE,
    MODE_NOISE,
    MODE_SIN,
    MODE_YEARS,
    MODE_EXPLODE,
    MODE_ARROW
};

struct Timer {
    int delay = 1000;
    int lastTriggered = 0;
    bool bLoop = true;
    bool bTriggered = false;
};

#ifndef USE_SPACEBREW
namespace Spacebrew {
    class Connection {
    public:
        void sendRange( string s, int r ){};
    };
}
#endif

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
    bool bInteractiveMode;
    int interactiveDuration;
    int nameDuration;
    int lastChanged;
    int currentName;
    int numNames;
    
    // animation vars
    int animationMode;
    int aniLastChanged, aniDuration;
    
    // letters + names
    int letterIndex;
    int letterRate;
    int letterLastChanged;
    
    vector<string>  names;
    vector<Timer>   delays;
    
    Clocks * clocks;
    Spacebrew::Connection * spacebrew;
    
};

