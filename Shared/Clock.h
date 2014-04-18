//
//  Clock.h
//  clocks
//
//  Created by Brett Renfer on 3/25/14.
//
//

#pragma once

#include "ofMain.h"
#include "ofxUI.h"
#include "ofxXmlSettings.h"

/**
 * @class
 */
class Clock : public ofVec2f {
public:
    
    int numFaces;
    int radius, liveRadius;
    int colorMode;
    float offset;
    bool bColorFace;
    int lastFroze;
    float lineWidth;
    bool bColorLetterFace;
    
    // test
    int colorIndex;
    int colorIncrement;
    
    bool bAnimating, bLetter;
    int  rotMode;
    
    vector<float> angles;
    vector<float> colorAngles;
    vector<float> targetAngle;
    int lastNext;
    
    ofVec2f vel;
    
    ofColor faceColor, liveFaceColor, liveLetterColor, letterColor, armColor, liveArmColor;
    void setColor( ofColor c );
    void setColor( ofColor facec, ofColor letterc, int startAngle, int endAngle, bool bGradient = true );
    
    Clock();
    
    void setup();
    void update();
    void draw();
    
    void rotateClockTo ( float angleA, float angleB );
    void rotateClockBy ( float angleA, float angleB );
    void rotateColorTo ( float angleA, float angleB );
    void reset();
    
    bool bMouseDown;
    bool bDoColor;
    
    void checkHit( int mx, int my, bool bFlip = false );
    void nextRotate();

    ofVec2f lastMouse;
    float lastAngle;
    
    // rotate like a clock!
    void onMousePressed( int mx, int my );
    void onMouseDragged( int mx, int my );
    void onMouseReleased( int mx, int my );
    
    void rotateTo( int mx, int my );
    
    float pointTo( int mx, int my );
    void magnet( int mx, int my, ofColor color, int freezeTime=1000 );
    
protected:
    ofMesh face, ticks, arm1, arm2;
};

/**
 * @struct
 */
struct Letter {
    vector<vector<float> > angles;
    vector<vector<float> > colorAngles;
};

/**
 * @class
 */
class Clocks {
public:
    
    
    // START CLOCKS CONSTANTS
    const int clockLetterWidth = 4;
    const int clockLetterHeight = 6;
    int numFaces;
    int mode;
    bool bColorFace;
    bool bPreciseDraw;
    float lineWidth;
    float hueVariance;
    bool guiVisible;
    bool bColorTextSeparately;
    
    vector<Clock> clocks;
    
    Clocks();
    ~Clocks();
    
    void setup( int gridX, int gridY, ofVec3f spacing, int startX = 100, int startY = 100, int radius = 25, float radiusMult = .9 );
    void setup( int gridX, int gridY, int spacing, int startX = 100, int startY = 100, int radius = 25, float radiusMult = .9 );

    void update( ofEventArgs & e );
    void draw();
    void drawGui();
    
    // GUI
    ofImage colorImage;
    ofxUICanvas * gui;
    ofxUICanvas * gui2;
    void setupGui();
    void onGui( ofxUIEventArgs &e );
    
    int currentAngleA;
    int currentAngleB;
    int currentClock;
    
    /**
     * @function
     * @param {vector<vector<float> >}  angles  4x6 array of 2 angles
     * @param {int}                     offsetX
     * @param {int}                     offsetY
     */
    void setClocks( Letter letter, int offsetX, int offsetY, int letterWidth = 4, float colorWeight = 1.0 );
    void setFaceColors( ofColor color, int hueVariance = 0, bool bArm = false );
    void setLetterColors( ofColor color, int hueVariance = 0 );
    void setNonLetterFaceColors( ofColor color, int hueVariance = 0, bool bArm = false );
    void setLetterFaceColors( ofColor color, int hueVariance = 0, bool bArm = false );
    
    // Letters
    map<string, Letter> letters;
    vector<string> alphabet;
    void loadLetters();
    
    // override events
    void magnet( float attractX, float attractY, ofColor color, int freezeTime=1000 );
    void pointTo( float attractX, float attractY );
    
    // Key && Mouse events
    
    void keyPressed(ofKeyEventArgs & e);
    void mouseDragged(ofMouseEventArgs & e);
    void mousePressed(ofMouseEventArgs & e);
    void mouseReleased(ofMouseEventArgs & e);
    
    // gui props
    int hueTweenStarted;
    float huePosition, hueSpeed, hueDirection;
    float faceHueMax, faceHueMin, armHueMax, armHueMin, letterHueMax, letterHueMin;
    float faceSat, faceBright, armSat, armBright, letterSat, letterBright;
    ofColor faceColorTop, faceColorBottom, letterColor, armColor;
    
};

