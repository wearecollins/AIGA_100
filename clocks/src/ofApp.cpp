#include "ofApp.h"

ofColor dum;

string name = "sean noreen charles dana ken leslie david kyle michael richard michael nancye stephen louise bob sylvia cheryl alexander chip michael abbott bill gael ann ";

//--------------------------------------------------------------
void ofApp::setup(){
    ofSetWindowPosition(0, 0);
    ofBackground(255);
    ofEnableSmoothing();
    ofEnableAlphaBlending();
    
    clocks.setup(10,10,60, 100, 100, 25);
}

int last = 0;
int ind = 0;

//--------------------------------------------------------------
void ofApp::update(){
}

//--------------------------------------------------------------
void ofApp::draw(){
    clocks.draw();
    ofSetColor(255);
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    if ( key == 'f' )ofToggleFullscreen();
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ }
