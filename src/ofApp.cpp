#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
	width = ofGetWidth();
	height = ofGetHeight();
	PointSize = 20.0;
	ConPointSize = 10.0;

	start = ofVec2f(0, height);		startCon = ofVec2f(50, height);
	end = ofVec2f(width, 0);		endCon = ofVec2f(width - 50, 0);

	AnchorPoint.resize(2);
	ControlPoint.resize(2);
	AnchorPoint[0] = start;			AnchorPoint[1] = end;
	ControlPoint[0] = startCon;		ControlPoint[1] = endCon;

	ofSetBackgroundColor(0);
}

//--------------------------------------------------------------
ofVec2f ofApp::Bezier(ofVec2f p0, ofVec2f p1, ofVec2f p2, ofVec2f p3, float t) {
	ofVec2f point;
	point = std::pow((1 - t), 3) * p0
		+ 3 * t * std::pow((1 - t), 2) * p1
		+ 3 * std::pow(t, 2) * (1 - t) * p2
		+ std::pow(t, 3) * p3;

	return point;
}

//--------------------------------------------------------------
void ofApp::update(){

}

//--------------------------------------------------------------
void ofApp::draw(){
	for (int i = 0; i < AnchorPoint.size(); i++) {
		ofSetColor(150);
		ofDrawCircle(AnchorPoint[i], PointSize);
		if (i % 2 == 0) {
			ofSetColor(ofColor::red);
		}
		else if (i % 2 == 1) {
			ofSetColor(ofColor::green);
		}
		ofDrawCircle(ControlPoint[i], ConPointSize);
	}

	// 曲線
	ofSetColor(ofColor::green);
	if (AnchorPoint.size() <= 2) { // 始点と終点しかない場合
		for (float t = 0; t < 1.0; t += interval) {
			ofVec2f p0 = Bezier(AnchorPoint[0], ControlPoint[0], ControlPoint[1], AnchorPoint[1], t);
			ofVec2f p1 = Bezier(AnchorPoint[0], ControlPoint[0], ControlPoint[1], AnchorPoint[1], t + interval);
			ofDrawLine(p0, p1);
		}
	}
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){

}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

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
