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
    
    void setup(){
        
        int circleRes = 20;
        float circleInt = 360.0 / circleRes;
        
        floatColor.setBrightness(1.0);
        floatColor.setSaturation(1.0);
        ran = ofRandom(1.0);
        floatColor.setHue(0.0);
        
        r = floatColor.r * 255;
        g = floatColor.g * 255;
        b = floatColor.b * 255;
        
        speed = ofRandom(0.0001, .001);
        state = 1;//ofRandom(1,5);
        bOver = false;
        stateMult = 12.0;
        width = (state * stateMult);
        
        ofCylinderPrimitive prim = ofCylinderPrimitive(width, 3.0, 30, 10);
        
        // setup mesh
        mesh = ofMesh(prim.getMesh());
        mesh.setMode( OF_PRIMITIVE_TRIANGLE_STRIP );
        
        int i=0;
        for ( i; i<mesh.getNumVertices(); i++){
            mesh.setVertex(i, mesh.getVertex(i).rotate(90.0, 0.0, 0.0) );
            ofVec3f vert = mesh.getVertex(i);
            mesh.setTexCoord(i, ofVec2f( (x + vert.x)/(ofGetWidth()/2.0) ,1-(y + vert.y)/(ofGetHeight()/2.0)) );
            mesh.addColor(ofFloatColor(1.0));
        }
        
        mode = MODE_INTERACTIVE_COLOR;
    }
    
    void draw( bool bRender = true){
        if ( lastMode != mode ){
            if (mode == MODE_COLOR ) ran = ofRandom(1.0);
            else if ( mode == MODE_INTERACTIVE_COLOR ){
                floatColor.setSaturation(0.1);
                r = floatColor.r * 255;
                g = floatColor.g * 255;
                b = floatColor.b * 255;
            }
        }
        lastMode = mode;
        if ( mode == MODE_COLOR ){
            float h = (float) (ran + sin(ofGetElapsedTimeMillis() * speed));
            floatColor.setHue( ofMap(h, -1.,1.,0,1.0) );
            
            r = floatColor.r * 255;
            g = floatColor.g * 255;
            b = floatColor.b * 255;
            
        } else if ( mode == MODE_INTERACTIVE_COLOR ){
            floatColor.r = floatColor.r * .9 + ( r / 255.0 ) * .1;
            floatColor.g = floatColor.g * .9 + ( g / 255.0 ) * .1;
            floatColor.b = floatColor.b * .9 + ( b / 255.0 ) * .1;
            
//            float h = (float) sin(ofGetElapsedTimeMillis() * (speed * 10) );
//            floatColor.setHue( floatColor.getHue() + ofMap(h, -1.,1.,-.001,.001) );
        } else if ( mode== MODE_INTERACTIVE_SWIPE || mode== MODE_INTERACTIVE_GRID ) {
            
            floatColor.r = floatColor.r * .9 + ( r / 255.0 ) * .1;
            floatColor.g = floatColor.g * .9 + ( g / 255.0 ) * .1;
            floatColor.b = floatColor.b * .9 + ( b/ 255.0 ) * .1;
        }
        
        
        for (int i=0; i<mesh.getNumVertices(); i++){
            if (mesh.getVertex(i).z == 1.5 ) mesh.setColor(i, floatColor);
        }
        
        width = width * .8 + (state * stateMult) * .2;
        ofPushMatrix();
        ofTranslate(x,y,z);
        ofScale(width / stateMult, width / stateMult, width/stateMult);
        mesh.draw();
        ofPopMatrix();
    }
    
    void mousePressed( ofVec2f m ){
        if ( bOver ) return;
        
        float testW = 5 * stateMult;
        if ( m.x > x-testW/2.0 && m.x < x + testW/2.0 ){
            if ( m.y > y-testW/2.0 && m.y < y + testW/2.0 ){
                state++;
                if ( state >= 5 ){
                    state = 1;
                }
                bOver = true;
            }
        }
    }
    
    void mouseReleased( ofVec2f m ){
        bOver = false;
    }
    
    void clear(){
        state = 1;
    }
    
    int state;
    
    int r, g, b;
    
    ofFloatColor floatColor;
    int width;
    
    Mode mode, lastMode;
    
private:
    float speed;
    float ran;
    int lastState;
    float stateMult;
    bool bOver;
    
    ofMesh mesh;
    
};