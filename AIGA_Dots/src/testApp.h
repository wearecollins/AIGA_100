#pragma once

#include "ofMain.h"
#include "Constants.h"
#include "Dot.h"
#include "Clock.h"
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

struct TextSquare {
    string text;
    string lastText;
    int index = 0;
    int lastChanged = 0;
    bool backwards = false;
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
        void onGui( ofxUIEventArgs &e );
    
        vector<Dot> dots;
    
        // images
        ofImage kidd;
    
        // clock faces
        vector<Clock> clocks;
        ofImage colorImage;
    
        ofLight simpleLight;
        ofEasyCam camera;

        // quick data thing
        vector<float> bars;
        float barInc;
    
        // rendering
        ofImage rendering;
        vector<TextSquare> currentStrings;
        ofTrueTypeFont font;
        void renderText();
        void renderClocks();
    
        // swipe
        vector<Drawing> drawings;
        vector<GridDrawing> gridDrawings;
        int index;
    
        // spacebrew
        Spacebrew::AdminConnection spacebrew;
        void onOpen( ofxLibwebsockets::Event& args );
        void onMessage( Spacebrew::Message & m );
        void onClientConnect( Spacebrew::Config & c);
    
    
        // GUI
        int currentMode, lastMode;
        ofxUICanvas * gui;
        ofxUICanvas * gui2;
    
        ofVideoPlayer video;
    
        ofFbo screen;
        ofFbo pattern;
        ofFbo type;
};
