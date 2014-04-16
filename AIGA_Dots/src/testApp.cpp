#include "testApp.h"

int realX, realY;
int circleRes = 20;
float circleInt = 360.0 / circleRes;
ofVec2f dims;

float scale = 1.0;

bool bMapChip = false;
bool bShowRender = false;
bool bShowPattern = false;
bool bRenderClocks = true;
bool bRenderMapamok = true;

string br_server = "spacebrew.robotconscience.com";

// message counter
int lastMessageReceived = 0;
int messageTimeout      = 2000;
bool bNeedToSend        = false;

// swipe counter
int lastSwipeChanged    = 0;
int swipeChangeTimer    = 30;
float maxDrawingAge     = 60 * 5;

int r = 255;
int g = 255;
int b = 255;

float texW = 1024;
float texH = 1024;

// camera mouse
bool bCameraMouse = false;

// mode + name switcher
bool bChangeModes       = true;
bool bInteractiveMode   = true;
int interactiveDuration = 30 * 1000;
int nameDuration        = 20 * 1000;
int lastChanged         = 0;
int currentName         = 0;
int numNames            = 24;
string names[] = {"sean ","noreen ","chuck ","dana ","bob ","david ","ken ","leslie ",
                "kyle ","michael ","stephen ","abbott ","louise ","sylvia ","cheryl ",
                "alex ","chip ","michael ","richard ","michael ","nancye ","bill ",
                "gael ","ann "};

// play out letters
int letterIndex         = 0;
int letterRate          = 2000;
int letterLastChanged   = 0;

