#include "testApp.h"

bool bRenderClocks = true;
bool bRenderMapamok = true;

string br_server = "spacebrew.robotconscience.com";

// message counter
int lastMessageReceived = 0;
int messageTimeout      = 2000;
bool bNeedToSend        = false;

// swipe counter
int swipeChangeTimer    = 30;
float maxDrawingAge     = 60 * 5;

float texW = 1024;
float texH = 1024;

//--------------------------------------------------------------
void testApp::setup(){
    ofSetFrameRate(60);
    ofBackground(0);
    
    font.loadFont("circular/CircularStd-Bold.otf", dotWidth * 1.7 );
    
    float sc = texW / 1024.0;
    float clockRadius = texW / 10.0 / 3.0;
    //clocks.setup(10, 10, ofVec2f(96 * sc, 81 * sc), 108 * sc, 54 * sc, clockRadius );
    clocks.setup(10, 10, ofVec2f(clockRadius * 2.7, clockRadius * 3.13), clockRadius - 2, clockRadius + 2, clockRadius );
    
    // setup clock FBO
    type.allocate(texW, texH, GL_RGB, 2);
    renderClocks();
    
    //ofEnableDepthTest();
    ofEnableSmoothing();
    
    //ofSetTextureWrap(GL_MIRRORED_REPEAT,GL_MIRRORED_REPEAT);
    //ofDisableArbTex();
    
    currentMode = MODE_INTERACTIVE_GRID;
    
    // setup gui
    gui = new ofxUICanvas(ofGetWidth() / 4.0 * 2.0 ,0,ofGetWidth() / 4.0, ofGetHeight() );
    gui->toggleVisible();
    
    gui->addIntSlider("Mode", MODE_COLOR, MODE_DATA, &currentMode);
    gui->addToggle("Map Clocks", &bRenderClocks);
    gui->addToggle("Use mapping", &bRenderMapamok);
    gui->addSlider("Map Near", 0, 100.0, &mapamok.near);
    gui->addSlider("Map Far", 0, 10000, &mapamok.far);
    gui->addTextInput("model", "model.dae");
    
    // clock gui
    gui->loadSettings("main-settings.xml");
    
    lastMode = currentMode;
    
//    ofSetLogLevel(OF_LOG_VERBOSE);
    
    spacebrew.addPublish("mode", Spacebrew::TYPE_RANGE);
    spacebrew.addPublish("name", Spacebrew::TYPE_RANGE);
    
    spacebrew.addSubscribe("grid", "grid" );
    spacebrew.addSubscribe("text", Spacebrew::TYPE_STRING );
    
    spacebrew.setAutoReconnect();
    spacebrew.connect(br_server, "grid_display");
    
    Spacebrew::addListener(this, spacebrew);
    ofAddListener(spacebrew.onOpenEvent, this, &testApp::onOpen);
    ofAddListener(spacebrew.onClientConnectEvent, this, &testApp::onClientConnect);
    
    // setup clock mgr
    clockManager.setup(spacebrew, clocks);
    
    // can i remove this?
    screen.allocate(ofGetWidth(), ofGetHeight());
    
    // setup mapamok
    string model = ((ofxUITextInput*) gui->getWidget("model"))->getTextString();
    cout <<"LOADING "<<model<<endl;
    mapamok.loadSettings("mapamok.xml");
    mapamok.loadMesh(model, texW, texH);
    mapamok.drawMode = DRAW_FACES;
    mapamok.useLights = false;
    
    ofDisableLighting();
}

//--------------------------------------------------------------
void testApp::update(){
    clockManager.update();

    if ( gridDrawings.size() != 0 ){
        // first update indices
        for ( int i=0; i<gridDrawings.size(); i++){
            if ( ofGetElapsedTimeMillis() - gridDrawings[i].lastChanged > swipeChangeTimer ){
                gridDrawings[i].index++;
                if (  gridDrawings[i].index >= gridDrawings[i].indices.size() ){
                    //                        gridDrawings[i].index = 0;
                    gridDrawings[i].age = 1000;
                    continue;
                }
                gridDrawings[i].lastChanged = ofGetElapsedTimeMillis();
            }
            
            int y = gridDrawings[i].indices[gridDrawings[i].index] % 10;
            int x = floor(gridDrawings[i].indices[gridDrawings[i].index]/10);
            int ind = gridDrawings[i].indices[gridDrawings[i].index];//x + y * 10;
            
            int indO = gridDrawings[i].indices[gridDrawings[i].index];
            if ( gridDrawings[i].grid[indO] ){
                float mult =(float) (maxDrawingAge-gridDrawings[i].age) / maxDrawingAge;
                //                            static float mult = texW / 10.0;
                float clockRadius = texW / 10.0 / 3.0;
                ofVec2f v = clocks.clocks[indO];
                clocks.magnet(v.x, v.y, gridDrawings[i].color );
            }
            gridDrawings[i].age++;
        }
        
        
        for ( int i=gridDrawings.size()-1; i>=0; i--){
            if ( gridDrawings[i].age >= maxDrawingAge ){
                gridDrawings.erase(gridDrawings.begin() + i);
            }
        }
    }
    
    if ( bRenderMapamok ){
        mapamok.update();
    }
    
    ofSetWindowTitle(ofToString(ofGetFrameRate(), 3));
}

