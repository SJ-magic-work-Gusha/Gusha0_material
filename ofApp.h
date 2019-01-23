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
class ALPHA_BASE__STROBEIMAGE{
private:
	enum STATE{
		STATE__OFF,
		STATE__STROBE,
		STATE__FADEOUT,
	};
	
	STATE State;
	float t_from;
	float d_Fade;
	
	double alpha;
	
public:
	ALPHA_BASE__STROBEIMAGE(double _d_Fade)
	: t_from(0), d_Fade(_d_Fade), alpha(1), State(STATE__OFF)
	{
		if(d_Fade <= 0) d_Fade = 1.0;
	}
	
	void update(float now, bool b_SendStrobe)
	{
		switch(State){
			case STATE__OFF:
				if(b_SendStrobe){
					State = STATE__STROBE;
					alpha = 1.0;
				}
				break;
				
			case STATE__STROBE:
				if(!b_SendStrobe){
					State = STATE__FADEOUT;
					t_from = now;
				}
				break;
				
			case STATE__FADEOUT:
				if(b_SendStrobe){
					State = STATE__STROBE;
					alpha = 1.0;
				}else{
					alpha = 1.0 - 1/d_Fade * (now - t_from);
					if(1 < alpha) alpha = 1.0; // ないはずだけど、一応
					
					if(alpha <= 0){
						State = STATE__OFF;
						alpha = 1.0;
					}
				}
				break;
		}
	}
	
	double get_Alpga() { return alpha; }
	
	bool Is_StrobeOn(){
		if( (State == STATE__STROBE) || (State == STATE__FADEOUT) )	return true;
		else														return false;
	}
};

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
	ALPHA_BASE__STROBEIMAGE AlphaBase_StrobeImage;
	
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
