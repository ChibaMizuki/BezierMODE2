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
	// �_�̕`��
	for (int i = 0; i < Anchor.size(); i++) {
		ofSetColor(ofColor::red);
		ofDrawCircle(Anchor[i].backward, PointSize);
		ofSetColor(255);
		ofDrawCircle(Anchor[i].center, PointSize);
		ofSetColor(ofColor::green);
		ofDrawCircle(Anchor[i].forward, PointSize);
	}

	// �_�Ɠ_
	ofSetColor(255);
	for (int i = 0; i < Anchor.size(); i++) {
		ofDrawLine(Anchor[i].center, Anchor[i].backward);
		ofDrawLine(Anchor[i].center, Anchor[i].forward);
	}

	// �Ȑ�
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
	// ����_�̓A���J�[�ɑ΂��đΏۈړ�
	if (button == OF_MOUSE_BUTTON_LEFT && forwardIndex != -1) {
		// control"&" �ɂ��邱�ƂŁA�R�s�[�ł͂Ȃ��Q�Ƃɂ���
		// �������Ȃ���anc���ҏW����邾����Anchor�ɕύX�����f����Ȃ�
		control& anc = Anchor[forwardIndex];
		ofVec2f currentPosition = anc.forward; // �ړ��O�̈ʒu
		ofVec2f newPosition(x - currentPosition.x, y - currentPosition.y); // �ړ���
		// forward�͏�ɉE���Ɉʒu����
		if (anc.center.x <= mousePosition.x) {
			anc.forward.set(x, y);
			anc.backward -= newPosition;
		}
		else if (anc.center.x > mousePosition.x) { // �z�����璆���Ŏ~�߂�
			anc.forward.x = anc.center.x;
			anc.backward.x = anc.center.x;
		}
		
	}
	else if (button == OF_MOUSE_BUTTON_LEFT && backwardIndex != -1) {
		// backward�͏�ɍ���
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
			if (centerIndex == 0 || centerIndex == Anchor.size() - 1) { // �[��x�Œ�
				anc.center.y = y;
			}
			else {
				if (mousePosition.x <= Anchor[centerIndex + 1].center.x
					&& mousePosition.x >= Anchor[centerIndex - 1].center.x) {
					// �A���J�[�Ɛ���_���ꏏ�ɓ����悤��
					ofVec2f currentPosition = anc.center;
					ofVec2f newPosition(x - currentPosition.x, y - currentPosition.y);
					anc.center.set(x, y);
					anc.backward += newPosition;
					anc.forward += newPosition;
				}
				// �A���J�[�̈ړ��ʂ𐧌�
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
				// ����_�Ƃ̋����Œ�
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
		// ���������ʒu��x�Ń\�[�g
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
