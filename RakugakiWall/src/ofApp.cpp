#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    gui_.setup();
    gui_.add(red_threshold_.setup("red",100,0,255));
    gui_.add(green_threshold_.setup("green",100,0,255));
    gui_.add(hue_range_.setup("hue",ofVec2f(0,10),ofVec2f(0,0),ofVec2f(255,255)));
    gui_.add(sat_range_.setup("sat",ofVec2f(120,255),ofVec2f(0,0),ofVec2f(255,255)));
    gui_.add(val_range_.setup("val",ofVec2f(70,255),ofVec2f(0,0),ofVec2f(255,255)));
    
    zoom_.allocate(100,100,GL_RGB);
    
    video_.load("test.mp4");
    video_.play();
    ofSetFrameRate(60);
}

//--------------------------------------------------------------
void ofApp::update(){
    video_.update();
    if(video_.getTexture().isAllocated()){
        video_.getTexture().readToPixels(pix_);
        cv::Mat resized;
        cv::resize(ofxCv::toCv(pix_), resized, cv::Size(1280,720));
        cv::Mat hsv;
        cv::cvtColor(resized, hsv, cv::COLOR_RGB2HSV);
        
        auto picked = hsv.at<cv::Vec3b>(min(720,max(0,ofGetMouseY())),min(1280,max(0,ofGetMouseX())));
        ofLogNotice()<<picked;
        cv::Scalar lower_red(hue_range_->x, sat_range_->x, val_range_->x);   // 例: 色調の下限、彩度の下限、明度の下限
        cv::Scalar upper_red(hue_range_->y, sat_range_->y, val_range_->y); // 例: 色調の上限、彩度の上限、明度の上限
        cv::Mat red;
        cv::inRange(hsv, lower_red, upper_red, red);
        
    //        cv::threshold(red, red, red_threshold_, 255, cv::THRESH_BINARY);
    //        cv::threshold(bgr[1], green, green_threshold_, 255, cv::THRESH_BINARY);
        ofxCv::toOf(red, red_);
    //        ofxCv::toOf(green,green_);
        red_tex_.loadData(red_);
//        green_tex_.loadData(green_);
        zoom_.begin();
        ofPushMatrix();
        ofPushStyle();
        ofSetRectMode(OF_RECTMODE_CENTER);
        ofScale(10);
        ofTranslate(video_.getWidth()*0.5, video_.getHeight()*0.5);
        ofTranslate(-ofGetMouseX(), -ofGetMouseY());
        video_.draw(0,0);
        ofPopMatrix();
        ofDrawLine(50,0,50,100);
        ofDrawLine(0,50,100,50);
        ofPopStyle();
        zoom_.end();
    }
}

//--------------------------------------------------------------
void ofApp::draw(){
    if(preview_id_==0){
        video_.draw(0,0,1280,720);
        ofPushMatrix();
        ofPushStyle();
        ofSetRectMode(OF_RECTMODE_CENTER);
        ofTranslate(ofGetMouseX(),ofGetMouseY());
        zoom_.draw(0,0);
        ofNoFill();
        ofDrawRectangle(0, 0, 100, 100);
        ofPopStyle();
        ofPopMatrix();
        ofDrawCircle(ofGetMouseX(),ofGetMouseY(),1);
    }
    else if(preview_id_==1&&red_tex_.isAllocated()){
        red_tex_.draw(0,0,1280,720);
    }
    else if(preview_id_==2&&green_tex_.isAllocated()){
        video_.draw(0,0,1280,720);
        ofPushMatrix();
        ofPushStyle();
        ofSetRectMode(OF_RECTMODE_CENTER);
        ofTranslate(ofGetMouseX(),ofGetMouseY());
//        zoom_.draw(0,0);
//        ofNoFill();
//        ofDrawRectangle(0, 0, 100, 100);
        ofPopStyle();
        ofPopMatrix();
        ofDrawCircle(ofGetMouseX(),ofGetMouseY(),1);
//        green_tex_.draw(0,0,1280,720);
    }
    gui_.draw();
}

void ofApp::drawSub(ofEventArgs &args){
    
}

//--------------------------------------------------------------
void ofApp::exit(){

}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){
    if(key=='0')preview_id_=0;
    else if(key=='1')preview_id_=1;
    else if(key=='2')preview_id_=2;
    else if(key==' '){
        if(video_.isPlaying())video_.setPaused(true);
        else video_.play();
    }
}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseScrolled(int x, int y, float scrollX, float scrollY){

}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}
