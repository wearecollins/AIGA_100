function setCircleStyle( object, rad, border, borderWidth, borderColor ){
	object.style.height = rad * 2  +"px";
	object.style.width = rad * 2 +"px";
	object.style["-moz-border-radius"] = rad + "px";
	object.style["-webkit-border-radius"] = rad + "px";
	if ( border ){
		object.style.border = borderWidth +"px solid "+borderColor;
		object.style["-moz-border-radius"] = rad + borderWidth + "px";
		object.style["-webkit-border-radius"] = rad + borderWidth + "px";
	}
	// object.style.position = "relative";
}

function setLineStyle ( div, lineWidth, height, color) {
	var line = document.createElement("div");
	div.style.width = lineWidth +"px";
	div.style.height = height * 2 +"px";
	div.style.position = "relative";
	div.appendChild(line);

	line.style.width = lineWidth +"px";
	line.style.height = height +"px";
	line.style.backgroundColor = color;
	return line;
}

// UTIL!
function HSVtoRGB(h, s, v) {
    var r, g, b, i, f, p, q, t;
    if (h && s === undefined && v === undefined) {
        s = h.s, v = h.v, h = h.h;
    }
    i = Math.floor(h * 6);
    f = h * 6 - i;
    p = v * (1 - s);
    q = v * (1 - f * s);
    t = v * (1 - (1 - f) * s);
    switch (i % 6) {
        case 0: r = v, g = t, b = p; break;
        case 1: r = q, g = v, b = p; break;
        case 2: r = p, g = v, b = t; break;
        case 3: r = p, g = q, b = v; break;
        case 4: r = t, g = p, b = v; break;
        case 5: r = v, g = p, b = q; break;
    }
    return {
        r: Math.floor(r * 255),
        g: Math.floor(g * 255),
        b: Math.floor(b * 255)
    };
}



var Clock = function(x,y,rad, isGL){
	THREE.Object3D.call( this );
	this.setup(x,y,rad, isGL);
}

Clock.prototype = new THREE.Object3D();
Clock.prototype.constructor = Clock;
Clock.prototype.supr = THREE.Object3D.prototype;

Clock.prototype.setup = function( x,y, rad, isGL ) {
	if ( !window.clockGeometry ){
		window.clockGeometry = new THREE.CircleGeometry( rad, 32 );
		window.clocks = [];
		window.clockCounter = 0;
	}
	this.radius = rad;
	this.grey   = {r:100, g:100, b:100};
	this.color  = {r:100, g:100, b:100};

	if ( isGL ){
		this.faceMaterial = new THREE.MeshBasicMaterial({color: 0xFD4F57, overdraw: true});
		this.face 	= new THREE.Mesh( window.clockGeometry, this.faceMaterial );
		this.armMaterial = new THREE.LineBasicMaterial( { color: 0xffffff, opacity: 1, linewidth: 3} );
		
		var lineOne = new THREE.Geometry();
		lineOne.vertices.push( new THREE.Vector3( 0,0,0 ));
		lineOne.vertices.push( new THREE.Vector3( 0,rad * .8,0 ));

		var lineTwo = new THREE.Geometry();
		lineTwo.vertices.push( new THREE.Vector3( 0,0,0 ));
		lineTwo.vertices.push( new THREE.Vector3( 0, rad * .8,0 ));

		this.armOne = new THREE.Line(lineOne, this.armMaterial );
		this.armTwo = new THREE.Line(lineTwo, this.armMaterial );

	} else {
		
		// create div, set style, append to body
		var div = document.createElement("div");
		setCircleStyle(div, rad, false);
		div.style.backgroundColor = this.getRBGStyle();
		div.id = "clock_"+window.clockCounter;
		div.className = "threeElement";
		document.getElementById("sud_container").appendChild(div);
		this.face = new THREE.CSS3DObject(div)

		var ad1 = document.createElement("div");
		setLineStyle(ad1, isIpad ? 5 : 3, rad* .8, "#fff");
		ad1.id = "arm1_"+window.clockCounter;
		ad1.className = "threeElement arm";
		document.getElementById("sud_container").appendChild(ad1);

		var ad2 = document.createElement("div");
		setLineStyle(ad2, isIpad ? 5 : 3, rad* .8, "#fff");
		ad2.id = "arm1_"+window.clockCounter;
		ad2.className = "threeElement arm";
		document.getElementById("sud_container").appendChild(ad2);

		this.armOne = new THREE.CSS3DObject(ad1);
		this.armTwo = new THREE.CSS3DObject(ad2)
	}

	this.add( this.face );
	this.add( this.armOne );
	this.add( this.armTwo );

	window.clockCounter++;

	this.position.x = x;
	this.position.y = y;

	window.clocks.push(this);
	this.animating = true;
	this.vel = new THREE.Vector2(( isIpad ? 3 : 1 ),0);
	this.lastFroze = new Date();
	this.lastAnim = new Date();
	this.currentMouse = {x:-1, y:-1};
	this.p2 = new THREE.Vector2(this.position.x, this.position.y);
	this.m = new THREE.Vector2(0,0);

	this.rotateArmOneBy(SUD.random(0,360));
	this.rotateArmTwoBy(SUD.random(0,360));
};

