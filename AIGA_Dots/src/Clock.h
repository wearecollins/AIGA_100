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
    int radius, liveRadius;
    int colorMode;
    float offset;
    bool bColorFace;
    int lastFroze;
    
    bool bAnimating;
    int  rotMode;
    
    vector<float> angles;
    vector<float> targetAngle;
    
    ofVec2f vel;
    
    ofColor faceColor, armColor;
    
    Clock(){
        numFaces = -1;
        radius = -1;
        vel.x = 1.0;
        colorMode = 0;
        offset = 0;
        ofSetCircleResolution(300);
        bColorFace = true;
        ofSetLineWidth(3.0);
        lastFroze = 0;
        rotMode = 0;
        
        bAnimating = true;
    }
    
    void draw(){
        if (numFaces == -1 ) numFaces = 1;
        if (radius == -1 ) radius = liveRadius = 100;
        
        ofColor faceColorGood(faceColor);
        
        switch ( colorMode ){
            case 0:
                faceColor.setHue(ofWrap(ofWrap(offset, 0, 255) + ofMap(x + y, 0, ofGetWidth() + ofGetHeight(), 0, 120), 0, 255));
                break;
            case 1:
                faceColor.setHue(ofWrap(ofWrap(offset, 0, 255) + ofMap(y, 0, ofGetHeight(), 0, 120), 0, 255));
                break;
                
            case 2:
                faceColor.setHue(ofWrap(ofWrap(offset, 0, 255) + ofMap(sin(x), -1.0, 1.0, 0, 255), 0, 255));
                break;
                
            case 3:
                faceColor.setHue(ofWrap(ofWrap(offset, 0, 255) + ofMap(tan(x + y), -1.0, 1.0, 0, 255), 0, 255));
                break;
                
            case 4:
                faceColor.setHue(ofWrap(ofWrap(offset, 0, 255) + ofMap(y * 10.0 + ofWrap(ofGetElapsedTimeMillis(), 0, ofGetHeight() * 50.0), 0.0, ofGetHeight() * 50.0, 0, 255), 0, 255));
                break;
            case 5:
                faceColor.setHue( ofWrap(faceColor.getHue() + ofWrap(offset, 0, 255), 0, 255) );
                break;
        }
        if ( colorMode != 5 ){
            faceColor.setSaturation(150);
            faceColor.setBrightness(250);
        }
        if ( angles.size() != numFaces * 2 ){
            angles.clear();
            targetAngle.clear();
            for (int i=0; i<numFaces; i++){
                angles.push_back(0 - 45);
                angles.push_back(90 - 45);
                
                targetAngle.push_back(0 - 45);
                targetAngle.push_back(90 - 45);
            }
        }
        ofPushMatrix();
        ofTranslate(x,y);
        for ( int i=numFaces-1; i>=0; i--){
            float r = (float) radius / numFaces;
            r *= (i+1);
            
            float cr = (float) liveRadius / numFaces;
            cr *= (i+1);
            
            float mult = numFaces == 1 ? 1.0 : ofMap(i, numFaces-1, 0, 2.0, 1.0);
            ofColor lc(faceColor);
            float hue = ofWrap(lc.getHue() * mult + sin( ofGetElapsedTimeMillis() * .0001) * 10.0, 0, 255.0f);
            lc.setHue( hue );
            
            if ( bColorFace ){
                ofSetColor(lc);
            } else {
                ofSetColor( i % 2 != 0 ? 200 : 255, numFaces == 1 ? 255 : (i % 2 != 0 ? 150 : 100) );
            }
            ofCircle(0,0,r);
            
            if ( bColorFace ){
                ofSetColor(armColor);
            } else {
                ofSetColor(lc);
            }
            
            // ease angles
            angles[i * 2]       = angles[i * 2] * .9 + targetAngle[i * 2] * .1;
            angles[i * 2 + 1]   = angles[i * 2 + 1] * .9 + targetAngle[i * 2 + 1] * .1;
            
            ofPushMatrix();
            ofVec2f p1(cr  * .7, cr * .7);
            p1.rotate(angles[i * 2], ofVec2f(0,0));
            ofLine(ofVec2f(0,0),p1);
            ofPopMatrix();
            
            hue = ofWrap(lc.getHue() * mult + sin( ofGetElapsedTimeMillis() * .0001) * 20.0, 0, 255.0f);
            lc.setHue( hue );
            
            if ( bColorFace ){
                ofSetColor(armColor);
            } else {
                ofSetColor(lc);
            }
            
            ofPushMatrix();
            ofVec2f p2(cr  * .7, cr * .7);
            p2.rotate(angles[i * 2 + 1], ofVec2f(0,0));
            ofLine(ofVec2f(0,0),p2);
            ofPopMatrix();
        }
        ofPopMatrix();
        
        if ( ofGetElapsedTimeMillis() - lastFroze > 2000 && bAnimating ){
            bool b = false;
            float m = numFaces == 1 ? 1 : .5;
            for ( auto & a : targetAngle ){
                b = !b;
                if ( b ){
                    a += .1 * vel.x * m;
                } else {
                    a += 1. * vel.x * m;
                    m += .5;
                }
            }
        }
        if ( ofGetElapsedTimeMillis() - lastFroze > 2000 ){
            vel.x = vel.x * .9 + 1 * .1;
            offset = offset * .9;
        }
        
        // reset face color
        if ( colorMode == 5 ) faceColor = faceColorGood;
    }
    
    void rotateClockTo ( float angleA, float angleB ){
        bool b = false;
        for ( auto & a : targetAngle ){
            if ( b ){
                a = angleA;
            } else {
                a = angleB;
            }
            b = !b;
        }
    }
    
    void reset(){
        bool b = false;
        for ( auto & a : angles ){
            if ( b ){
                a = 0 - 45;
            } else {
                a = 90 - 45;
            }
            b = !b;
        }
    }
    
    void checkHit( int mx, int my, bool bFlip = false ){
//        if ( bAnimating ) return;
        
        ofVec2f m(mx,my);
        float dist = distance(m);
        int a1 = 0;
        int a2 = 90;
        if ( dist < radius ){
            if ( bFlip ){
                bColorFace = !bColorFace;
                bAnimating = !bAnimating;
                return;
            }
            rotMode++;
            if ( rotMode > 6 ) rotMode = 0;
            switch (rotMode) {
                case 0:
                    a1 = 0;
                    a2 = 90;
                    liveRadius = radius;
                    break;
                    
                case 1:
                    a1 = 0;
                    a2 = 180;
                    liveRadius = radius;
                    break;
                    
                case 2:
                    a1 = 90;
                    a2 = 180;
                    liveRadius = radius;
                    break;
                    
                case 3:
                    a1 = 90;
                    a2 = 270;
                    liveRadius = radius;
                    break;
                    
                case 4:
                    a1 = 180;
                    a2 = 270;
                    liveRadius = radius;
                    break;
                    
                case 5:
                    a1 = 0;
                    a2 = 270;
                    liveRadius = radius;
                    break;
                    
                case 6:
                    liveRadius = 0;
                    break;
    
            }
            
            bool b = false;
            for ( auto & a : targetAngle ){
                if ( b ){
                    a = a1 - 45;
                } else {
                    a = a2 - 45;
                }
                b = !b;
            }
        }
        
    }
    
    bool bMouseDown;
    ofVec2f lastMouse;
    float lastAngle;
    
    // rotate like a clock!
    void onMousePressed( int mx, int my ){
        ofVec2f m(mx,my);
        bMouseDown = fabs(distance(m)) < radius * 3;
        if ( bMouseDown ){
            rotateTo(mx, my);
            m -= *this;
            lastAngle = ofMap(angle(m), -180, 180, 0, 360) - 90;
            lastMouse.set(mx,my);
        }
    }
    
    // rotate like a clock!
    void onMouseDragged( int mx, int my ){
        if ( bMouseDown ){
            ofVec2f m(mx,my);
            m -= *this;
            float an = ofMap(angle(m), -180, 180, 0, 360) - 90;
            
            float diff = lastAngle - an;
            
            cout << diff << ":" << ofSign(diff)<< endl;
            
            if ( abs(diff) > 60 ){
                
                rotateClockTo(targetAngle[0] + (90) * -ofSign(diff), targetAngle[1] + (90) * -ofSign(diff));
            
                lastAngle = an;
            }
            lastMouse.set(mx,my);
        }
    }
    
    // rotate like a clock!
    void onMouseReleased( int mx, int my ){
        bMouseDown = false;
    }
    
    void rotateTo( int mx, int my ){
        ofVec2f m(mx,my);
        float dist = distance(m);
        m -= *this;
        
        if ( fabs(dist) < 3000 ){
            
            ofVec2f line = *this + ofVec2f(liveRadius  * .5, liveRadius * .5);
            bool b = false;
            int mult = 1.0;
            float an = ofWrap(ofMap(angle(m), -180, 180, 0, 360) - 90, 0, 360);
            
            cout << an << endl;
            
            for ( auto & a : targetAngle ){
                if ( an >= 30 && an <= 60 ){
                    a = 180;
                    if ( b ){
                        a += 90;
                    }
                    a += 45;
                } else if ( an > 60 && an <= 120 ){
                    a = 180;
                    if ( b ){
                        a += 180;
                    }
                    a += 45;
                } else if ( an > 120 && an <= 150 ){
                    a = 270;
                    if ( b ){
                        a += 90;
                    }
                    a += 45;
                    
                } else if ( an > 150 && an <= 210 ){
                    a = 270;
                    if ( b ){
                        a += 180;
                    }
                    a += 45;
                } else if ( an > 210 && an <= 240 ){
                    a = 360;
                    if ( b ){
                        a += 90;
                    }
                    a += 45;
                } else if ( an > 240 && an <= 300 ){
                    a = 360;
                    if ( b ){
                        a += 180;
                    }
                    a += 45;
                } else if ( an > 300 && an <= 330 ){
                    a = 450;
                    if ( b ){
                        a += 90;
                    }
                    a += 45;
                } else if ( (an > 330 && an <= 390)){// || (an >=0 && an < 30) ){
                    a = 450;
                    if ( b ){
                        a += 180;
                    }
                    a += 45;
                }
                
                
                b = !b;
            }
        }
        
    }
    
    void magnet( int mx, int my ){
        ofVec2f m(mx,my);
        float dist = distance(m);
        m -= *this;
    
        if ( (dist) < 20000 ){
            ofVec2f line = *this + ofVec2f(liveRadius  * .5, liveRadius * .5);
            bool b = false;
            int mult = 1.0;
            for ( auto & a : targetAngle ){
                a = ofMap(angle(m), -180, 180, 0, 360) * mult;// + 135;
                if ( b ){
                    a += 180;
                    //mult += 2.0;
                }
                b = !b;
            }
            vel.x += 10.0;
            offset += ofMap(dist, 0, 200, 10, 0, true);
            offset = ofClamp(offset, 0, 150);
            lastFroze = ofGetElapsedTimeMillis() - ofMap(dist, 0, 1000, 1000, 0, true);
        }
    }
};