#include "testApp.h"

int realX, realY;
int circleRes = 20;
float circleInt = 360.0 / circleRes;
ofVec2f dims;

float scale = 1.0;
int spacing = 36;
int gridSize = 10;

bool bMapChip = false;
bool bShowRender = false;

string br_server = "spacebrew.robotconscience.com";

// message counter
int lastMessageReceived = 0;
int messageTimeout      = 2000;
bool bNeedToSend        = false;

// swipe counter
int lastSwipeChanged    = 0;
int swipeChangeTimer    = 100;
float maxDrawingAge     = 60 * 10;

//--------------------------------------------------------------
void testApp::setup(){
    ofSetFrameRate(60);
    ofBackground(0);
    realX = ofGetWidth()/2.0 - (gridSize/2.0 * spacing) + spacing/2.0;
    realY = ofGetHeight()/2.0 - (gridSize/2.0 * spacing) + spacing/2.0;
    for ( int x = 0; x < gridSize; x ++){
        for (int y=0; y<gridSize; y++){
            dots.push_back(Dot());
            dots.back().x = x * spacing;
            dots.back().y = y * spacing;
            dots.back().z = y * spacing;
            dots.back().setup();
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
    
    // setup gui
    gui = new ofxUICanvas(0,0, ofGetWidth()/2.0, ofGetHeight());
    gui->toggleVisible();
    
    gui->addToggle("Map Chip", &bMapChip);
    gui->addIntSlider("Mode", MODE_COLOR, MODE_DATA, &currentMode);
    gui->addToggle("Show Rendering", &bShowRender );
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
    
    //spacebrew.setAutoReconnect();
    spacebrew.connect(br_server, "grid_display");
    Spacebrew::addListener(this, spacebrew);
    
    // video map
    video.loadMovie("test.mov");
    video.stop();
    
    // background rendering
    rendering.loadImage("view.jpg");
    
    screen.allocate(ofGetWidth(), ofGetHeight());
}

//--------------------------------------------------------------
void testApp::update(){
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
                dots[i].r = dots[i].g = dots[i].b = 0;
            }
        }
    }
    
    // MODE: DATA
    if ( currentMode == MODE_DATA ){
        
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
    // MODE: VIDEO
    } else if ( currentMode == MODE_VIDEO ){
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
    }
    
    // MODE: INTERACTIVE COLOR
    else if ( currentMode == MODE_INTERACTIVE_COLOR ){
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
    }
    
    // MODE: Interactive swipe
    else if ( currentMode == MODE_INTERACTIVE_SWIPE ){
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
                        float mult =(float) (maxDrawingAge-drawings[i].age) / maxDrawingAge;
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
    }
}

//--------------------------------------------------------------
void testApp::draw(){
//    kidd.draw(realX - 25, realY - 25);
    if (bShowRender) {
        ofSetColor(255);
        float scale = (float) ofGetWidth()/rendering.width;
        rendering.draw(0,0, rendering.width * scale, rendering.height * scale);
    }
    
    ofEnableLighting();
    ofEnableDepthTest();
    camera.begin();
    bool bMap = bMapChip;
    if ( bMap ) kidd.bind();
    ofPushMatrix();
    ofTranslate(-ofGetWidth()/4.0, -ofGetHeight()/4.0);//realX, realY);
    for (int i=0; i<dots.size(); i++){
        dots[i].draw();
    }
    ofPopMatrix();
    if ( bMap ) kidd.unbind();
    camera.end();
    ofDisableDepthTest();
    ofDisableLighting();
    
    ofDrawBitmapString("Press ' ' to clear", 20,20);
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
            float x, y;
            if ( v["x"].isString() ){
                x = ofToInt( v["x"].asString() );
                y = ofToInt( v["y"].asString() );
            } else if ( v["x"].isInt() ){
                x = v["x"].asInt();
                y = v["y"].asInt();
            } else if ( v["x"].isDouble() ){
                x = v["x"].asDouble();
                y = v["y"].asDouble();
            } else {
                cout << "hm "<< v["x"].type() << endl;
            }
            d.points.push_back(ofVec2f(x,y));
        }
        drawings.push_back(d);
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
