/************************************************************
************************************************************/
#include "sj_common.h"

/************************************************************
************************************************************/
/********************
********************/
int GPIO_0 = 0;
int GPIO_1 = 0;

/********************
********************/
GUI_GLOBAL* Gui_Global = NULL;

FILE* fp_Log = NULL;


/************************************************************
class
************************************************************/

/******************************
******************************/
void GUI_GLOBAL::setup(string GuiName, string FileName, float x, float y)
{
	/********************
	********************/
	gui.setup(GuiName.c_str(), FileName.c_str(), x, y);
	
	/********************
	********************/
	gui.add(b_DispImage_SourceCode.setup("DispImage=SourceCode", false));
	
	gui.add(Cam1_to_Fbo[0].setup("Cam1 / Fbo0", false));
	gui.add(Cam1_to_Fbo[1].setup("Cam1 - Fbo1", false));
	
	gui.add(BoostBrightness_for_Strobe.setup("BoostBright", 0, 0, 1.0));
	gui.add(b_SendStrobeImage.setup("SendStrobeImage", false));
	gui.add(d_Strobe_sec.setup("d_Strobe_sec", 0.095, 0.095, 0.4));
	
	/********************
	********************/
	gui.minimizeAll();
}

