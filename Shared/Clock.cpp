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
    bColorFace = true;
    bMouseDown = false;
    bLetter = false;
    bDoColor = false;
    lastFroze = 0;
    rotMode = 0;
    lastNext = 0;
    colorIndex = 0;
    lineWidth = 1.0;
    
    bAnimating = true;
}

ofVec2f getCircleVec( float angle, float rad ){
    float x = cos( ofDegToRad(angle) ) * rad;
    float y = -sin( ofDegToRad(angle)) * rad;
    return ofVec2f(x,y);
}

//--------------------------------------------------------------
void Clock::setup(){
    ofSetCircleResolution(300);
    faceColor.set(255,0,0);
    liveFaceColor.set(255, 0, 0);
    
    // build face mesh
    float res = 100;
    float inc = (float) 360.0 / res;
    
    int i = 0;
    colorIncrement = 90;
    
    ofVec2f center(0,0);
    ofFloatColor color;
    color.set( liveFaceColor.r / 255.0, liveFaceColor.g / 255.0, liveFaceColor.b / 255.0 );
    
    ticks.setMode(OF_PRIMITIVE_LINES);
    
    for ( float a = 0; a < 360.0; a+= inc ){
        face.addVertex(getCircleVec(a, radius));
        face.addIndex(i);
        face.addColor(color);
        i++;
        
        face.addVertex( getCircleVec(a + inc, radius) );
        face.addIndex(i);
        face.addColor(color);
        i++;
        
        face.addVertex(center);
        face.addIndex(i);
        face.addColor(color);
        i++;
    }
    
    i = 0;
    
    for ( float a = 0; a < 360.0; a+= 15 ){
        ticks.addVertex( getCircleVec(a, radius * .9) );
        ticks.addIndex(i);
        ticks.addColor( ofFloatColor::black );
        i++;
        
        float mult = .85;
        if ( (int) a % 90 == 0 ) {
            mult = .7;
        }
        
        ticks.addVertex( getCircleVec(a, radius * mult) );
        ticks.addIndex(i);
        ticks.addColor( ofFloatColor::black );
        i++;
        
    }
    
    colorIndex = face.getNumVertices()-1;
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
        colorAngles.clear();
        targetAngle.clear();
        for (int i=0; i<numFaces; i++){
            angles.push_back(0);
            angles.push_back(90);
            
            targetAngle.push_back(0);
            targetAngle.push_back(90);
            
            colorAngles.push_back(0);
            colorAngles.push_back(0);
        }
    }
}

//--------------------------------------------------------------
void Clock::setColor( ofColor c ){
    ofFloatColor color(c.r/255.0, c.g/255.0, c.b/255.0, c.a/255.0f);
    
    for ( int i=0; i<face.getNumColors(); i+=3){
        face.setColor(i, color);
        face.setColor(i + 1, color);
        face.setColor(i + 2, ofColor(255));
    }
    
    /*for (int i=colorIndex; i>colorIndex-colorIncrement; i--){
        face.setColor(i, color);
    }
    colorIndex -= colorIncrement;
    if ( colorIndex < colorIncrement - 1 ){
        colorIndex = face.getNumVertices()-1;
    }*/
}

