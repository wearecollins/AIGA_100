SUD.require("js/clock.js");
SUD.require("js/letters.js");

var Drawing = function() {
    this.points = [];
    this.index = 0;
    this.age = 0;
    this.lastChanged = 0;
    this.color;
};

var GridDrawing = function() {
    this.grid = [];
    this.indices = [];
    this.index = 0;
    this.age = 0;
    this.lastChanged = 0;
    this.color;
};

var TextSquare = function(){
    this.text = "";
    this.lastText = "";
    this.index = 0;
    this.lastChanged = 0;
    this.backwards = false;
};

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
    THREE.Object3D.call(this);

    var numFaces = 1;
    var mode = 0;
    var bColorFace = true;
    var bPreciseDraw = false;
    var lineWidth = 1.0;
    var hueVariance = 0;

    //vector<Clock>
    this.clocks = []
    
    var currentAngleA = 0;
    var currentAngleB;
    var currentClock;
    
    // Letters
    //map<string, Letter> 
    this.letters = {};
    
    // gui props
    var hueTweenStarted = false;
    var huePosition     = 0;
    var hueSpeed        = 0; 
    var hueDirection    = 1;

    var faceHueMax, faceHueMin, armHueMax, armHueMin, letterHueMax, letterHueMin;
    var faceSat, faceBright, armSat, armBright, letterSat, letterBright;
    
    var faceColorTop, faceColorBottom, letterColor, armColor;
    
    // spacing
    var spacing;

    //int gridX, int gridY, ofVec3f spacing, int startX = 100, int startY = 100, int radius = 25, float radiusMult = .9
    this.setup = function( gridX, gridY, spacing, startX, startY, radius, radiusMult ){
        for (var x=0; x<gridX; x++){
            var yInc = startY;
            for (var y=0; y<gridY; y++){
                var c = new Clock();
                this.clocks.push( c );
                c.radius = radius;
                c.liveRadius =  radius * radiusMult;
                c.position.x = startX + x * (spacing.x);
                c.position.y = yInc;
                c.origin.set(c.position);
                yInc = c.position.y + spacing.y;
            }
        }
        
        this.loadLetters();
        
        for ( var i=0; i<this.clocks.length; i++ ){
            this.clocks[i].setup();
            this.add(this.clocks[i]);
        }
    }

    this.update = function(){
        for ( var i=0; i<this.clocks.length; i++ ){
            this.clocks[i].numFaces = numFaces;
            this.clocks[i].colorMode = mode;
            this.clocks[i].lineWidth = lineWidth;
            
            var yIndex = i % 10;
            
            //clocks[i].position.x = clocks[i].origin.x + spacing.x;
            //clocks[i].position.y = clocks[i].origin.y + (yIndex <=4 ? spacing.y : spacing.z);
            
            this.clocks[i].update();
        }
    }

    this.draw = function(){
        for ( var i=0; i<this.clocks.length; i++ ){
            this.clocks[i].draw();
        }
    }

    this.loadLetters = function(){
        // vector<float> 
        var ca1Vec = [];
        var ca2Vec = [];
        
        var ind = 0;
        
        var alphabet = Object.keys(LETTERS);

        for ( var i=0; i<alphabet.length; i++ ){
            var s = alphabet[i];
            this.letters[s] = new Letter();
            for ( var j=0; j<LETTERS[s].letter.clock.length; j++){

                var a1 = LETTERS[s].letter.clock[j]["a1"] + (ind < 3 ? 45 : 0);
                var a2 = LETTERS[s].letter.clock[j]["a2"] + (ind < 3 ? 45 : 0);
                this.letters[s].angles.push([]);
                var back = this.letters[s].angles.length-1;
                this.letters[s].angles[back].push(a1);
                this.letters[s].angles[back].push(a2);

            }
        }
        ind++;
    }

    //Letter letter, offsetX, offsetY, letterWidth = 4, float colorWeight = 1.0
    this.setClocks = function( letter, offsetX, offsetY, letterWidth, colorWeight ){
        if ( offsetX >= 10 ) return;
        if ( offsetX + letterWidth >= 10 ) letterWidth = 10 - offsetX;
        for (var x=offsetX; x<offsetX + letterWidth; x++){
            for (var y=offsetY; y<offsetY + clockLetterHeight; y++){
                if ( x >= 0 ){
                    var ind = (y - offsetY) + (x - offsetX) * clockLetterHeight;
                    var cind = y + x * 10.0;
                    if ( (letter.colorAngles[ind][0] != letter.colorAngles[ind][1]) && ( Math.abs( letter.colorAngles[ind][0] - letter.colorAngles[ind][1]) != 360) ){
                        
                        for ( var i=0; i < this.clocks[cind].angles.length; i++ ){
                            this.clocks[cind].angles[i] = SUD.wrap(this.clocks[cind].angles[i], 0, 360);
                        }
                        
                        for ( var i=0; i < this.clocks[cind].targetAngle.length; i++ ){
                            this.clocks[cind].targetAngle[i] = SUD.wrap(this.clocks[cind].targetAngle[i], 0, 360);
                        }
                        
                        var color = letterColor.clone();
                        if ( !this.clocks[cind].bLetter ) this.clocks[cind].liveLetterColor.set(this.clocks[cind].faceColor);
                        this.clocks[cind].letterColor.set(color);
                        this.clocks[cind].rotateClockTo( letter.angles[ind][0], letter.angles[ind][1]);
                        //this.clocks[cind].setColor( this.clocks[cind].faceColor, this.clocks[cind].letterColor, letter.colorAngles[ind][0], letter.colorAngles[ind][1] );
                        //this.clocks[cind].rotateColorTo( letter.colorAngles[ind][0], letter.colorAngles[ind][1]);
                        this.clocks[cind].lastFroze = app.getElapsedTimeMillis();
                        this.clocks[cind].bLetter = true;
                    }
                }
            }
        }
    }

    // ofColor color, hueVariance, bool bArm = false
    this.setFaceColors = function( color, hueVariance, bArm ){
        for ( var i=0; i<this.clocks.length; i++ ){
            this.clocks[i].setColor(color);
        }
    }

    //ofColor color, hueVariance = 0
    this.setLetterColors = function( color, hueVariance){}

    //ofColor color, hueVariance = 0, bool bArm = false
    this.setNonLetterFaceColors = function( color, hueVariance, bArm ){}

    //ofColor color, hueVariance = 0, bool bArm = false
    this.setLetterFaceColors = function( color, hueVariance, bArm ){}

    this.setColor = function( color ){

    }

    this.setArmColor = function( color ){

    }

    this.magnet = function( x, y, color, freezeTime ){
        for ( var i=0; i<this.clocks.length; i++ ){
            this.clocks[i].magnet(x,y,color, freezeTime);
        }
    }

    this.pointTo = function( attractX, attractY ){
        for ( var i=0; i<this.clocks.length; i++ ){
            this.clocks[i].pointTo(attractX, attractY);
        }
    }
}

Clocks.prototype = Object.create( THREE.Object3D.prototype );