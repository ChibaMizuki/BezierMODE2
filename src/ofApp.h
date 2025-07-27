#pragma once

#include "ofMain.h"

class ofApp : public ofBaseApp{
	private:
		ofVec2f start, end, startCon, endCon;
		vector<ofVec2f> AnchorPoint;
		vector<ofVec2f> ControlPoint;
		float PointSize;
		float ConPointSize;

		float interval = 0.01;
		int width, height;

	public:
		void setup();
		void update();
		void draw();

		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void mouseEntered(int x, int y);
		void mouseExited(int x, int y);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);

		ofVec2f Bezier(ofVec2f, ofVec2f, ofVec2f, ofVec2f, float);
		
};