//--------------------------------------------------------------
void Clock::setColor( ofColor facec, ofColor letterc, int startAngle, int endAngle, bool bGradient ){
    ofFloatColor colorF(facec.r/255.0, facec.g/255.0, facec.b/255.0, facec.a/255.0f);
    ofFloatColor colorFB(colorF);
    colorFB.setHue(colorFB.getHue() + (bGradient ? .1 : .0));
    
    ofFloatColor colorL(letterc.r/255.0, letterc.g/255.0, letterc.b/255.0, letterc.a/255.0f);
    ofFloatColor colorLB(colorL);
    //colorLB.setHue(colorLB.getHue() + (bGradient ? .1 : .0));
    
    int sAngleWrap = ofWrap(startAngle, 0, 450);
    int eAngleWrap = ofWrap(endAngle, 0, 450);
    
    float res = 100;
    float inc = (float) 360.0 / res;
    int indA = sAngleWrap / inc;
    int indB = eAngleWrap / inc;
    
    bool bBackwards = false;
    
    if ( indA > indB ){
        if (indB == 0 && indA != 0 ){
            indB = 360.0 / inc;
        } else {
            bBackwards = true;
        }
    }
//    if ( bDoColor ) cout << indA << ":" << indB << endl;
    
    if ( !bBackwards ){
        for (int i=0; i<indA * 3; i += 3){
            face.setColor(i, colorF);
            face.setColor(i + 1, colorF);
            face.setColor(i + 2, colorFB);
        }
        
        for (int i=indA * 3; i<indB * 3; i++){
            face.setColor(i, colorL);
            face.setColor(i + 1, colorL);
            face.setColor(i + 2, colorLB);
        }
        
        for (int i=indB * 3; i<face.getNumVertices(); i += 3){
            face.setColor(i, colorF);
            face.setColor(i + 1, colorF);
            face.setColor(i + 2, colorFB);
        }
    } else {
        for (int i=0; i<indB * 3; i += 3){
            face.setColor(i, colorL);
            face.setColor(i + 1, colorL);
            face.setColor(i + 2, colorLB);
        }
        
        for (int i=indB * 3; i<indA * 3; i++){
            face.setColor(i, colorF);
            face.setColor(i + 1, colorF);
            face.setColor(i + 2, colorFB);
        }
        
        for (int i=indA * 3; i<face.getNumVertices(); i += 3){
            face.setColor(i, colorL);
            face.setColor(i + 1, colorL);
            face.setColor(i + 2, colorLB);
        }
    }
}

