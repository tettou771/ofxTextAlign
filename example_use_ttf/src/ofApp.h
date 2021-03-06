#pragma once

#include "ofMain.h"
#include "ofxTextAlignTTF.h"
#include "ofxGui.h"

class ofApp : public ofBaseApp {
	
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
	void windowResized(int w, int h);
	void dragEvent(ofDragInfo dragInfo);
	void gotMessage(ofMessage msg);
private:
	ofxTextAlignTTF text_;
	ofxPanel panel_;
	ofxSlider<int> x_, y_;
	
	ofxSlider<int> halign_, valign_;
	ofxLabel align_str_;
	void refreshAlignStr(int&);
	
	ofxSlider<int> min_width_, max_width_, min_height_, max_height_;
	ofxSlider<int> letter_spacing_, line_height_;
	void callbackMinWidth(int& val);
	void callbackMaxWidth(int& val);
	void callbackMinHeight(int& val);
	void callbackMaxHeight(int& val);
	void callbackLetterSpacing(int& val);
	void callbackLineHeight(int& val);
};
