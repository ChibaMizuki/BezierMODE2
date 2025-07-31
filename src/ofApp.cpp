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
	ofVec2f point = std::pow((1 - t), 3) * p0
					+ 3 * t * std::pow((1 - t), 2) * p1
					+ 3 * std::pow(t, 2) * (1 - t) * p2
					+ std::pow(t, 3) * p3;
	return point;
}

//--------------------------------------------------------------
void ofApp::update(){
	width = ofGetWidth();
	height = ofGetHeight();
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
		// control"&" にすることで、コピーではなく参照にする
		// そうしないとancが編集されるだけでAnchorに変更が反映されない
		control& anc = Anchor[forwardIndex];
		ofVec2f currentPosition = anc.forward; // 移動前の位置
		ofVec2f newPosition(x - currentPosition.x, y - currentPosition.y); // 移動量
		// forwardは常に右側に位置する
		if (anc.center.x <= mousePosition.x) {
			anc.forward.set(x, y);
			anc.backward -= newPosition;
		}
		else if (anc.center.x > mousePosition.x) { // 越えたら中央で止める
			anc.forward.x = anc.center.x;
			anc.backward.x = anc.center.x;
		}
		
	}
	else if (button == OF_MOUSE_BUTTON_LEFT && backwardIndex != -1) {
		// backwardは常に左側
		control& anc = Anchor[backwardIndex];
		ofVec2f currentPosition = anc.backward;
		ofVec2f newPosition(x - currentPosition.x, y - currentPosition.y);
		if (anc.center.x >= mousePosition.x) {
			anc.backward.set(x, y);
			anc.forward -= newPosition;
		}
		else if (anc.center.x < mousePosition.x) {
			anc.backward.x = anc.center.x;
			anc.forward.x = anc.center.x;
		}
		
	}
	else if (button == OF_MOUSE_BUTTON_LEFT && centerIndex != -1) {
		control& anc = Anchor[centerIndex];
		if (x > 0 && x < width && y >= 0 && y <= height) {
			if (centerIndex == 0 || centerIndex == Anchor.size() - 1) { // 端はx固定
				anc.center.y = y;
			}
			else {
				if (mousePosition.x <= Anchor[centerIndex + 1].center.x
					&& mousePosition.x >= Anchor[centerIndex - 1].center.x) {
					// アンカーと制御点が一緒に動くように
					ofVec2f currentPosition = anc.center;
					ofVec2f newPosition(x - currentPosition.x, y - currentPosition.y);
					anc.center.set(x, y);
					anc.backward += newPosition;
					anc.forward += newPosition;
				}
				// アンカーの移動量を制限
				else if (mousePosition.x > Anchor[centerIndex + 1].center.x) {
					anc.center.x = Anchor[centerIndex + 1].center.x;
					anc.forward = anc.center + backwardDistance;
					anc.backward = anc.center + forwardDistance;
				}
				else if (mousePosition.x < Anchor[centerIndex - 1].center.x) {
					anc.center.x = Anchor[centerIndex - 1].center.x;
					anc.backward = anc.center + forwardDistance;
					anc.forward = anc.center + backwardDistance;
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
				// 制御点との距離固定
				forwardDistance = Anchor[centerIndex].center - Anchor[centerIndex].forward;
				backwardDistance = Anchor[centerIndex].center - Anchor[centerIndex].backward;
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
		// 生成した位置のxでソート
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