//--------------------------------------------------------------
void Clock::draw(){
    ofColor faceColorGood(liveFaceColor);
    
    switch ( colorMode ){
        case 0:
            liveFaceColor.setHue(ofWrap(ofWrap(offset, 0, 255) + ofMap(x + y, 0, ofGetWidth() + ofGetHeight(), 0, 120), 0, 255));
            break;
        case 1:
            liveFaceColor.setHue(ofWrap(ofWrap(offset, 0, 255) + ofMap(y, 0, ofGetHeight(), 0, 120), 0, 255));
            break;
            
        case 2:
            liveFaceColor.setHue(ofWrap(ofWrap(offset, 0, 255) + ofMap(sin(x), -1.0, 1.0, 0, 255), 0, 255));
            break;
            
        case 3:
            liveFaceColor.setHue(ofWrap(ofWrap(offset, 0, 255) + ofMap(tan(x + y), -1.0, 1.0, 0, 255), 0, 255));
            break;
            
        case 4:
            liveFaceColor.setHue(ofWrap(ofWrap(offset, 0, 255) + ofMap(y * 10.0 + ofWrap(ofGetElapsedTimeMillis(), 0, ofGetHeight() * 50.0), 0.0, ofGetHeight() * 50.0, 0, 255), 0, 255));
            break;
        case 5:
            //liveFaceColor.setHue( ofWrap(liveFaceColor.getHue() + ofWrap(offset, 0, 255), 0, 255) );
            break;
    }
    if ( colorMode != 5 ){
        liveFaceColor.setSaturation(150);
        liveFaceColor.setBrightness(250);
    }
    
    ofPushMatrix();
    ofTranslate(x,y);
    for ( int i=numFaces-1; i>=0; i--){
        float r = (float) radius / numFaces;
        r *= (i+1);
        
        float cr = (float) liveRadius / numFaces;
        cr *= (i+1);
        
        float mult = numFaces == 1 ? 1.0 : ofMap(i, numFaces-1, 0, 2.0, 1.0);
        ofColor lc(liveFaceColor);
        float hue = ofWrap(lc.getHue() * mult + sin( ofGetElapsedTimeMillis() * .0001) * 10.0, 0, 255.0f);
        lc.setHue( hue );
        
        if ( !bColorLetterFace ) setColor(liveFaceColor);
        else {
            setColor(liveFaceColor, liveLetterColor, colorAngles[i * 2], colorAngles[i * 2 + 1], !bLetter);
        }
        
        face.draw();
        
        ticks.draw();
        
//        if ( bColorFace ){
//            ofSetColor(liveLetterColor);
//        } else {
//            ofSetColor(lc);
//        }
        
        // ticks
        
        // ease angles
        angles[i * 2]       = angles[i * 2] * .9 + targetAngle[i * 2] * .1;
        angles[i * 2 + 1]   = angles[i * 2 + 1] * .9 + targetAngle[i * 2 + 1] * .1;
        
        ofPushMatrix();
        ofVec2f o1(0, - lineWidth/2.0);
        ofVec2f o2(0, + lineWidth/2.0);
        ofVec2f p1(cr  * .9, 0 - lineWidth/2.0);
        ofVec2f p2(cr  * .9, 0 + lineWidth/2.0);
        //o1.rotate(angles[i * 2], ofVec2f(0,0));
        //o2.rotate(angles[i * 2], ofVec2f(0,0));
        //p1.rotate(angles[i * 2], ofVec2f(0,0));
        //p2.rotate(angles[i * 2], ofVec2f(0,0));
        
        
        ofFloatColor fc;
        fc.r = liveArmColor.r/255.0f;
        fc.g = liveArmColor.g/255.0f;
        fc.b = liveArmColor.b/255.0f;
        
        arm1.clear();
        arm1.addVertex(o1); arm1.addVertex(o2); arm1.addVertex(p1); arm1.addVertex(p2);
        arm1.addColor(fc); arm1.addColor(fc); arm1.addColor(fc); arm1.addColor(fc);
        arm1.addIndex(0); arm1.addIndex(1); arm1.addIndex(2);
        arm1.addIndex(2); arm1.addIndex(3); arm1.addIndex(1);
        
        ofPushMatrix();
        ofRotateZ(angles[i * 2]);
        arm1.draw();
        ofPopMatrix();
        //ofLine(ofVec2f(0,0),p1);
        ofPopMatrix();
        
        hue = ofWrap(lc.getHue() * mult + sin( ofGetElapsedTimeMillis() * .0001) * 20.0, 0, 255.0f);
        lc.setHue( hue );
        
//        if ( bColorFace ){
//            ofSetColor(liveLetterColor);
//        } else {
//            ofSetColor(lc);
//        }
        
        ofPushMatrix();
        o1.set(0,-lineWidth/2.0);
        o2.set(0,+lineWidth/2.0);
        p1.set(cr  * .9, -lineWidth/2.0);
        p2.set(cr  * .9, +lineWidth/2.0);
        
//        o1.rotate(angles[i * 2 + 1], ofVec2f(0,0));
//        o2.rotate(angles[i * 2 + 1], ofVec2f(0,0));
//        p1.rotate(angles[i * 2 + 1], ofVec2f(0,0));
//        p2.rotate(angles[i * 2 + 1], ofVec2f(0,0));
//        ofLine(ofVec2f(0,0),p2);
        
        arm2.clear();
        arm2.addVertex(o1); arm2.addVertex(o2); arm2.addVertex(p1); arm2.addVertex(p2);
        arm2.addColor(fc); arm2.addColor(fc); arm2.addColor(fc); arm2.addColor(fc);
        arm2.addIndex(0); arm2.addIndex(1); arm2.addIndex(2);
        arm2.addIndex(2); arm2.addIndex(3); arm2.addIndex(1);
        
        ofPushMatrix();
        ofRotateZ(angles[i * 2 + 1]);
        arm2.draw();
        ofPopMatrix();
        //ofLine(ofVec2f(0,0),p1);
        ofPopMatrix();
        
        ofPopMatrix();
    }
    ofPopMatrix();
    
    // animate
    if ( ofGetElapsedTimeMillis() - lastFroze > 1500 && bAnimating ){
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
        b = false;
        
        //if ( colorAngles[0] != colorAngles[1] ){
        float val = ofMap((ofGetElapsedTimeMillis() - lastFroze)-2000, 0, 1500, 0, 1.0, true);
        if (!bColorLetterFace) liveLetterColor.lerp(armColor, val);
    
        liveArmColor.lerp(armColor, val);
    
        //else liveLetterColor.lerp(faceColor, val);
        if (val >= 1.0 ){
            for ( auto & a : colorAngles ){
                a = 0;
            }
        }
        //}
        
        liveFaceColor.lerp(faceColor, val);
        
        bLetter = false;
    } else if ( bLetter ) {
        // hax
        if ( colorAngles[0] != colorAngles[1] ){
            if ( ofGetElapsedTimeMillis() - lastFroze > 500 ){
                float v = ofMap((ofGetElapsedTimeMillis() - lastFroze), 500, 1000, 0, 1.0, true);
                liveLetterColor.lerp(faceColor, v);
            } else {
                liveLetterColor.lerp(letterColor, ofMap((ofGetElapsedTimeMillis() - lastFroze), 0, 500, 0, 1.0, true));
            }
        }
    }
    if ( ofGetElapsedTimeMillis() - lastFroze > 2000 ){
        vel.x = vel.x * .9 + 1 * .1;
        offset = offset * .9;
    }
    
    // reset face color
    //if ( colorMode == 5 ) liveFaceColor = faceColorGood;
}

