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
    bInteractiveMode   = true;
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
        for ( auto n : names ){
            ofLogVerbose()<<n;
        }
    }
    resetAnimation( (AnimationMode ) animationMode );
}

//--------------------------------------------------------------
void ClockMode::update(){
    if ( !clocks->clocks[0].bAnimating ){
        return;
    }
    // auto-rate changer
    if ( bChangeModes ){
        
        // interactive to quote
        if (bInteractiveMode){
            if ( ofGetElapsedTimeMillis() - lastChanged > interactiveDuration){
                lastChanged = ofGetElapsedTimeMillis();
                bInteractiveMode = false;
                currentName++;
                if ( currentName >= numNames ){
                    currentName = 0;
                }
                if ( spacebrew != NULL ) spacebrew->sendRange("name", currentName);
                if ( spacebrew != NULL ) spacebrew->sendRange("mode", 1);
                letterIndex = 0;
                letterLastChanged = ofGetElapsedTimeMillis();
                ofLogVerbose()<<"name mode";
            }
            
            // quote to interactive
        } else {
            if ( ofGetElapsedTimeMillis() - lastChanged > nameDuration && letterIndex == 0){
                lastChanged = ofGetElapsedTimeMillis();
                bInteractiveMode = true;
                if ( spacebrew != NULL ) spacebrew->sendRange("mode", 0);
                letterIndex = 0;
                letterLastChanged = ofGetElapsedTimeMillis();
                cout<<"interactive mode"<<endl;
                
                // randomize animation mode
                animationMode = (AnimationMode) ofRandom(MODE_ARROW + 1);
                aniLastChanged = ofGetElapsedTimeMillis();
                
                resetAnimation( (AnimationMode) animationMode );
            }
        }
    }
    
    if (!bInteractiveMode){
        char c = names[currentName][letterIndex];
        string kz = ofToString(c);
        
        int w = (kz == "w" || kz == "m")? 6 : 4;
        
        //int start = ofMap(ofGetElapsedTimeMillis() - letterLastChanged, 0, letterRate, (ks == "w" || ks == "m")? 4 : 6, 0);
        int ind = 3 - letterIndex * w;
        for (int i=0; i<names[currentName].length(); i++){
            char k = names[currentName][i];
            string ks = ofToString(k);
//            letterIndex;
            //cout << "set "<<ks<<":"<<((i+4) - letterIndex)<<endl;
            float weight = letterIndex == i ? 1.0 : .3;
            clocks->setClocks(clocks->letters[ks], ind, 3, (ks == "w" || ks == "m")? 6 : 4, weight);
            ind += (ks == "w" || ks == "m")? 6 : 4;
        }
        if ( ofGetElapsedTimeMillis() - letterLastChanged > letterRate ){
            letterLastChanged = ofGetElapsedTimeMillis();
            
            letterIndex++;
            if ( letterIndex >= names[currentName].length() ){
                letterIndex = 0;
            }
        }
    } else {
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
    switch (animationMode) {
        case MODE_NORMAL:
            // shhhh
            break;
            
        case MODE_WAVE:
            for ( auto & c : clocks->clocks ){
                delays[index].lastTriggered = ofGetElapsedTimeMillis();
                delays[index].delay = ofMap(c.x, 0, ofGetWidth(), min, max, true);
                delays[index].bTriggered = false;
                index++;
            }
            break;
            
        case MODE_NOISE:
            break;
            
        case MODE_EXPLODE:
            for ( auto & c : clocks->clocks ){
                delays[index].lastTriggered = ofGetElapsedTimeMillis();
                delays[index].delay = ofMap(fabs(p.distance(c)), 0, ofGetWidth(), 500, 2000, true);
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
    return bInteractiveMode;
}

int ClockMode::getCurrentName(){
    return currentName;
}
