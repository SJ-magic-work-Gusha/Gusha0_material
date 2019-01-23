/************************************************************
************************************************************/
#include "ofApp.h"
#include <stdio.h>

/************************************************************
************************************************************/

/******************************
******************************/
ofApp::ofApp(int _CamId_0, int _CamId_1)
: b_DispGUI(true)
, VisualizedSourceCode(NULL)
, AlphaBase_StrobeImage(1.0)
{
	VisualizedSourceCode = new SOURCE_CODE;
	
	CamId[0] = _CamId_0;
	CamId[1] = _CamId_1;
	
	fp_Log = fopen("../../../data/Log.csv", "w");
}

/******************************
******************************/
ofApp::~ofApp()
{
	if(fp_Log) fclose(fp_Log);
	
	if(VisualizedSourceCode) delete VisualizedSourceCode;
}

/******************************
******************************/
void ofApp::exit()
{
}

/******************************
******************************/
void ofApp::setup(){

	/********************
	********************/
	font.load("font/RictyDiminishedDiscord-Bold.ttf", 15, true, true, true);
	
	/********************
	********************/
	ofSetBackgroundAuto(true);
	
	ofSetWindowTitle("Gusha:material");
	ofSetVerticalSync(true);
	ofSetFrameRate(20);
	ofSetWindowShape(WINDOW_WIDTH, WINDOW_HEIGHT);
	ofSetEscapeQuitsApp(false);
	
	ofEnableAlphaBlending();
	ofEnableBlendMode(OF_BLENDMODE_ALPHA);
	// ofEnableBlendMode(OF_BLENDMODE_ADD);
	ofEnableSmoothing();
	
	/********************
	********************/
	setup_Gui();
	
	VisualizedSourceCode->setup();
	
	setup_VideoCam();
	
	/********************
	********************/
	for(int i = 0; i < NUM_CAMS; i++){
		fbo[i].allocate(CAM_WIDTH, CAM_HEIGHT, GL_RGBA, 0);
		Clear_fbo(fbo[i]);
		
		char buf[BUF_SIZE_S];
		sprintf(buf, "Gusha_material_fbo%d", i);
		SyphonServer[i].setName(buf);
	}
	
	fbo_Black.allocate(CAM_WIDTH, CAM_HEIGHT, GL_RGBA, 0);
	Clear_fbo(fbo_Black);
	
	fbo_StrobeImage.allocate(CAM_WIDTH, CAM_HEIGHT, GL_RGBA, 0);
	Clear_fbo(fbo_StrobeImage);
	shader_GrayBoostStrobe.load( "shader/GrayBoostStrobe.vert", "shader/GrayBoostStrobe.frag" );
	SyphonServer_StrobeImage.setName("Gusha_material_StrobeImage");
}

/******************************
******************************/
void ofApp::setup_VideoCam()
{
	/********************
	********************/
	for(int i = 0; i < NUM_CAMS; i++){
		VideoCam[i] = new ofVideoGrabber;
		
		ofSetLogLevel(OF_LOG_VERBOSE);
		VideoCam[i]->setVerbose(true);
		
		vector< ofVideoDevice > Devices = VideoCam[i]->listDevices();// 上の行がないと、List表示されない.
		
		if(CamId[i] == -1){
			std::exit(1);
		}else{
			if(Devices.size() <= CamId[i]) { ERROR_MSG(); std::exit(1); }
			
			VideoCam[i]->setDeviceID(CamId[i]);
			VideoCam[i]->initGrabber(CAM_WIDTH, CAM_HEIGHT);
			
			printf("> video size asked = (%d, %d), actual = (%d, %d)\n", int(CAM_WIDTH), int(CAM_HEIGHT), int(VideoCam[i]->getWidth()), int(VideoCam[i]->getHeight()));
			fflush(stdout);
		}
	}
}

/******************************
description
	memoryを確保は、app start後にしないと、
	segmentation faultになってしまった。
******************************/
void ofApp::setup_Gui()
{
	/********************
	********************/
	Gui_Global = new GUI_GLOBAL;
	Gui_Global->setup("Gusha:material", "gui.xml", 10, 10);
}

