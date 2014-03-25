#include "ofApp.h"

int mode = 0;
ofMesh circlePie, circleConcentric, circleVortex;
vector<ofPath> p;
ofPath circleVortex2;

//--------------------------------------------------------------
void ofApp::setup(){
    glLineWidth(3.0);
    ofEnableSmoothing();
    ofBackground(50);
    float radius = 200.;
    
    ofPoint c(0, 0 );
    float div = 360.0 / 100.0;
    for (float a=0; a<360.0; a+=div){
        ofVec3f v;
        v.x = cos(a * PI / 180.0) * radius;
        v.y = -sin(a * PI / 180.0) * radius;
        circlePie.addVertex(v);
        
        v.x = cos((a + div) * PI / 180.0) * radius;
        v.y = -sin((a + div) * PI / 180.0) * radius;
        circlePie.addVertex(v);
        
        circlePie.addVertex(c);
    }
    
    // add colors to mesh
    for ( int i=0; i<circlePie.getNumVertices(); i+=3){
        circlePie.addColor(ofFloatColor(1.0));
        circlePie.addColor(ofFloatColor(1.0));
        circlePie.addColor(ofFloatColor(1.0));
        circlePie.addIndex(i);
        circlePie.addIndex(i+1);
        circlePie.addIndex(i+2);
    }
    
    // concentric
    circleConcentric.setMode(OF_PRIMITIVE_LINES);
    for ( float r=10.0; r<=radius; r += 10.0 ){
        for (float a=0; a<360.0; a+=div){
            ofVec3f v;
            v.x = cos(a * PI / 180.0) * r;
            v.y = -sin(a * PI / 180.0) * r;
            circleConcentric.addVertex(v);
            
            v.x = cos((a + div) * PI / 180.0) * r;
            v.y = -sin((a + div) * PI / 180.0) * r;
            circleConcentric.addVertex(v);
        }
    }
    
    // add colors to mesh
    for ( int i=0; i<circleConcentric.getNumVertices(); i+=2){
        circleConcentric.addColor(ofFloatColor(1.0));
        circleConcentric.addColor(ofFloatColor(1.0));
        circleConcentric.addColor(ofFloatColor(1.0));
        circleConcentric.addIndex(i);
        circleConcentric.addIndex(i+1);
    }
    
    //vortex
    //p.setMode(ofPath::POLYLINES);
    for (float a=0; a<360.0; a+=div){
        ofVec3f v;
        v.x = cos(a * PI / 180.0) * radius;
        v.y = -sin(a * PI / 180.0) * radius;
        
        ofVec3f v2;
        v2.x = cos((a - 90) * PI / 180.0) * radius / 10.0;
        v2.y = -sin((a - 90) * PI / 180.0) * radius / 10.0;
        
        //if ( a != 0 ) p.newSubPath();
        p.push_back(ofPath());
        p.back().moveTo(v);
        p.back().lineTo(v2);
        p.back().setFilled(false);
        p.back().setStrokeWidth(1.0);
    }
    
//    circleVortex.append(p.getTessellation());
    circleVortex.setMode(OF_PRIMITIVE_LINES);
    
    circleVortex2.setFilled(false);
    circleVortex2.setStrokeWidth(1.0);
//    circleVortex2.setCurveResolution(300);
    
    for (float a=0; a<360.0f; a+=div){
        
        ofVec3f v2;
        v2.x = cos((a) * PI / 180.0) * radius / 10.0;
        v2.y = -sin((a) * PI / 180.0) * radius / 10.0;
        
        ofVec3f v3;
        v3.x = cos((a - 60) * PI / 180.0) * radius;
        v3.y = -sin((a - 60) * PI / 180.0) * radius;
        
        ofVec3f cp1;
        cp1.x = cos((a + 15) * PI / 180.0) * radius / 2.0;
        cp1.y = -sin((a + 15) * PI / 180.0) * radius / 2.0;
        
        ofVec3f cp2;
        cp2.x = cos((a + 90) * PI / 180.0) * (radius * 1.5);
        cp2.y = -sin((a + 90) * PI / 180.0) * (radius * 1.5);
        
        if ( a != 0 ) circleVortex2.newSubPath();
        
        circleVortex2.moveTo(v2);
        circleVortex2.bezierTo(cp1, cp2, v3);
    }
}

//--------------------------------------------------------------
void ofApp::update(){

}

//--------------------------------------------------------------
void ofApp::draw(){
    ofTranslate(ofGetWidth()/2.0, ofGetHeight()/2.0);
    ofRotateZ(ofGetElapsedTimef() * 10.0);
    switch (mode) {
        case 0:
            circlePie.drawWireframe();
            break;
            
        case 1:
            circleConcentric.drawWireframe();
            break;
            
        case 2:
            circleVortex2.draw();
            break;
            
        case 3:
            for ( auto & path : p ){
                path.draw();
            }
            break;
            
        default:
            break;
    }
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    mode++;
    if ( mode > 3 ) mode = 0;
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}