//--------------------------------------------------------------
void Clock::rotateClockTo ( float angleA, float angleB ){
    bool b = false;
    for ( auto & a : targetAngle ){
        if ( b ){
            float an = angleA;
            while ( an < a ){
                an += 360;
            }
            a = an;
        } else {
            float an = angleB;
            while ( an < a ){
                an += 360;
            }
            a = an;
        }
        b = !b;
    }
}

//--------------------------------------------------------------
void Clock::rotateColorTo ( float angleA, float angleB ){
    bool b = false;
    for ( auto & a : colorAngles ){
        if ( b ){
            a = angleA;
        } else {
            a = angleB;
        }
        b = !b;
    }
}

//--------------------------------------------------------------
void Clock::rotateClockBy ( float angleA, float angleB ){
    bool b = false;
    for ( auto & a : targetAngle ){
        if ( b ){
            a += angleA;
        } else {
            a += angleB;
        }
        b = !b;
    }
}

//--------------------------------------------------------------
void Clock::reset(){
    bool b = false;
    for ( auto & a : angles ){
        if ( b ){
            a = 0;// - 45;
        } else {
            a = 90;// - 45;
        }
        b = !b;
    }
    for ( auto & a : targetAngle ){
        if ( b ){
            a = 0;// - 45;
        } else {
            a = 90;// - 45;
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
        bDoColor = ofGetKeyPressed( OF_KEY_SHIFT );
//        if( bDoColor ) faceColor.set(255,0,0);
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
    if ( rotMode > 9 * 10 ) rotMode = 0;
    
    int a1 = floor(rotMode / 9) * 45;
    int a2 = (rotMode % 9) * 45;
    //        a2 = (rotMode % 9) * 45;// + floor(rotMode / 9) * 45;
    
    bool b = false;
    if ( bDoColor ){
        for ( auto & a : colorAngles ){
            if ( b ){
                a = a1;// + 45;
            } else {
                a = a2;// + 45;
            }
            b = !b;
        }
    } else {
        for ( auto & a : targetAngle ){
            if ( b ){
                a = a1;// - 45;
            } else {
                a = a2;// - 45;
            }
            b = !b;
        }
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
                //a += 45;
            } else if ( an > 60 && an <= 120 ){
                a = 180;
                if ( b ){
                    a += 180;
                }
                //a += 45;
            } else if ( an > 120 && an <= 150 ){
                a = 270;
                if ( b ){
                    a += 90;
                }
                //a += 45;
                
            } else if ( an > 150 && an <= 210 ){
                a = 270;
                if ( b ){
                    a += 180;
                }
                //a += 45;
            } else if ( an > 210 && an <= 240 ){
                a = 360;
                if ( b ){
                    a += 90;
                }
                //a += 45;
            } else if ( an > 240 && an <= 300 ){
                a = 360;
                if ( b ){
                    a += 180;
                }
                //a += 45;
            } else if ( an > 300 && an <= 330 ){
                a = 450;
                if ( b ){
                    a += 90;
                }
                //a += 45;
            } else if ( (an > 330 && an <= 390)){// || (an >=0 && an < 30) ){
                a = 450;
                if ( b ){
                    a += 180;
                }
                //a += 45;
            }
            
            
            b = !b;
        }
    }
    
}

//--------------------------------------------------------------
float Clock::pointTo( int mx, int my ){
    ofVec2f m(mx,my);
    float dist = distance(m);
    m -= *this;
    
    //if ( (dist) < 20000 ){
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
    return dist;
}

