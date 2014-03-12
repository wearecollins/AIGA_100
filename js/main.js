var app;

$(document).ready( function() {
	App.prototype = $.extend(true, SUD.app.BaseApp.prototype, App.prototype);
	app 	= new App();
	app.begin();
});

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
	      
// Spacebrew Object
var sb, 
	app_name = "grid_"
	;

var App = function(){
	SUD.app.BaseApp.call( this );

	this.r = 0;
	this.g = 0;
	this.b = 0;

	this.rE = 0;
	this.gE = 0;
	this.bE = 0;

	//-------------------------------------------------------
	this.setup = function() {
		var random_id = "0000" + Math.floor(Math.random() * 10000);

		app_name = app_name + ' ' + random_id.substring(random_id.length-4);

		// create spacebrew client object
		sb = new Spacebrew.Client();
		sb.send_interval = 0;

		// set the base description
		sb.name(app_name);
		sb.description("This spacebrew example sends and receives custom messages.");

		// configure the publication and subscription feeds
		sb.addPublish( "red", "range", "0" );
		sb.addPublish( "green", "range", "0" );
		sb.addPublish( "blue", "range", "0" );
		
		sb.addSubscribe( "mode", "range" );
		sb.addSubscribe( "randomR", "range" );
		sb.addSubscribe( "randomG", "range" );
		sb.addSubscribe( "randomB", "range" );

		// override Spacebrew events - this is how you catch events coming from Spacebrew
		sb.onCustomMessage = onCustomMessage;
		sb.onOpen = onOpen;

		// connect to spacbrew
		sb.connect();

		document.getElementById("can").addEventListener("touchstart", this.onTouch.bind(this), false);
		document.getElementById("can").addEventListener("touchmove", this.onTouch.bind(this), false);
		document.getElementById("can").addEventListener("touchend", this.onTouchUp.bind(this), false);
	}

	//-------------------------------------------------------
	this.update = function(){
	}

	//-------------------------------------------------------
	this.draw 	= function(){
		this.rE = this.rE * .75 + this.r * .25;
		this.gE = this.gE * .75 + this.g * .25;
		this.bE = this.bE * .75 + this.b * .25;
		$("body").css("background-color", "rgb(" + Math.round(this.rE) +","+ Math.round(this.gE) +"," + Math.round(this.bE) + ")");
	}

	//-------------------------------------------------------
	this.onMouseMoved = function( x,y ){
		
	};

	this.onTouch = function( e ){
	    e.preventDefault();
		this.onMousePressed(e.layerX, e.layerY);
	}

	this.onTouchUp = function( e ){
	    e.preventDefault();

		sb.send("red", "range", this.r);
		sb.send("green", "range", this.g);
		sb.send("blue", "range", this.b);
	};

	//-------------------------------------------------------
	this.onMousePressed = function( x,y ){
		var hue = SUD.clamp(SUD.map(x,0,window.innerWidth, 0, 1.0), 0,1.0);
		var sat = SUD.clamp(SUD.map(y,0,window.innerHeight, 0, 1.0),0.,1.0);

		var rgb = HSVtoRGB(hue, sat, sat)

		this.r = Math.round(rgb.r);
		this.g = Math.round(rgb.g);
		this.b = Math.round(rgb.b);

		sb.send("red", "range", this.r);
		sb.send("green", "range", this.g);
		sb.send("blue", "range", this.b);
	};

	//-------------------------------------------------------
	this.onMouseDragged = function( x,y ){
		this.onMousePressed(x,y);
	};

	//-------------------------------------------------------
	this.onMouseReleased = function( x,y ){
		
	};
}

/**
 * Function that is called when Spacebrew connection is established
 */
function onOpen() {
	var message = "Connected as <strong>" + sb.name() + "</strong>. ";
	if (sb.name() === app_name) {
		message += "<br>You can customize this app's name in the query string by adding <strong>name=your_app_name</strong>."
	}
	$("#name").html( message );
}

/**
 * @param  {[type]} name  [description]
 * @param  {[type]} value [description]
 * @return {[type]}       [description]
 */
function onRangeMessage( name, value ){
	if ( name == "mode" ){

	} else if ( name == "randomR" ){
		this.r = Math.round(value);
	} else if ( name == "randomG" ){
		this.g = Math.round(value);
	} else if ( name == "randomB" ){
		this.b = Math.round(value);
	}
}

/**
 * onCustomMessage Function that is called whenever new spacebrew custom messages are received.
 *          It accepts three parameters:
 * @param  {String} name    Holds name of the subscription feed channel
 * @param  {String} value 	Holds value received from the subscription feed
 * @param  {String} type 	Holds the custom message type
 */
function onCustomMessage( name, value, type ){
	console.log("[onCustomMessage] remote dice roll ", value);
	if (type = "dice") {
		if (isNaN(value)) {
			value = "you can't cheat the house, that's not a dice roll";
		} else {
			if (value > 6 || value <= 0) value = "you can't cheat the house, that's not a dice roll";
		}
		$("#msg_received").text(value); // display the sent message in the browser         
	}
}