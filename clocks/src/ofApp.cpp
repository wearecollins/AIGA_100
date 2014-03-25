#include "ofApp.h"

vector<Clock> clocks;

int numFaces = 1;
int mode = 0;
bool bColorFace = true;

//--------------------------------------------------------------
void ofApp::setup(){
    ofBackground(0);
    ofEnableSmoothing();
    ofEnableAlphaBlending();
    for (int x=0; x<10; x++){
        for (int y=0; y<10; y++){
            clocks.push_back(Clock());
            clocks.back().radius = 25;
            clocks.back().x = 100 + x * 60;
            clocks.back().y = 100 + y * 60;
        }
    }
}

//--------------------------------------------------------------
void ofApp::update(){

}

//--------------------------------------------------------------
void ofApp::draw(){
    for ( auto & c : clocks ){
        c.numFaces = numFaces;
        c.colorMode = mode;
        c.bColorFace = bColorFace;
        c.draw();
    }
    ofDrawBitmapString("Num faces "+ofToString( numFaces ), 20, 20);
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    if ( key == 'b' ){
        bColorFace = !bColorFace;
    }
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){
    for ( auto & c : clocks ){
        c.rotateTo(x,y);
    }
}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){
    numFaces = ofMap(x, 0, ofGetWidth(), 1, 50, true);
    mode = ofMap(y, 0, ofGetHeight(), 0, 5, true);
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
