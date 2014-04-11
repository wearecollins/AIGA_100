SUD.require("js/clock.js");

var app;
var isIpad = false;

$(document).ready( function() {
	DemoApp.prototype = $.extend(true, SUD.app.ThreeApp.prototype, DemoApp.prototype);

	isIpad = navigator.userAgent.match(/iPad/i) != null;

	console.log(isIpad);

	app 	= new DemoApp();
	//var cv = document.getElementById("gridCanvas");
	app.renderWidth = isIpad ? 2048 : 640;
	app.renderHeight = isIpad ? 1536 : 1136;
	// cv.width = width;
	// cv.height = height;
	app.begin({canvasId:"gridCanvas", renderType: "css3d", width: app.renderWidth, height: app.renderHeight });
});

// ===========================================
// ===== DEMO APP
// ===========================================

	DemoApp = function(){
		SUD.app.ThreeApp.call( this );

		var sb;

		// color vars		
		this.r = 0;
		this.g = 0;
		this.b = 0;

		this.rE = 0;
		this.gE = 0;
		this.bE = 0;

		this.names = ["sean","noreen","chuck","dana","bob","david","ken","leslie","kyle","michael","stephen","abbott","louise","sylvia","cheryl","alex","chip","michael_m","richard","michael_d","nancye","bill_m","gael","ann"];
		
		//-------------------------------------------------------
		this.setup = function (){
			//this.unregisterMouseEvents();
			this.registerTouchEvents();

			// grid
			this.grid = [];
			this.meshes = [];

			// setup geom
			var w = (this.renderWidth * .8) / 10 * .7;
			var sp = w * .5;
			var cx = this.renderWidth / 2.0 - (((w + sp) * 9 )/2.0);
			var cy = this.renderHeight * .4 - (((w + sp)  * 9) /2.0);

			for ( var x=0; x<10; x++){
				for ( var y=0; y<10; y++){
					this.grid.push({x: x, y: y, filled: false });

					var clock = new Clock( cx + x * (w + sp), cy + y * (w + sp), w/2.0);
					this.scene.add(clock);
					this.meshes.push( clock );
				}
			}

			// ortho cam
			this.scene.remove(this.camera);
			this.camera = new THREE.OrthographicCamera( this._canvas.width / - 2, this._canvas.width / 2, this._canvas.height / 2, this._canvas.height / - 2, - 500, 1000 );
			this.scene.add(this.camera);

			this.renderer.setClearColor( 0xffffff );

			this.currentDrawing = [];
			this.touchId 		= -1;
			this.gridIndices = [];

			// this.gridCanvas = document.getElementById("gridCanvas");
			// todo: set w/h
			//this.gridCtx 	= this.gridCanvas.getContext('2d');

			this.r = SUD.randomInt(0,255);
			this.g = SUD.randomInt(0,255);
			this.b = SUD.randomInt(0,255);

			this.modes = [];
			this.modes.push("#grid");
			this.modes.push("#quote");

			window.scrollTo(0,1);
			this.mode = 0;

			// add "touch" div
			var touchDiv = document.createElement("div");
			touchDiv.className = "touch";
			touchDiv.id = "touch";
			touchDiv.innerHTML = "Touch";

			var releaseDiv = document.createElement("div");
			releaseDiv.className = "touch";
			releaseDiv.id = "release";
			releaseDiv.innerHTML = "Release";

			document.getElementById("sud_container").appendChild(touchDiv);
			document.getElementById("sud_container").appendChild(releaseDiv);
			$("#touch").css("opacity", 1);

			this.setupSpacebrew();
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
			
			// stuff we're subscribing to
			sb.addSubscribe( "mode", "range" );
			sb.addSubscribe( "name", "range");

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

			for ( var ind in this.meshes ){
				this.meshes[ind].update();
			}
		}

		//-------------------------------------------------------
		this.draw = function (){
			switch( this.mode ){
				// grid
				case 0:
					break;

				// quotes
				case 1:
					break;
			}
			this.camera.lookAt( this.scene.position );
			this.renderer.render( this.scene, this.camera );//, null, true );
		}	

		//-------------------------------------------------------
		this.checkGrid = function(x,y){
			var w = (this.renderWidth * .8) / 10 * .7;
			var sp = w * .5;
			var cx = this.renderWidth / 2.0 - (((w + sp) * 9 )/2.0);
			var cy = this.renderHeight * .4 - (((w + sp)  * 9) /2.0);

			for ( var i=0; i<this.grid.length; i++){
				var rx = cx + this.grid[i].x * (w + sp);
				var ry = cy + this.grid[i].y * (w + sp);

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
			var t = document.forms[0].textField.value;
			console.log(t);
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
			// this.onTouchMove(1,x,y);
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

			switch ( this.mode ){
				case 0:
					if ( this.touchId == -1 ){
						this.touchId = id;
						$("#touch").css("opacity", 0);
						$("#release").css("opacity", 1);
						//this.currentDrawing.push({x:x, y:y});
						
						for ( var ind in this.meshes ){
							this.meshes[ind].magnet(x,y);
						}

						// are we over any grid squares?
						var ind = this.checkGrid(x,y);
						if ( ind != -1 ){
							this.gridIndices.push(ind);
							if ( ind != this.lastGrid ){
								this.meshes[ind].color.r = this.r;
								this.meshes[ind].color.g = this.g;
								this.meshes[ind].color.b = this.b;
							}
						}
						this.lastGrid = ind;
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
					if ( id == this.touchId ){
						//this.currentDrawing.push({x:x, y:y});
						var ind = this.checkGrid(x,y);
						if ( ind != -1 ){
							this.gridIndices.push(ind);
							if ( ind != this.lastGrid ){
								this.meshes[ind].color.r = this.r;
								this.meshes[ind].color.g = this.g;
								this.meshes[ind].color.b = this.b;
							}
						}
						this.lastGrid = ind;

						for ( var ind in this.meshes ){
							this.meshes[ind].magnet(x,y);
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
					if ( id == this.touchId ){
						this.touchId = -1;

						// send grid obj + color
						sb.send("grid", "grid", {grid: this.grid, color: {r:this.r, g:this.g, b:this.b}, indices:this.gridIndices })

						for ( var i=0; i<this.grid.length; i++){
							this.grid[i].filled = false;
						}

						$("#touch").css("opacity", 1);
						$("#release").css("opacity", 0);

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
					// grid
					case 0:
						window.clearTimeout(window.modeTimeout);
						$("#names").css("opacity", 0);
						$("#quotes").css("opacity", 0);
						$("#bigClock").css("opacity", 0);

						window.modeTimeout = window.setTimeout(function(){
							$("#sud_container").css("opacity", 1);
						}, 1500);

						break;

					// quotes
					case 1:
						window.clearTimeout(window.modeTimeout);
						$("#sud_container").css("opacity", 0);
						$("#quotes").css("opacity", 0);

						window.modeTimeout = window.setTimeout(function(){
							$("#names").css("opacity", 1);
							window.modeTimeout = window.setTimeout(function(){
								console.log("clock");
								$("#names").css("opacity", 0);
								$("#bigClock").css("opacity", 1);

								window.modeTimeout = window.setTimeout(function(){
									console.log("quote");
									$("#bigClock").css("opacity", 0);
									$("#quotes").css("opacity", 1);
								}, 1500 );
							}, 2000 );
						}, 1500 );
						break;
				}
			}else if ( name == "name" ){
				console.log( this.names[value]);
				if ( value < 24 ){
					this.showName(this.names[value]);
				}
			}
		}

		this.showName = function(name){
			$( "#names" ).children().css( "visibility", "hidden" );
			$( "#quotes" ).children().css( "visibility", "hidden" );
			$("#name_"+name).css("visibility", "visible");
			$("#quote_"+name).css("visibility", "visible");
		}

		this.showHide = function( show ) {
			for ( var i=0; i<this.modes.length; i++){
				$(this.modes[i]).css("visibility", "hidden");
				$(this.modes[i]).css("display", "none");
			}

			$(show).css("visibility", "visible");
			$(show).css("display", "block");
		}

	}
	