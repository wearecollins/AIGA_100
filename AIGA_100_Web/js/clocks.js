/**
 * @class
 */
var Letter = function() {
    //vector<vector<float> > 
    this.angles = [];
    //vector<vector<float> > 
    this.colorAngles = []
};

var clockLetterWidth = 4;
var clockLetterHeight = 6;

/**
 * @class
 */

var Clocks = function(){

    var numFaces;
    var mode;
    var bColorFace, bPreciseDraw;
    var lineWidth;
    var hueVariance;

    //vector<Clock>
    var clocks = []
    
    var currentAngleA;
    var currentAngleB;
    var currentClock;
    
    // Letters
    //map<string, Letter> 
    var letters = {};
    var alphabet = []; //vector<string>
    void loadLetters();
    
    // gui props
    var hueTweenStarted;
    var huePosition, hueSpeed, hueDirection;
    var faceHueMax, faceHueMin, armHueMax, armHueMin, letterHueMax, letterHueMin;
    var faceSat, faceBright, armSat, armBright, letterSat, letterBright;
    
    var bUseColorFade;
    var faceColorTop, faceColorBottom, letterColor, armColor;
    
    // spacing
    var spacing;

    //int gridX, int gridY, ofVec3f spacing, int startX = 100, int startY = 100, int radius = 25, float radiusMult = .9
    this.setup = function( gridX, gridY, spacing, startX, startY, radius, radiusMult ){

    }

    this.update = function(){

    }

    this.draw = function(){

    }

    //Letter letter, offsetX, offsetY, letterWidth = 4, float colorWeight = 1.0
    this.setClocks = function( Letter letter, offsetX, offsetY, letterWidth, colorWeight ){

    }

    // ofColor color, hueVariance, bool bArm = false
    this.setFaceColors = function( color, hueVariance, bArm ){

    }
    //ofColor color, hueVariance = 0
    this.setLetterColors = function( color, hueVariance){

    }
    //ofColor color, hueVariance = 0, bool bArm = false
    this.setNonLetterFaceColors = function( color, hueVariance, bArm ){

    }

    //ofColor color, hueVariance = 0, bool bArm = false
    this.setLetterFaceColors = function( color, hueVariance, bArm ){

    }

    this.setColor = function( color ){

    }

    this.setArmColor = function( color ){

    }

    this.magnet = function( attractX, attractY, color, freezeTime ){

    }

    this.pointTo = function( attractX, attractY ){

    }
}