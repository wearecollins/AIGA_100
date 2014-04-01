//
//  Clock.cpp
//  clocks
//
//  Created by Brett Renfer on 4/1/14.
//
//

#include "Clock.h"

#pragma mark Clock

//--------------------------------------------------------------
Clock::Clock(){
    numFaces = -1;
    radius = -1;
    vel.x = 1.0;
    colorMode = 0;
    offset = 0;
    ofSetCircleResolution(300);
    bColorFace = true;
    ofSetLineWidth(3.0);
    lastFroze = 0;
    rotMode = 0;
    lastNext = 0;
    
    bAnimating = true;
}

//--------------------------------------------------------------
void Clock::update(){
    if ( bMouseDown && ofGetElapsedTimeMillis() - lastNext > 100 ){
        lastNext = ofGetElapsedTimeMillis();
        nextRotate();
    }
    if (numFaces == -1 ) numFaces = 1;
    if (radius == -1 ) radius = liveRadius = 100;
    
    if ( angles.size() != numFaces * 2 ){
        angles.clear();
        targetAngle.clear();
        for (int i=0; i<numFaces; i++){
            angles.push_back(0 - 45);
            angles.push_back(90 - 45);
            
            targetAngle.push_back(0 - 45);
            targetAngle.push_back(90 - 45);
        }
    }
}

//--------------------------------------------------------------
void Clock::draw(){
    ofColor faceColorGood(faceColor);
    
    switch ( colorMode ){
        case 0:
            faceColor.setHue(ofWrap(ofWrap(offset, 0, 255) + ofMap(x + y, 0, ofGetWidth() + ofGetHeight(), 0, 120), 0, 255));
            break;
        case 1:
            faceColor.setHue(ofWrap(ofWrap(offset, 0, 255) + ofMap(y, 0, ofGetHeight(), 0, 120), 0, 255));
            break;
            
        case 2:
            faceColor.setHue(ofWrap(ofWrap(offset, 0, 255) + ofMap(sin(x), -1.0, 1.0, 0, 255), 0, 255));
            break;
            
        case 3:
            faceColor.setHue(ofWrap(ofWrap(offset, 0, 255) + ofMap(tan(x + y), -1.0, 1.0, 0, 255), 0, 255));
            break;
            
        case 4:
            faceColor.setHue(ofWrap(ofWrap(offset, 0, 255) + ofMap(y * 10.0 + ofWrap(ofGetElapsedTimeMillis(), 0, ofGetHeight() * 50.0), 0.0, ofGetHeight() * 50.0, 0, 255), 0, 255));
            break;
        case 5:
            faceColor.setHue( ofWrap(faceColor.getHue() + ofWrap(offset, 0, 255), 0, 255) );
            break;
    }
    if ( colorMode != 5 ){
        faceColor.setSaturation(150);
        faceColor.setBrightness(250);
    }
    
    ofPushMatrix();
    ofTranslate(x,y);
    for ( int i=numFaces-1; i>=0; i--){
        float r = (float) radius / numFaces;
        r *= (i+1);
        
        float cr = (float) liveRadius / numFaces;
        cr *= (i+1);
        
        float mult = numFaces == 1 ? 1.0 : ofMap(i, numFaces-1, 0, 2.0, 1.0);
        ofColor lc(faceColor);
        float hue = ofWrap(lc.getHue() * mult + sin( ofGetElapsedTimeMillis() * .0001) * 10.0, 0, 255.0f);
        lc.setHue( hue );
        
        if ( bColorFace ){
            ofSetColor(lc);
        } else {
            ofSetColor( i % 2 != 0 ? 200 : 255, numFaces == 1 ? 255 : (i % 2 != 0 ? 150 : 100) );
        }
        ofCircle(0,0,r);
        
        if ( bColorFace ){
            ofSetColor(armColor);
        } else {
            ofSetColor(lc);
        }
        
        // ease angles
        angles[i * 2]       = angles[i * 2] * .9 + targetAngle[i * 2] * .1;
        angles[i * 2 + 1]   = angles[i * 2 + 1] * .9 + targetAngle[i * 2 + 1] * .1;
        
        ofPushMatrix();
        ofVec2f p1(cr  * .7, cr * .7);
        p1.rotate(angles[i * 2], ofVec2f(0,0));
        ofLine(ofVec2f(0,0),p1);
        ofPopMatrix();
        
        hue = ofWrap(lc.getHue() * mult + sin( ofGetElapsedTimeMillis() * .0001) * 20.0, 0, 255.0f);
        lc.setHue( hue );
        
        if ( bColorFace ){
            ofSetColor(armColor);
        } else {
            ofSetColor(lc);
        }
        
        ofPushMatrix();
        ofVec2f p2(cr  * .7, cr * .7);
        p2.rotate(angles[i * 2 + 1], ofVec2f(0,0));
        ofLine(ofVec2f(0,0),p2);
        ofPopMatrix();
    }
    ofPopMatrix();
    
    if ( ofGetElapsedTimeMillis() - lastFroze > 2000 && bAnimating ){
        bool b = false;
        float m = numFaces == 1 ? 1 : .5;
        for ( auto & a : targetAngle ){
            b = !b;
            if ( b ){
                a += .1 * vel.x * m;
            } else {
                a += 1. * vel.x * m;
                m += .5;
            }
        }
    }
    if ( ofGetElapsedTimeMillis() - lastFroze > 2000 ){
        vel.x = vel.x * .9 + 1 * .1;
        offset = offset * .9;
    }
    
    // reset face color
    if ( colorMode == 5 ) faceColor = faceColorGood;
}

