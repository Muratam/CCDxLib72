#ifndef  _APP_DELEGATE_H_
#define  _APP_DELEGATE_H_

#include "cocos2d.h"

/**
@brief    The cocos2d Application.

The reason for implement as private inheritance is to hide some interface call by Director.
*/
class  AppDelegate : private cocos2d::Application
{
public:
    AppDelegate();
    virtual ~AppDelegate();

    virtual void initGLContextAttrs();

    /**
    @brief    Implement Director and Scene init code here.
    @return true    Initialize success, app continue.
    @return false   Initialize failed, app terminate.
    */
    virtual bool applicationDidFinishLaunching();

    /**
    @brief  The function be called when the application enter background
    @param  the pointer of the application
    */
    virtual void applicationDidEnterBackground();
	
    /**
    @brief  The function be called when the application enter foreground
    @param  the pointer of the application
    */
    virtual void applicationWillEnterForeground();

private:
		
	bool hasDxInited = false;
	bool isPlayingBGM = false;
	std::string ViewName = "CCDxLib";
	virtual void CCDxInit();
	bool BeforeDxInitEmulateTouchByMouseFunctions = false;
	bool isWindowed = false;
	const int DX_CHANGESCREEN_OK = 0;
	const int DX_CHANGESCREEN_RETURN = 1;
	const int DX_CHANGESCREEN_DEFAULT = 2;
	int SetGraphMode(int SizeX, int SizeY, int Dummy = 32);
	int ChangeWindowMode(int Flag);
	int SetBackgroundColor(int Red, int Green, int Blue);
	void EMULATE_TOUCH_BY_MOUSEFUNCTIONS(bool Emulate = true);
public:
	int SetBackGroundColorR = -72, SetBackGroundColorB = -72, SetBackGroundColorG = -72;
	int SetMainWindowText(char *WindowText);
};



#endif // _APP_DELEGATE_H_

