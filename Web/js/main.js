var app;

$(document).ready( function() {
	DemoApp.prototype = $.extend(true, SUD.app.BaseApp.prototype, DemoApp.prototype);
	app 	= new DemoApp();
	app.begin();
});

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

// ===========================================
// ===== DEMO APP
// ===========================================

	DemoApp = function(){
		SUD.app.BaseApp.call( this );

		var sb;

		// color vars		
		this.r = 0;
		this.g = 0;
		this.b = 0;

		this.rE = 0;
		this.gE = 0;
		this.bE = 0;
		
		//-------------------------------------------------------
		this.setup = function (){
			//this.unregisterMouseEvents();
			this.registerTouchEvents();

			// setup spacebrew
			var random_id = "0000" + Math.floor(Math.random() * 10000);
			var app_name = "Swipe_" + random_id.substring(random_id.length-4);

			this.setupSpacebrew();

			this.currentDrawing = [];
			this.touchId 		= -1;

			this.gridCanvas = document.getElementById("gridCanvas");
			// todo: set w/h
			this.gridCtx 	= this.gridCanvas.getContext('2d');

			this.modes = [];
			this.modes.push("#grid");
			this.modes.push("#text");
			this.modes.push("#color");

			window.scrollTo(0,1);
			this.mode = 1;
		}

		//-------------------------------------------------------
		this.setupSpacebrew = function() {
			// create spacebrew client object
			sb = new Spacebrew.Client();
			sb.send_interval = 0;
			sb.name(app_name);

			// configure the publication and subscription feeds
			// grid
			sb.addPublish( "drawing", "drawing", "[]" );

			// color
			sb.addPublish( "red", "range", "0" );
			sb.addPublish( "green", "range", "0" );
			sb.addPublish( "blue", "range", "0" );

			// text
			sb.addPublish( "text", "string" );
			
			// stuff we're subscribing to
			sb.addSubscribe( "mode", "range" );
			sb.addSubscribe( "randomR", "range" );
			sb.addSubscribe( "randomG", "range" );
			sb.addSubscribe( "randomB", "range" );

			// override Spacebrew events - this is how you catch events coming from Spacebrew
			sb.onRangeMessage  = this.onRangeMessage.bind(this);

			sb.connect();
		};

		//-------------------------------------------------------
		this.update = function (){
			// color update
			this.rE = this.rE * .75 + this.r * .25;
			this.gE = this.gE * .75 + this.g * .25;
			this.bE = this.bE * .75 + this.b * .25;
			if ( this.mode == 0 ) $("#colorCanvas").css("background-color", "rgb(" + Math.round(this.rE) +","+ Math.round(this.gE) +"," + Math.round(this.bE) + ")");
		}

		//-------------------------------------------------------
		this.draw = function (){
			switch( this.mode ){
				// color
				case 0:
					showHide("#color");
					break;

				// grid
				case 1:
					console.log("grid")
					this.gridCanvas.width = this.gridCanvas.width;
					this.gridCtx.clearRect(0,0,this.gridCanvas.width,this.gridCanvas.height);
					if ( this.currentDrawing.length > 0 ){
						this.gridCtx.beginPath();
						this.gridCtx.moveTo(this.currentDrawing[0].x, this.currentDrawing[0].y);
						for ( var i=1; i<this.currentDrawing.length; i++){
							this.gridCtx.lineTo(this.currentDrawing[i].x, this.currentDrawing[i].y);
						}
						this.gridCtx.stroke();
					}
					break;

				// text
				case 2:
					showHide("#text");
					break;
			}
		}	

		this.updateAndSendColor = function(x,y){
			var hue = SUD.clamp(SUD.map(x,0,window.innerWidth, 0, 1.0), 0,1.0);
			var sat = SUD.clamp(SUD.map(y,0,window.innerHeight, 0, 1.0),0.,1.0);

			var rgb = HSVtoRGB(hue, sat, sat)

			this.r = Math.round(rgb.r);
			this.g = Math.round(rgb.g);
			this.b = Math.round(rgb.b);

			sb.send("red", "range", this.r);
			sb.send("green", "range", this.g);
			sb.send("blue", "range", this.b);
		}

		//-------------------------------------------------------
		this.onTouchStart = function( id, x,y ){
			switch ( this.mode ){
				case 0:
					this.updateAndSendColor(x,y);
					break;
				case 1:
					if ( this.touchId == -1 ){
						this.touchId = id;
						this.currentDrawing.push({x:x, y:y});
					}
					break;
				case 2:
					break;
			}
		};

		//-------------------------------------------------------
		this.onTouchMove = function( id, x,y ){
			switch ( this.mode ){
				case 0:
					this.updateAndSendColor(x,y);
					break;
				case 1:
					if ( id == this.touchId ){
						this.currentDrawing.push({x:x, y:y});
					}
					break;
				case 2:
					break;
			}
		};

		//-------------------------------------------------------
		this.onTouchEnd = function( id, x,y ){
			switch ( this.mode ){
				case 0:
					// this.updateAndSendColor(x,y);
					break;
				case 1:
					if ( id == this.touchId ){
						this.touchId = -1;

						// normalize current drawing
						var cdNorm = [];
						for ( var i=0; i<this.currentDrawing.length; i++){
							cdNorm.push( { 
											x: this.currentDrawing[i].x / parseInt(window.innerWidth), 
											y: this.currentDrawing[i].y / parseInt(window.innerHeight)
										});
						}

						console.log( cdNorm );

						sb.send("drawing","drawing", cdNorm);
						this.currentDrawing = [];
					}
					break;
				case 2:
					break;
			}
		};

		//-------------------------------------------------------
		this.onRangeMessage = function( name, value ){
			if ( name == "mode" ){
				this.mode = value;
				switch( value ){
					// color
					case 0:
						showHide("#color");
						break;

					// grid
					case 1:
						showHide("#grid");
						break;

					// text
					case 2:
						showHide("#text");
						break;
				}
			} else if ( name == "randomR" ){
				this.r = Math.round(value);
			} else if ( name == "randomG" ){
				this.g = Math.round(value);
			} else if ( name == "randomB" ){
				this.b = Math.round(value);
			}
		};

		function showHide ( show ) {
			for ( var i=0; i<modes.length; i++){
				$(modes[i]).css("visibility", "hidden");
				$(modes[i]).css("display", "none");
			}

			$(show).css("visibility", "visible");
			$(show).css("display", "block");
		}

	}
	