//--------------------------------------------------------------
void Clock::rotateClockTo ( float angleA, float angleB ){
    bool b = false;
    for ( auto & a : targetAngle ){
        if ( b ){
            a = angleA;
        } else {
            a = angleB;
        }
        b = !b;
    }
}

//--------------------------------------------------------------
void Clock::reset(){
    bool b = false;
    for ( auto & a : angles ){
        if ( b ){
            a = 0 - 45;
        } else {
            a = 90 - 45;
        }
        b = !b;
    }
    for ( auto & a : targetAngle ){
        if ( b ){
            a = 0 - 45;
        } else {
            a = 90 - 45;
        }
        b = !b;
    }
}

//--------------------------------------------------------------
void Clock::checkHit( int mx, int my, bool bFlip ){
    //        if ( bAnimating ) return;
    
    ofVec2f m(mx,my);
    float dist = distance(m);
    if ( dist < radius ){
        bMouseDown = true;
        lastNext = ofGetElapsedTimeMillis();
        if ( bFlip ){
            bColorFace = !bColorFace;
            bAnimating = !bAnimating;
            return;
        } else {
            //                nextRotate();
        }
    }
}

//--------------------------------------------------------------
void Clock::nextRotate(){
    rotMode++;
    if ( rotMode > 9 * 9 ) rotMode = 0;
    
    int a1 = floor(rotMode / 9) * 45;
    int a2 = (rotMode % 9) * 45;
    //        a2 = (rotMode % 9) * 45;// + floor(rotMode / 9) * 45;
    
    bool b = false;
    for ( auto & a : targetAngle ){
        if ( b ){
            a = a1 - 45;
        } else {
            a = a2 - 45;
        }
        b = !b;
    }
}

//--------------------------------------------------------------
void Clock::onMousePressed( int mx, int my ){
    ofVec2f m(mx,my);
    bMouseDown = fabs(distance(m)) < radius * 3;
    if ( bMouseDown ){
        rotateTo(mx, my);
        m -= *this;
        lastAngle = ofMap(angle(m), -180, 180, 0, 360) - 90;
        lastMouse.set(mx,my);
    }
}

//--------------------------------------------------------------
void Clock::onMouseDragged( int mx, int my ){
    if ( bMouseDown ){
        ofVec2f m(mx,my);
        m -= *this;
        float an = ofMap(angle(m), -180, 180, 0, 360) - 90;
        
        float diff = lastAngle - an;
        
        if ( abs(diff) > 60 ){
            
            rotateClockTo(targetAngle[0] + (90) * -ofSign(diff), targetAngle[1] + (90) * -ofSign(diff));
            
            lastAngle = an;
        }
        lastMouse.set(mx,my);
    }
}

