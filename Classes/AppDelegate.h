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
	std::string ViewName = "CCDxLib";
	virtual void CCDxInit();
	//DxLib_Init()ÇåƒÇ‘ëOÇ…ê›íËÇ∑ÇÈä÷êîÇÕÇ±Ç±Ç≈AooDelegateÇÃä÷êîÇ∆ÇµÇƒíËã`Ç∑ÇÈ
	int SetGraphMode(int SizeX, int SizeY, int Dummy = 32);
	const int  DX_CHANGESCREEN_OK = 0;
	int ChangeWindowMode(int Flag);
	bool isWindowed = false;
	const int DX_CHANGESCREEN_RETURN = 1;
	const int DX_CHANGESCREEN_DEFAULT = 2;
public:
	int SetMainWindowText(char *WindowText);
};



#endif // _APP_DELEGATE_H_