//--------------------------------------------------------------
void testApp::setup(){
    ofSetFrameRate(60);
    ofBackground(0);
    realX = ofGetWidth()/2.0 - (gridSize/2.0 * spacing) + spacing/2.0;
    realY = ofGetHeight()/2.0 - (gridSize/2.0 * spacing) + spacing/2.0;
    
    font.loadFont("circular/CircularStd-Bold.otf", dotWidth * 1.7 );
    
    for ( int x = 0; x < gridSize; x ++){
        TextSquare ts;
        ts.text = ts.lastText = "";
        currentStrings.push_back( ts );
        for (int y=0; y<gridSize; y++){
            dots.push_back(Dot());
            dots.back().index = x + y * gridSize;
            dots.back().x = x * spacing;
            dots.back().y = y * spacing;
            dots.back().z = y * spacing;
            dots.back().setup();
            dots.back().loadFont(font);
            
            dots.back().r = ofRandom(255);
            dots.back().g = ofRandom(255);
            dots.back().b = ofRandom(255);
            dots.back().floatColor.r = dots.back().r / 255.;
            dots.back().floatColor.g = dots.back().g / 255.;
            dots.back().floatColor.b = dots.back().b / 255.;
        }
    }
    
    float sc = texW / 1024.0;
    float clockRadius = texW / 10.0 / 3.0;
    //clocks.setup(10, 10, ofVec2f(96 * sc, 81 * sc), 108 * sc, 54 * sc, clockRadius );
    clocks.setup(10, 10, ofVec2f(clockRadius * 2.7, clockRadius * 3.13), clockRadius - 2, clockRadius + 2, clockRadius );
    
    ofEnableDepthTest();
    ofEnableSmoothing();
    
    ofSetTextureWrap(GL_MIRRORED_REPEAT,GL_MIRRORED_REPEAT);
    //ofDisableArbTex();
    kidd.loadImage("kidd.jpg");
    kidd.getTextureReference().setTextureWrap(GL_MIRRORED_REPEAT,GL_MIRRORED_REPEAT);
    kidd.update();
    
    camera.setDistance(1000);
    camera.move(-150, 0, -200);
    camera.disableMouseInput();
    simpleLight.enable();
    simpleLight.setPosition(ofGetWidth(), ofGetHeight(), 1);
    simpleLight.setPointLight();
    
    // dummy data bar
    for (int i=0; i<10; i++){
        bars.push_back(0.0);
    }
    
    currentMode = MODE_INTERACTIVE_GRID;
    
    // setup gui
    gui = new ofxUICanvas(ofGetWidth() / 4.0 * 2.0 ,0,ofGetWidth() / 4.0, ofGetHeight() );
    gui->toggleVisible();
    
    gui->addToggle("Map Chip", &bMapChip);
    gui->addIntSlider("Mode", MODE_COLOR, MODE_DATA, &currentMode);
    gui->addToggle("Show Rendering", &bShowRender );
    gui->addToggle("Show Pattern", &bShowPattern);
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
    
    // background rendering
    rendering.loadImage("view.jpg");
    
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
    // auto-rate changer
    if ( bChangeModes ){
        
        // interactive to quote
        if (bInteractiveMode){
            if ( ofGetElapsedTimeMillis() - lastChanged > interactiveDuration){
                lastChanged = ofGetElapsedTimeMillis();
                bInteractiveMode = false;
                currentName++;
                if ( currentName >= numNames ){
                    currentName = 0;
                }
                spacebrew.sendRange("name", currentName);
                spacebrew.sendRange("mode", 1);
                letterIndex = 0;
                letterLastChanged = ofGetElapsedTimeMillis();
                cout<<"name mode"<<endl;
            }
        
        // quote to interactive
        } else {
            if ( ofGetElapsedTimeMillis() - lastChanged > nameDuration && letterIndex == 0){
                lastChanged = ofGetElapsedTimeMillis();
                bInteractiveMode = true;
                spacebrew.sendRange("mode", 0);
                letterIndex = 0;
                letterLastChanged = ofGetElapsedTimeMillis();
                cout<<"interactive mode"<<endl;
            }
        }
    }
    
    if ( !pattern.isAllocated() ){
        // render pattern
        pattern.allocate(texW, texH);
        pattern.getTextureReference().setTextureWrap(GL_MIRRORED_REPEAT,GL_MIRRORED_REPEAT);
        
        ofDirectory dir;
        int n = dir.listDir("patterns");
        vector<ofImage> ptns;
        for (int i=0; i<n; i++){
            ptns.push_back(ofImage());
            ptns.back().loadImage(dir.getPath(i));
        }
        
//        ptns[0].loadImage("patterns/dots.png");
//        ptns[1].loadImage("patterns/stripes.png");
        
        pattern.begin();
        ofDisableAlphaBlending();
        ofClear(255);
        ofSetColor(255,255,255,255);
        for (int x=0; x<pattern.getWidth(); x+=64){
            for (int y=0; y<pattern.getHeight(); y+=64){
                // draw two rows
                ofImage p = ptns[(int) floor(ofRandom(ptns.size()))];
                for ( int i=0; i<4; i++){
                    for ( int j=0; j<4; j++){
                        p.draw(x + i * 16,y + j * 16);
                    }
                }
            }
        }
        pattern.end();
    }
    
    if ( !type.isAllocated() ){
        
        // render pattern
        type.allocate(texW, texH, GL_RGB, 2);
        renderClocks();
    }
    
    for (Dot & d : dots){
        d.mode = (Mode) currentMode;
    }
    
    if (!bInteractiveMode){
        char k = names[currentName][letterIndex];
        string ks = ofToString(k);
        
        int start = (ks == "w" || ks == "m")? 2 : 3;
        //int start = ofMap(ofGetElapsedTimeMillis() - letterLastChanged, 0, letterRate, (ks == "w" || ks == "m")? 4 : 6, 0);
        clocks.setClocks(clocks.letters[ks], start, 3, (ks == "w" || ks == "m")? 6 : 4 );
        
        if ( ofGetElapsedTimeMillis() - letterLastChanged > letterRate ){
            letterLastChanged = ofGetElapsedTimeMillis();
            
            letterIndex++;
            if ( letterIndex >= names[currentName].length() ){
                letterIndex = 0;
            }
        }
    }
    
    // do change this!
    if ( lastMode != currentMode ){
        int sendMode = 0;
        if ( currentMode == MODE_INTERACTIVE_GRID ) sendMode = 0;
        else if ( currentMode == MODE_INTERACTIVE_SWIPE ) sendMode = 1;
        else if ( currentMode == MODE_INTERACTIVE_TEXT ) sendMode = 2;
        spacebrew.sendRange("mode", sendMode);
        lastMode = currentMode;
        
        if ( currentMode == MODE_DATA ){
            for (auto & b : bars){
                b = ofRandom(10);
            }
            barInc = 0.0;
        } else if ( currentMode == MODE_VIDEO ){
            video.setFrame(0);
            video.play();
            video.setLoopState(OF_LOOP_NORMAL);
        } else if ( currentMode == MODE_INTERACTIVE_SWIPE ){
            for (Dot & d : dots){
                d.r = d.g = d.b = 255;
            }
        } else {
            for (Dot & d : dots){
                d.r = ofRandom(255);
                d.g = ofRandom(255);
                d.b = ofRandom(255);
                d.floatColor.r = d.r / 255.;
                d.floatColor.g = d.g / 255.;
                d.floatColor.b = d.b / 255.;
            }
        }
    }
    
    switch (currentMode) {
        case MODE_DATA:
            for (int y=0; y<gridSize; y++){
                float val = sin(barInc += .002);
                if ( val > 0 ){
                    val = val * bars[y];
                } else if ( barInc > .05 ){
                    for (float & i : bars ){
                        i = ofRandom(10);
                    }
                    barInc = 0.0;
                } else {
                    val = 0;
                }
                for ( int x = 0; x < gridSize; x ++){
                    int ind = x + y*gridSize;
                    if ( x < val - 1 ){
                        dots[ ind ].floatColor.setSaturation(1.0);
                        dots[ind].floatColor.setBrightness(1.0);
                        dots[ind].floatColor.setHue(bars[y] / 10.0);
                    } else {
                        dots[ind].floatColor.setSaturation(dots[ind].floatColor.getSaturation() * .7);
                        dots[ind].floatColor.setBrightness(dots[ind].floatColor.getSaturation() * .7);
                    }
                }
            }
            break;
            
        case MODE_VIDEO:
            video.update();
            if ( video.isFrameNew() ){
                for (int y=0; y<gridSize; y++){
                    for ( int x = 0; x < gridSize; x ++){
                        int ind = x + y*gridSize;
                        float sX = (float) x/gridSize * video.getWidth();
                        float sY = (float) y/gridSize * video.getHeight();
                        ofColor c = video.getPixelsRef().getColor(sX, sY);
                        ofFloatColor fc = ofFloatColor(c.r/255.0, c.g/255.0, c.b/255.0);
                        dots[ind].floatColor = dots[ind].floatColor * .9 + fc * .1;
                    }
                }
            }
            break;
            
        case MODE_INTERACTIVE_COLOR:
            if (ofGetElapsedTimeMillis() - lastMessageReceived > messageTimeout && bNeedToSend ){
                cout << "SEND RANDOM "<<endl;
                //...do something!
                r = ofRandom(255);
                g = ofRandom(255);
                b = ofRandom(255);
                
                for ( Dot & d : dots){
                    d.r = r;
                    d.g = g;
                    d.b = b;
                }
                
                bNeedToSend = false;
                spacebrew.sendRange("randomR", r );
                spacebrew.sendRange("randomG", g );
                spacebrew.sendRange("randomB", b );
            } else {
                
                for ( Dot & d : dots){
                    d.r = r;
                    d.g = g;
                    d.b = b;
                }
            }
            break;
            
        case MODE_INTERACTIVE_SWIPE:
            if ( drawings.size() != 0 ){
                // first update indices
                for ( int i=0; i<drawings.size(); i++){
                    if ( ofGetElapsedTimeMillis() - drawings[i].lastChanged > swipeChangeTimer ){
                        drawings[i].index++;
                        if (  drawings[i].index >= drawings[i].points.size() ){
                            drawings[i].index = 0;
                        }
                        drawings[i].lastChanged = ofGetElapsedTimeMillis();
                    }
                    
                    ofVec2f sv = drawings[i].points[drawings[i].index];
                    
                    sv.x *= gridSize * spacing;
                    sv.y *= gridSize * spacing;
                    sv.y = gridSize * spacing - sv.y;
                    
                    for (int j=0; j<dots.size(); j++){
                        ofVec2f d = ofVec2f(dots[j].x, dots[j].y);
                        if ( d.distance(sv) < dots[j].width * 2 ){
                            float mult = 1.0;//(float) (maxDrawingAge-drawings[i].age) / maxDrawingAge;
                            dots[j].r = drawings[i].color.r * mult;
                            dots[j].g = drawings[i].color.g * mult;
                            dots[j].b = drawings[i].color.b * mult;
                        }
                    }
                    drawings[i].age++;
                }
                
                for ( int i=drawings.size()-1; i>=0; i--){
                    if ( drawings[i].age >= maxDrawingAge ){
                        drawings.erase(drawings.begin() + i);
                    }
                }
            }
            break;
            
        case MODE_INTERACTIVE_GRID:
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
                        if ( !bRenderClocks ){
                            dots[ind].r = gridDrawings[i].color.r * mult;
                            dots[ind].g = gridDrawings[i].color.g * mult;
                            dots[ind].b = gridDrawings[i].color.b * mult;
                        } else {
                            //                            static float mult = texW / 10.0;
                            float clockRadius = texW / 10.0 / 3.0;
                            ofVec2f v = clocks.clocks[indO];
                            clocks.magnet(v.x, v.y, gridDrawings[i].color );
                        }
                        
                        if ( mult >= .99 ){
                            dots[ind].index++;
                        }
                    }
                    gridDrawings[i].age++;
                }
                
                
                for ( int i=gridDrawings.size()-1; i>=0; i--){
                    if ( gridDrawings[i].age >= maxDrawingAge ){
                        gridDrawings.erase(gridDrawings.begin() + i);
                    }
                }
            }
            break;
            
        case MODE_INTERACTIVE_TEXT:
            for ( int i=0; i<currentStrings.size(); i++){
                if ( currentStrings[i].text.size() > 0 ){
                    for ( int x=0; x<gridSize; x++){
                        // i == y
                        int index = x + i * gridSize;
                        
                        TextSquare & ts = currentStrings[i];
                        // first update textsquare
                        if (ts.index + 1 < currentStrings[i].text.size()
                            && ofGetElapsedTimeMillis() - ts.lastChanged > swipeChangeTimer ){
                            ts.index++;
                            ts.lastChanged = ofGetElapsedTimeMillis();
                        }
                        
                        int in = x;
                        if ( ts.backwards ){
                            in = x - (gridSize - currentStrings[i].text.size());
                        }
                        
                        if ( currentStrings.size() > i
                            && currentStrings[i].text.size() > in
                            && ts.index >= in){
                            dots[index].setText(currentStrings[i].text.substr(in,1));
                        } else {
                            dots[index].setText("");
                        }
                    }
                }
            }
            break;
            
        default:
            break;
    }
    
    if ( bRenderMapamok ){
        mapamok.update();
    }
    
    ofSetWindowTitle(ofToString(ofGetFrameRate(), 3));
}

