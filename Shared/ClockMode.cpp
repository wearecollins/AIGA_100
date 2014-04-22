//
//  ClockMode.cpp
//  AIGA_Dots
//
//  Created by Brett Renfer on 4/16/14.
//  Copyright (c) 2014 __MyCompanyName__. All rights reserved.
//

#include "ClockMode.h"

//--------------------------------------------------------------
ClockMode::ClockMode(){
    bChangeModes       = true;
    mode                = STATE_INTERACTIVE;
    interactiveDuration = 30 * 1000;
    nameDuration        = 20 * 1000;
    lastChanged         = 0;
    currentName         = 0;
    letterIndex         = 0;
    letterRate          = 2000;
    letterLastChanged   = 0;
    animationMode       = MODE_ARROW;
    aniDuration         = 3000;
}

//--------------------------------------------------------------
void ClockMode::setup(Spacebrew::Connection & sb, Clocks & c,  string path){
    // set references
    spacebrew = &sb;
    clocks = &c;
    setup( spacebrew, clocks, path );
}

//--------------------------------------------------------------
void ClockMode::setup(Spacebrew::Connection * sb, Clocks * c,  string path){
    spacebrew = sb;
    clocks = c;

    for ( auto c : clocks->clocks ){
        delays.push_back(Timer());
    }
    
    ofxXmlSettings settings;
    if ( settings.load(path) ){
        interactiveDuration = settings.getValue("interactiveDuration", interactiveDuration);
        nameDuration = settings.getValue("nameDuration", nameDuration);
        settings.pushTag("names");{
            numNames = settings.getNumTags("name");
            for (int i=0; i<numNames; i++){
                names.push_back( settings.getValue("name", "", i) );
            }
        } settings.popTag();
        
        settings.pushTag("colors");{
            int n = settings.getNumTags("color");
            for (int i=0; i<n; i++){
                colors.push_back( ofColor() );
                colors.back().r = settings.getAttribute("color", "r", 255, i);
                colors.back().g = settings.getAttribute("color", "g", 255, i);
                colors.back().b = settings.getAttribute("color", "b", 255, i);
                colors.back().a = settings.getAttribute("color", "h", 255, i);
            }
        } settings.popTag();
        
        for ( auto n : names ){
            ofLogVerbose()<<n;
        }
    }
    resetAnimation( animationMode );
    clocks->setColor(ofColor(255));
    clocks->setArmColor(ofColor(0));
}