//--------------------------------------------------------------
void Clock::magnet( int mx, int my, ofColor color, int freezeTime ){
    float dist = pointTo(mx, my);
    
    if ( fabs(dist) < radius * 2 ){
        if ( ofGetElapsedTimeMillis() - lastFroze <= 1500 ){
            liveFaceColor = liveFaceColor * .5 + color * .5;
            cout << "mixing colors!" << endl;
        } else {
            liveFaceColor.set(color);
        }
        liveArmColor.set(255);
    }
    lastFroze = ofGetElapsedTimeMillis() - ofMap(dist, 0, 1000, freezeTime, 0, true);
}

#pragma mark clocks

//--------------------------------------------------------------
Clocks::Clocks(){
    currentAngleA = 0;
    currentAngleB = 0;
    currentClock = 0;
    numFaces = 1;
    mode = 0;
    bColorFace = true;
    bPreciseDraw = false;
    lineWidth = 1.0;
    hueVariance = 0;
    guiVisible = false;
    bColorTextSeparately = false;
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
    setup(gridX, gridY, ofVec2f(spacing, spacing), startX, startY, radius, radiusMult);
}

//--------------------------------------------------------------
void Clocks::setup( int gridX, int gridY, ofVec3f spacing, int startX, int startY, int radius, float radiusMult ){
    for (int x=0; x<gridX; x++){
        float yInc = startY;
        for (int y=0; y<gridY; y++){
            clocks.push_back(Clock());
            clocks.back().radius = radius;
            clocks.back().liveRadius =  radius * radiusMult;
            clocks.back().x = startX + x * (spacing.x);
            clocks.back().y = yInc;
            yInc = clocks.back().y + (y >= 4 ? spacing.z : spacing.y);
        }
    }
    
    loadLetters();
    
    for ( auto & c : clocks ){
        c.setup();
    }
    
    setupGui();
    
    ofAddListener(ofEvents().update, this, &Clocks::update);
    ofAddListener(ofEvents().keyPressed, this, &Clocks::keyPressed);
    ofAddListener(ofEvents().mouseDragged, this, &Clocks::mouseDragged);
    ofAddListener(ofEvents().mousePressed, this, &Clocks::mousePressed);
    ofAddListener(ofEvents().mouseReleased, this, &Clocks::mouseReleased);
}

//--------------------------------------------------------------
void Clocks::update( ofEventArgs & e ){
    int i = 0;
    for ( auto & c : clocks ){
        c.numFaces = numFaces;
        c.colorMode = mode;
        c.lineWidth = lineWidth;
        c.bColorLetterFace = bColorTextSeparately;
        if ( i == currentClock ){
            if ( ofGetKeyPressed( OF_KEY_CONTROL ) && ofGetKeyPressed(OF_KEY_SHIFT) ){
                c.rotateClockTo(currentAngleA, currentAngleB);
            } else if ( ofGetKeyPressed( OF_KEY_COMMAND ) ){
                c.rotateColorTo(currentAngleA, currentAngleB);
            }
        }
        c.update();
        i++;
    }
}

//--------------------------------------------------------------
void Clocks::draw(){
    ofPushStyle();
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
    gui->addSlider("lineWidth", 1.0, 30.0, &lineWidth);
    gui->addSlider("hueVariance", 0.0, 255., &hueVariance);
    
    gui2 = new ofxUICanvas(ofGetWidth() / 2.0,0,ofGetWidth() / 4.0, ofGetHeight() );
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
    gui2->addToggle("Color text area separately", &bColorTextSeparately);
    
    gui2->addLabel("Edit angle");
    gui2->addIntSlider("Which Clock", 0, 100, &currentClock);
    gui2->addIntSlider("currentAngleA", 0, 360, &currentAngleA);
    gui2->addIntSlider("currentAngleB", 0, 360, &currentAngleB);
    
    gui->loadSettings("gui-settings.xml");
    gui2->loadSettings("gui2-settings.xml");
}

