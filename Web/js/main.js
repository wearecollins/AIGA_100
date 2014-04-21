SUD.require("js/clock.js");

var app;
var isIpad = false;

$(document).ready( function() {
	DemoApp.prototype = $.extend(true, SUD.app.ThreeApp.prototype, DemoApp.prototype);

	isIpad = navigator.userAgent.match(/iPad/i) != null;

	console.log("is ipad? "+isIpad);

	app 	= new DemoApp();
	//var cv = document.getElementById("gridCanvas");
	app.renderWidth = isIpad ? 1536 : 640;
	app.renderHeight = isIpad ? 1620 : 700;
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

		this.meshParams = {};

		/** */
		this.bigCircle = null;

		this.nameIndex = 0;
		this.names = ["sean","noreen","chuck","dana","bob","david","ken","leslie","kyle","michael","stephen","abbott","louise","sylvia","cheryl","alex","chip","michael_m","richard","michael_d","nancye","bill_m","gael","ann"];
		this.years = [1979, 1982, 1981, 1968, 1977, 1980, 1971, 1975, 1985, 1964, 1978, 1980, 1973, 1976, 1980, 1970, 1986, 1976, 1957, 1968, 1971, 1965, 1975, 1957]

		//-------------------------------------------------------
		this.setup = function (){
			//this.unregisterMouseEvents();
			this.registerTouchEvents();

			// grid
			this.grid = [];
			this.meshes = [];

			// setup geom
			var w = (this.renderWidth * .75) / 10 * .7;
			var sp = w * .5;
			var cx = this.renderWidth / 2.0 - (((w + sp) * 9 )/2.0);
			var cy = cx * 1.5;// - (((w + sp)  * 9) /2.0);

			// build clocks + storage
			for ( var x=0; x<10; x++){
				for ( var y=0; y<10; y++){
					this.grid.push({x: x, y: y, filled: false });

					var clock = new Clock( cx + x * (w + sp), cy + y * (w + sp), w/2.0);
					this.scene.add(clock);
					this.meshes.push( clock );
				}
			}

			// populate mesh param storage obj
			this.meshParams.radius = w;
			this.meshParams.spacing = sp;
			this.meshParams.x = cx;
			this.meshParams.y = cy;

			// "big clock"
				
			this.createBigClock();

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

			var rgb = HSVtoRGB(SUD.random(0,1), .7, .9);

			this.r = rgb.r;
			this.g = rgb.g;
			this.b = rgb.b;

			this.modes = [];
			this.modes.push("#grid");
			this.modes.push("#quote");

			window.scrollTo(0,1);
			this.mode = 0;

			// add "touch" div
			var touchDiv = document.createElement("div");
			touchDiv.className = "threeElement touch";
			touchDiv.id = "touch";
			touchDiv.innerHTML = "Touch";

			var releaseDiv = document.createElement("div");
			releaseDiv.className = "threeElement touch";
			releaseDiv.id = "release";
			releaseDiv.innerHTML = "Release";

			document.getElementById("sud_container").appendChild(touchDiv);
			document.getElementById("sud_container").appendChild(releaseDiv);
			$("#touch").css("opacity", 1);

			if (isIpad){
				$("#quotes").addClass("quotesiPad");
				$(".name").addClass("nameiPad");
				$("#touch").addClass("touchiPad");
				$("#release").addClass("touchiPad");
				$(".quoteInner").addClass("quoteInneriPad");
				$(".quoteName").addClass("quoteNameiPad");
				$(".italic").addClass("italiciPad");
			}

			$('.quoteInner').widowFix();

			this.setupSpacebrew();
		}

		//-------------------------------------------------------
		this.createBigClock = function(){
			var bigCircle = document.createElement("div");
			var rad = this.renderWidth * .8 / 2.0;
			setCircleStyle(bigCircle, rad, true, isIpad ? 40 : 20, "#FD4F57");
			bigCircle.id = "bigclock_visual";
			bigCircle.className = "bigClock";
			document.getElementById("sud_container").appendChild(bigCircle);
			this.bigCircle = new THREE.CSS3DObject(bigCircle);
			this.bigCircle.position.x = this.renderWidth/2.0;
			this.bigCircle.position.y = this.renderHeight/2.0;
			this.scene.add(this.bigCircle);

			var ad1 = document.createElement("div");
			setLineStyle(ad1, isIpad ? 30 : 15, rad * .8, "#FD4F57");
			ad1.id = "clockArm1";
			ad1.className = "bigClock";
			document.getElementById("sud_container").appendChild(ad1);

			var ad2 = document.createElement("div");
			setLineStyle(ad2, isIpad ? 30 : 15, rad * .8, "#FD4F57");
			ad2.id = "clockArm2";
			ad2.className = "bigClock";
			document.getElementById("sud_container").appendChild(ad2);

			this.armOne = new THREE.CSS3DObject(ad1);
			this.armTwo = new THREE.CSS3DObject(ad2);
			this.bigCircle.add(this.armOne);
			this.bigCircle.add(this.armTwo);
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
			for ( var i=0; i<this.grid.length; i++){
				if ( this.meshes[i].mousePressed(x,y)){
					this.grid[i].filled = true;
					return i;
				}
			}
			return -1;
		}

		this.rotateBigClockTo = function(deg1, deg2){
			this.armOne.rotation.z = SUD.degToRad(180 + deg1);
			this.armTwo.rotation.z = SUD.degToRad(180 + deg2);

			// to-do: make sure rotation is always positive!!!
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

						for ( var i=0; i<this.meshes.length; i++){
							this.meshes[i].mouseReleased();
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
				if ( this.mode == value ) return;
				this.mode = value;
				switch( value ){
					// grid
					case 0:
						this.touchId = -1;
						window.clearTimeout(window.modeTimeout);
						$("#names").css("opacity", 0);
						$("#quotes").css("opacity", 0);
						$(".bigClock").css("opacity", 0);

						window.modeTimeout = window.setTimeout(function(){
							$(".threeElement").css("opacity", 1);
							$("#release").css("opacity", 0);
						}, 1500);

						break;

					// quotes
					case 1:
						window.clearTimeout(window.modeTimeout);
						$(".threeElement").css("opacity", 0);
						$("#quotes").css("opacity", 0);

						window.modeTimeout = window.setTimeout(function(){
							for ( var i=0; i<this.meshes.length; i++){
								this.meshes[i].mouseReleased();
							}
							$("#names").css("opacity", 1);
							window.modeTimeout = window.setTimeout(function(){
								console.log("clock");
								$("#names").css("opacity", 0);
								$(".bigClock").css("opacity", 1);
								this.rotateBigClockTo(0, (2014 - this.years[this.nameIndex])/100*360);

								window.modeTimeout = window.setTimeout(function(){
									console.log("quote");
									$(".bigClock").css("opacity", 0);
									$("#quotes").css("opacity", 1);
									this.rotateBigClockTo(0, 0);
									window.textTimeout = window.setTimeout(function(){
										var $q  = $("#quote_"+this.names[this.nameIndex]);
										var $qa = $q.find(".quoteInner");
										var $qn = $q.find(".quoteName");
										if ( $qa.height() > $q.height()){
											$qa.css("top", ($q.height() - $qa.height()- 100) +"px")
											$qn.css("top", ($q.height() - $qa.height()- 100) +"px")
										}
										
									}.bind(this), 5000);
								}.bind(this), 3000 );
							}.bind(this), 2000 );
						}.bind(this), 1500 );
						break;
				}
			}else if ( name == "name" ){
				// console.log( this.names[value]);
				if ( value < 24 ){
					this.nameIndex = value;
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
	
