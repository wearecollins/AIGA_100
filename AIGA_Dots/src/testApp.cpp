#include "testApp.h"

int realX, realY;
int circleRes = 20;
float circleInt = 360.0 / circleRes;
ofVec2f dims;

float scale = 1.0;

bool bMapChip = false;
bool bShowRender = false;
bool bShowPattern = false;

string br_server = "spacebrew.robotconscience.com";

// message counter
int lastMessageReceived = 0;
int messageTimeout      = 2000;
bool bNeedToSend        = false;

// swipe counter
int lastSwipeChanged    = 0;
int swipeChangeTimer    = 30;
float maxDrawingAge     = 60 * 5;

//--------------------------------------------------------------
void testApp::setup(){
    ofSetFrameRate(60);
    ofBackground(0);
    realX = ofGetWidth()/2.0 - (gridSize/2.0 * spacing) + spacing/2.0;
    realY = ofGetHeight()/2.0 - (gridSize/2.0 * spacing) + spacing/2.0;
    
    font.loadFont("circular/CircularStd-Bold.otf", dotWidth * .9 );
    
    for ( int x = 0; x < gridSize; x ++){
        TextSquare ts;
        ts.text = ts.lastText = "chill";
        currentStrings.push_back( ts );
        for (int y=0; y<gridSize; y++){
            dots.push_back(Dot());
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
    ofEnableDepthTest();
    ofEnableSmoothing();
    
    ofSetTextureWrap(GL_MIRRORED_REPEAT,GL_MIRRORED_REPEAT);
    ofDisableArbTex();
    kidd.loadImage("kidd.jpg");
    kidd.getTextureReference().setTextureWrap(GL_MIRRORED_REPEAT,GL_MIRRORED_REPEAT);
    kidd.update();
    
    camera.setDistance(1000);
    simpleLight.enable();
    simpleLight.setPosition(ofGetWidth(), ofGetHeight(), 1);
    simpleLight.setPointLight();
    
    // dummy data bar
    for (int i=0; i<10; i++){
        bars.push_back(0.0);
    }
    
    currentMode = MODE_INTERACTIVE_GRID;
    
    // setup gui
    gui = new ofxUICanvas(0,0, ofGetWidth()/2.0, ofGetHeight());
    gui->toggleVisible();
    
    gui->addToggle("Map Chip", &bMapChip);
    gui->addIntSlider("Mode", MODE_COLOR, MODE_DATA, &currentMode);
    gui->addToggle("Show Rendering", &bShowRender );
    gui->addToggle("Show Pattern", &bShowPattern);
    lastMode = currentMode;
    
//    ofSetLogLevel(OF_LOG_VERBOSE);
    
    spacebrew.addPublish("mode", Spacebrew::TYPE_RANGE);
    
    spacebrew.addPublish("randomR", Spacebrew::TYPE_RANGE);
    spacebrew.addPublish("randomG", Spacebrew::TYPE_RANGE);
    spacebrew.addPublish("randomB", Spacebrew::TYPE_RANGE);
    
    // add 100 listeners...
    for ( int i=0; i<dots.size(); i++){
        spacebrew.addSubscribe("dot_"+ofToString(i)+"_r", &dots[i].r );
        spacebrew.addSubscribe("dot_"+ofToString(i)+"_g", &dots[i].g );
        spacebrew.addSubscribe("dot_"+ofToString(i)+"_b", &dots[i].b );
    }
    spacebrew.addSubscribe("drawing", "drawing" );
    spacebrew.addSubscribe("grid", "grid" );
    spacebrew.addSubscribe("text", Spacebrew::TYPE_STRING );
    
    //spacebrew.setAutoReconnect();
    spacebrew.connect(br_server, "grid_display");
    Spacebrew::addListener(this, spacebrew);
    
    // video map
    video.loadMovie("test.mov");
    video.stop();
    
    // background rendering
    rendering.loadImage("view.jpg");
    
    screen.allocate(ofGetWidth(), ofGetHeight());
    
    ofDisableLighting();
}

//--------------------------------------------------------------
void testApp::update(){
    
    if ( !pattern.isAllocated() ){
        // render pattern
        pattern.allocate(1024,1024);
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
        type.allocate(512, 512);
        renderText();
    }
    
    for (int i=0; i<dots.size(); i++){
        dots[i].mode = (Mode) currentMode;
    }
    if ( lastMode != currentMode ){
        spacebrew.sendRange("mode", currentMode);
        lastMode = currentMode;
        
        if ( currentMode == MODE_DATA ){
            for (int i=0; i<bars.size(); i++){
                bars[i] = ofRandom(10);
            }
            barInc = 0.0;
        } else if ( currentMode == MODE_VIDEO ){
            video.setFrame(0);
            video.play();
            video.setLoopState(OF_LOOP_NORMAL);
        } else if ( currentMode == MODE_INTERACTIVE_SWIPE ){
            for (int i=0; i<dots.size(); i++){
                dots[i].r = dots[i].g = dots[i].b = 255;
            }
        } else {
            for (int i=0; i<dots.size(); i++){
                dots[i].r = ofRandom(255);
                dots[i].g = ofRandom(255);
                dots[i].b = ofRandom(255);
                dots[i].floatColor.r = dots[i].r / 255.;
                dots[i].floatColor.g = dots[i].g / 255.;
                dots[i].floatColor.b = dots[i].b / 255.;
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
                    for (int i=0; i<bars.size(); i++){
                        bars[i] = ofRandom(10);
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
                int r = ofRandom(255);
                int g = ofRandom(255);
                int b = ofRandom(255);
                
                for (int i=0; i<dots.size(); i++){
                    dots[i].r = r;
                    dots[i].g = g;
                    dots[i].b = b;
                }
                
                bNeedToSend = false;
                spacebrew.sendRange("randomR", r );
                spacebrew.sendRange("randomG", g );
                spacebrew.sendRange("randomB", b );
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
                    
                    int x = 9 - gridDrawings[i].indices[gridDrawings[i].index] % 10;
                    int y = floor(gridDrawings[i].indices[gridDrawings[i].index]/10);
                    int ind = x + y * 10;
                    
                    int indO = gridDrawings[i].indices[gridDrawings[i].index];
                    if ( gridDrawings[i].grid[indO] ){
                        float mult =(float) (maxDrawingAge-gridDrawings[i].age) / maxDrawingAge;
                        dots[ind].r = gridDrawings[i].color.r * mult;
                        dots[ind].g = gridDrawings[i].color.g * mult;
                        dots[ind].b = gridDrawings[i].color.b * mult;
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
}

//--------------------------------------------------------------
void testApp::draw(){
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
    }
    
    //ofEnableLighting();
    ofEnableDepthTest();
    camera.begin();
    ofSetColor(255);
    bool bMap = bMapChip;
    bool bound = currentMode == MODE_INTERACTIVE_TEXT;
    if ( bMap ) kidd.bind();
    else if ( bound ) {
        type.getTextureReference().bind();
    }
    
    ofPushMatrix();
    ofTranslate(-ofGetWidth()/4.0, -ofGetHeight()/4.0);//realX, realY);
    for (int i=0; i<dots.size(); i++){
        dots[i].draw();
    }
    ofPopMatrix();
    if ( bMap ) kidd.unbind();
    else if ( bound ) {
        type.getTextureReference().unbind();
    }
    
    camera.end();
    ofDisableDepthTest();
    ofDisableLighting();
    
    ofDrawBitmapString("Press ' ' to clear", 20,20);
    if ( bShowPattern ){
        ofSetColor(255);
        type.draw(0,0);
    }
}


//--------------------------------------------------------------
void testApp::renderText(){
    
    type.begin();
    
    ofDisableAlphaBlending();
    ofClear(255);
    ofSetColor(255,255,255,255);
    
    float typeSpacing = (float) 512.0/gridSize;
    float letterWidth = (float) typeSpacing * ((float) dotWidth/spacing);
    
    for ( int x = 0; x < gridSize; x ++){
        for (int y=0; y<gridSize; y++){
            int sqX = x * typeSpacing;
            int sqY = y * typeSpacing;
            Dot & d = dots[ x + y * gridSize ];
            ofColor c = ofColor(d.floatColor.r * 255., d.floatColor.g * 255., d.floatColor.b * 255. );
            ofSetColor(c);
            ofRect(sqX, sqY, typeSpacing, typeSpacing + 10); //??
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
        for (int i=0; i<dots.size(); i++){
            dots[i].clear();
        }
    } else if ( key == '='){
        scale += .1;
    } else if ( key == '-'){
        scale -= .1;
    } else if ( key == 'g' ){
        gui->toggleVisible();
        if ( gui->isVisible() ){
            camera.disableMouseInput();
        } else {
            camera.enableMouseInput();
        }
    }
}

void testApp::keyReleased(int key){}
void testApp::mouseMoved(int x, int y ){}

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

float jsonAsFloat( Json::Value val, string prop ){
    return jsonAsFloat(val[prop]);
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

void testApp::mouseDragged(int x, int y, int button){
    for (int i=0; i<dots.size(); i++){
//        dots[i].mousePressed(ofVec2f(x-realX,y-realY));
    }
}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button){
    for (int i=0; i<dots.size(); i++){
//        dots[i].mousePressed(ofVec2f(x-realX,y-realY));
    }

}
void testApp::mouseReleased(int x, int y, int button){
    for (int i=0; i<dots.size(); i++){
//        dots[i].mouseReleased(ofVec2f(x-realX,y-realY));
    }
}
void testApp::windowResized(int w, int h){}
void testApp::gotMessage(ofMessage msg){}
void testApp::dragEvent(ofDragInfo dragInfo){ }
