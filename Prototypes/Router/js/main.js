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
	this.sb = new Spacebrew.Client();

	// add admin functionality
	this.sb.extend(Spacebrew.Admin);
	this.sb.onNewClient 	= this.onNewClient.bind(this);
	this.sb.onRemoveClient 	= this.onRemoveClient.bind(this);

	this.sb.onOpen = function(){
		$("#status").html("Connected");
	}
	this.sb.onClose = function(){
		$("#status").html("Disconnected");
	}

	this.sb.connect();
};

GridRouter.prototype.onNewClient = function( client ){
	if ( name == "grid_display" ) return;
	count++;
	this.updateRoutes();
}

GridRouter.prototype.onRemoveClient = function( name, address ){
	if ( name == "grid_display" ) return;
	count--;
	this.updateRoutes();
}

Route.prototype.updateRoutes = function() {
	// route to outputs based on number
	var clientMinus = sb.admin.clients;
	for (var i=clientMinus.length-1; i>=0; i--){
		if ( clientMinus[i].name == "grid_display" ||
			 clientMinus[i].name == ""
	}
};

GridRouter.prototype.randomize = function() {
	// break old connections
	if ( this.currentSub || this.currentPub ){
		this.sb.removeRoute( this.currentPub, this.currentSub );
	}

	// get most current list
	var currentPublishers = this.sb.publishListByType(this.typeToRandomize);
	var currentSubscribers = this.sb.subscribeListByType(this.typeToRandomize);

	// remove ourselves from these lists
	for( var p in currentPublishers ){
		if ( this.sb.isThisApp(currentPublishers[p].clientName, currentPublishers[p].remoteAddress) ){
			currentPublishers.splice(currentPublishers.indexOf(p),1);
		}
	}
	for( var p in currentSubscribers ){
		if ( this.sb.isThisApp(currentSubscribers[p].clientName, currentSubscribers[p].remoteAddress) ){
			currentSubscribers.splice(currentSubscribers.indexOf(p),1);
		}
	}

	// randomize!
	var pubKeys = Object.keys(currentPublishers);
	this.currentPub = currentPublishers[pubKeys[pubKeys.length * Math.random() << 0]];

	var subKeys = Object.keys(currentSubscribers);
	this.currentSub = currentSubscribers[subKeys[subKeys.length * Math.random() << 0]];

	this.sb.addRoute( this.currentPub, this.currentSub );
	this.setHTML();
};

GridRouter.prototype.setHTML = function() {
};