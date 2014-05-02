//
//  CircleTransition.h
//  CircleTransitions
//
//  Created by Brett Renfer on 3/18/14.
//
//

// Requires: https://github.com/satoruhiga/ofxEasingFunc.git

#pragma once

#include "ofMain.h"
#include "ofxEasingFunc.h"

class Transition {
public:
    
    virtual void setup(){
        _id = ofRandom(10000);
        ofAddListener(ofEvents().update, this, &Transition::update);
        easing = new ofxEasingFunc::Linear();
    }
    
    virtual int begin( float duration = 1.0, float delay = 0.0, bool back = false, bool bForce = false ){
        if ( !bForce && bRunning ) return;
        timeStarted = ofGetElapsedTimeMillis() + delay * 1000.0;
        timeEnd     = timeStarted + ( duration * 1000.0 );
        
        bRunning    = true;
        bBackwards  = back;
        position    = bBackwards ? 1.0 : 0.0;
        
        return _id;
    }
    
    virtual void stop(){
        // update position then stop
        time = ofMap(ofGetElapsedTimeMillis(), timeStarted, timeEnd, bBackwards ? 1.0 : 0.0, bBackwards ? 0.0 : 1.0, true);
        position = easing->easeOut(time);
        bRunning = false;
    }
    
    virtual void update( ofEventArgs & e ){
        if ( !bRunning ) return;
        
        time = ofMap(ofGetElapsedTimeMillis(), timeStarted, timeEnd, bBackwards ? 1.0 : 0.0, bBackwards ? 0.0 : 1.0, true);
        position = easing->easeInOut(time);
        if ( ofGetElapsedTimeMillis() >= timeEnd ){
            bRunning = false;
            ofNotifyEvent(transitionFinished, _id, this);
        }
    }
    
    virtual void draw() = 0;
    
    ofEvent<int> transitionFinished;
    
    int getId(){
        return _id;
    }
    
protected:
    
    int  _id;
    
    bool bRunning;
    bool bBackwards;
    
    int timeStarted, timeEnd;
    float position, time;
    ofxEasingFunc::Easing * easing;
};


/**
 * @class
 */
class CircleTransition : public Transition {
public:
    
    CircleTransition(){
        radius = 0;
        color.r = ofRandom(1.0);
        color.g = ofRandom(1.0);
        color.b = ofRandom(1.0);
    }
    
    virtual void setup(){
        Transition::setup();
        // this is a leak
//        delete easing;
        easing = new ofxEasingFunc::Quint();
        
        if ( radius == 0 ) radius = 100;
        
        // build mesh
        buildCircle(circleMesh);
        position = 1.0;
    }
    
    void buildCircle( ofMesh & m ){
        ofPoint c(0, 0 );
        float div = 360.0 / 100.0;
        for (float a=0; a<360.0; a+=div){
            ofVec3f v;
            v.x = cos(a * PI / 180.0) * radius;
            v.y = -sin(a * PI / 180.0) * radius;
            m.addVertex(v);
            
            v.x = cos((a + div) * PI / 180.0) * radius;
            v.y = -sin((a + div) * PI / 180.0) * radius;
            m.addVertex(v);
            
            m.addVertex(c);
        }
        
        // add colors to mesh
        for ( int i=0; i<m.getNumVertices(); i+=3){
            m.addColor(ofFloatColor(1.0));
            m.addColor(ofFloatColor(1.0));
            m.addColor(ofFloatColor(1.0));
            m.addIndex(i);
            m.addIndex(i+1);
            m.addIndex(i+2);
        }
    }
    
    int radius;
    ofFloatColor color;
    
protected:
    ofMesh circleMesh;
    ofPath path;
};

