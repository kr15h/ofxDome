#include "ofMain.h"
#include "testApp.h"
#include "ofAppGLFWWindow.h"

//========================================================================
int main( ){
    ofPtr<ofAppGLFWWindow> window = ofPtr<ofAppGLFWWindow>(new ofAppGLFWWindow());
    window->setMultiDisplayFullscreen(true);
    
	ofSetupOpenGL(window, 1280*3, 800, OF_FULLSCREEN);
    ofSetWindowShape(ofGetScreenWidth()*3, ofGetScreenHeight());

	// this kicks off the running of my app
	// can be OF_WINDOW or OF_FULLSCREEN
	// pass in width and height too:
	ofRunApp(new testApp());

}