//--------------------------------------------------------------
void testApp::draw(){
    if ( bRenderMapamok ){
        renderClocks();
        mapamok.draw( &type.getTextureReference() );
        
    } else {
        ofColor white;
        ofColor offWhite(200,200,200);
        ofBackgroundGradient(white, offWhite);
        
        if (bShowRender) {
            ofSetColor(255);
            float scale = (float) ofGetWidth()/rendering.width;
            rendering.draw(0,0, rendering.width * scale, rendering.height * scale);
        }
        
        if (currentMode == MODE_INTERACTIVE_TEXT){
            renderText();
        } else if ( bRenderClocks ){
            renderClocks();
        }
        
        //ofEnableLighting();
        ofEnableDepthTest();
        camera.begin();
        ofSetColor(255);
        bool bMap = bMapChip;
        bool bound = currentMode == MODE_INTERACTIVE_TEXT || bRenderClocks;
        if ( bMap ){
            kidd.bind();
        } else if ( bound ) {
            type.getTextureReference().bind();
        }
        
        ofPushMatrix();
        ofTranslate(-ofGetWidth()/4.0, -ofGetHeight()/4.0);//realX, realY);
        
        for ( Dot & d : dots){
            d.draw();
        }
        
        ofPopMatrix();
        if ( bMap ) kidd.unbind();
        else if ( bound ) {
            type.getTextureReference().unbind();
        }
        
        camera.end();
        ofDisableDepthTest();
        ofDisableLighting();
        
        if ( bShowPattern ){
            ofSetColor(255);
            type.draw(0,0);
        }
    }
}

