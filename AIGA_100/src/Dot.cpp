//
//  Dot.cpp
//  AIGA_Dots
//
//  Created by Brett Renfer on 3/17/14.
//
//

#include "Dot.h"

//--------------------------------------------------------------
void Dot::setup(){
    
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
    bOver = false;
    width = dotWidth;
    
    ofCylinderPrimitive prim = ofCylinderPrimitive(width / 2.0, 3.0, 30, 10);
    
    // setup mesh
    mesh = ofMesh(prim.getMesh());
    mesh.setMode( OF_PRIMITIVE_TRIANGLE_STRIP );
    
    int i=0;
    ofVec2f min(10000,10000);
    ofVec2f max(-10000,-10000);
    for ( i; i<mesh.getNumVertices(); i++){
        mesh.setVertex(i, mesh.getVertex(i).rotate(90.0, 0.0, 0.0) );
        ofVec3f vert = mesh.getVertex(i);
        mesh.addColor(ofFloatColor(1.0));
        if ( vert.x < min.x ) min.x = vert.x;
        if ( vert.y < min.y ) min.y = vert.y;
        if ( vert.x > max.x ) max.x = vert.x;
        if ( vert.y > max.y ) max.y = vert.y;
    }
    
    float w = spacing * gridSize;
    float h = spacing * gridSize;
    
    // set texcoords
    for (i=0; i<mesh.getNumVertices(); i++){
        ofVec3f vert = mesh.getVertex(i);
        if ( vert.z >= 1.5 ){
            mesh.setTexCoord(i, ofVec2f( ofMap(vert.x / width, .5, -.5, x/w, x/w + .1), ofMap( ((vert.y) / width), -.5, .5, (h-y)/h - .1, (h-(y+.1))/h) ) );
        } else {
            mesh.setTexCoord(i, ofVec2f(0,0) );
        }
    }
    
    mode = MODE_INTERACTIVE_COLOR;
}
//--------------------------------------------------------------
void Dot::draw( bool bMap, bool bRender ){
    if ( lastMode != mode ){
        if (mode == MODE_COLOR ) ran = ofRandom(1.0);
        else if ( mode == MODE_INTERACTIVE_COLOR ){
        }
    }
    lastMode = mode;
    switch (mode) {
        case MODE_COLOR:
            {
                float h = (float) (ran + sin(ofGetElapsedTimeMillis() * speed));
                floatColor.setHue( ofMap(h, -1.,1.,0,1.0) );
                
                r = floatColor.r * 255;
                g = floatColor.g * 255;
                b = floatColor.b * 255;
            }
            break;
            
        case MODE_INTERACTIVE_COLOR:
            floatColor.r = floatColor.r * .9 + ( r / 255.0 ) * .1;
            floatColor.g = floatColor.g * .9 + ( g / 255.0 ) * .1;
            floatColor.b = floatColor.b * .9 + ( b / 255.0 ) * .1;
            break;
            
        case MODE_INTERACTIVE_GRID:
        case MODE_INTERACTIVE_SWIPE:
            
            floatColor.r = floatColor.r * .9 + ( r / 255.0 ) * .1;
            floatColor.g = floatColor.g * .9 + ( g / 255.0 ) * .1;
            floatColor.b = floatColor.b * .9 + ( b/ 255.0 ) * .1;
            
            r = r * .9 + 255 * .1;
            g = g * .9 + 255 * .1;
            b = b * .9 + 255 * .1;
            
            break;
        
        case MODE_INTERACTIVE_TEXT:
            if ( text == "" ){
                floatColor.r = floatColor.r * .9 + .4 * .1;
                floatColor.g = floatColor.g * .9 + .4 * .1;
                floatColor.b = floatColor.b * .9 + .4 * .1;
            } else if (text == " ") {
                floatColor.r = floatColor.r * .9 + ( r * .5 / 255.0 ) * .1;
                floatColor.g = floatColor.g * .9 + ( g * .5 / 255.0 ) * .1;
                floatColor.b = floatColor.b * .9 + ( b * .5/ 255.0 ) * .1;
            } else {
                floatColor.r = floatColor.r * .9 + ( r / 255.0 ) * .1;
                floatColor.g = floatColor.g * .9 + ( g / 255.0 ) * .1;
                floatColor.b = floatColor.b * .9 + ( b/ 255.0 ) * .1;
            }
            break;
            
        default:
            break;
    }
    
    for (int i=0; i<mesh.getNumVertices(); i++){
//        if (mesh.getVertex(i).z == 1.5 ){
            if ( mode != MODE_INTERACTIVE_TEXT ) mesh.setColor(i, floatColor);
            else mesh.setColor(i, ofFloatColor(255) );
//        }
    }
    
    ofPushMatrix();
    ofTranslate(x,y,z);
    mesh.draw();
    ofPopMatrix();
}
//--------------------------------------------------------------
void Dot::loadFont( ofTrueTypeFont & font ){
    drawFont = &font;
}
//--------------------------------------------------------------
void Dot::mousePressed( ofVec2f m ){
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
//--------------------------------------------------------------
void Dot::mouseReleased( ofVec2f m ){
    bOver = false;
}
//--------------------------------------------------------------
void Dot::clear(){
    state = 1;
}

//--------------------------------------------------------------
void Dot::setText( string letter ){
    text = ofToUpper( letter );
}
//--------------------------------------------------------------
string Dot::getText(){
    return text;
}
