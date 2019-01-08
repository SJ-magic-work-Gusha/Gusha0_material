/************************************************************
note
	if web cam on mac does not work, try
		> sudo killall VDCAssistant
************************************************************/
#pragma once

/************************************************************
************************************************************/
#include "ofMain.h"

#include "sj_common.h"
#include "SourceCode.h"

/************************************************************
************************************************************/

/**************************************************
**************************************************/
class ofApp : public ofBaseApp{
private:
	/****************************************
	****************************************/
	enum{
		NUM_CAMS = 2,
	};
	
	/****************************************
	****************************************/
	bool b_DispGUI;
	SOURCE_CODE* VisualizedSourceCode;
	ofTrueTypeFont font;
	
	int CamId[NUM_CAMS];
	ofVideoGrabber *VideoCam[NUM_CAMS];
	
	ofFbo fbo[NUM_CAMS];
	ofxSyphonServer SyphonServer[NUM_CAMS];
	
	ofFbo fbo_Black;
	ofFbo fbo_StrobeImage;
	ofShader shader_GrayBoostStrobe;
	ofxSyphonServer SyphonServer_StrobeImage;
	
	/****************************************
	****************************************/
	void setup_VideoCam();
	void setup_Gui();
	void Clear_fbo(ofFbo& fbo);
	void draw_CamImage();
	void draw_fbo();
	void draw_fbo_StrobeImage();
	void publish_syphon();
	
public:
	/****************************************
	****************************************/
	ofApp(int _CamId_0, int _CamId_1);
	~ofApp();
	void exit();
	
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
	
};