//--------------------------------------------------------------
void testApp::draw(){
    renderClocks();
    mapamok.draw( &type.getTextureReference() );
}

//--------------------------------------------------------------
void testApp::renderClocks(){
    type.begin();
    ofClear(255);
    ofPushMatrix();
    //ofScale(-1,1);
    //ofTranslate(-type.getWidth(), 0);
    clocks.draw();
    ofPopMatrix();
    type.end();
}


//--------------------------------------------------------------
void testApp::renderText(){
}

//--------------------------------------------------------------
void testApp::keyPressed(int key){
    if ( key == ' ' ){
    } else if ( key == 'g' ){
        gui->toggleVisible();
    } else if ( key == 's' ){
        gui->saveSettings("main-settings.xml");
    } else if ( key == 'f' ){
        ofToggleFullscreen();
    } else if ( key == 'S'){
        mapamok.saveSettings();
        mapamok.saveCalibration();
    }
    if ( key == OF_KEY_SHIFT ){
    }
}

//--------------------------------------------------------------
void testApp::keyReleased(int key){
    if ( key == OF_KEY_SHIFT ){
    }
}

//--------------------------------------------------------------
float jsonAsFloat( Json::Value val ){
    if ( val.isString() ){
        return ofToFloat( val.asString() );
    } else if ( val.isInt() ){
        return val.asInt();
    } else if ( val.isDouble() ){
        return val.asDouble();
    } else {
        cout <<"Mystery type "<<val.type()<<endl;
        return 0.0;
    }
}

//--------------------------------------------------------------
float jsonAsFloat( Json::Value val, string prop ){
    return jsonAsFloat(val[prop]);
}

//--------------------------------------------------------------
void testApp::onOpen( ofxLibwebsockets::Event& args ){
    int sendMode = 0;
    if ( currentMode == MODE_INTERACTIVE_COLOR ) sendMode = 0;
    else if ( currentMode == MODE_INTERACTIVE_GRID ) sendMode = 1;
    else if ( currentMode == MODE_INTERACTIVE_TEXT ) sendMode = 2;
}

//--------------------------------------------------------------
void testApp::onMessage( Spacebrew::Message & m ){
    lastMessageReceived = ofGetElapsedTimeMillis();
    bNeedToSend = true;
    
    if ( m.type == "drawing" ){
        Json::Value val;
        Json::Reader r;
        r.parse(m.value, val);
        
        // drawing is json array...
        Drawing d;
        d.color = ofColor(ofRandom(255),ofRandom(255),ofRandom(255));
        
        for (int i=0; i<val.size(); i++){
            Json::Value v = val[i];
            float x = jsonAsFloat(v, "x");
            float y = jsonAsFloat(v, "y");
            d.points.push_back(ofVec2f(x,y));
        }
        drawings.push_back(d);
    } else if ( m.type == "grid" ){
        Json::Value val;
        Json::Reader r;
        r.parse(m.value, val);
        
        // drawing is json array...
        
        GridDrawing d;
        // set color
        
        if ( val["color"].isObject() ){
            d.color.r = jsonAsFloat(val["color"], "r");
            d.color.g = jsonAsFloat(val["color"], "g");
            d.color.b = jsonAsFloat(val["color"], "b");
        }
        
        if ( val["grid"].isArray() ){
            for (int i=0; i<val["grid"].size(); i++){
                Json::Value v = val["grid"][i];
                bool filled = false;
                if ( v["filled"].isBool() ){
                    filled = v["filled"].asBool();
                } else if ( v["filled"].isInt() ){
                    filled = v["filled"].asBool();
                } else {
                    cout << v["filled"].type();
                }
                d.grid.push_back(filled);
            }
        }
        
        if ( val["indices"].isArray() ){
            for (int i=0; i<val["indices"].size(); i++){
                Json::Value v = val["indices"][i];
                d.indices.push_back(jsonAsFloat(v));
            }
        }
        
        gridDrawings.push_back(d);
    } else if ( m.name == "text" ){
        //hm
        int ind = floor(ofRandom(currentStrings.size()));
        currentStrings[ind].lastText = currentStrings[ind].text;
        currentStrings[ind].text = m.valueString();
        currentStrings[ind].index = 0;
        currentStrings[ind].lastChanged = ofGetElapsedTimeMillis();
        currentStrings[ind].backwards = ofRandom(10) > 4 ? true : false;
    }
}

//--------------------------------------------------------------
void testApp::onClientConnect( Spacebrew::Config & c){
    cout << "connect"<<endl;
    spacebrew.sendRange("name", clockManager.getCurrentName());
    spacebrew.sendRange("mode", clockManager.isInteractive() ? 0 : 1);
}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button){}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y ){}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button){}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button){}
void testApp::windowResized(int w, int h){}
void testApp::gotMessage(ofMessage msg){}
void testApp::dragEvent(ofDragInfo dragInfo){ }