//--------------------------------------------------------------
void Clock::onMouseReleased( int mx, int my ){
    bMouseDown = false;
}

//--------------------------------------------------------------
void Clock::rotateTo( int mx, int my ){
    ofVec2f m(mx,my);
    float dist = distance(m);
    m -= *this;
    
    if ( fabs(dist) < 3000 ){
        
        ofVec2f line = *this + ofVec2f(liveRadius  * .5, liveRadius * .5);
        bool b = false;
        int mult = 1.0;
        float an = ofWrap(ofMap(angle(m), -180, 180, 0, 360) - 90, 0, 360);
        
        for ( auto & a : targetAngle ){
            if ( an >= 30 && an <= 60 ){
                a = 180;
                if ( b ){
                    a += 90;
                }
                a += 45;
            } else if ( an > 60 && an <= 120 ){
                a = 180;
                if ( b ){
                    a += 180;
                }
                a += 45;
            } else if ( an > 120 && an <= 150 ){
                a = 270;
                if ( b ){
                    a += 90;
                }
                a += 45;
                
            } else if ( an > 150 && an <= 210 ){
                a = 270;
                if ( b ){
                    a += 180;
                }
                a += 45;
            } else if ( an > 210 && an <= 240 ){
                a = 360;
                if ( b ){
                    a += 90;
                }
                a += 45;
            } else if ( an > 240 && an <= 300 ){
                a = 360;
                if ( b ){
                    a += 180;
                }
                a += 45;
            } else if ( an > 300 && an <= 330 ){
                a = 450;
                if ( b ){
                    a += 90;
                }
                a += 45;
            } else if ( (an > 330 && an <= 390)){// || (an >=0 && an < 30) ){
                a = 450;
                if ( b ){
                    a += 180;
                }
                a += 45;
            }
            
            
            b = !b;
        }
    }
    
}

//--------------------------------------------------------------
void Clock::magnet( int mx, int my ){
    ofVec2f m(mx,my);
    float dist = distance(m);
    m -= *this;
    
    if ( (dist) < 20000 ){
        ofVec2f line = *this + ofVec2f(liveRadius  * .5, liveRadius * .5);
        bool b = false;
        int mult = 1.0;
        for ( auto & a : targetAngle ){
            a = ofMap(angle(m), -180, 180, 0, 360) * mult;// + 135;
            if ( b ){
                a += 180;
                //mult += 2.0;
            }
            b = !b;
        }
        vel.x += 10.0;
        offset += ofMap(dist, 0, 200, 10, 0, true);
        offset = ofClamp(offset, 0, 150);
        lastFroze = ofGetElapsedTimeMillis() - ofMap(dist, 0, 1000, 1000, 0, true);
    }
}

#pragma mark clocks

//--------------------------------------------------------------
Clocks::Clocks(){
    numFaces = 1;
    mode = 0;
    bColorFace = true;
    bPreciseDraw = false;
    lineWidth = 1.0;
    hueVariance = 0;
    guiVisible = false;
}

//--------------------------------------------------------------
Clocks::~Clocks(){
    ofRemoveListener(ofEvents().update, this, &Clocks::update);
    ofRemoveListener(ofEvents().keyPressed, this, &Clocks::keyPressed);
    ofRemoveListener(ofEvents().mouseDragged, this, &Clocks::mouseDragged);
    ofRemoveListener(ofEvents().mousePressed, this, &Clocks::mousePressed);
    ofRemoveListener(ofEvents().mouseReleased, this, &Clocks::mouseReleased);
}