Clock.prototype.getRBGStyle = function() {
	var rgb = this.getRGB();
	return "rgb(" + rgb.r +"," + rgb.g +","+rgb.b +")";
};

Clock.prototype.getRGB = function() {
	var rgb = {r:this.color.r, g:this.color.g, b:this.color.b}
	rgb.r = Math.round(rgb.r);
	rgb.g = Math.round(rgb.g);
	rgb.b = Math.round(rgb.b);
	return rgb;
};

Clock.prototype.angle = function(a,b, toDeg){
	var toDeg = toDeg !== undefined ? toDeg : true;
	return Math.atan2( a.x*b.y-a.y*b.x, a.x*b.x + a.y*b.y ) * (toDeg ? 180/Math.PI : 1.0);
}

Clock.prototype.magnet = function(mx,my, now) {
	if ( !this.mouseDown ){
		this.armOne.element.style["-webkit-transition"] = "";
		this.armTwo.element.style["-webkit-transition"] = "";
	}
	this.m.x = mx;
	this.m.y = my;
	this.mouseDown = true;
	this.animating = false;
}

Clock.prototype.update = function( time ) {
	if ( time - this.lastFroze > 1000 && !this.mouseDown ){

		this.color.r = /*this.color.r * .9 +*/ this.grey.r;// * .1;
		this.color.g = /*this.color.g * .9 +*/ this.grey.g;// * .1;
		this.color.b = /*this.color.b * .9 +*/ this.grey.b;// * .1;

		if ( this.animating ){
			if ( this.armOne.element.style["-webkit-transition"] != "" ){
				this.armOne.element.style["-webkit-transition"] = "";
				this.armTwo.element.style["-webkit-transition"] = "";
				this.vel.x = ( isIpad ? 3 : 1 );
			}
			this.rotateArmOneBy(this.vel.x);
			this.rotateArmTwoBy(this.vel.x * 1/60.0);
		} else {
			this.armOne.element.style["-webkit-transition"] = "opacity .5s ease-in-out, -webkit-transform 1s ease-in-out";
			this.armTwo.element.style["-webkit-transition"] = "opacity .5s ease-in-out, -webkit-transform 1s ease-in-out";
			this.rotateArmOneBy(180);
			this.rotateArmTwoBy(180 * 1/60.0);
			this.animating = true;
			this.lastFroze = time;
		}
	} else if ( this.mouseDown ) {
		if ( this.m.x != -1 ){
			
		    var dist = this.p2.distanceTo(this.m);
		    
		    if ( (dist) < 20000 && !isIpad){
		    	this.m = this.m.sub(this.p2);
		        var a = this.angle(this.p2, this.m, false);
		        var angle = SUD.map(a, -180, 180, 90, 360);

				this.armOne.rotation.z = angle;
				this.armTwo.rotation.z = angle + Math.PI;
		        this.vel.x += 10.0;
		        this.lastAnim = time;
		    }
		    this.m.x = -1;
    		this.lastFroze = time - SUD.map(dist, 0, 1000, 1000, 0);
		}
	}

	var style = this.getRBGStyle();
	if ( this.lastStyle != style ){//&& isIpad ){
		this.face.element.style.backgroundColor = this.getRBGStyle();
	}

	this.lastStyle = style;
};

Clock.prototype.mousePressed = function(x,y) {
	//this.mouseDown = x <= this.position.x + this.radius * 2.0 && x >= this.position.x - this.radius * 2.0 && y <= this.position.y + this.radius * 2.0 && y >= this.position.y - this.radius  * 2.0;
	return x <= this.position.x + this.radius * 2.0 && x >= this.position.x - this.radius * 2.0 && y <= this.position.y + this.radius * 2.0 && y >= this.position.y - this.radius  * 2.0;
};

Clock.prototype.mouseReleased = function(){
	this.mouseDown = false;
}

Clock.prototype.setFaceStyle = function(style) {
	this.face.element.style.backgroundColor = style;
};

Clock.prototype.rotateArmOneBy = function(deg) {
	this.armOne.rotation.z += SUD.degToRad(deg);
};

Clock.prototype.rotateArmTwoBy = function(deg) {
	this.armTwo.rotation.z += SUD.degToRad(deg);
};