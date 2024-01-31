#pragma once

#include "ofMain.h"
#include "ofxCv.h"
#include "ofxGui.h"

class ofApp : public ofBaseApp{

	public:
		void setup() override;
		void update() override;
		void draw() override;
    void drawSub(ofEventArgs & args);
		void exit() override;

		void keyPressed(int key) override;
		void keyReleased(int key) override;
		void mouseMoved(int x, int y ) override;
		void mouseDragged(int x, int y, int button) override;
		void mousePressed(int x, int y, int button) override;
		void mouseReleased(int x, int y, int button) override;
		void mouseScrolled(int x, int y, float scrollX, float scrollY) override;
		void mouseEntered(int x, int y) override;
		void mouseExited(int x, int y) override;
		void windowResized(int w, int h) override;
		void dragEvent(ofDragInfo dragInfo) override;
		void gotMessage(ofMessage msg) override;
    
    ofxCv::ContourFinder contour_;
    ofxPanel gui_;
    ofxVec2Slider hue_range_;
    ofxVec2Slider sat_range_;
    ofxVec2Slider val_range_;
    ofxFloatSlider red_threshold_;
    ofxIntSlider brightness_;
    ofxVec2Slider lt_;
    ofxVec2Slider rt_;
    ofxVec2Slider lb_;
    ofxVec2Slider rb_;
    
    ofFbo drawing_;

    ofImage back_;
    ofVideoGrabber camera_;
    ofVideoPlayer video_;
    ofPixels pix_;
    ofPixels red_;
    ofTexture red_tex_;
    ofPixels green_;
    ofTexture green_tex_;
    int preview_id_=0;
		
};
