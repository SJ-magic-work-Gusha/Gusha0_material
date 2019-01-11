/************************************************************
************************************************************/
#pragma once

/************************************************************
************************************************************/
#include "stdio.h"

#include "ofMain.h"
#include "ofxGui.h"

/************************************************************
************************************************************/
#define ERROR_MSG(); printf("Error in %s:%d\n", __FILE__, __LINE__);

/************************************************************
************************************************************/

enum{
	WINDOW_WIDTH	= 1280,
	WINDOW_HEIGHT	= 1000,
	WINDOW_HEIGHT_PRESSED	= 720,
	
	CAM_WIDTH	= 320,
	CAM_HEIGHT	= 180,
};

enum{
	BUF_SIZE_S = 1000,
	BUF_SIZE_L = 6000,
};

/************************************************************
************************************************************/

/**************************************************
Derivation
	class MyClass : private Noncopyable {
	private:
	public:
	};
**************************************************/
class Noncopyable{
protected:
	Noncopyable() {}
	~Noncopyable() {}

private:
	void operator =(const Noncopyable& src);
	Noncopyable(const Noncopyable& src);
};

/**************************************************
**************************************************/
class GUI_GLOBAL{
private:
	/****************************************
	****************************************/
	
public:
	/****************************************
	****************************************/
	void setup(string GuiName, string FileName = "gui.xml", float x = 10, float y = 10);
	
	/****************************************
	****************************************/
	ofxPanel gui;
	
	ofxToggle b_DispImage_SourceCode;
	// ofxToggle b_SourceCode;
	
	ofxToggle Cam1_to_Fbo[2];
	
	ofxFloatSlider BoostBrightness_for_Strobe;
	ofxToggle b_SendStrobeImage;
	ofxFloatSlider d_Strobe_sec;
};

/************************************************************
************************************************************/
extern GUI_GLOBAL* Gui_Global;

extern FILE* fp_Log;

extern int GPIO_0;
extern int GPIO_1;

/************************************************************
************************************************************/

