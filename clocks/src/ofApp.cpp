#include "ofApp.h"

vector<Clock> clocks;

int numFaces = 1;
int mode = 0;
bool bColorFace = true;
bool bPreciseDraw = false;
float lineWidth = 1.0;
float hueVariance = 0;
ofColor dum;

//--------------------------------------------------------------
void ofApp::setup(){
    ofBackground(0);
    ofEnableSmoothing();
    ofEnableAlphaBlending();
    for (int x=0; x<10; x++){
        for (int y=0; y<10; y++){
            clocks.push_back(Clock());
            clocks.back().radius = 25;
            clocks.back().liveRadius =  23;
            clocks.back().x = 100 + x * 60;
            clocks.back().y = 100 + y * 60;
        }
    }
    
    // setup gui
    colorImage.loadImage("spectrum.png");
    gui = new ofxUICanvas(0,0,ofGetWidth() / 4.0, ofGetHeight() );
    gui->toggleVisible();
    
    gui->addLabel("Face color");
    gui->addImageSampler("color", &colorImage, ofGetWidth() * .2, ofGetWidth() * .2);
    gui->addSlider("Face saturation", 0, 255, 0.0);
    gui->addSlider("Face brightness", 0, 255, 0.0);
    gui->addIntSlider("numFaces", 1, 25, &numFaces);
    gui->addIntSlider("color mode", 0, 5, &mode);
    gui->addSlider("lineWidth", 1.0, 10.0, &lineWidth);
    gui->addSlider("hueVariance", 0.0, 255., &hueVariance);
    gui->loadSettings("gui-settings.xml");
    
    gui2 = new ofxUICanvas(ofGetWidth() / 4.0,0,ofGetWidth() / 4.0, ofGetHeight() );
    ofAddListener(gui->newGUIEvent, this, &ofApp::onGui );
    ofAddListener(gui2->newGUIEvent, this, &ofApp::onGui );
    
    gui2->toggleVisible();
    gui2->addLabel("Arm color");
    gui2->addImageSampler("arm color", &colorImage, ofGetWidth() * .2, ofGetWidth() * .2);
    gui2->addSlider("arm saturation", 0, 255, 0.0);
    gui2->addSlider("arm brightness", 0, 255, 0.0);
    gui2->addToggle("magnet or draw", &bPreciseDraw);
    gui2->loadSettings("gui2-settings.xml");
}

//--------------------------------------------------------------
void ofApp::onGui( ofxUIEventArgs &e ){
    if ( e.getName() == "color"){
        ofxUIImageSampler * s = (ofxUIImageSampler*) e.widget;
        for ( auto & c : clocks ){
            c.faceColor = s->getColor();
            c.faceColor.setHue( ofWrap(s->getColor().getHue() + ofMap(c.y, 0, 700.0, 0, hueVariance), 0, 255));
        }
    } else if ( e.getName() == "arm color"){
        ofxUIImageSampler * s = (ofxUIImageSampler*) e.widget;
        for ( auto & c : clocks ){
            c.armColor = s->getColor();
        }
    } else if ( e.getName() == "face brightness" ){
        for ( auto & c : clocks ){
            c.faceColor.setBrightness(e.getSlider()->getScaledValue());
        }
    } else if ( e.getName() == "face saturation" ){
        for ( auto & c : clocks ){
            c.faceColor.setSaturation(e.getSlider()->getScaledValue());
        }
    }else if ( e.getName() == "arm brightness" ){
        for ( auto & c : clocks ){
            c.armColor.setBrightness(e.getSlider()->getScaledValue());
        }
    } else if ( e.getName() == "arm saturation" ){
        for ( auto & c : clocks ){
            c.armColor.setSaturation(e.getSlider()->getScaledValue());
        }
    }
}

//--------------------------------------------------------------
void ofApp::update(){
    ofSetLineWidth(lineWidth);
}

//--------------------------------------------------------------
void ofApp::draw(){
    for ( auto & c : clocks ){
        c.numFaces = numFaces;
        c.colorMode = mode;
        c.draw();
    }
    ofSetColor(255);
    ofDrawBitmapString("Num faces "+ofToString( numFaces ), 20, 20);
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
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
        ofxXmlSettings settings;
        if ( settings.load("letters/" + ofToString(key) + ".xml")){
            int ind = 0;
            for ( auto & c : clocks ){
                settings.pushTag("clock", ind );
                c.rotateClockTo(settings.getValue("a1", c.angles[0]), settings.getValue("a2", c.angles[1]) );
                //c.bColorFace = settings.getValue("face", c.bColorFace);
                settings.popTag();
                ind++;
                c.lastFroze = ofGetElapsedTimeMillis();
            }
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
        gui2->toggleVisible();
    } else if ( key == 's' ){
        gui->saveSettings("gui-settings.xml");
        gui2->saveSettings("gui2-settings.xml");
    }
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){
//    for ( auto & c : clocks ){
////        if ( c.bAnimating )
//            c.rotateTo(x,y);
//    }
}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){
    for ( auto & c : clocks ){
        //        if ( c.bAnimating )
        if ( !bPreciseDraw ) c.magnet(x,y);
        //c.onMouseDragged(x, y);
    }
    
}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){
    for ( auto & c : clocks ){
        if ( bPreciseDraw )
            c.checkHit(x,y);
    }
}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){
    
    for ( auto & c : clocks ){
        //c.onMouseReleased(x, y);
    }
    
    if (!bPreciseDraw) return;
    for ( auto & c : clocks ){
        c.checkHit(x,y, ofGetKeyPressed(OF_KEY_CONTROL));
    }
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
