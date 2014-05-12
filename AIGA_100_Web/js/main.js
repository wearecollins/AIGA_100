SUD.require("js/clocks.js");

var app;
var isIpad = false;

$(document).ready( function() {
	AIGA100.prototype = $.extend(true, SUD.app.ThreeApp.prototype, AIGA100.prototype);

	isIpad = navigator.userAgent.match(/iPad/i) != null;

	app 	= new AIGA100();

	app.begin();
});

// ===========================================
// ===== DEMO APP
// ===========================================

	AIGA100 = function(){
		SUD.app.ThreeApp.call( this );

		var sb;

		// color vars		
		this.r = 0;
		this.g = 0;
		this.b = 0;

		this.meshParams = {};

		var gridDrawings 	= [];
		var index 			= 0;
		var swipeChangeTimer    = 15;
		var maxDrawingAge     = 60 * 5;

		this.nameIndex = 0;
		this.names = ["sean","noreen","chuck","dana","bob","david","ken","leslie","kyle","michael","stephen","abbott","louise","sylvia","cheryl","alex","chip","michael_m","richard","michael_d","nancye","bill_m","gael","ann"];
		this.years = [1979, 1982, 1981, 1968, 1977, 1980, 1971, 1975, 1985, 1964, 1978, 1980, 1973, 1976, 1980, 1970, 1986, 1976, 1957, 1968, 1971, 1965, 1975, 1957]

		this.colors = [
			{ r:51, g:255, b:153, h:0 },
			{ r:204, g:0, b:0 , h:255 },
			{ r:255, g:206, b:0 , h:0 },
			{ r:153, g:0, b:204 , h:255 },
			{ r:255, g:153, b:51 , h:0 },
			{ r:255, g:108, b:12 , h:255 },
			{ r:255, g:102, b:255 , h:0 },
			{ r:102, g:0, b:102 , h:255 },
			{ r:0, g:0, b:153 , h:0 },
			{ r:247, g:50, b:63 , h:255 },
			{ r:0, g:255, b:51 , h:0 },
			{ r:0, g:0, b:255 , h:255 },
			{ r:0, g:204, b:204 , h:0 },
			{ r:255, g:0, b:0 , h:255 },
			{ r:102, g:0, b:255 , h:0 },
			{ r:255, g:0, b:102 , h:255 },
			{ r:255, g:0, b:153 , h:0 },
			{ r:0, g:51, b:102 , h:255 },
			{ r:158, g:158, b:158 , h:0 },
			{ r:102, g:0, b:51 , h:255 },
			{ r:255, g:51, b:0 , h:0 },
			{ r:51, g:102, b:255 , h:255 },
			{ r:0, g:255, b:204 , h:0 },
			{ r:0, g:153, b:0 , h:255 }
		]

		//-------------------------------------------------------
		this.setup = function (){

			// grid
			this.grid = [];
			this.meshes = [];

			this.renderer.setClearColor( 0x666666 );

			this.currentDrawing = [];
			this.touchId 		= -1;
			this.gridIndices = [];

			this.modes = [];
			this.modes.push("#grid");
			this.modes.push("#quote");

			this.mode = 0;

			this.setupSpacebrew();
			this.camera.lookAt( this.scene.position );

			// build clocks...
			this.clocks = new Clocks();
			// 				gridX, gridY, spacing, startX, startY, radius, radiusMult
			this.clocks.setup(10, 10, new THREE.Vector3(4,4,4), -20, -20, 1.5, 1);
			this.scene.add( this.clocks );
		}

		//-------------------------------------------------------
		this.setupSpacebrew = function() {
			var random_id = "0000" + Math.floor(Math.random() * 10000);
			var app_name = "AIGA100_" + random_id.substring(random_id.length-4);

			// create spacebrew client object
			sb = new Spacebrew.Client("spacebrew.robotconscience.com");
			sb.send_interval = 0;
			sb.name(app_name);

			// configure the publication and subscription feeds
			sb.addPublish( "mode", "range", 0 );
			sb.addPublish( "name", "range", 0);
			
			// stuff we're subscribing to
			sb.addSubscribe( "grid", "grid" );

			// override Spacebrew events - this is how you catch events coming from Spacebrew
			sb.onRangeMessage  = this.onRangeMessage.bind(this);
			sb.onCustomMessage = this.onCustomMessage.bind(this);

			sb.connect();
		};

		//-------------------------------------------------------
		this.update = function (){
			this.elapsedTime = Date.now() - this.startTime;
			// this should be in the clockmode!
			if ( gridDrawings.length != 0 ){//&& clockManager.isInteractive() ){
		        // first update indices
		        for ( var i=0; i<gridDrawings.length; i++){
		            if ( this.getElapsedTimeMillis() - gridDrawings[i].lastChanged > swipeChangeTimer ){
		                gridDrawings[i].index++;
		                if (  gridDrawings[i].index >= gridDrawings[i].indices.length ){
		                    gridDrawings[i].age = 1000;
		                    continue;
		                }
		                gridDrawings[i].lastChanged = this.getElapsedTimeMillis();
		            }
		            
		            var y = gridDrawings[i].indices[gridDrawings[i].index] % 10;
		            var x = Math.floor(gridDrawings[i].indices[gridDrawings[i].index]/10);
		            var ind = gridDrawings[i].indices[gridDrawings[i].index];//x + y * 10;
		            
		            var indO = gridDrawings[i].indices[gridDrawings[i].index];
		            if ( gridDrawings[i].grid[indO] ){
		                var mult = (maxDrawingAge-gridDrawings[i].age) / maxDrawingAge;
		                //                            static float mult = texW / 10.0;
		                var v = this.clocks.clocks[indO].position;
		                this.clocks.magnet(v.x, v.y, gridDrawings[i].color );
		            }
		            gridDrawings[i].age++;
		        }
		        
		        
		        for ( var i=gridDrawings.length-1; i>=0; i--){
		            if ( gridDrawings[i].age >= maxDrawingAge ){
		                gridDrawings.slice(i, 1);
		            }
		        }
		    }


			this.clocks.update();
			this.clocks.draw();
		}

		//-------------------------------------------------------
		this.draw = function (){
			this.renderer.render(this.scene, this.camera );
		}

		//-------------------------------------------------------
		this.onMousePressed = function( x,y ){}

		//-------------------------------------------------------
		this.onMouseMoved = function( x,y ){}

		//-------------------------------------------------------
		this.onMouseDragged = function( x,y ){}

		//-------------------------------------------------------
		this.onMouseReleased = function( x,y ){}

		//-------------------------------------------------------
		this.onRangeMessage = function( name, value ){
			
		}

		//-------------------------------------------------------
		this.onCustomMessage = function( name, val, type ){
	        
	        // drawing is json array...
	        var d = new GridDrawing();

	        // set color
	        if ( val["color"] ){
	        	d.color = new THREE.Color();
	            d.color.setRGB( val.color.r / 255.0, val.color.g / 255.0, val.color.b / 255.0 );
	        }
          
            for (var i=0; i<val["grid"].length; i++){
                var v = val["grid"][i];
                var filled = v["filled"] == true | v["filled"] == "true";
                d.grid.push(filled);
            }
	        
            for (var i=0; i<val["indices"].length; i++){
                var v = val["indices"][i];
                d.indices.push( v );
            }
	        
	        gridDrawings.push(d);
		}
	}
	
