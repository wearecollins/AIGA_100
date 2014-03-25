//
//  Clock.h
//  clocks
//
//  Created by Brett Renfer on 3/25/14.
//
//

#include "ofMain.h"

class Clock : public ofVec2f {
public:
    
    int numFaces;
    int radius;
    int colorMode;
    float offset;
    bool bColorFace;
    int lastFroze;
    
    vector<float> angles;
    
    ofVec2f vel;
    
    ofColor c;
    
    Clock(){
        numFaces = -1;
        radius = -1;
        vel.x = 1.0;
        colorMode = 0;
        offset = 0;
        ofSetCircleResolution(300);
        bColorFace = true;
        ofSetLineWidth(2.0);
        lastFroze = 0;
    }
    
    void draw(){
        if (numFaces == -1 ) numFaces = 1;
        if (radius == -1 ) radius = 100;
        switch ( colorMode ){
            case 0:
                c.setHue(ofWrap(ofWrap(offset, 0, 255) + ofMap(x + y, 0, ofGetWidth() + ofGetHeight(), 0, 120), 0, 255));
                break;
            case 1:
                c.setHue(ofWrap(ofWrap(offset, 0, 255) + ofMap(y, 0, ofGetHeight(), 0, 120), 0, 255));
                break;
                
            case 2:
                c.setHue(ofWrap(ofWrap(offset, 0, 255) + ofMap(sin(x), -1.0, 1.0, 0, 255), 0, 255));
                break;
                
            case 3:
                c.setHue(ofWrap(ofWrap(offset, 0, 255) + ofMap(tan(x + y), -1.0, 1.0, 0, 255), 0, 255));
                break;
                
            case 4:
                c.setHue(ofWrap(ofWrap(offset, 0, 255) + ofMap(y * 10.0 + ofWrap(ofGetElapsedTimeMillis(), 0, ofGetHeight() * 50.0), 0.0, ofGetHeight() * 50.0, 0, 255), 0, 255));
                break;
        }
        c.setSaturation(150);
        c.setBrightness(250);
        
        if ( angles.size() != numFaces * 2 ){
            angles.clear();
            for (int i=0; i<numFaces; i++){
                angles.push_back(ofRandom(360));
                angles.push_back(ofRandom(360));
            }
        }
        ofPushMatrix();
        ofTranslate(x,y);
        for ( int i=numFaces-1; i>=0; i--){
            float r = (float) radius / numFaces;
            r *= (i+1);
            float mult = numFaces == 1 ? 1.0 : ofMap(i, numFaces-1, 0, 2.0, 1.0);
            ofColor lc(c);
            float hue = ofWrap(lc.getHue() * mult + sin( ofGetElapsedTimeMillis() * .0001) * 10.0, 0, 255.0f);
            lc.setHue( hue );
            
            if ( bColorFace ){
                ofSetColor(lc);
            } else {
                ofSetColor( i % 2 != 0 ? 200 : 255, numFaces == 1 ? 255 : (i % 2 != 0 ? 150 : 100) );
            }
            ofCircle(0,0,r);
            
            if ( bColorFace ){
                ofSetColor(255);
            } else {
                ofSetColor(lc);
            }
            ofPushMatrix();
//            ofTranslate(r, r);
//            ofTranslate(-r, -r);
            ofVec2f p1(r  * .7, r * .7);
            p1.rotate(angles[i * 2], ofVec2f(0,0));
            ofLine(ofVec2f(0,0),p1);
            ofPopMatrix();
            
            hue = ofWrap(lc.getHue() * mult + sin( ofGetElapsedTimeMillis() * .0001) * 20.0, 0, 255.0f);
            lc.setHue( hue );
            
            if ( bColorFace ){
                ofSetColor(255);
            } else {
                ofSetColor(lc);
            }
            
            ofPushMatrix();
//            ofTranslate(r, r);
            //            ofTranslate(-r, -r);
            ofVec2f p2(r  * .7,r * .7);
            p2.rotate(angles[i * 2 + 1], ofVec2f(0,0));
            ofLine(ofVec2f(0,0),p2);
            ofPopMatrix();
        }
        ofPopMatrix();
        
        if ( ofGetElapsedTimeMillis() - lastFroze > 2000 ){
            bool b = false;
            float m = numFaces == 1 ? 1 : .1;
            for ( auto & a : angles ){
                b = !b;
                if ( b ){
                    a += .1 * vel.x * m;
                } else {
                    a += 1. * vel.x * m;
                    m += .5;
                }
            }
            vel.x = vel.x * .9 + 1 * .1;
            offset = offset * .99;
        }
        
    }
    
    void rotateTo( int mx, int my ){
        ofVec2f m(mx,my);
        float dist = distance(m);
        m -= *this;
        if ( dist < 200 ){
            ofVec2f line = *this + ofVec2f(radius  * .5, radius * .5);
            bool b = false;
            int mult = 1.0;
            for ( auto & a : angles ){
                a = ofMap(angle(m), -180, 180, 0, 360) * mult;
                if ( b ){
                    a += 180;
                    //mult += 2.0;
                }
                b = !b;
            }
            vel.x += 10.0;
            offset += ofMap(dist, 0, 200, 10, 0, true);
            lastFroze = ofGetElapsedTimeMillis() - ofMap(dist, 0, 200, 1000, 0);
        }
    }
};