/******************************
******************************/
void ofApp::Clear_fbo(ofFbo& fbo)
{
	fbo.begin();
	
	// Clear with alpha, so we can capture via syphon and composite elsewhere should we want.
	glClearColor(0.0, 0.0, 0.0, 0.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	ofClear(0, 0, 0, 0);
	
	fbo.end();
}

/******************************
******************************/
void ofApp::update(){
	/********************
	********************/
	float now = ofGetElapsedTimef();
	
	/********************
	********************/
	for(int i = 0; i < NUM_CAMS; i++){
		VideoCam[i]->update();
	}
	
	AlphaBase_StrobeImage.update(now, Gui_Global->b_SendStrobeImage);
	
	/********************
	********************/
	if(VisualizedSourceCode) VisualizedSourceCode->update(now);
}

/******************************
******************************/
void ofApp::draw(){
	/********************
	********************/
	ofClear(0, 0, 0, 0);
	ofSetColor(255);

	/********************
	update fbo
	********************/
	draw_fbo();
	draw_fbo_StrobeImage();
	
	/********************
	draw
	********************/
	if(Gui_Global->b_DispImage_SourceCode){
		if(VisualizedSourceCode) VisualizedSourceCode->draw();
	}else{
		draw_CamImage();
	}
	
	/********************
	syphon
	********************/
	if(VisualizedSourceCode) VisualizedSourceCode->publish_syphon();
	publish_syphon();
	
	/********************
	********************/
	if(b_DispGUI) Gui_Global->gui.draw();
	
	/********************
	********************/
	char buf[BUF_SIZE_S];
	sprintf(buf, "%7.2f", ofGetFrameRate());
	ofSetColor(255);
	font.drawString(buf, ofGetWidth() - font.stringWidth("xxxxxxxxxx"), ofGetHeight() - font.stringHeight("xxxxxxxxxx"));
}

/******************************
******************************/
void ofApp::publish_syphon()
{
	/********************
	********************/
	for(int i = 0; i < NUM_CAMS; i++){
		ofTexture tex = fbo[i].getTextureReference();
		SyphonServer[i].publishTexture(&tex);
	}
	
	/********************
	********************/
	// if(Gui_Global->b_SendStrobeImage){
	if(AlphaBase_StrobeImage.Is_StrobeOn()){
		ofTexture tex = fbo_StrobeImage.getTextureReference();
		SyphonServer_StrobeImage.publishTexture(&tex);
	}else{
		ofTexture tex = fbo_Black.getTextureReference();
		SyphonServer_StrobeImage.publishTexture(&tex);
	}
}

/******************************
******************************/
void ofApp::draw_CamImage()
{
	/********************
	********************/
	ofEnableAlphaBlending();
	ofEnableBlendMode(OF_BLENDMODE_ALPHA);
	// ofEnableBlendMode(OF_BLENDMODE_ADD);
	// ofEnableSmoothing();
	
	/********************
	********************/
	const float DISP_WIDTH	= WINDOW_WIDTH/3;
	const float DISP_HEIGHT	= WINDOW_HEIGHT_PRESSED/3;
	
	/********************
	********************/
	ofSetColor(255);
	
	VideoCam[0]->draw(0, DISP_HEIGHT * 1, DISP_WIDTH, DISP_HEIGHT);
	VideoCam[1]->draw(0, DISP_HEIGHT * 2, DISP_WIDTH, DISP_HEIGHT);
	
	fbo_StrobeImage.draw(DISP_WIDTH * 2, 0, DISP_WIDTH, DISP_HEIGHT);
	fbo[0].draw(DISP_WIDTH * 2, DISP_HEIGHT * 1, DISP_WIDTH, DISP_HEIGHT);
	fbo[1].draw(DISP_WIDTH * 2, DISP_HEIGHT * 2, DISP_WIDTH, DISP_HEIGHT);
	
	/********************
	line
	********************/
	ofSetLineWidth(2);
	if(Gui_Global->Cam1_to_Fbo[0])	ofDrawLine(DISP_WIDTH, DISP_HEIGHT * 5/2, DISP_WIDTH * 2, DISP_HEIGHT * 3/2);
	else							ofDrawLine(DISP_WIDTH, DISP_HEIGHT * 3/2, DISP_WIDTH * 2, DISP_HEIGHT * 3/2);
	
	if(Gui_Global->Cam1_to_Fbo[1])	ofDrawLine(DISP_WIDTH, DISP_HEIGHT * 5/2, DISP_WIDTH * 2, DISP_HEIGHT * 5/2);
	else							ofDrawLine(DISP_WIDTH, DISP_HEIGHT * 3/2, DISP_WIDTH * 2, DISP_HEIGHT * 5/2);
	
	{ // draw line
		const int x0 = 800;
		const int x1 = DISP_WIDTH * 2;
		const int y0 = 200;
		const int y1 = 280;
		
		ofDrawLine(x0, y0, x1, y0);
		ofDrawLine(x0, y0, x0, y1);
		ofDrawLine(x0, y1, x1, y1);
	}
}

/******************************
******************************/
void ofApp::draw_fbo_StrobeImage()
{
	ofDisableAlphaBlending();
	
	fbo_StrobeImage.begin();
		ofClear(0, 0, 0, 0);
		
		shader_GrayBoostStrobe.begin();
			shader_GrayBoostStrobe.setUniform1f( "Boost", Gui_Global->BoostBrightness_for_Strobe);
			
			// if(Gui_Global->b_SendStrobeImage){
			if(AlphaBase_StrobeImage.Is_StrobeOn()){
				/*
				const float max = 1.0;
				const float min = 0.2;
				const float tan = (min - max)/Gui_Global->d_Strobe_sec;
				
				float dt = fmod(double(ofGetElapsedTimef()), double(Gui_Global->d_Strobe_sec));
				float alpha = tan * dt + 1;
				
				shader_GrayBoostStrobe.setUniform1f( "alpha", alpha);
				*/
				
				const float max = 1.0;
				const float min = 0.2;
				float dt = fmod(double(ofGetElapsedTimef()), double(Gui_Global->d_Strobe_sec));
				float alpha;
				
				if(dt < Gui_Global->d_Strobe_sec/2)	alpha = max;
				else								alpha = min;
				
				shader_GrayBoostStrobe.setUniform1f( "alpha", alpha * AlphaBase_StrobeImage.get_Alpga());
				
			}else{
				shader_GrayBoostStrobe.setUniform1f( "alpha", 1);
			}
			
			ofSetColor( 255, 255, 255, 255 );
			fbo[0].draw(0, 0);
		
		shader_GrayBoostStrobe.end();		
	fbo_StrobeImage.end();
}

/******************************
******************************/
void ofApp::draw_fbo()
{
	ofDisableAlphaBlending();
	
	for(int i = 0; i < NUM_CAMS; i++){
		fbo[i].begin();
			ofClear(0, 0, 0, 0);
			ofSetColor(255, 255, 255, 255);
			
			if(Gui_Global->Cam1_to_Fbo[i])	VideoCam[1]->draw( 0, 0 );
			else							VideoCam[0]->draw( 0, 0 );
			
		fbo[i].end();
	}
}

/******************************
******************************/
void ofApp::keyPressed(int key){
	switch(key){
		case '0':
			Gui_Global->Cam1_to_Fbo[0] = false;
			Gui_Global->Cam1_to_Fbo[1] = false;
			break;
			
		case '1':
			Gui_Global->Cam1_to_Fbo[0] = true;
			Gui_Global->Cam1_to_Fbo[1] = true;
			break;
			
		case 'd':
			b_DispGUI = !b_DispGUI;
			break;
			
		case ' ':
			Gui_Global->b_SendStrobeImage = !Gui_Global->b_SendStrobeImage;
			break;
	}
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