//--------------------------------------------------------------
void Clocks::setup( int gridX, int gridY, int spacing, int startX, int startY, int radius, float radiusMult ){
    for (int x=0; x<gridX; x++){
        for (int y=0; y<gridY; y++){
            clocks.push_back(Clock());
            clocks.back().radius = radius;
            clocks.back().liveRadius =  radius * radiusMult;
            clocks.back().x = startX + x * spacing;
            clocks.back().y = startY + y * spacing;
        }
    }
    
    setupGui();
    loadLetters();
    
    ofAddListener(ofEvents().update, this, &Clocks::update);
    ofAddListener(ofEvents().keyPressed, this, &Clocks::keyPressed);
    ofAddListener(ofEvents().mouseDragged, this, &Clocks::mouseDragged);
    ofAddListener(ofEvents().mousePressed, this, &Clocks::mousePressed);
    ofAddListener(ofEvents().mouseReleased, this, &Clocks::mouseReleased);
}

//--------------------------------------------------------------
void Clocks::update( ofEventArgs & e ){
    for ( auto & c : clocks ){
        c.numFaces = numFaces;
        c.colorMode = mode;
        c.update();
    }
}

//--------------------------------------------------------------
void Clocks::draw(){
    ofPushStyle();
    ofSetLineWidth(lineWidth);
    for ( auto & c : clocks ){
        c.draw();
    }
    ofPopStyle();
}

// GUI

//--------------------------------------------------------------
void Clocks::setupGui(){
    // setup gui
    colorImage.loadImage("spectrum.png");
    gui = new ofxUICanvas(0,0,ofGetWidth() / 4.0, ofGetHeight() );
    gui->setTriggerWidgetsUponLoad(true);
    gui->toggleVisible();
    
    gui->addLabel("Face color");
    gui->addImageSampler("color", &colorImage, ofGetWidth() * .2, ofGetWidth() * .2);
    gui->addSlider("Face saturation", 0, 255, 0.0);
    gui->addSlider("Face brightness", 0, 255, 0.0);
    gui->addLabel("Letters face color");
    gui->addImageSampler("letterColor", &colorImage, ofGetWidth() * .2, ofGetWidth() * .2);
    
    gui->addIntSlider("numFaces", 1, 25, &numFaces);
    gui->addIntSlider("color mode", 0, 5, &mode);
    gui->addSlider("lineWidth", 1.0, 10.0, &lineWidth);
    gui->addSlider("hueVariance", 0.0, 255., &hueVariance);
    
    gui2 = new ofxUICanvas(ofGetWidth() / 4.0,0,ofGetWidth() / 4.0, ofGetHeight() );
    gui2->setTriggerWidgetsUponLoad(true);
    ofAddListener(gui->newGUIEvent, this, &Clocks::onGui );
    ofAddListener(gui2->newGUIEvent, this, &Clocks::onGui );
    
    gui2->toggleVisible();
    gui2->addLabel("Arm color");
    gui2->addImageSampler("arm color", &colorImage, ofGetWidth() * .2, ofGetWidth() * .2);
    
    gui2->addSlider("arm saturation", 0, 255, 0.0);
    gui2->addSlider("arm brightness", 0, 255, 0.0);
    
    gui2->addLabel("Letters arm color");
    gui2->addImageSampler("letterArmColor", &colorImage, ofGetWidth() * .2, ofGetWidth() * .2);
    gui2->addSlider("letter arm saturation", 0, 255, 0.0);
    gui2->addSlider("letter arm brightness", 0, 255, 0.0);
    
    gui2->addToggle("magnet or draw", &bPreciseDraw);
    
    gui->loadSettings("gui-settings.xml");
    gui2->loadSettings("gui2-settings.xml");
}

