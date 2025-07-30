#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
	width = ofGetWidth();
	height = ofGetHeight();
	PointSize = 20.0;

	start = ofVec2f(0, height);
	end = ofVec2f(width, 0);

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
	// 点の描画
	for (int i = 0; i < Anchor.size(); i++) {
		ofSetColor(ofColor::red);
		ofDrawCircle(Anchor[i].backward, PointSize);
		ofSetColor(255);
		ofDrawCircle(Anchor[i].center, PointSize);
		ofSetColor(ofColor::green);
		ofDrawCircle(Anchor[i].forward, PointSize);
	}

	// 点と点
	ofSetColor(255);
	for (int i = 0; i < Anchor.size(); i++) {
		ofDrawLine(Anchor[i].center, Anchor[i].backward);
		ofDrawLine(Anchor[i].center, Anchor[i].forward);
	}

	// 曲線
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
	ofVec2f mousePosition(x, y);
	// 制御点はアンカーに対して対象移動
	if (button == OF_MOUSE_BUTTON_LEFT && forwardIndex != -1) {
		ofVec2f currentPosition = Anchor[forwardIndex].forward;
		ofVec2f newPosition(x - currentPosition.x, y - currentPosition.y);
		// forwardは常に右側に位置する
		if (Anchor[forwardIndex].center.x <= mousePosition.x) {
			Anchor[forwardIndex].forward.set(x, y);
			Anchor[forwardIndex].backward -= newPosition;
		}
		else if (Anchor[forwardIndex].center.x > mousePosition.x) {
			Anchor[forwardIndex].forward.x = Anchor[forwardIndex].center.x;
			Anchor[forwardIndex].backward.x = Anchor[forwardIndex].center.x;
		}
		
	}
	else if (button == OF_MOUSE_BUTTON_LEFT && backwardIndex != -1) {
		// backwardは常に左側
		ofVec2f currentPosition = Anchor[backwardIndex].backward;
		ofVec2f newPosition(x - currentPosition.x, y - currentPosition.y);
		if (Anchor[backwardIndex].center.x >= mousePosition.x) {
			Anchor[backwardIndex].backward.set(x, y);
			Anchor[backwardIndex].forward -= newPosition;
		}
		else if (Anchor[backwardIndex].center.x < mousePosition.x) {
			Anchor[backwardIndex].backward.x = Anchor[backwardIndex].center.x;
			Anchor[backwardIndex].forward.x = Anchor[backwardIndex].center.x;
		}
		
	}
	else if (button == OF_MOUSE_BUTTON_LEFT && centerIndex != -1) {
		if (x > 0 && x < width && y >= 0 && y <= height) {
			if (centerIndex == 0 || centerIndex == Anchor.size() - 1) {
				Anchor[centerIndex].center.y = y;
			}
			else {
				if (mousePosition.x <= Anchor[centerIndex + 1].center.x
					&& mousePosition.x >= Anchor[centerIndex - 1].center.x) {
					// アンカーと制御点が一緒に動くように
					ofVec2f currentPosition = Anchor[centerIndex].center;
					ofVec2f newPosition(x - currentPosition.x, y - currentPosition.y);
					Anchor[centerIndex].center.set(x, y);
					Anchor[centerIndex].backward += newPosition;
					Anchor[centerIndex].forward += newPosition;
				}
				// アンカーの移動量を制限
				else if (mousePosition.x > Anchor[centerIndex + 1].center.x) {
					ofVec2f offset(Anchor[centerIndex].center - Anchor[centerIndex].backward);
					Anchor[centerIndex].center.x = Anchor[centerIndex + 1].center.x;
					Anchor[centerIndex].forward = Anchor[centerIndex].center + offset;
				}
				else if (mousePosition.x < Anchor[centerIndex - 1].center.x) {
					ofVec2f offset(Anchor[centerIndex].center - Anchor[centerIndex].forward);
					Anchor[centerIndex].center.x = Anchor[centerIndex - 1].center.x;
					Anchor[centerIndex].backward = Anchor[centerIndex].center + offset;
				}
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
		if (x > 0 && x < width && y >= 0 && y <= height) {
			control insertPoint = {
			mousePosition - Initdif,
			mousePosition,
			mousePosition + Initdif
			};
			Anchor.insert(Anchor.end() - 1, insertPoint);
		}
		sort(Anchor.begin(), Anchor.end(), [](const control& a, const control& b) {
			return a.center.x < b.center.x;
			});
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
