var ClockGeometry = null;
var ArmGeometry   = null;

var Clock = function() {
    THREE.Object3D.call(this);
    
    var face, arm1, arm2;
    var faceMat, armMat;

    this.radius = -1;
    this.vel.x = ofRandom(.1, 2.0);
    this.colorMode = 0;
    this.offset = 0;
    this.bColorFace = true;
    this.bMouseDown = false;
    this.bLetter = false;
    this.bInteracted = false;
    this.bDoColor = false;
    this.lastFroze = 0;
    this.rotMode = 0;
    this.lastNext = 0;
    this.colorIndex = 0;
    this.lineWidth = 2;

    var angles         = [0,90];
    var targetAngle    = [0,90];

    var currentColors  = [];

    this.setup = function(){
        this.lastNext;

        // colors
        this.faceColor      = new THREE.Color(0xFFFFFF);
        this.liveFaceColor  = new THREE.Color(0xFFFFFF);
        this.armColor       = new THREE.Color(0x000000);
        this.liveArmColor   = new THREE.Color(0x000000);

        // 
        this.vel = new THREE.Vector2(0,0);
        this.origin = new THREE.Vector2();
        this.lastMouse = new THREE.Vector2();
        this.lastAngle;

        this.bAnimating = true;

        //ofMesh face, ticks, arm1, arm2;
        if ( ClockGeometry == null ){
            ClockGeometry   = new THREE.CircleGeometry( this.radius );
            ArmGeometry     = new THREE.BoxGeometry( this.lineWidth, this.radius, 1 );
        }

        // build face mesh
        faceMat = new THREE.MeshBasicMaterial({color: this.faceColor });
        face = new THREE.Mesh( ClockGeometry, faceMat );

        armMat = new THREE.MeshBasicMaterial({color: this.armColor});
        arm1  = new THREE.Mesh( ArmGeometry, armMat );
        arm2  = new THREE.Mesh( ArmGeometry, armMat );

        this.add(armMat);
        this.add(arm1);
        this.add(arm2);
    }

    this.update = function(){
        arm1.rotation.set(0,0,angles[0]);
        arm2.rotation.set(0,0,angles[1]);
    }

    this.draw = function(){

        setColor(liveFaceColor);

        // ease angles
        angles[0]   = angles[0] * .9 + targetAngle[0] * .1;
        angles[1]   = angles[1] * .9 + targetAngle[1] * .1;
        
        if ( angles[0] > 360 ){
            angles[0]      -= 360;
            targetAngle[0] -= 360;
        }
        if ( angles[1] > 360 ){
            angles[1]      -= 360;
            targetAngle[1] -= 360;
        }
        
        // animate
        if ( SUD.getElapsedTimeMillis() - lastFroze > 1500 && bAnimating ){
            bInteracted = false;
            currentColors.clear();
            
            targetAngle[0] += 1. * vel.x;
            targetAngle[1] += .1 * vel.x;
            
            liveLetterColor.set( liveLetterColor.r * .8 + faceColor.r * .2,
                                 liveLetterColor.g * .8 + faceColor.g * .2,
                                 liveLetterColor.b * .8 + faceColor.b * .2);

            bLetter = false;
        } else if ( bLetter ) {
            // hax
            if ( colorAngles[0] != colorAngles[1] ){
                if ( SUD.getElapsedTimeMillis() - lastFroze > 500 ){
                    liveLetterColor.set( liveLetterColor.r * .8 + faceColor.r * .2,
                                         liveLetterColor.g * .8 + faceColor.g * .2,
                                         liveLetterColor.b * .8 + faceColor.b * .2);
                } else {
                    // TO-DO!!!
                    //liveLetterColor.lerp(letterColor, ofMap((ofGetElapsedTimeMillis() - lastFroze), 0, 500, 0, 1.0, true));
                }
            }
        }
        
        if ( SUD.getElapsedTimeMillis() - lastFroze > 2000 ){
            vel.x = vel.x * .9 + 1 * .1;
            offset = offset * .9;
        }
        
        var val = .1;
        if ( bInteracted ) val = .05;
        var inVal = 1.0 - val;
        
        liveFaceColor.r = inVal * liveFaceColor.r + val * faceColor.r;
        liveFaceColor.g = inVal * liveFaceColor.g + val * faceColor.g;
        liveFaceColor.b = inVal * liveFaceColor.b + val * faceColor.b;
    }
        
    this.rotateClockTo = function( angleA, angleB ){

    }
    this.rotateClockBy = function( angleA, angleB ){

    }

    this.rotateColorTo = function( angleA, angleB ){

    }

    this.setColor = function( c ){
        face.color.set(c);
    }

    // bgradient = true
    this.setColor = function( facec, letterc, startAngle, endAngle, bGradient ){

    }

    this.reset = function(){
        
    }

    //bFlip = false
    this.checkHit = function( mx, my, bFlip ){

    }

    this.nextRotate = function(){
        
    }

    // rotate like a clock!
    // this.onMousePressed( int mx, int my );
    // this.onMouseDragged( int mx, int my );
    // this.onMouseReleased( int mx, int my );
    // 
    this.rotateTo = function( mx, my ){

    }

    this.pointTo = function( mx, my ){

    }

    //freezeTime=1000
    this.magnet = function( mx, my, color, freezeTime=1000 ){

    }

    this.limitRotation = function(){
        
    }
}