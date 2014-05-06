// sudjs-0.4.2.js - http://github.com/labatrockwell/LAB_JS_Framework
var SUD=SUD||{};SUD.global=this;SUD.version="r2";var included={};SUD.inherit=function(a){function b(){}if(null!=a){if(Object.create)return Object.create(a);var c=typeof a;if("object"!==c&&"function"!==c)throw TypeError();b.prototype=a;return new b}};SUD.writeScriptTag_=function(a){SUD.global.document.write('<script type="text/javascript" src="'+a+'"><\/script>');return included[a]=!0};SUD.require=function(a){a in included?console.log("already included"):SUD.writeScriptTag_(a)};
SUD.getScriptPath=function(a){for(var b=document.getElementsByTagName("script"),c=b.length-1;0<=c;--c){var d=b[c].src,e=d.length,g=a.length;if(d.substr(e-g)==a)return d.substr(0,e-g)}return""};SUD.toScriptPath=function(a){return SUD.src+a+".js"};SUD.src=SUD.getScriptPath("sudBase.js");SUD.SHIFT=16;SUD.CONTROL=17;SUD.ALT=18;SUD.COMMAND=91;SUD=SUD||{};SUD.random=function(a,b){return Math.random()*(b-a)+a};SUD.randomInt=function(a,b){return Math.floor(SUD.random(a,b))};
SUD.randomObject=function(a){return a[Math.min(SUD.randomInt(0,a.length),a.length-1)]};SUD.map=function(a,b,c,d,e){return d+(a-b)/(c-b)*(e-d)};SUD.lerp=function(a,b,c){return a+(b-a)*c};SUD.lerpObjects=function(a,b,c){var d={},e;for(e in a)d[e]=b.hasOwnProperty(e)?SUD.lerp(a[e],b[e],c):a[e];return d};SUD.wrap=function(a,b,c){if(b>c)var d=b,b=c,c=d;d=c-b;return 0==d?c:a-d*Math.floor((a-b)/d)};SUD.clamp=function(a,b,c){return Math.min(Math.max(a,b),c)};SUD.degToRad=function(a){return 0.0174532925*a};
SUD.radToDeg=function(a){return 57.2957795*a};Number.prototype.sign=function(){return 0<this?1:0>this?-1:0};SUD.DEBUG=0;SUD.WARNING=1;SUD.ERROR=2;SUD.FATAL_ERROR=3;SUD.logLevel=SUD.WARNING;SUD.setLogLevel=function(a){if(a>SUD.FATAL_ERROR)a=SUD.FATAL_ERROR;if(a<SUD.DEBUG)a=SUD.DEBUG;SUD.logLevel=a};SUD.log=function(a,b){b=null==b?SUD.DEBUG:b;SUD.logLevel>b||(window.console&&window.console.log?window.console.log(a):window.dump&&window.dump(a))};
if(!window.requestAnimationFrame)window.requestAnimationFrame=function(){return window.webkitRequestAnimationFrame||window.mozRequestAnimationFrame||window.oRequestAnimationFrame||window.msRequestAnimationFrame||function(a){window.setTimeout(a,1E3/60)}}();SUD.getQueryString=function(a,b){null==b&&(b="");var a=a.replace(/[\[]/,"\\[").replace(/[\]]/,"\\]"),c=RegExp("[\\?&]"+a+"=([^&#]*)").exec(window.location.href);return null==c?b:c[1]};SUD.alphabet=SUD.alphabet||"#,A,B,C,D,E,F,G,H,I,J,K,L,M,N,O,P,Q,R,S,T,U,V,W,X,Y,Z".split(",");
if(!Function.prototype.bind)Function.prototype.bind=function(a){if("function"!==typeof this)throw new TypeError("Function.prototype.bind - what is trying to be bound is not callable");var b=Array.prototype.slice.call(arguments,1),c=this,d=function(){},e=function(){return c.apply(this instanceof d?this:a||window,b.concat(Array.prototype.slice.call(arguments)))};d.prototype=this.prototype;e.prototype=new d;return e};
window.namespace=function(a){var a=a.split("."),b=window,c;for(c=0;c<a.length;c+=1)"undefined"===typeof b[a[c]]&&(b[a[c]]={}),b=b[a[c]];return b};SUD.EventDispatcher=function(a){var b=a||null;this.getTarget=function(){return b};this.events=[]};
SUD.EventDispatcher.prototype={constructor:SUD.EventDispatcher,addEventListener:function(a,b){this.events[a]=this.events[a]||[];this.events[a]&&this.events[a].push(b)},removeEventListener:function(a,b){if(this.events[a])for(var c=this.events[a],d=c.length-1;0<=d;--d)if(c[d]===b)return c.splice(d,1),!0;return!1},hasEventListener:function(a){return this.events[a]?!0:!1},dispatchEvent:function(a,b){if("string"===typeof a)var c=new SUD.Event(a);else if("object"===typeof a)c=a;else throw"First parameter of dispatchEvent must be of type object or string";
c.target=this.getTarget();for(var d in b)c[d.toString()]=b[d];if(this.events[c.type]){d=this.events[c.type];for(var e=d.length;e--;)d[e](c);return!0}return!1}};SUD.Event=function(a){this.type=a;this.target=null};SUD.Event.CONNECTED="connected";SUD.Event.CHANGE="change";SUD.Event.COMPLETE="complete";
SUD.TouchGestureHandler=function(){function a(c){window.addEventListener("mouseup",d);f.addEventListener("mousemove",b);f.removeEventListener("mousedown",a);i=c.clientX;j=c.clientY;g(SUD.TouchEvent.PRESS);x=(new Date).getTime();h=new SUD.geom.Point(c.clientX,c.clientY)}function b(a){i=a.clientX;j=a.clientY;var b=new Date,a=b.getTime(),c=new SUD.geom.Point(i,j),d=e(c,h),f=m(c.x-h.x,h.y-c.y);d>D&&(l||(b=new Date,y=b.getTime(),n=new SUD.geom.Point(i,j)),l=!0);d>E&&(k=d,z=f,b=0<k&&0<a-t?k/(a-t):-1,-1<
b&&o.push(b),p=!0);p&&g(SUD.TouchEvent.DRAG);h=new SUD.geom.Point(c.x,c.y);t=a}function c(a){a.target==f&&(f.removeEventListener("mouseout",c),g(SUD.TouchEvent.OUT))}function d(c){i=c.clientX;j=c.clientY;f.addEventListener("mousedown",a);f.removeEventListener("mousemove",b);window.removeEventListener("mouseup",d);u=(new Date).getTime();var c=u-x,C=u-y;c<=F&&!p&&!l&&g(SUD.TouchEvent.TAP);for(var s=0,h=0,k=o.length;h<k;h++)s+=o[h];v=s/o.length;C<=w&&l&&(q=new SUD.geom.Point(i,j),A=m(q.x-n.x,n.y-q.y),
r=e(q,n),B=0<r&&0<c?r/c:-1,g(SUD.TouchEvent.FLICK));g(SUD.TouchEvent.RELEASE);l=p=!1;v=[]}function e(a,b){return Math.sqrt(Math.pow(b.x-a.x,2)+Math.pow(b.y-a.y,2))}function g(a){G.dispatchEvent(new SUD.TouchEvent(a))}function m(a,b){var c=Math.atan2(b,a);0>c&&(c+=6.283185);return 57.29578*c}SUD.EventDispatcher.call(this,this);var w=250,F=80,D=20,E=5,G=this,f=null,p=!1,l=!1,h,A,r,n,q,B,y,x,u,t=-1,v=[],o=[],z,k,i,j;this.register=function(b){f=b;f.addEventListener("mousedown",a)};this.unregister=function(){null!==
f&&(f.removeEventListener("mousedown",a),f.removeEventListener("mouseout",c),f.removeEventListener("mousemove",b),window.removeEventListener("mouseup",d))};this.getDragDirection=function(){return z};this.getDragDistance=function(){return k};this.getFlickDirection=function(){return A};this.getFlickDistance=function(){return r};this.getTouchX=function(){return i};this.getTouchY=function(){return j};this.getAverageVelocity=function(){return v};this.getFlickVelocity=function(){return B}};
SUD.TouchGestureHandler.prototype=SUD.inherit(SUD.EventDispatcher.prototype);SUD.TouchGestureHandler.prototype.constructor=SUD.TouchGestureHandler;SUD.TouchEvent=function(a){SUD.Event.call(this,a)};SUD.TouchEvent.TAP="touchtap";SUD.TouchEvent.FLICK="touchflick";SUD.TouchEvent.DRAG="touchdrag";SUD.TouchEvent.RELEASE="touchrelease";SUD.TouchEvent.OUT="touchout";SUD.TouchEvent.PRESS="touchPress";SUD.TouchEvent.prototype=SUD.inherit(SUD.Event.prototype);SUD.TouchEvent.prototype.constructor=SUD.TouchEvent;
SUD.agents=SUD.agents||{};SUD.agents.Agent=function(){};SUD.agents.Agent.prototype.setup=function(a,b,c,d,e,g,m,w){this.pos=[];this.myID=a||0;this.rotation=e||0;this.currentWidth=g||0;this.currentHeight=m||0;this.isVisible=w||!0;this.yScale=this.xScale=1;this.pos.x=b||0;this.pos.y=c||0;this.pos.z=d||0;this.Behaviors=[]};SUD.agents.Agent.prototype.addBehavior=function(a){this.Behaviors.push(a);a.setup(this);console.log(this.Behaviors)};SUD.agents.Agent.prototype.removeBehavior=function(){};
SUD.agents.Agent.prototype.applyBehaviors=function(){for(var a=0,a=0;a<this.Behaviors.length;a++)this.Behaviors[a].apply(this)};SUD.agents.Agent.prototype.checkIt=function(){console.log("check!")};SUD.agents=SUD.agents||{};SUD.agents.Behavior=function(){};SUD.agents.Behavior.prototype.checkIt=function(){console.log("behaving")};SUD.agents=SUD.agents||{};SUD.agents.Group=function(){this.Agents=[]};SUD.agents.Group.prototype.addToGroup=function(a){console.log(this.Agents);this.Agents.push(a)};
SUD.agents.Group.prototype.update=function(){for(var a=0,a=0;a<this.Agents.length;a++)this.Agents[a].update(),this.Agents[a].applyBehaviors()};SUD.agents.Group.prototype.draw=function(){for(var a=0,a=0;a<this.Agents.length;a++)this.Agents[a].draw()};var thing=$.extend(!0,SUD.agents.Group.prototype,SUD.agents.Agent.prototype);SUD.app=SUD.app||{};
SUD.app.BaseApp=function(){SUD.EventDispatcher.call(this,this);this.mouse={x:0,y:0,bDown:!1};this.touches={};this.windowEventsRegistered=this.keyEventsRegistered=this.touchEventsRegistered=this.mouseEventsRegistered=!1;this.startTime=Date.now();this.elapsedTime=0};SUD.app.BaseApp.prototype=new SUD.EventDispatcher;SUD.app.BaseApp.prototype.constructor=SUD.app.BaseApp;SUD.app.BaseApp.prototype.supr=SUD.EventDispatcher.prototype;
SUD.app.BaseApp.prototype.begin=function(){this.registerWindowEvents();this.registerKeyEvents();this.registerMouseEvents();this.setup();this.animate()};SUD.app.BaseApp.prototype.setup=function(){};SUD.app.BaseApp.prototype.preupdate=function(){};SUD.app.BaseApp.prototype.update=function(){};SUD.app.BaseApp.prototype.predraw=function(){};SUD.app.BaseApp.prototype.draw=function(){};SUD.app.BaseApp.prototype.postdraw=function(){};
SUD.app.BaseApp.prototype.animate=function(a){if(void 0!==a)this.elapsedTime=a-this.startTime;requestAnimationFrame(this.animate.bind(this),this);this.preupdate();this.update();this.predraw();this.draw();this.postdraw()};
SUD.app.BaseApp.prototype.registerWindowEvents=function(){if(!this.windowEventsRegistered)window.addEventListener("focus",this._onWindowFocus.bind(this)),window.addEventListener("blur",this._onWindowBlur.bind(this)),window.addEventListener("resize",this._onWindowResized.bind(this)),this.windowEventsRegistered=!0};
SUD.app.BaseApp.prototype.unregisterWindowEvents=function(){if(this.windowEventsRegistered)window.removeEventListener("focus",this._onWindowFocus.bind(this)),window.removeEventListener("blur",this._onWindowBlur.bind(this)),window.removeEventListener("resize",this._onWindowResized.bind(this)),this.windowEventsRegistered=!1};SUD.app.BaseApp.prototype._onWindowResized=function(){this.onWindowResized(window.innerWidth,window.innerHeight)};SUD.app.BaseApp.prototype.onWindowResized=function(){};
SUD.app.BaseApp.prototype._onWindowFocus=function(){this.onWindowFocus()};SUD.app.BaseApp.prototype.onWindowFocus=function(){};SUD.app.BaseApp.prototype._onWindowBlur=function(){this.onWindowBlur()};SUD.app.BaseApp.prototype.onWindowBlur=function(){};
SUD.app.BaseApp.prototype.registerMouseEvents=function(){if(!this.mouseEventsRegistered)window.addEventListener("mousemove",this._onMouseMoved.bind(this)),window.addEventListener("mousedown",this._onMousePressed.bind(this)),window.addEventListener("mouseup",this._onMouseReleased.bind(this)),this.mouseEventsRegistered=!0};
SUD.app.BaseApp.prototype.unregisterMouseEvents=function(){if(this.mouseEventsRegistered)window.removeEventListener("mousemove",this._onMouseMoved.bind(this)),window.removeEventListener("mousedown",this._onMousePressed.bind(this)),window.removeEventListener("mouseup",this._onMouseReleased.bind(this)),this.mouseEventsRegistered=!1};SUD.app.BaseApp.prototype.onMouseMoved=function(){};SUD.app.BaseApp.prototype.onMousePressed=function(){};SUD.app.BaseApp.prototype.onMouseDragged=function(){};
SUD.app.BaseApp.prototype.onMouseReleased=function(){};SUD.app.BaseApp.prototype._onMouseMoved=function(a){this.mouse.bDown?this._onMouseDragged(a):(this.mouse.x=a.clientX,this.mouse.y=a.clientY,this.dispatchEvent("onMouseMoved",this.mouse),this.onMouseMoved(this.mouse.x,this.mouse.y))};SUD.app.BaseApp.prototype._onMousePressed=function(a){this.mouse.x=a.clientX;this.mouse.y=a.clientY;this.mouse.bDown=!0;this.dispatchEvent("onMousePressed",this.mouse);this.onMousePressed(this.mouse.x,this.mouse.y)};
SUD.app.BaseApp.prototype._onMouseReleased=function(a){this.mouse.x=a.clientX;this.mouse.y=a.clientY;this.mouse.bDown=!1;this.dispatchEvent("onMouseReleased",this.mouse);this.onMouseReleased(this.mouse.x,this.mouse.y)};SUD.app.BaseApp.prototype._onMouseDragged=function(a){this.mouse.x=a.clientX;this.mouse.y=a.clientY;this.dispatchEvent("onMouseDragged",this.mouse);this.onMouseDragged(this.mouse.x,this.mouse.y)};
SUD.app.BaseApp.prototype.registerTouchEvents=function(){if(!this.touchEventsRegistered)window.addEventListener("touchstart",this._onTouchStart.bind(this)),window.addEventListener("touchmove",this._onTouchMove.bind(this)),window.addEventListener("touchend",this._onTouchEnd.bind(this)),this.touchEventsRegistered=!0};
SUD.app.BaseApp.prototype.unregisterTouchEvents=function(){if(this.touchEventsRegistered)window.removeEventListener("touchstart",this._onTouchStart.bind(this)),window.removeEventListener("touchmove",this._onTouchMove.bind(this)),window.removeEventListener("touchend",this._onTouchEnd.bind(this)),this.touchEventsRegistered=!1};SUD.app.BaseApp.prototype.onTouchMove=function(){};SUD.app.BaseApp.prototype.onTouchStart=function(){};SUD.app.BaseApp.prototype.onTouchEnd=function(){};
SUD.app.BaseApp.prototype._onTouchMove=function(a){a.preventDefault();for(var b=0;b<a.touches.length;b++){var c=a.touches[b].identifier;this.touches[c].x=a.touches[b].pageX;this.touches[c].y=a.touches[b].pageY;this.dispatchEvent("onTouchMove",this.touches[c]);this.onTouchMove(c,this.touches[c].x,this.touches[c].y)}};
SUD.app.BaseApp.prototype._onTouchStart=function(a){a.preventDefault();for(var b=0;b<a.touches.length;b++){var c=a.touches[b].identifier;this.touches[c]={};this.touches[c].x=a.touches[b].pageX;this.touches[c].y=a.touches[b].pageY;this.dispatchEvent("onTouchStart",this.touches[c]);this.onTouchStart(c,this.touches[c].x,this.touches[c].y)}};
SUD.app.BaseApp.prototype._onTouchEnd=function(a){a.preventDefault();for(var b=0;b<a.changedTouches.length;b++){var c=a.changedTouches[b].identifier;delete this.touches[c];var d={};d.x=a.changedTouches[b].pageX;d.y=a.changedTouches[b].pageY;this.dispatchEvent("onTouchEnd",d);this.onTouchEnd(c,d.x,d.y)}};
SUD.app.BaseApp.prototype.registerKeyEvents=function(){if(!this.keyEventsRegistered)window.addEventListener("keydown",this._onDocumentKeyDown.bind(this)),window.addEventListener("keyup",this._onDocumentKeyUp.bind(this)),this.keyEventsRegistered=!0};
SUD.app.BaseApp.prototype.unregisterKeyEvents=function(){if(this.keyEventsRegistered)window.removeEventListener("keydown",this._onDocumentKeyDown.bind(this)),window.removeEventListener("keyup",this._onDocumentKeyUp.bind(this)),this.keyEventsRegistered=!1};SUD.app.BaseApp.prototype.onDocumentKeyDown=function(){};SUD.app.BaseApp.prototype._onDocumentKeyDown=function(a){this.dispatchEvent("onDocumentKeyDown",a);this.onDocumentKeyDown(a.keyCode)};SUD.app.BaseApp.prototype.onDocumentKeyUp=function(){};
SUD.app.BaseApp.prototype._onDocumentKeyUp=function(a){this.dispatchEvent("onDocumentKeyUp",a);this.onDocumentKeyUp(a.keyCode)};SUD.getElapsedTimeMillis=SUD.app.BaseApp.prototype.getElapsedTimeMillis=function(){return this.elapsedTime};SUD.getElapsedTimeSeconds=SUD.app.BaseApp.prototype.getElapsedTimeSeconds=function(){return this.elapsedTime/1E3};SUD.geom=SUD.geom||{};SUD.geom.Point=function(a,b,c){this.x=a||0;this.y=b||0;this.z=c||0};SUD.geom=SUD.geom||{};
SUD.geom.Rect=function(a,b,c,d,e){this.x=a||0;this.y=b||0;this.z=c||0;this.width=d||0;this.height=e||0};SUD.sound=SUD.sound||{};if("function"==typeof webkitAudioContext)SUD.sound.Context=SUD.sound.Context||new webkitAudioContext;SUD.sound=SUD.sound||{};
SUD.sound.Player=function(a,b,c,d){SUD.EventDispatcher.call(this,this);this.loop=b||!1;this.autoplay=c||!1;this.controls=d||!1;this.eventsRegistered=!1;a?(this.playerElement=document.createElement("audio"),this.playerElement.style.border="0px none",this.loop&&this.playerElement.setAttribute("loop","loop"),this.autoplay&&this.playerElement.setAttribute("autoplay","autoplay"),this.controls&&this.playerElement.setAttribute("controls","controls"),a.appendChild(this.playerElement),this.playerElement.parent=
a):console.log("ERROR! please pass in an HTML element to attach the player to")};SUD.sound.Player.prototype=new SUD.EventDispatcher;SUD.sound.Player.prototype.constructor=SUD.sound.Player;SUD.sound.Player.prototype.supr=SUD.EventDispatcher.prototype;SUD.sound.Player.prototype.loadFile=function(a){this.registerEvents();this.playerElement.setAttribute("src",a);this.playerElement.load()};SUD.sound.Player.prototype.play=function(){this.playerElement.play()};SUD.sound.Player.prototype.pause=function(){this.playerElement.pause()};
SUD.sound.Player.prototype.setVolume=function(a){this.playerElement.volume=a};SUD.sound.Player.prototype.seekTo=function(a){this.playerElement.currentTime=a};SUD.sound.Player.prototype.registerEvents=function(){if(!this.eventsRegistered)this.playerElement.addEventListener("load",this.onLoad),this.playerElement.addEventListener("ended",this.onEnded),this.playerElement.addEventListener("error",this.onError),this.eventsRegistered=!0};
SUD.sound.Player.prototype.onLoad=function(a){this.dispatchEvent("onLoad",a)};SUD.sound.Player.prototype.onEnded=function(a){this.dispatchEvent("onEnded",a)};SUD.sound.Player.prototype.onError=function(a){console.log(a);this.dispatchEvent("onError",a)};SUD.sound.Player.prototype.getElement=function(){return this.playerElement};SUD.sound.Player.prototype.isLooping=function(){return this.loop};SUD.sound.Player.prototype.isAutoplay=function(){return this.autoplay};
SUD.sound.Player.prototype.hasControls=function(){return this.controls};SUD.sound.Player.prototype.setLooping=function(a){(this.loop=a)?this.playerElement.setAttribute("loop","loop"):this.playerElement.removeAttribute("loop")};SUD.sound.Player.prototype.setAutoplay=function(a){(this.autoplay=a)?this.playerElement.setAttribute("autoplay","autoplay"):this.playerElement.removeAttribute("autoplay")};
SUD.sound.Player.prototype.setControls=function(a){(this.controls=a)?this.playerElement.setAttribute("controls","controls"):this.playerElement.removeAttribute("controls")};SUD.sound=SUD.sound||{};SUD.sound.WAPlayer=function(){SUD.EventDispatcher.call(this,this);this.buffer=null;this.pan=new SUD.geom.Point;this.gain=this.pitch=0;this.currentSource=this.filter=null;this.currentTime=0;this.panner=SUD.sound.Context.createPanner();this.panner.setPosition(0,0,0);this.end=this.start=0};
SUD.sound.WAPlayer.prototype.loadFile=function(a){var b=new XMLHttpRequest;b.parent=this;b.open("GET",a,!1);b.responseType="arraybuffer";b.parent=this;b.onload=function(){b.parent.buffer=SUD.sound.Context.createBuffer(b.response,!0);b.parent.end=Math.floor(b.parent.buffer.duration)};b.send()};SUD.sound.WAPlayer.prototype.setFilter=function(a){this.filter=a};
SUD.sound.WAPlayer.prototype.play=function(){var a=SUD.sound.Context.createBufferSource();a.playbackRate.value=this.pitch;a.gain=this.gain;a.buffer=this.buffer;null!=this.filter?(a.connect(this.filter),this.filter.connect(this.panner)):a.connect(this.panner);this.panner.connect(SUD.sound.Context.destination);a.noteGrainOn(0,this.start,this.end)};SUD.sound.WAPlayer.prototype.setGain=function(a){this.gain=a};
SUD.sound.WAPlayer.prototype.setPan=function(a,b,c){this.pan.x=a;this.pan.y=b||this.pan.y;this.pan.z=c||this.pan.z;this.panner.setPosition(this.pan.x,this.pan.y,this.pan.z)};SUD.sound.WAPlayer.prototype.setPitch=function(a){this.pitch=Math.pow(2,a/1200)};SUD=SUD||{};SUD.utils=SUD.utils||{};
SUD.utils.DOMElement=function(a,b){this.timeouts={};if(a)if(b)if("string"==typeof b)this.parent=document.getElementById(b),null!=this.parent?(this.element=this.parent.querySelector("#"+a),this.element._parent=this):console.warn("parent is null!");else{if(b.type&&"SUD.utils.DOMElement"==b.type)this.parent=b,this.element=this.parent.element.querySelector("#"+a),this.element._parent=this}else{if(this.element=document.getElementById(a),null==this.element)this.element=document.createElement("div"),this.element._parent=
this,console.warn("invalid id, this.element = blank div")}else this.element=document.createElement("div")};
SUD.utils.DOMElement.prototype={set x(a){if(this.left!=a)this.element.style.left=a+"px"},set y(a){if(this.top!=a)this.element.style.top=a+"px"},set left(a){if(this.left!=a)this.x=a},set top(a){if(this.top!=a)this.y=a},set offsetTop(a){if(this.offsetTop!=a)this.element.offsetTop=a+"px"},set right(a){if(this.right!=a)this.element.style.right=a+"px"},set offsetBottom(a){if(this.offsetBottom!=a)this.element.offsetBottom=a+"px"},set bottom(a){if(this.bottom!=a)this.element.style.bottom=a+"px"},set width(a){if(this.width!=
a)this.element.style.width=a+"px"},set height(a){if(this.height!=a)this.element.style.height=a+"px"},set opacity(a){if(this.opacity!=a)this.element.style.opacity=a},set html(a){this.element.innerHTML=a},get html(){return this.element.innerHTML},get x(){return this.element.clientLeft},get y(){return this.element.clientTop},get offsetX(){return this.element.offsetLeft},get offsetY(){return this.element.offsetTop},get left(){return this.element.clientLeft},get top(){return this.element.clientTop},get offsetTop(){return this.offsetY},
get right(){return this.element.clientRight},get bottom(){return this.element.clientBottom},get offsetBottom(){return this.element.offsetBottom},get width(){return this.element.clientWidth},get height(){return this.element.clientHeight},get offsetWidth(){return this.element.offsetWidth},get offsetHeight(){return this.element.offsetHeight},get opacity(){return this.element.style.opacity},get hidden(){return"hidden"==this.element.style.visibility},get type(){return"SUD.utils.DOMElement"}};
SUD.utils.DOMElement.prototype.clearTimeout=function(a){this.timeouts[a]&&clearTimeout(this.timeouts[a])};SUD.utils.DOMElement.prototype.setTimeout=function(a,b,c){this.clearTimeout(c);this.timeouts[c]=window.setTimeout(a,b)};SUD.utils.DOMElement.prototype.hide=function(){this.element.style.visibility="hidden";this.element.style.display="none"};SUD.utils.DOMElement.prototype.show=function(){this.element.style.visibility="visible";this.element.style.display="block"};
SUD.utils.DOMElement.prototype.update=function(){"0"==this.opacity&&!this.hidden&&this.hide()};SUD.utils.DOMElement.prototype.clone=function(a){var a=a||!0,b=$.extend(!0,{},this);b.element=this.element.cloneNode(a);return b.element._parent=b};SUD.utils.DOMElement.prototype.add=function(a){a.type&&"SUD.utils.DOMElement"==a.type?this.element.appendChild(a.element):this.element.appendChild(a)};SUD.utils.DOMElement.prototype.remove=function(a){if(a.type&&"SUD.utils.DOMElement"==a.type)try{this.element.removeChild(a.element)}catch(b){}else try{this.element.removeChild(a)}catch(c){}};
SUD.utils=SUD.utils||{};SUD.utils.WebSocket=function(a,b){this.host=a||"ws://localhost:8888";if(b)this.protocol=b};SUD.utils.WebSocket.prototype.connect=function(){try{this.socket=this.protocol?new WebSocket(this.host,this.protocol):new WebSocket(this.host),this.socket._parent=this,this.socket.onmessage=this._onMessageReceived,this.socket.onopen=this._onConnectionOpened,this.socket.onclose=this._onConnectionClosed}catch(a){console.log(a)}};
SUD.utils.WebSocket.prototype.sendData=function(a){console.log("sending: "+a);this.socket.send(a)};SUD.utils.WebSocket.prototype._onConnectionOpened=function(){this._parent.onConnectionOpened()};SUD.utils.WebSocket.prototype.onConnectionOpened=function(){};SUD.utils.WebSocket.prototype._onConnectionClosed=function(){this._parent.onConnectionClosed()};SUD.utils.WebSocket.prototype.onConnectionClosed=function(){};SUD.utils.WebSocket.prototype._onMessageReceived=function(a){var b=a.data;try{b=jQuery.parseJSON(a.data)}catch(c){}this._parent.onMessageReceived(b)};
SUD.utils.WebSocket.prototype.onMessageReceived=function(a){console.log(a)};