//--------------------------------------------------------------
void ClockMode::update(){
    if ( !clocks->clocks[0].bAnimating ){
        return;
    }
    // auto-rate changer
    if ( bChangeModes ){
        
        // interactive to quote
        switch (mode) {
            case STATE_INTERACTIVE:
                
                if ( ofGetElapsedTimeMillis() - lastChanged > interactiveDuration){
                    lastChanged = ofGetElapsedTimeMillis();
                    mode = STATE_TRANSITION;
                    nextState = STATE_LETTERS;
                    
                    currentName++;
                    if ( currentName >= numNames ){
                        currentName = 0;
                    }
                    if ( spacebrew != NULL ) spacebrew->sendRange("name", currentName);
                    if ( spacebrew != NULL ) spacebrew->sendRange("mode", 1);
                    
                    letterIndex = 0;
                    ofLogVerbose()<<"name mode";
                    
                    // transition to color
                    nextColor = ofColor(colors[currentName].r, colors[currentName].g, colors[currentName].b);
                    nextArmColor = ofColor(colors[currentName].a);
                    //clocks->setColor( ofColor(colors[currentName].r, colors[currentName].g, colors[currentName].b) );
                    //clocks->setArmColor(ofColor(colors[currentName].a));
                    
                    // setup transition
                    transitionMode =  (AnimationMode) floor(ofRandom(1, MODE_NOISE));
                    resetAnimation( transitionMode );
                }
                break;
                
            case STATE_TRANSITION:
                break;
                
            case STATE_LETTERS:
                if ( ofGetElapsedTimeMillis() - lastChanged > nameDuration && letterIndex == 0 ){
                    lastChanged = ofGetElapsedTimeMillis();
                    mode = STATE_TRANSITION;
                    nextState = STATE_INTERACTIVE;
                    
                    if ( spacebrew != NULL ) spacebrew->sendRange("mode", 0);
                    letterIndex = 0;
                    letterLastChanged = ofGetElapsedTimeMillis();
                    ofLogVerbose()<<"interactive mode"<<endl;
                    
                    // randomize animation mode
                    animationMode = (AnimationMode) ofRandom(MODE_ARROW + 1);
                    aniLastChanged = ofGetElapsedTimeMillis();
                    
                    resetAnimation( animationMode );
                    
                    // transition to white/black
                    nextColor       = ofColor(255);
                    nextArmColor    = ofColor(0);
                    //clocks->setColor(ofColor(255));
                    //clocks->setArmColor(ofColor(0));
                    
                    // setup transition
                    transitionMode =  (AnimationMode) floor(ofRandom(1, MODE_NOISE));
                    resetAnimation( transitionMode );
                }
                break;
                
            default:
                break;
        }
    }
    
    
    static bool threeLetter = false;
    
    // letter switching
    if ( mode == STATE_LETTERS ){
        char c = names[currentName][letterIndex];
        string kz = ofToString(c);
        
        int w = (kz == "w" || kz == "m")? 6 : 4;
        
        if ( threeLetter ){
            
            //int start = ofMap(ofGetElapsedTimeMillis() - letterLastChanged, 0, letterRate, (ks == "w" || ks == "m")? 4 : 6, 0);
            int ind = 3 - letterIndex * w;
            for (int i=0; i<names[currentName].length(); i++){
                char k = names[currentName][i];
                string ks = ofToString(k);
    //            letterIndex;
                //cout << "set "<<ks<<":"<<((i+4) - letterIndex)<<endl;
                float weight = letterIndex == i ? 1.0 : .3;
                clocks->setClocks(clocks->letters[ks], ind, (ks == "w" || ks == "m")? 3 : 4, 3, weight);
                ind += (ks == "w" || ks == "m")? 6 : 4;
            }
        }
        if ( ofGetElapsedTimeMillis() - letterLastChanged > letterRate ){
            letterLastChanged = ofGetElapsedTimeMillis();
            
            letterIndex++;
            if ( letterIndex >= names[currentName].length() ){
                letterIndex = 0;
            }
            
            if ( !threeLetter && (ofGetElapsedTimeMillis() - lastChanged < nameDuration || letterIndex != 0) ){
                char k = names[currentName][letterIndex];
                string ks = ofToString(k);
                //float weight = letterIndex == i ? 1.0 : .3;
                clocks->setClocks(clocks->letters[ks], 3, (ks == "w" || ks == "m")? 2 : 3,  (ks == "w" || ks == "m")? 6 : 4, 1.0);
            }
        }
    } else if (mode == STATE_TRANSITION ){
        int index = 0;
        int triggered = 0;
        
        for ( auto & c : clocks->clocks ){
            
            if ( ofGetElapsedTimeMillis() - delays[index].lastTriggered <= delays[index].delay ){
                // just chill
            } else {
                if ( !delays[index].bTriggered ){
                    delays[index].bTriggered = true;
                    c.vel.x = 100;
                    c.faceColor.set(nextColor);
                    c.armColor.set(nextArmColor);
                    
                    triggered++;
                } else {
                    triggered++;
                }
            }
            index++;
        }
        
        // CHANGE STATES
        if ( triggered == clocks->clocks.size() ){
            mode = nextState;
            
            if ( nextState == STATE_LETTERS ){
                letterLastChanged = ofGetElapsedTimeMillis();
                if ( !threeLetter ){
                    char k = names[currentName][letterIndex];
                    string ks = ofToString(k);
                    //float weight = letterIndex == i ? 1.0 : .3;
                    clocks->setClocks(clocks->letters[ks], (ks == "w" || ks == "m")? 4 : 3, 3, (ks == "w" || ks == "m")? 6 : 4, 1.0);
                }
            }
        }
        
    } else if (mode == STATE_INTERACTIVE ){
        if ( clocks->clocks[0].bAnimating ){
            
            // start changing ambient animations
            switch (animationMode) {
                case MODE_NORMAL:
                    // shhhh
                    break;
                    
                case MODE_NOISE:
                    for ( auto & c : clocks->clocks ){
                        float offsetX = ofGetElapsedTimeMillis() * .0005;
                        float offsetY = ofGetElapsedTimeMillis() * .01;
                        c.rotateClockBy( ofNoise(c.x + offsetX) * 2,
                                        ofNoise(c.x + offsetX * .9) * 2);
                    }
                    break;
                    
                case MODE_WAVE:
                case MODE_EXPLODE:
                    if ( ofGetElapsedTimeMillis() - aniLastChanged > aniDuration ){
                        aniLastChanged = ofGetElapsedTimeMillis();
                        
                        resetAnimation( (AnimationMode) animationMode );
                    } else {
                        int index = 0;
                        for ( auto & c : clocks->clocks ){
                            if ( ofGetElapsedTimeMillis() - delays[index].lastTriggered <= delays[index].delay ){
                                //cout << delays[index].delay << endl;
                                //c.rotateClockTo(0,180);
                                c.vel.x = 0;
                                c.lastFroze = ofGetElapsedTimeMillis();
                            } else {
                                if ( !delays[index].bTriggered ){
                                    delays[index].bTriggered = true;
                                    c.vel.x = 100;
                                } else {
                                    c.vel.x = c.vel.x * .9 + 5 * .1;
                                }
                                c.lastFroze = ofGetElapsedTimeMillis() - 1500;
                            }
                            index++;
                        }
                    }
                    break;
                    
                case MODE_ARROW:
                case MODE_YEARS:
                    if ( ofGetElapsedTimeMillis() - aniLastChanged > aniDuration ){
                        aniLastChanged = ofGetElapsedTimeMillis();
                        resetAnimation( (AnimationMode) animationMode );
                    }
                    break;
                    
                case MODE_SIN:
                    for ( auto & c : clocks->clocks ){
                        c.rotateClockTo( sin(c.x / ofGetWidth() + ofGetElapsedTimeMillis() * .0001 ) * 360,
                                         sin(c.x/ ofGetWidth()  + ofGetElapsedTimeMillis() * .0001 ) * 360 + 180 );
                    }
                    break;
                    
                default:
                    break;
            }
        }
    }
}

