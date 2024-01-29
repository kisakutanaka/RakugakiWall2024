#include "ofMain.h"
#include "ofApp.h"
#include "ofAppGLFWWindow.h"

//========================================================================
int main( ){

	//Use ofGLFWWindowSettings for more options like multi-monitor fullscreen
	ofGLFWWindowSettings settings;
	settings.setSize(1280, 720);
	settings.windowMode = OF_WINDOW; //can also be OF_FULLSCREEN
    
	auto window = ofCreateWindow(settings);
    
    settings.shareContextWith=window;
    
    auto sub_window = ofCreateWindow(settings);
    
    auto mainApp = make_shared<ofApp>();
    
    ofAddListener(sub_window->events().draw, mainApp.get(), &ofApp::drawSub);

	ofRunApp(window, mainApp);
	ofRunMainLoop();

}