//--------------------------------------------------------------
void testApp::renderClocks(){
    
    type.begin();
//    ofDisableAlphaBlending();
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
    
    type.begin();
    
    ofDisableAlphaBlending();
    ofClear(255);
    ofSetColor(255,255,255,255);
    
    float typeSpacing = (float) texW/gridSize;
    float letterWidth = (float) typeSpacing * ((float) dotWidth/spacing);
    
    for ( int x = 0; x < gridSize; x ++){
        for (int y=0; y<gridSize; y++){
            float sqX = x * typeSpacing;
            float sqY = y * typeSpacing;
            Dot & d = dots[ x + y * gridSize ];
            // hax
            Dot & dc = dots[ x + (y+1 < gridSize ? y+1 : y) * gridSize ];
            ofColor c = ofColor(dc.floatColor.r * 255., dc.floatColor.g * 255., dc.floatColor.b * 255.  );
            if ( y+1 >= gridSize ){
                c.set(102,102,102);
            }
            ofSetColor(c);
            ofRect(sqX, sqY, typeSpacing, typeSpacing); //??
            if ( d.getText().size() > 0 ){
                ofSetColor(255);
                ofDisableDepthTest();
                float offx = font.stringWidth(d.getText()) / 2.0;
                float offy = font.stringHeight(d.getText()) / 2.0 + font.getLineHeight() * .5;
                font.drawString( d.getText(), sqX + letterWidth/2.0 - offx, sqY + letterWidth/2.0 - offy);
            }
        }
    }
    type.end();
}