void ClockMode::resetAnimation( AnimationMode mode ){
    int index = 0;
    ofVec2f p(ofRandom(ofGetWidth()), ofRandom(ofGetHeight()));
    float min = ofRandom(50, 500);
    float max = ofRandom(min, 2000);
    float maxX = clocks->clocks[clocks->clocks.size()-1].x;
    float maxY = clocks->clocks[clocks->clocks.size()-1].y;
    int dir = floor( ofRandom(4));
    bool bFlip = false;
    
    switch (mode) {
        case MODE_NORMAL:
            // shhhh
            break;
            
        case MODE_RANDOM:
            for ( auto & c : clocks->clocks ){
                delays[index].lastTriggered = ofGetElapsedTimeMillis();
                delays[index].delay = ofRandom(min, max);
                delays[index].bTriggered = false;
                index++;
            }
            break;
            
        case MODE_WAVE:
            for ( auto & c : clocks->clocks ){
                delays[index].lastTriggered = ofGetElapsedTimeMillis();
                float val = 0;
                float minIn = 0; float maxIn = 0;
                float minOut = 0; float maxOut = 0;
                switch (dir) {
                    case 0:
                        val = c.x;
                        minIn = 0; maxIn = maxX;
                        minOut = min; maxOut = max;
                        break;
                        
                    case 1:
                        val = c.x;
                        minIn = 0; maxIn = maxX;
                        minOut = max; maxOut = min;
                        break;
                        
                    case 2:
                        val = c.y;
                        minIn = 0; maxIn = maxY;
                        minOut = min; maxOut = max;
                        break;
                        
                    case 3:
                        val = c.y;
                        minIn = 0; maxIn = maxY;
                        minOut = max; maxOut = min;
                        break;
                }
                delays[index].delay = ofMap(val, minIn, maxIn, minOut, maxOut, true);
                delays[index].bTriggered = false;
                index++;
            }
            break;
            
        case MODE_NOISE:
            break;
            
        case MODE_EXPLODE:
            for ( auto & c : clocks->clocks ){
                delays[index].lastTriggered = ofGetElapsedTimeMillis();
                delays[index].delay = ofMap(fabs(p.distance(c)), 0, maxX, min, max, true);
                delays[index].bTriggered = false;
                index++;
            }
            break;
        
        case MODE_ORDERED:
            for ( auto & c : clocks->clocks ){
                delays[index].lastTriggered = ofGetElapsedTimeMillis();
                float minIn = 0; float maxIn = 0;
                float minOut = 0; float maxOut = 0;
                int val = index;
                switch (dir) {
                    case 0:
                    case 3:
                        val = index;
                        minIn = 0; maxIn = 100;
                        break;
                        
                    case 1:
                    case 2:
                        val = index;
                        minIn = 100; maxIn = 0;
                        break;
                }
                delays[index].delay = ofMap(val, minIn, maxIn, min * 2, max * 2, true);
                delays[index].bTriggered = false;
                index++;
            }
            break;
            
        case MODE_SHADES:
            for ( auto & c : clocks->clocks ){
                int x = index % 10;
                int y = index / 10.0;
                
                float minIn = 0; float maxIn = 10;
                if ( bFlip ){
                    minIn = 10;
                    maxIn = 0;
                }
                
                delays[index].lastTriggered = ofGetElapsedTimeMillis();
                delays[index].delay = ofMap(x, minIn, maxIn, min, max, true);
                delays[index].bTriggered = false;
                
                index++;
            }
            break;
            
        case MODE_ARROW:
            for ( auto & c : clocks->clocks ){
                int x = (float) index / 10.0f;
                int y = index % 10;
                float angle = (float) ( y )  / 10.0 * 360.0f;
                c.rotateClockTo( -90, angle-90 );
                c.vel.x = 0;
                c.lastFroze = ofGetElapsedTimeMillis() + aniDuration / 2.0;
                index++;
            }
            break;
            
        case MODE_YEARS:
            for ( auto & c : clocks->clocks ){
                int x = (float) index / 10.0f;
                int y = index % 10;
                float angle = (float) ( x + y * 10 )  / 100.0 * 360.0f;
                c.rotateClockTo( -90, angle-90 );
                c.vel.x = 0;
                c.lastFroze = ofGetElapsedTimeMillis() + aniDuration / 2.0;
                index++;
            }
            break;
            
        case MODE_SIN:
            break;
            
        default:
            break;
    }
}

bool ClockMode::isInteractive(){
    return mode == STATE_INTERACTIVE;
}

int ClockMode::getCurrentName(){
    return currentName;
}
