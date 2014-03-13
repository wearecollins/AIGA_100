var app;

$(document).ready( function() {
	DemoApp.prototype = $.extend(true, SUD.app.BaseApp.prototype, DemoApp.prototype);
	app 	= new DemoApp();
	app.begin();
});

// ===========================================
// ===== DEMO APP
// ===========================================

	DemoApp = function(){
		SUD.app.BaseApp.call( this );

		var sb;
		
		//-------------------------------------------------------
		this.setup = function (){
			//this.unregisterMouseEvents();
			this.registerTouchEvents();

			// setup spacebrew
			var random_id = "0000" + Math.floor(Math.random() * 10000);
			var app_name = "Swipe_" + random_id.substring(random_id.length-4);

			// create spacebrew client object
			sb = new Spacebrew.Client();
			sb.send_interval = 0;
			sb.name(app_name);

			// configure the publication and subscription feeds
			sb.addPublish( "drawing", "drawing", "[]" );
			
			sb.addSubscribe( "mode", "range" );
			sb.addSubscribe( "randomR", "range" );
			sb.addSubscribe( "randomG", "range" );
			sb.addSubscribe( "randomB", "range" );

			// override Spacebrew events - this is how you catch events coming from Spacebrew
			sb.onRangeMessage  = this.onRangeMessage.bind(this);

			sb.connect();

			this.currentDrawing = [];
			this.touchId 		= -1;

			this.canvas = document.getElementById("canvas");
			// todo: set w/h
			this.ctx 	= this.canvas.getContext('2d');
			window.scrollTo(0,1);
		}

		//-------------------------------------------------------
		this.update = function (){

		}

		//-------------------------------------------------------
		this.draw = function (){
			this.canvas.width = this.canvas.width;
			this.ctx.clearRect(0,0,this.canvas.width,this.canvas.height);
			if ( this.currentDrawing.length > 0 ){
				this.ctx.beginPath();
				this.ctx.moveTo(this.currentDrawing[0].x, this.currentDrawing[0].y);
				for ( var i=1; i<this.currentDrawing.length; i++){
					this.ctx.lineTo(this.currentDrawing[i].x, this.currentDrawing[i].y);
				}
				this.ctx.stroke();
			}
		}	

		//-------------------------------------------------------
		this.onTouchStart = function( id, x,y ){
			if ( this.touchId == -1 ){
				this.touchId = id;
				this.currentDrawing.push({x:x, y:y});
			}
		};

		//-------------------------------------------------------
		this.onTouchMove = function( id, x,y ){
			if ( id == this.touchId ){
				this.currentDrawing.push({x:x, y:y});
			}
		};

		//-------------------------------------------------------
		this.onTouchEnd = function( id, x,y ){
			if ( id == this.touchId ){
				this.touchId = -1;

				// normalize current drawing
				var cdNorm = [];
				for ( var i=0; i<this.currentDrawing.length; i++){
					cdNorm.push( { 
									x:this.currentDrawing.x/this.canvas.width, 
									y: this.currentDrawing.y / this.canvas.height
								});
				}

				sb.send("drawing","drawing", cdNorm);
				this.currentDrawing = [];
			}
		};

		//-------------------------------------------------------
		this.onRangeMessage = function( name, value ){
			//todo
		};

		
	}
	