//--------------------------------------------------------------
void Clocks::onGui( ofxUIEventArgs &e ){
    if ( e.getName() == "color"){
        ofxUIImageSampler * s = (ofxUIImageSampler*) e.widget;
        setNonLetterFaceColors(s->getColor(), hueVariance);
        
    } else if ( e.getName() == "letterColor"){
        ofxUIImageSampler * s = (ofxUIImageSampler*) e.widget;
        setLetterFaceColors(s->getColor(), 0);
        
    } else if ( e.getName() == "arm color"){
        ofxUIImageSampler * s = (ofxUIImageSampler*) e.widget;
        setNonLetterFaceColors(s->getColor(), hueVariance, true);
    } else if ( e.getName() == "letterArmColor" ){
        ofxUIImageSampler * s = (ofxUIImageSampler*) e.widget;
        setLetterFaceColors(s->getColor(), hueVariance, true);
    } else if ( e.getName() == "face brightness" ){
        for ( auto & c : clocks ){
            c.faceColor.setBrightness(e.getSlider()->getScaledValue());
        }
    } else if ( e.getName() == "face saturation" ){
        for ( auto & c : clocks ){
            c.faceColor.setSaturation(e.getSlider()->getScaledValue());
        }
    } else if ( e.getName() == "arm brightness" ){
        
        ofxUIImageSampler * s = (ofxUIImageSampler*) gui2->getWidget("arm color");
        ofColor c = s->getColor();
        c.setBrightness(e.getSlider()->getScaledValue());
        setNonLetterFaceColors(c, hueVariance, true);
        
    } else if ( e.getName() == "arm saturation" ){
        
        ofxUIImageSampler * s = (ofxUIImageSampler*) gui2->getWidget("arm color");
        ofColor c = s->getColor();
        c.setSaturation(e.getSlider()->getScaledValue());
        setNonLetterFaceColors(c, hueVariance, true);
        
    } else if ( e.getName() == "letter arm brightness" ){
        
        ofxUIImageSampler * s = (ofxUIImageSampler*) gui2->getWidget("letterArmColor");
        ofColor c = s->getColor();
        c.setBrightness(e.getSlider()->getScaledValue());
        setLetterFaceColors(c, hueVariance, true);
        
    } else if ( e.getName() == "letter arm saturation" ){
        
        ofxUIImageSampler * s = (ofxUIImageSampler*) gui2->getWidget("letterArmColor");
        ofColor c = s->getColor();
        c.setSaturation(e.getSlider()->getScaledValue());
        setLetterFaceColors(c, hueVariance, true);
    }
}

// METHODS
//--------------------------------------------------------------
void Clocks::setClocks( Letter letter, int offsetX, int offsetY, int letterWidth ){
    for (int x=offsetX; x<offsetX + letterWidth; x++){
        for (int y=offsetY; y<offsetY + clockLetterHeight; y++){
            int ind = (y - offsetY) + (x - offsetX) * clockLetterHeight;
            int cind = y + x * 10.0;
            clocks[cind].rotateClockTo( letter.angles[ind][0], letter.angles[ind][1]);
            clocks[cind].lastFroze = ofGetElapsedTimeMillis();
        }
    }
}

//--------------------------------------------------------------
void Clocks::setFaceColors( ofColor color, int hueVariance, bool bArm ){
    ofColor * editColor;
    for ( auto & c : clocks ){
        editColor = bArm ? &c.armColor : &c.faceColor;
        
        editColor->set( color );
        editColor->setHue( ofWrap(color.getHue() + ofMap(c.y, 0, 700.0, 0, hueVariance), 0, 255));
    }
}

//--------------------------------------------------------------
void Clocks::setNonLetterFaceColors( ofColor color, int hueVariance, bool bArm ){
    int gridX = 2;
    int gridW = 6;
    int gridY = 2;
    int gridH = 6;
    
    ofColor * editColor;
    
    for (int x=0; x<10; x++){
        for (int y=0; y<10; y++){
            int cind = y + x * 10.0;
            if ( x < gridX || y < gridY || x >= gridX + gridW || y >= gridY + gridH ){
                editColor = bArm ? &clocks[cind].armColor : &clocks[cind].faceColor;
                
                editColor->set( color );
                editColor->setHue( ofWrap(color.getHue() + ofMap(clocks[cind].y, 0, 700.0, 0, hueVariance), 0, 255));
            }
        }
    }
}

//--------------------------------------------------------------
void Clocks::setLetterFaceColors( ofColor color, int hueVariance, bool bArm ){
    int gridX = 2;
    int gridW = 6;
    int gridY = 2;
    int gridH = 6;
    
    ofColor * editColor;
    
    for (int x=0; x<10; x++){
        for (int y=0; y<10; y++){
            int cind = y + x * 10.0;
            if ( (x >= gridX && (x < gridX + gridW))
                && (y >= gridY && y < gridY + gridH)){
                editColor = bArm ? &clocks[cind].armColor : &clocks[cind].faceColor;
                
                editColor->set( color );
                editColor->setHue( ofWrap(color.getHue() + ofMap(clocks[cind].y, 0, 700.0, 0, hueVariance), 0, 255));
            }
        }
    }
}

