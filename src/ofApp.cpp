#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
	width = ofGetWidth();
	height = ofGetHeight();
	PointSize = 20.0;

	start = ofVec2f(0, height);		startCon = ofVec2f(50, height);
	end = ofVec2f(width, 0);		endCon = ofVec2f(width - 50, 0);

	AnchorPoint.resize(2);
	ControlPoint.resize(2);
	AnchorPoint[0] = start;			AnchorPoint[1] = end;
	ControlPoint[0] = startCon;		ControlPoint[1] = endCon;

	Anchor.resize(2);
	Initdif = ofVec2f(50, 0);
	Anchor[0].backward = start - Initdif;
	Anchor[0].center = start;
	Anchor[0].forward = start + Initdif;

	Anchor[1].backward = end - Initdif;
	Anchor[1].center = end;
	Anchor[1].forward = end + Initdif;

	ofSetBackgroundColor(0);
	ofEnableDepthTest();
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
	// “_‚Ì•`‰æ
	for (int i = 0; i < Anchor.size(); i++) {
		ofSetColor(ofColor::red);
		ofDrawCircle(Anchor[i].backward, PointSize);
		ofSetColor(255);
		ofDrawCircle(Anchor[i].center, PointSize);
		ofSetColor(ofColor::green);
		ofDrawCircle(Anchor[i].forward, PointSize);
	}

	// “_‚Æ“_
	ofSetColor(255);
	for (int i = 0; i < Anchor.size(); i++) {
		ofDrawLine(Anchor[i].center, Anchor[i].backward);
		ofDrawLine(Anchor[i].center, Anchor[i].forward);
	}

	// ‹Èü
	ofSetColor(ofColor::green);
	for (int i = 0; i < Anchor.size() - 1; i++) {
		for (float t = 0; t < 1; t += interval) {
			ofVec2f p0 = Bezier(Anchor[i].center, Anchor[i].forward, Anchor[i + 1].backward, Anchor[i + 1].center, t);
			ofVec2f p1 = Bezier(Anchor[i].center, Anchor[i].forward, Anchor[i + 1].backward, Anchor[i + 1].center, t + interval);
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
	if (button == OF_MOUSE_BUTTON_LEFT && forwardIndex != -1) {
		Anchor[forwardIndex].forward.set(x, y);
	}
	else if (button == OF_MOUSE_BUTTON_LEFT && backwardIndex != -1) {
		Anchor[backwardIndex].backward.set(x, y);
	}
	else if (button == OF_MOUSE_BUTTON_LEFT && centerIndex != -1) {
		if (x >= 0 && x <= width && y >= 0 && y <= height) {
			if (centerIndex == 0 || centerIndex == Anchor.size() - 1) {
				Anchor[centerIndex].center.y = y;
			}
			else {
				Anchor[centerIndex].center.set(x, y);
			}
		}
	}
}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){
	ofVec2f mousePosition(x, y);
	if (button == OF_MOUSE_BUTTON_LEFT) {
		for (int i = 0; i < Anchor.size(); i++) {
			if (mousePosition.distance(Anchor[i].forward) <= PointSize) {
				forwardIndex = i;
				break;
			}
			else if (mousePosition.distance(Anchor[i].backward) <= PointSize) {
				backwardIndex = i;
				break;
			}
			else if (mousePosition.distance(Anchor[i].center) <= PointSize) {
				centerIndex = i;
				break;
			}
		}
	}

	if (button == OF_MOUSE_BUTTON_RIGHT) {
		control insertPoint = {
			mousePosition - Initdif,
			mousePosition,
			mousePosition + Initdif
		};
		Anchor.insert(Anchor.end() - 1, insertPoint);
	}
}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){
	if (button == OF_MOUSE_BUTTON_LEFT) {
		forwardIndex = -1;
		centerIndex = -1;
		backwardIndex = -1;
	}
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
