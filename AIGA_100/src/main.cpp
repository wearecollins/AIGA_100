#include "ofMain.h"
#include "ofAppGLFWWindow.h"
#include "testApp.h"

//========================================================================
int main( ){
    ofAppGLFWWindow window;
    window.setNumSamples(8);
	ofSetupOpenGL(&window, 1366,800,OF_WINDOW);			// <-------- setup the GL context

	// this kicks off the running of my app
	// can be OF_WINDOW or OF_FULLSCREEN
	// pass in width and height too:
	ofRunApp(new testApp());

}