//--------------------------------------------------------------
void testApp::keyPressed(int key){
    if ( key == ' ' ){
        for ( Dot & d : dots){
            d.clear();
        }
    } else if ( key == '='){
        scale += .1;
    } else if ( key == '-'){
        scale -= .1;
    } else if ( key == 'g' ){
        gui->toggleVisible();
        if ( gui->isVisible() ){
            camera.disableMouseInput();
        } else if ( bCameraMouse ){
            camera.enableMouseInput();
        }
    } else if ( key == 's' ){
        gui->saveSettings("main-settings.xml");
    } else if ( key == 'f' ){
        ofToggleFullscreen();
    } else if ( key == 'S'){
        mapamok.saveSettings();
        mapamok.saveCalibration();
    }
    if ( key == OF_KEY_SHIFT ){
        bCameraMouse = true;
        camera.enableMouseInput();
    }
}

//--------------------------------------------------------------
void testApp::keyReleased(int key){
    if ( key == OF_KEY_SHIFT ){
        bCameraMouse = false;
        camera.disableMouseInput();
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


void testApp::onClientConnect( Spacebrew::Config & c){
    cout << "connect"<<endl;
    spacebrew.sendRange("name", currentName);
    spacebrew.sendRange("mode", bInteractiveMode ? 0 : 1);
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
