#include "ofApp.h"

ofColor dum;

string name = "sean noreen charles dana ken leslie david kyle michael richard michael nancye stephen louise bob sylvia cheryl alexander chip michael abbott bill gael ann ";

//--------------------------------------------------------------
void ofApp::setup(){
    ofBackground(0);
    ofEnableSmoothing();
    ofEnableAlphaBlending();
    
    clocks.setup(10,10,60);
}

int last = 0;
int ind = 0;

//--------------------------------------------------------------
void ofApp::update(){
    if (!clocks.bPreciseDraw){
        static bool bDidZero = false;
        if (!bDidZero ){
            bDidZero = true;
            string l = name.substr(ind,1);
            int w = (l == "w" || l == "m" || l == " ")? 6 : 4;
            clocks.setClocks( clocks.letters[l], (10 - w)/2, 2, w );
        }
        
        if ( ofGetElapsedTimeMillis() - last > 1000 ){
            last = ofGetElapsedTimeMillis();
            ind++;
            if ( ind >= name.length() ){
                ind = 0;
            }
            string l = name.substr(ind,1);
            int w = (l == "w" || l == "m" || l == " ")? 6 : 4;
            clocks.setClocks( clocks.letters[l], (10 - w)/2, 2, w );
        }
    }
}

//--------------------------------------------------------------
void ofApp::draw(){
    clocks.draw();
    ofSetColor(255);
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){}

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
