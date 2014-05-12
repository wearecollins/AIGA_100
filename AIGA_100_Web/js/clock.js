var ClockGeometry = null;
var ArmGeometry   = null;

// utils

var PI = 3.14159265358979323846;
var DEG_TO_RAD = (PI/180.0);
var RAD_TO_DEG = (180.0/PI);

var angle = function( vecA, vecB ) {
    return parseFloat(Math.atan2( vecA.x*vecB.y-vecA.y*vecB.x, vecA.x*vecB.x + vecA.y*vecB.y )*RAD_TO_DEG);
}

var Clock = function() {
    THREE.Object3D.call(this);
    
    var face, arm1, arm2;
    var faceMat, armMat;

    this.radius = -1;

    this.vel = new THREE.Vector2(0,0);
    this.origin = new THREE.Vector2();
    this.vel.x = SUD.random(.1, 10.0);
    this.colorMode = 0;
    this.offset = 0;
    this.bColorFace = true;
    this.bMouseDown = false;
    var bLetter = false;
    var bInteracted = false;
    var bAnimating = true;
    this.bDoColor = false;
    this.lastFroze = 0;
    this.rotMode = 0;
    this.lastNext = 0;
    this.colorIndex = 0;
    this.lineWidth = .1;

    var angles         = [0,90];
    var targetAngle    = [0,90];

    var currentColors  = [];

    this.setup = function(){
        this.lastNext;

        // colors
        this.faceColor      = new THREE.Color(0xFFFFFF);
        this.liveFaceColor  = new THREE.Color(0xFF0000);
        this.armColor       = new THREE.Color(0x000000);
        this.liveArmColor   = new THREE.Color(0x000000);

        // 
        this.lastMouse = new THREE.Vector2();
        this.lastAngle;


        //ofMesh face, ticks, arm1, arm2;
        if ( ClockGeometry == null ){
            ClockGeometry   = new THREE.CircleGeometry( this.radius, 16 );
            ArmGeometry     = new THREE.BoxGeometry( this.lineWidth, this.radius, .1 );
        }

        // build face mesh
        faceMat = new THREE.MeshBasicMaterial({color: this.faceColor });
        face = new THREE.Mesh( ClockGeometry, faceMat );

        armMat = new THREE.MeshBasicMaterial({color: this.armColor});
        arm1  = new THREE.Object3D();
        arm2  = new THREE.Object3D();

        var armmat = new THREE.Mesh( ArmGeometry, armMat );
        var arm2mat = new THREE.Mesh( ArmGeometry, armMat );
        arm2mat.position.y = this.radius/2;
        armmat.position.y = this.radius/2;
        arm1.add(armmat);
        arm2.add(arm2mat);

        this.add(face);
        this.add(arm1);
        this.add(arm2);
        //this.rotation.x = DEG_TO_RAD * 90;
    }

    this.update = function(){
        arm1.rotation.set(0,0,angles[0] * DEG_TO_RAD);
        arm2.rotation.set(0,0,angles[1] * DEG_TO_RAD);
    }

    this.draw = function(){

        this.setColor(this.liveFaceColor);

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
        if ( app.getElapsedTimeMillis() - this.lastFroze > 1500 && bAnimating ){
            bInteracted = false;
            
            targetAngle[0] += -1. * this.vel.x;
            targetAngle[1] += -.1 * this.vel.x;
            
            // this.liveFaceColor.set( this.liveFaceColor.r * .8 + this.faceColor.r * .2,
            //                      this.liveFaceColor.g * .8 + this.faceColor.g * .2,
            //                      this.liveFaceColor.b * .8 + this.faceColor.b * .2);

            bLetter = false;
        } else if ( bLetter ) {
            // hax
            if ( colorAngles[0] != colorAngles[1] ){
                if ( app.getElapsedTimeMillis() - this.lastFroze > 500 ){
                    this.liveFaceColor.set( this.liveFaceColor.r * .8 + this.faceColor.r * .2,
                                         this.liveFaceColor.g * .8 + this.faceColor.g * .2,
                                         this.liveFaceColor.b * .8 + this.faceColor.b * .2);
                } else {
                    // TO-DO!!!
                    //liveLetterColor.lerp(letterColor, ofMap((ofGetElapsedTimeMillis() - this.lastFroze), 0, 500, 0, 1.0, true));
                }
            }
        }
        
        if ( app.getElapsedTimeMillis() - this.lastFroze > 2000 ){
            this.vel.x = this.vel.x * .9 + 1 * .1;
            this.offset = this.offset * .9;
        }
        
        var val = .1;
        if ( bInteracted ) val = .05;
        var inVal = 1.0 - val;
        
        this.liveFaceColor.r = inVal * this.liveFaceColor.r + val * this.faceColor.r;
        this.liveFaceColor.g = inVal * this.liveFaceColor.g + val * this.faceColor.g;
        this.liveFaceColor.b = inVal * this.liveFaceColor.b + val * this.faceColor.b;
    }
        
    this.rotateClockTo = function( angleA, angleB ){
        angleA = SUD.wrap(angleA, 0, 360);
        angleB = SUD.wrap(angleB, 0, 360);
        
        var an = angleA;
        while ( an < targetAngle[0] ){
            an += 360;
        }
        targetAngle[0] = an;
        
        an = angleB;
        while ( an < targetAngle[1] ){
            an += 360;
        }
        targetAngle[1] = an;
    }

    this.setColor = function( c ){
        faceMat.color.set(c);
    }

    // letter functions (don't need)
    // this.reset = function(){}
    // //bFlip = false
    // this.checkHit = function( mx, my, bFlip ){}
    // this.nextRotate = function(){}
    // this.onMousePressed( int mx, int my );
    // this.onMouseDragged( int mx, int my );
    // this.onMouseReleased( int mx, int my );
    // 
    // this.rotateTo = function( mx, my ){}

    this.pointTo = function( mx, my ){
        var m = new THREE.Vector3(mx,my,0);
        var dist = this.position.distanceTo(m);
        m = m.sub( this.position );
        
        //if ( (dist) < 20000 ){
        var line = new THREE.Vector3(this.liveRadius  * .5, this.liveRadius * .5, 0).add(this.position);
        
        targetAngle[0] = SUD.map(angle(this.position, m), -180, 180, 0, 360) * 1.0;// + 135;
        targetAngle[1] = targetAngle[0] + 180;

        this.vel.x += 10.0;
        this.offset += SUD.map(dist, 0, 200, 10, 0, true);
        this.offset = SUD.clamp(this.offset, 0, 150);
        return dist;
    }

    //freezeTime=1000
    this.magnet = function( mx, my, color, freezeTime ){
        if ( freezeTime === undefined ){
            freezeTime = 1000;
        }
        var dist = this.pointTo(mx, my);
        if ( Math.abs(dist) < this.radius * 2 ){
            this.liveFaceColor.set(color);
            this.liveArmColor.set(255);
            bInteracted = true;
        }
    
        if ( Math.abs(dist) < 1000 ) this.lastFroze = app.getElapsedTimeMillis() - SUD.map(Math.abs(dist), 0, 500, freezeTime, 0, true);
    }

    this.limitRotation = function(){
        
    }
}

Clock.prototype = Object.create( THREE.Object3D.prototype );
