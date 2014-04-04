//
//  Clock.h
//  clocks
//
//  Created by Brett Renfer on 3/25/14.
//
//

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
    
    bool bAnimating;
    int  rotMode;
    
    vector<float> angles;
    vector<float> targetAngle;
    int lastNext;
    
    ofVec2f vel;
    
    ofColor faceColor, armColor;
    
    Clock();
    
    void update();
    void draw();
    
    void rotateClockTo ( float angleA, float angleB );
    void reset();
    
    bool bMouseDown;
    
    void checkHit( int mx, int my, bool bFlip = false );
    void nextRotate();

    ofVec2f lastMouse;
    float lastAngle;
    
    // rotate like a clock!
    void onMousePressed( int mx, int my );
    void onMouseDragged( int mx, int my );
    void onMouseReleased( int mx, int my );
    
    void rotateTo( int mx, int my );
    void magnet( int mx, int my );
    
};

/**
 * @struct
 */
struct Letter {
    vector<vector<float> > angles;
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
    
    void setup( int gridX, int gridY, int spacing, int startX = 100, int startY = 100, int radius = 25, float radiusMult = .9 );

    void update( ofEventArgs & e );
    void draw();
    
    // GUI
    ofImage colorImage;
    ofxUICanvas * gui;
    ofxUICanvas * gui2;
    void setupGui();
    void onGui( ofxUIEventArgs &e );
    
    /**
     * @function
     * @param {vector<vector<float> >}  angles  4x6 array of 2 angles
     * @param {int}                     offsetX
     * @param {int}                     offsetY
     */
    void setClocks( Letter letter, int offsetX, int offsetY, int letterWidth = 4 );
    void setFaceColors( ofColor color, int hueVariance = 0, bool bArm = false );
    void setNonLetterFaceColors( ofColor color, int hueVariance = 0, bool bArm = false );
    void setLetterFaceColors( ofColor color, int hueVariance = 0, bool bArm = false );
    
    // Letters
    map<string, Letter> letters;
    vector<string> alphabet;
    void loadLetters();
    
    // override events
    void magnet( float attractX, float attractY );
    
    // Key && Mouse events
    
    void keyPressed(ofKeyEventArgs & e);
    void mouseDragged(ofMouseEventArgs & e);
    void mousePressed(ofMouseEventArgs & e);
    void mouseReleased(ofMouseEventArgs & e);
    
};