// letters
//--------------------------------------------------------------
void Clocks::loadLetters(){
    if ( letters.size() == 0){
        char a = 'a';
        for ( int i=0; i<26; i++){
            alphabet.push_back(ofToString(a));
            ++a;
        }
        alphabet.push_back(" ");
    }
    ofxXmlSettings settings;
    
    for ( string s : alphabet ){
        if ( settings.load("letters/" + s +".xml")){
            letters[s] = Letter();
            
            int ind = 0;
            
            for ( int i=0; i<settings.getNumTags("clock"); i++){
                settings.pushTag("clock", i );
                float a1 = settings.getValue("a1", 0.0);
                float a2 = settings.getValue("a2", 0.0);
                letters[s].angles.push_back(vector<float>());
                letters[s].angles.back().push_back(a1);
                letters[s].angles.back().push_back(a2);
                
                settings.popTag();
            }
        } else {
            cout <<"beef: "<< s << endl;
        }
    }
}



/************************************************
    KEY & MOUSE
************************************************/

//--------------------------------------------------------------
void Clocks::keyPressed( ofKeyEventArgs & e ){
    int key = e.key;
    
    //    if ( key == 'b' ){
    //        bColorFace = !bColorFace;
    //    }
    if ( ofGetKeyPressed( OF_KEY_ALT ) ){
        ofxXmlSettings settings;
        for ( auto & c : clocks ){
            settings.addTag("clock");
            settings.pushTag("clock", settings.getNumTags("clock") - 1 );
            settings.addValue("a1", c.angles[0]);
            settings.addValue("a2", c.angles[1]);
            settings.addValue("face", c.bColorFace);
            settings.popTag();
        }
        settings.save("letters/" + ofToString(key) + ".xml");
    } else if ( ofGetKeyPressed( OF_KEY_CONTROL ) ){
        
        char k = key;
        string ks = ofToString(k);
        if ( letters.count(ks) > 0){
            setClocks( letters[ks], ofRandom(0, 7), ofRandom(0,5), (ks == "w" || ks == "m")? 6 : 4 );
        }
        
    } else if ( key == 'r' ){
        for ( auto & c : clocks ){
            c.reset();
        }
    } else if ( key == 'b' ){
        for ( auto & c : clocks ){
            c.bColorFace = !c.bColorFace;
        }
    } else if ( key == 'a' ){
        for ( auto & c : clocks ){
            c.bAnimating = !c.bAnimating;
        }
    } if ( key == 'g' ){
        gui->toggleVisible();
        gui2->toggleVisible();
    } else if ( key == 's' ){
        gui->saveSettings("gui-settings.xml");
        gui2->saveSettings("gui2-settings.xml");
    }
    guiVisible = gui->isVisible();
}

//--------------------------------------------------------------
void Clocks::mouseDragged(ofMouseEventArgs & e ){
    int x = e.x;
    int y = e.y;
    int button = e.button;
    
    if ( guiVisible ) return;
    for ( auto & c : clocks ){
        //        if ( c.bAnimating )
        if ( !bPreciseDraw ) c.magnet(x,y);
        //c.onMouseDragged(x, y);
    }
    
}

//--------------------------------------------------------------
void Clocks::mousePressed(ofMouseEventArgs & e ){
    int x = e.x;
    int y = e.y;
    int button = e.button;
    
    if ( guiVisible ) return;
    for ( auto & c : clocks ){
        if ( bPreciseDraw )
            c.checkHit(x,y);
    }
}

//--------------------------------------------------------------
void Clocks::mouseReleased(ofMouseEventArgs & e ){
    int x = e.x;
    int y = e.y;
    int button = e.button;
    
    if ( guiVisible ) return;
    for ( auto & c : clocks ){
        c.onMouseReleased(x, y);
    }
}

