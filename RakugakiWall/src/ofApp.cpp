#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    gui_.setup();
    gui_.add(red_threshold_.setup("red",100,0,255));
    gui_.add(hue_range_.setup("hue",ofVec2f(0,10),ofVec2f(0,0),ofVec2f(255,255)));
    gui_.add(sat_range_.setup("sat",ofVec2f(120,255),ofVec2f(0,0),ofVec2f(255,255)));
    gui_.add(val_range_.setup("val",ofVec2f(70,255),ofVec2f(0,0),ofVec2f(255,255)));
    gui_.add(brightness_.setup("brightness",255,0,255));
    gui_.add(lt_.setup("lt",ofVec2f(0,0),ofVec2f(0,0),ofVec2f(1920,1080)));
    gui_.add(rt_.setup("rt",ofVec2f(1920,0),ofVec2f(0,0),ofVec2f(1920,1080)));
    gui_.add(lb_.setup("lb",ofVec2f(0,1080),ofVec2f(0,0),ofVec2f(1920,1080)));
    gui_.add(rb_.setup("rb",ofVec2f(1920,1080),ofVec2f(0,0),ofVec2f(1920,1080)));
    
    for(auto & l:camera_.listDevices()){
        ofLogNotice()<<l.deviceName;
        if(l.deviceName.find("Capture")!=string::npos){
            camera_.setDeviceID(l.id);
            camera_.setup(1920,1080);
        }
    }
    ofLogNotice()<<camera_.getWidth();
    
    back_.load("back.png");
//    video_.load("test.mp4");
//    video_.play();
    ofSetFrameRate(60);
    
    contour_.setMinArea(0);
    
    drawing_.allocate(1920,1080,GL_RGBA);
    drawing_.begin();
    ofClear(0,0);
    drawing_.end();
}

//--------------------------------------------------------------
void ofApp::update(){
//    video_.update();
    camera_.update();
    if(camera_.getTexture().isAllocated()){
        // warp parspective
        vector<cv::Point2f> points1;
        points1.emplace_back(ofxCv::toCv(lt_));
        points1.emplace_back(ofxCv::toCv(rt_));
        points1.emplace_back(ofxCv::toCv(rb_));
        points1.emplace_back(ofxCv::toCv(lb_));
        vector<cv::Point2f> points2={
            cv::Point2f(0,0),
            cv::Point2f(1920,0),
            cv::Point2f(1920,1080),
            cv::Point2f(0,1080)
        };
        
        cv::Mat H = cv::findHomography(points1, points2);
        
        cv::Mat warped;
        
        cv::warpPerspective(ofxCv::toCv(camera_.getPixels()), warped, H, cv::Size(1920,1080));
        // ~warp parspective
        
        cv::Mat hsv;
        cv::cvtColor(warped, hsv, cv::COLOR_BGR2HSV);
        
        auto picked = hsv.at<cv::Vec3b>(min(1080,max(0,ofGetMouseY())),min(1920,max(0,ofGetMouseX())));
        ofLogNotice()<<picked;
        cv::Scalar lower_red(hue_range_->x, sat_range_->x, val_range_->x);   // 例: 色調の下限、彩度の下限、明度の下限
        cv::Scalar upper_red(hue_range_->y, sat_range_->y, val_range_->y); // 例: 色調の上限、彩度の上限、明度の上限
        cv::Mat red;
        cv::inRange(hsv, lower_red, upper_red, red);
        
        cv::threshold(red, red, red_threshold_, 255, cv::THRESH_BINARY);
        
        contour_.findContours(red);
        
    //        cv::threshold(bgr[1], green, green_threshold_, 255, cv::THRESH_BINARY);
        ofxCv::toOf(red, red_);
    //        ofxCv::toOf(green,green_);
        red_tex_.loadData(red_);
//        green_tex_.loadData(green_);
       
        drawing_.begin();
        ofPushStyle();
        for(int i = 0; i < contour_.size(); i++) {
            ofSetColor(0);
            ofDrawCircle(ofxCv::toOf(contour_.getCenter(i)),10);
        }
        ofPopStyle();
        drawing_.end();
    }
    
}

//--------------------------------------------------------------
void ofApp::draw(){
    if(preview_id_==0){
        camera_.draw(0,0);
        camera_.getTexture().drawSubsection(ofRectangle(ofGetMouseX()-50,ofGetMouseY()-50,100,100), ofRectangle(ofGetMouseX()-5,ofGetMouseY()-5,10,10));
        ofDrawCircle(ofGetMouseX(),ofGetMouseY(),1);
        ofDrawCircle(lt_,1);
        ofDrawCircle(rt_,1);
        ofDrawCircle(rb_,1);
        ofDrawCircle(lb_,1);
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
    ofPushStyle();
    ofSetColor(brightness_);
    back_.draw(0,0);
    drawing_.draw(0, 0);
//    for(int i = 0; i < contour_.size(); i++) {
//        ofSetColor(0);
//        ofDrawCircle(ofxCv::toOf(contour_.getCenter(i)),10);
//    }
    ofPopStyle();
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
    else if(key=='c'){
        drawing_.begin();
        ofClear(0,0);
        drawing_.end();
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
