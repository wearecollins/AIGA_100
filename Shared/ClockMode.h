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
    MODE_NOISE,
    MODE_SIN,
    MODE_YEARS,
    MODE_EXPLODE,
    MODE_ARROW
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
    
    vector<string> names;
    
    Clocks * clocks;
    Spacebrew::Connection * spacebrew;
    
};

