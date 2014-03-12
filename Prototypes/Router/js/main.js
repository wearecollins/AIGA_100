var app = null;
var count = 0;

$(document).ready(function(){
	app = new GridRouter();
});

var GridRouter = function(){
	this.sb 	= null;

	this.setupSpacebrew();
}

GridRouter.prototype.setupSpacebrew = function() {
	// setup with default paramz
	this.sb = new Spacebrew.Client("spacebrew.robotconscience.com", "router");

	// add admin functionality
	this.sb.extend(Spacebrew.Admin);
	this.sb.onNewClient 	= this.onNewClient.bind(this);
	this.sb.onRemoveClient 	= this.onRemoveClient.bind(this);
	// this.sb.debug = true;

	this.sb.onOpen = function(){
		$("#status").html("Connected");
	}
	this.sb.onClose = function(){
		$("#status").html("Disconnected");
	}

	this.sb.connect();
};
// TODO: Should be some way to understand if we've received any routes yet.
GridRouter.prototype.onNewClient = function( client ){
	if ( client.name == "grid_display" || client.name == "router" ){
		if ( client.name == "grid_display" ){
			grid = client;
		}
		return;
	}
	count++;
	if ( this.updateTimeout ) clearTimeout( this.updateTimeout );
	this.updateTimeout = setTimeout( this.updateRoutes.bind(this), 500);
}

GridRouter.prototype.onRemoveClient = function( name, address ){
	//if ( name == "grid_display" ) return;
	//count--;
	//if ( this.updateTimeout ) clearTimeout( this.updateTimeout );
	//this.updateTimeout = setTimeout( this.updateRoutes.bind(this), 500);
}


var grid;

GridRouter.prototype.removeAllRoutes = function() {
	// first remove all routes
	for (var i = this.sb.admin.routes.length-1; i >= 0; i--) {
		console.log( this.sb.admin.routes[i] );
		if ( this.sb.admin.routes[i] ) this.sb.removeRoute(this.sb.admin.routes[i].route.publisher, this.sb.admin.routes[i].route.subscriber);
	}
};

GridRouter.prototype.updateRoutes = function() {
	// route to outputs based on number
	var clientMinus = this.sb.admin.clients.slice(0);
	for (var i=clientMinus.length-1; i>=0; i--){
		if ( clientMinus[i].name == "grid_display" || clientMinus[i].name == "router" ){
			clientMinus.splice(i,1);
		}
	}

	if ( clientMinus.length == 0 || grid === undefined ) return;

	console.log("RM "+this.sb.admin.routes.length);

	this.removeAllRoutes();

	// set timeout to add new ones
	if ( this.setupTimeout ) clearTimeout( this.setupTimeout );
	this.setupTimeout = setTimeout( this.setupGridRoutes.bind(this), 500 );
};

GridRouter.prototype.setupGridRoutes = function() {
	// route to outputs based on number
	var clientMinus = this.sb.admin.clients.slice(0);
	for (var i=clientMinus.length-1; i>=0; i--){
		if ( clientMinus[i].name == "grid_display" || clientMinus[i].name == "router" ){
			clientMinus.splice(i,1);
		}
	}

	if ( clientMinus.length == 0 || grid === undefined ) return;

	var div = 10/clientMinus.length;
	var w 	= 0;
	var who = clientMinus[w];

	// add routes dep on number
	for ( var y =0; y<10; y ++){
		if ( y - (w * div) >= div ){
			w++;
			who = clientMinus[w];
			console.log( clientMinus[w] );
		}
		for ( var x =0; x<10; x ++){
			var ind = x + y * 10;
			var routeBase = "dot_"+ind;
			this.sb.addRoute( who.name, who.remoteAddress, "red", grid.name, grid.remoteAddress,routeBase+"_r");
			this.sb.addRoute( who.name, who.remoteAddress, "green", grid.name, grid.remoteAddress, routeBase+"_g" );
			this.sb.addRoute( who.name, who.remoteAddress, "blue", grid.name, grid.remoteAddress, routeBase+"_b" );
		}	
	}
	console.log(div +":"+ this.sb.admin.routes.length);
};