//--------------------------------------------------------------
void Clocks::onGui( ofxUIEventArgs &e ){
    if ( e.getName() == "color"){
        ofxUIImageSampler * s = (ofxUIImageSampler*) e.widget;
        ofxUISlider * b = (ofxUISlider*) gui->getWidget("Face brightness");
        ofxUISlider * sat = (ofxUISlider*) gui->getWidget("Face saturation");
        ofColor c(s->getColor());
        c.setSaturation(sat->getScaledValue());
        c.setBrightness(b->getScaledValue());
        
        setFaceColors(c, hueVariance);
        faceColor.set(s->getColor());
        
    } else if ( e.getName() == "letterColor"){
        ofxUIImageSampler * s = (ofxUIImageSampler*) e.widget;
        setLetterColors(s->getColor(), hueVariance);
        letterColor.set(s->getColor());
        
    } else if ( e.getName() == "arm color"){
        ofxUIImageSampler * s = (ofxUIImageSampler*) e.widget;
        
        ofxUISlider * b = (ofxUISlider*) gui2->getWidget("arm saturation");
        ofxUISlider * sat = (ofxUISlider*) gui2->getWidget("arm brightness");
        ofColor c(s->getColor());
        c.setSaturation(sat->getScaledValue());
        c.setBrightness(b->getScaledValue());
        
        cout << "arm? "<<c<<endl;
        
        setFaceColors(c, hueVariance, true);
        armColor.set(s->getColor());
        
    } else if ( e.getName() == "letterArmColor" ){
        ofxUIImageSampler * s = (ofxUIImageSampler*) e.widget;
        setFaceColors(s->getColor(), hueVariance, true);
    } else if ( e.getName() == "Face brightness" ){
        for ( auto & c : clocks ){
            c.faceColor.setBrightness(e.getSlider()->getScaledValue());
        }
    } else if ( e.getName() == "Face saturation" ){
        for ( auto & c : clocks ){
            c.faceColor.setSaturation(e.getSlider()->getScaledValue());
        }
    } else if ( e.getName() == "arm brightness" ){
        for ( auto & c : clocks ){
            c.armColor.setBrightness(e.getSlider()->getScaledValue());
        }
    } else if ( e.getName() == "arm saturation" ){
        
        for ( auto & c : clocks ){
            c.armColor.setSaturation(e.getSlider()->getScaledValue());
        }
    } else if ( e.getName() == "letter arm brightness" ){
        
        ofxUIImageSampler * s = (ofxUIImageSampler*) gui2->getWidget("letterArmColor");
        ofColor c = s->getColor();
        c.setBrightness(e.getSlider()->getScaledValue());
        setFaceColors(s->getColor(), hueVariance, true);
        
    } else if ( e.getName() == "letter arm saturation" ){
        
        ofxUIImageSampler * s = (ofxUIImageSampler*) gui2->getWidget("letterArmColor");
        ofColor c = s->getColor();
        c.setSaturation(e.getSlider()->getScaledValue());
        setFaceColors(s->getColor(), hueVariance, true);
    }
}