class Radial : public CircleTransition {
public:
    void draw(){
        int n =circleMesh.getNumIndices()-1;
        int i=0;
        for (i; i<n * position; i+=3){
            circleMesh.setColor(circleMesh.getIndex(i), ofFloatColor(color.r,color.g, color.b, 1.0));
            circleMesh.setColor(circleMesh.getIndex(i + 1), ofFloatColor(color.r,color.g, color.b, 1.0));
            circleMesh.setColor(circleMesh.getIndex(i + 2), ofFloatColor(color.r,color.g, color.b, 1.0));
            //            circleMesh.setColor(i + 2, ofFloatColor(1.0, 1.0));
        }
        for (i; i<n; i+=3){
            circleMesh.setColor(circleMesh.getIndex(i), ofFloatColor(color.r,color.g, color.b, 0.0));
            circleMesh.setColor(circleMesh.getIndex(i + 1), ofFloatColor(color.r,color.g, color.b, 0.0));
            circleMesh.setColor(circleMesh.getIndex(i + 2), ofFloatColor(color.r,color.g, color.b, 0.0));
            //            circleMesh.setColor(i + 2, ofFloatColor(1.0, 1.0));
        }
        circleMesh.draw();
    }

};

class Grow : public CircleTransition {
public:
    void draw(){
        int n =circleMesh.getNumIndices();
        int i=0;
        for (i; i<n; i+=3){
            circleMesh.setColor(circleMesh.getIndex(i), ofFloatColor(color.r,color.g, color.b, 1.0));
            circleMesh.setColor(circleMesh.getIndex(i + 1), ofFloatColor(color.r,color.g, color.b, 1.0));
            circleMesh.setColor(circleMesh.getIndex(i + 2), ofFloatColor(color.r,color.g, color.b, 1.0));
        }
        ofPushMatrix();
        ofScale(position,position);
        circleMesh.draw();
        ofPopMatrix();
    }
    
};

class Flip : public CircleTransition {
public:
    void draw(){
        int n =circleMesh.getNumIndices();
        int i=0;
        for (i; i<n; i+=3){
            circleMesh.setColor(circleMesh.getIndex(i), ofFloatColor(color.r,color.g, color.b, 1.0));
            circleMesh.setColor(circleMesh.getIndex(i + 1), ofFloatColor(color.r,color.g, color.b, 1.0));
            circleMesh.setColor(circleMesh.getIndex(i + 2), ofFloatColor(color.r,color.g, color.b, 1.0));
        }
        ofPushMatrix();
        ofScale(position,1.0);
        circleMesh.draw();
        ofPopMatrix();
    }
    
};

class Moon : public CircleTransition {
public:
    virtual void setup(){
        CircleTransition::setup();
        fbo.allocate(radius * 2, radius*2, GL_RGBA, 8);
        buildCircle(frontMesh);
    }
    void draw(){
        ofPushStyle();
        ofDisableDepthTest();
        int n =circleMesh.getNumIndices();
        int i=0;
        for (i; i<n; i+=3){
            circleMesh.setColor(circleMesh.getIndex(i), ofFloatColor(color.r,color.g, color.b, 1.0));
            circleMesh.setColor(circleMesh.getIndex(i + 1), ofFloatColor(color.r,color.g, color.b, 1.0));
            circleMesh.setColor(circleMesh.getIndex(i + 2), ofFloatColor(color.r,color.g, color.b, 1.0));
        }
        
        i=0;
        for (i; i<n; i+=3){
            frontMesh.setColor(frontMesh.getIndex(i), ofFloatColor(color.r,color.g, color.b, position));
            frontMesh.setColor(frontMesh.getIndex(i + 1), ofFloatColor(color.r,color.g, color.b, position));
            frontMesh.setColor(frontMesh.getIndex(i + 2), ofFloatColor(color.r,color.g, color.b, position));
        }
//        circleMesh.draw();
        fbo.begin();
        ofClear(255,0);
        ofPushMatrix();
        ofTranslate(radius + (radius * 2.0) * (1.0 -position), radius);
        frontMesh.draw();
        ofPopMatrix();
        ofPopStyle();
        fbo.end();
        fbo.draw(0,0);
    }

protected:
    ofMesh frontMesh;
    ofFbo fbo;
    
};
