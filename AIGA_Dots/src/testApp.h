#pragma once

#include "ofMain.h"
#include "Constants.h"
#include "Dot.h"
#include "ofxUI.h"
#include "ofxSpacebrew.h"

struct Drawing {
    vector<ofVec2f> points;
    int index = 0;
    int age = 0;
    int lastChanged = 0;
    ofColor color;
};

struct GridDrawing {
    vector<bool> grid;
    vector<int> indices;
    int index = 0;
    int age = 0;
    int lastChanged = 0;
    ofColor color;
};

class testApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();

		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);
    
        void onMessage( Spacebrew::Message & m );
		
        vector<Dot> dots;
        ofImage kidd;
        ofLight simpleLight;
        ofEasyCam camera;

        // quick data thing
        vector<float> bars;
        float barInc;
    
        // rendering
        ofImage rendering;
    
        // swipe
        vector<Drawing> drawings;
        vector<GridDrawing> gridDrawings;
        int index;
    
        // spacebrew
        Spacebrew::Connection spacebrew;
    
        // GUI
        int currentMode, lastMode;
        ofxUICanvas * gui;
    
        ofVideoPlayer video;
    
        ofFbo screen;
        ofFbo pattern;
};
