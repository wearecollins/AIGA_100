SUD.require("js/clock.js");

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

			this.renderer.setClearColor( 0xffffff );

			this.currentDrawing = [];
			this.touchId 		= -1;
			this.gridIndices = [];

			this.modes = [];
			this.modes.push("#grid");
			this.modes.push("#quote");

			this.mode = 0;

			this.setupSpacebrew();
			this.camera.lookAt( this.scene.position );
		}

		//-------------------------------------------------------
		this.setupSpacebrew = function() {
			var random_id = "0000" + Math.floor(Math.random() * 10000);
			var app_name = "AIGA100_" + random_id.substring(random_id.length-4);

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
	}
	
