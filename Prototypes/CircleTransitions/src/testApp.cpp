#include "testApp.h"

bool bBack = false;
int mode = 4;
int type = 0;

//--------------------------------------------------------------
void testApp::setup(){
    for ( int x=0; x<10;x++){
        for (int y=0; y<10; y++){
            // grow
            grow.push_back(new Grow());
            grow.back()->radius = 20;
            grow.back()->setup();
            grow.back()->color.set(1.0,1.0,1.0);
            ofAddListener(grow.back()->transitionFinished, this, &testApp::finished );
            
            growBack.push_back(new Grow());
            growBack.back()->radius = 20;
            growBack.back()->setup();
            growBack.back()->color.set(.5,.5,.5);
            
            // radial
            radial.push_back(new Radial());
            radial.back()->radius = 20;
            radial.back()->setup();
            radial.back()->color.set(1.0,1.0,1.0);
            ofAddListener(radial.back()->transitionFinished, this, &testApp::finished );
            
            radialBack.push_back(new Radial());
            radialBack.back()->radius = 20;
            radialBack.back()->setup();
            radialBack.back()->color.set(.5,.5,.5);
            
            // flip
            flip.push_back(new Moon());
            flip.back()->radius = 20;
            flip.back()->setup();
            flip.back()->color.set(1.0,1.0,1.0);
            ofAddListener(flip.back()->transitionFinished, this, &testApp::finished );
            
            flipBack.push_back(new Moon());
            flipBack.back()->radius = 20;
            flipBack.back()->setup();
            flipBack.back()->color.set(.5,.5,.5);
        }
    }
    
    ofEnableAlphaBlending();
    
    circles = &grow;
    circlesBack = &growBack;
}

//--------------------------------------------------------------
void testApp::update(){

}


void testApp::finished( int & id ){
}

//--------------------------------------------------------------
void testApp::draw(){
    ofPushMatrix(); {
        ofTranslate(100, 200);
        for ( int x=0; x<10;x++){
            for (int y=0; y<10; y++){
                ofPushMatrix(); {
                    ofTranslate(x * 50, y * 50);
                    circlesBack->at( x + y * 10 )->draw();
                    circles->at( x + y * 10 )->draw();
                }; ofPopMatrix();
            }
        }
    }; ofPopMatrix();
    ofDrawBitmapString("Press 'm' to change mode\nMode is "+ofToString(mode), 20,20);
    ofDrawBitmapString("Press 't' to change type\nType is "+ofToString(type), 20,60);
    ofDrawBitmapString("Press ' ' to trigger transition", 20,90);
}

//--------------------------------------------------------------
void testApp::keyPressed(int key){
    if ( key == ' ' ){
        ofVec2f center(ofRandom(10) * 50, ofRandom(10) * 50);
        for ( int x=0; x<10;x++){
            for (int y=0; y<10; y++){
                float dist = center.distance(ofVec2f(x * 50, y * 50));
                switch (mode){
                    case 0:
                        circles->at(x + y * 10)->begin(.3, y / 20.0, bBack, true);
                        circlesBack->at( x + y * 10)->begin(.3, y / 20.0, !bBack, true);
                        break;
                    case 1:
                        circles->at(x + y * 10)->begin(.3, x / 20.0, bBack);
                        circlesBack->at( x + y * 10)->begin(.3, x / 20.0, !bBack, true);
                        break;
                    case 2:
                        circles->at(x + y * 10)->begin(.3, x * y / 50.0, bBack, true);
                        circlesBack->at( x + y * 10)->begin(.3, x * y / 50.0, !bBack, true);
                        break;
                    case 3:
                        circles->at(x + y * 10)->begin(.3, abs(sin((y)/3.0)), bBack, true);
                        circlesBack->at( x + y * 10)->begin(.3, abs(sin((y)/3.0)), !bBack, true);
                        break;
                    case 4:
                        circles->at(x + y * 10)->begin(.3, dist / 1000.0, bBack, true);
                        circlesBack->at( x + y * 10)->begin(.3, dist / 1000.0, !bBack, true);
                        break;
                }
            }
        }
        bBack = !bBack;
    } else if ( key == 'm'){
        mode++;
        if ( mode >= 5 ) mode = 0;
    } else if ( key == 't'){
        type++;
        if ( type > 2 ) type = 0;
        switch (type){
            case 0:
                circles = &grow;
                circlesBack = &growBack;
                break;
            case 1:
                circles = &radial;
                circlesBack = &radialBack;
                break;
            case 2:
                circles = &flip;
                circlesBack = &flipBack;
                break;
        }
    }
}

//--------------------------------------------------------------
void testApp::keyReleased(int key){

}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void testApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void testApp::dragEvent(ofDragInfo dragInfo){ 

}
