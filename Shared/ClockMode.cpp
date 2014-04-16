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
    animationMode       = MODE_SIN;
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
            cout << "set "<<ks<<":"<<((i+4) - letterIndex)<<endl;
            float weight = letterIndex == i ? 1.0 : .6;
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
                    
                case MODE_EXPLODE:
                    if ( ofGetElapsedTimeMillis() - aniLastChanged > aniDuration ){
                        aniLastChanged = ofGetElapsedTimeMillis();
                        ofVec2f p(ofRandom(ofGetWidth()), ofRandom(ofGetHeight()));
                        
                        for ( auto & c : clocks->clocks ){
                            c.magnet(p.x, p.y, c.faceColor, 500);
                            //float d = p.distance(c);
                            //float v = ofMap(d, 0, ofGetWidth(), 1.0, 0.0, true);
                            //c.rotateClockTo(v * 360, (v * 360) + 180);
                            //c.vel.x += v * 100;
                        }
                        cout << "explod" << endl;
                    }
                    break;
                    
                case MODE_ARROW:
                    break;
                    
                case MODE_YEARS:
                    break;
                    
                case MODE_SIN:
                    for ( auto & c : clocks->clocks ){
                        float offsetX = sin( ofGetElapsedTimeMillis() * .001 );
                        float offsetY = cos( ofGetElapsedTimeMillis() * .002 );
                        c.rotateClockBy( ofSignedNoise((c.x + offsetX) ) * 5.0, ofSignedNoise((c.x + offsetY) ) * 5.0);// * (c.y/ofGetHeight() + offsetY), 0);
                    }
                    break;
                    
                default:
                    break;
            }
        }
    }
}

bool ClockMode::isInteractive(){
    return bInteractiveMode;
}

int ClockMode::getCurrentName(){
    return currentName;
}