// METHODS
//--------------------------------------------------------------
void Clocks::setClocks( Letter letter, int offsetX, int offsetY, int letterWidth, float colorWeight ){
    ofColor color = (letterColor * colorWeight) + (faceColor * (1.0 - colorWeight));
    
    if ( offsetX >= 10 ) return;
    if ( offsetX + letterWidth >= 10 ) letterWidth = 10 - offsetX;
    for (int x=offsetX; x<offsetX + letterWidth; x++){
        for (int y=offsetY; y<offsetY + clockLetterHeight; y++){
            if ( x >= 0 ){
                int ind = (y - offsetY) + (x - offsetX) * clockLetterHeight;
                int cind = y + x * 10.0;
                clocks[cind].letterColor.set(color);
                clocks[cind].rotateClockTo( letter.angles[ind][0], letter.angles[ind][1]);
                //clocks[cind].setColor( clocks[cind].faceColor, clocks[cind].letterColor, letter.colorAngles[ind][0], letter.colorAngles[ind][1] );
                clocks[cind].rotateColorTo( letter.colorAngles[ind][0], letter.colorAngles[ind][1]);
                clocks[cind].lastFroze = ofGetElapsedTimeMillis();
                clocks[cind].bLetter = true;
            }
        }
    }
}
//--------------------------------------------------------------
void Clocks::setLetterColors( ofColor color, int hueVariance ){
    ofColor editColor;
    for ( auto & c : clocks ){
        c.letterColor.set(color);
        c.letterColor.setHue( ofWrap(color.getHue() + ofMap(c.y, 0, 700.0, 0, hueVariance), 0, 255));
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
    
    vector<float> ca1Vec;
    vector<float> ca2Vec;
    
    int ind = 0;
    
    for ( string s : alphabet ){
        if ( settings.load("letters/" + s +".xml")){
            letters[s] = Letter();
            
            
            for ( int i=0; i<settings.getNumTags("clock"); i++){
                settings.pushTag("clock", i );
                float a1 = settings.getValue("a1", 0.0) + (ind < 3 ? 45 : 0);
                float a2 = settings.getValue("a2", 0.0) + (ind < 3 ? 45 : 0);
                letters[s].angles.push_back(vector<float>());
                letters[s].angles.back().push_back(a1);
                letters[s].angles.back().push_back(a2);
                
                //if ( s == "a" ){
                    float ca1 = settings.getValue("ca1", 0.0);
                    float ca2 = settings.getValue("ca2", 0.0);
                    letters[s].colorAngles.push_back(vector<float>());
                    if ( ind > 2 ){
                        swap(ca1, ca2);
                    }
                    letters[s].colorAngles.back().push_back(ca1);
                    letters[s].colorAngles.back().push_back(ca2);
                    ca1Vec.push_back(ca1);
                    ca2Vec.push_back(ca2);
                /*} else {
                    letters[s].colorAngles.push_back(vector<float>());
                    if ( ca1Vec.size() > i ){
                        letters[s].colorAngles.back().push_back(ca1Vec[i]);
                        letters[s].colorAngles.back().push_back(ca2Vec[i]);
                    } else {
                        letters[s].colorAngles.back().push_back(ca1Vec[ca1Vec.size()-1]);
                        letters[s].colorAngles.back().push_back(ca2Vec[ca1Vec.size()-1]);
                    }
                }*/
                
                settings.popTag();
            }
        } else {
            cout <<"beef: "<< s << endl;
        }
        ind++;
    }
}


//--------------------------------------------------------------
void Clocks::magnet( float x, float y, ofColor color, int freezeTime ){
    for ( auto & c : clocks ){
        c.magnet(x,y,color, freezeTime);
    }
}

//--------------------------------------------------------------
void Clocks::pointTo( float x, float y ){
    for ( auto & c : clocks ){
        c.pointTo(x,y);
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
        char k = key;
        string ks = ofToString(k);
        int w = (ks == "w" || ks == "m")? 6 : 4;
        int h = clockLetterHeight;
        
        //int ind = y + x * clockLetterHeight;
        
        for (int x=0; x<w; x++){
            for (int y=0; y<h; y++){
                int cind = y + x * 10.0;
                Clock c = clocks[ cind ];
                settings.addTag("clock");
                settings.pushTag("clock", settings.getNumTags("clock") - 1 );
                settings.addValue("a1", c.angles[0]);
                settings.addValue("a2", c.angles[1]);
                settings.addValue("ca1", c.colorAngles[0]);
                settings.addValue("ca2", c.colorAngles[1]);
                settings.addValue("face", c.bColorFace);
                settings.popTag();
            }
        }
        settings.save("letters/" + ks + ".xml");
    } else if ( ofGetKeyPressed( OF_KEY_CONTROL ) ){
        
        char k = key;
        string ks = ofToString(k);
        if ( letters.count(ks) > 0){
            setClocks( letters[ks], 0, 0, (ks == "w" || ks == "m")? 6 : 4 );
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
    } else if ( key == 'g' ){
        gui->toggleVisible();
        
    } else if ( key == 'G' ){
        gui2->toggleVisible();
    } else if ( key == 's' ){
        gui->saveSettings("gui-settings.xml");
        gui2->saveSettings("gui2-settings.xml");
    }
    guiVisible = gui->isVisible();
}

//--------------------------------------------------------------
void Clocks::mouseDragged(ofMouseEventArgs & e ){
    if ( !bPreciseDraw ) return;
    int x = e.x;
    int y = e.y;
    int button = e.button;
    
    if ( guiVisible ) return;
    for ( auto & c : clocks ){
        //        if ( c.bAnimating )
        if ( !bPreciseDraw ) c.magnet(x,y,ofColor(255));
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

