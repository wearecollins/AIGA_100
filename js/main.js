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

			this.setupSpacebrew();

			// grid
			this.grid = [];
			for ( var x=0; x<10; x++){
				for ( var y=0; y<10; y++){
					this.grid.push({x: x, y: y, filled: false });
				}
			}
			this.currentDrawing = [];
			this.touchId 		= -1;
			this.gridIndices = [];

			this.gridCanvas = document.getElementById("gridCanvas");
			// todo: set w/h
			this.gridCtx 	= this.gridCanvas.getContext('2d');

			// text
			this.button = document.getElementById("button");
			this.button.onclick = this.textPressed.bind(this);

			this.color = {};
			this.color.r = SUD.randomInt(0,255);
			this.color.g = SUD.randomInt(0,255);
			this.color.b = SUD.randomInt(0,255);

			this.modes = [];
			this.modes.push("#grid");
			this.modes.push("#text");
			this.modes.push("#color");

			window.scrollTo(0,1);
			this.mode = 1;
		}

		this.registerTouchEvents = function(){
			document.body.ontouchstart = this._onTouchStart.bind(this);
			document.body.ontouchmove = this._onTouchMove.bind(this);
			document.body.ontouchend = this._onTouchEnd.bind(this);
		}

		this.unregisterTouchEvents = function(){
			document.body.ontouchstart = null;
			document.body.ontouchmove = null;
			document.body.ontouchend = null;
		}

		//-------------------------------------------------------
		this.setupSpacebrew = function() {
			var random_id = "0000" + Math.floor(Math.random() * 10000);
			var app_name = "Swipe_" + random_id.substring(random_id.length-4);

			// create spacebrew client object
			sb = new Spacebrew.Client();
			sb.send_interval = 0;
			sb.name(app_name);

			// configure the publication and subscription feeds
			// grid
			sb.addPublish( "grid", "grid", "[]" );

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
		}

		//-------------------------------------------------------
		this.draw = function (){
			switch( this.mode ){
				// color
				case 0:
					$("#color").css("background-color", "rgb(" + Math.round(this.rE) +","+ Math.round(this.gE) +"," + Math.round(this.bE) + ")");
					break;

				// grid
				case 1:
					//console.log("grid")
					var w = 50;
					var x = parseInt(this.gridCanvas.width) / 2.0 - ((w * 9)/2.0);
					var y = parseInt(this.gridCanvas.height) / 2.0 - ((w * 9) /2.0);
					
					this.gridCanvas.width = this.gridCanvas.width;
					this.gridCtx.clearRect(0,0,this.gridCanvas.width,this.gridCanvas.height);
			

					this.gridCtx.fillStyle = ("rgb("+ this.color.r+"," + this.color.g + "," + this.color.b + ")");
					this.gridCtx.strokeStyle = ("rgb("+ this.color.r+"," + this.color.g + "," + this.color.b + ")");

					for ( var i=0; i<this.grid.length; i++){
						this.gridCtx.beginPath();
						this.gridCtx.arc( 	x + this.grid[i].x * w,
										y + this.grid[i].y * w, 
										(w * .8)/2,
										0,2*Math.PI);
						if ( this.grid[i].filled ){
							this.gridCtx.fill();
						} else {
							this.gridCtx.stroke();
						}
					}
					break;

				// text
				case 2:
					break;
			}
		}	

		//-------------------------------------------------------
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
		this.checkGrid = function(x,y){

			var w = 50;
			var cx = parseInt(this.gridCanvas.width) / 2.0 - ((w * 10)/2.0);
			var cy = parseInt(this.gridCanvas.height) / 2.0 - ((w * 10) /2.0);

			for ( var i=0; i<this.grid.length; i++){
				var rx = cx + this.grid[i].x * w;
				var ry = cy + this.grid[i].y * w;

				if ( x <= rx + w/2 && x >= rx - w/2 &&
					 y <= ry + w/2 && y >= ry - w/2 ){
					this.grid[i].filled = true;
					return i;
				}
			}
			return -1;
		}

		//-------------------------------------------------------
		this.textPressed = function(e){
			var t =  $("#textField").val();
			if ( t.length > 0 ){
				sb.send("text", "string", t);
				document.forms[0].textField.value = "";
			}
		}

		//-------------------------------------------------------
		this.onMousePressed = function( x,y ){
			this.onTouchStart(1,x,y);
		};

		//-------------------------------------------------------
		this.onMouseMoved = function( x,y ){
			this.onTouchMove(1,x,y);
		};

		//-------------------------------------------------------
		this.onMouseDragged = function( x,y ){
			this.onTouchMove(1,x,y);
		};

		//-------------------------------------------------------
		this.onMouseReleased = function( x,y ){
			this.onTouchEnd(1,x,y);
		};

		//-------------------------------------------------------
		this.onTouchStart = function( id, x,y ){
			console.log("ys");
			switch ( this.mode ){
				case 0:
					this.updateAndSendColor(x,y);
					break;
				case 1:
					if ( this.touchId == -1 ){
						this.touchId = id;
						//this.currentDrawing.push({x:x, y:y});
						
						// are we over any grid squares?
						var ind = this.checkGrid(x,y);
						if ( ind != -1 ){
							this.gridIndices.push(ind);
						}
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
						//this.currentDrawing.push({x:x, y:y});
						var ind = this.checkGrid(x,y);
						if ( ind != -1 ){
							this.gridIndices.push(ind);
						}
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

						// send grid obj + color
						sb.send("grid", "grid", {grid: this.grid, color: this.color, indices:this.gridIndices })

						for ( var i=0; i<this.grid.length; i++){
							this.grid[i].filled = false;
						}

						this.gridIndices = [];
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
						this.showHide("#color");
						this.registerTouchEvents();
						break;

					// grid
					case 1:
						this.showHide("#grid");
						this.registerTouchEvents();
						break;

					// text
					case 2:
						this.showHide("#text");
						this.unregisterTouchEvents();
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

		this.showHide = function( show ) {
			for ( var i=0; i<this.modes.length; i++){
				$(this.modes[i]).css("visibility", "hidden");
				$(this.modes[i]).css("display", "none");
			}

			$(show).css("visibility", "visible");
			$(show).css("display", "block");
		}

	}
	
