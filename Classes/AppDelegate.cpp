#include "AppDelegate.h"
#include "DxLib.h"

USING_NS_CC;

static cocos2d::Size designResolutionSize = cocos2d::Size(480, 320);
static cocos2d::Size smallResolutionSize = cocos2d::Size(480, 320);
static cocos2d::Size mediumResolutionSize = cocos2d::Size(1024, 768);
static cocos2d::Size largeResolutionSize = cocos2d::Size(2048, 1536);

AppDelegate::AppDelegate() {}

AppDelegate::~AppDelegate() {}

void AppDelegate::initGLContextAttrs(){
    //set OpenGL context attributions,now can only set six attributions:
    //red,green,blue,alpha,depth,stencil
    GLContextAttrs glContextAttrs = {8, 8, 8, 8, 24, 8};
    GLView::setGLContextAttrs(glContextAttrs);
}

static int register_all_packages(){
    return 0; //flag for packages manager
}

bool AppDelegate::applicationDidFinishLaunching() {
	CCDxInit();
	hasDxInited = true;
    auto director = Director::getInstance();
    auto glview = director->getOpenGLView();
    if(!glview) {
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32) || (CC_TARGET_PLATFORM == CC_PLATFORM_MAC) || (CC_TARGET_PLATFORM == CC_PLATFORM_LINUX)
		if (isWindowed)	glview = GLViewImpl::createWithRect(ViewName, Rect(0, 0, designResolutionSize.width, designResolutionSize.height));
		else glview = GLViewImpl::createWithFullScreen(ViewName);
#else
		glview = GLViewImpl::create(ViewName);
#endif
        director->setOpenGLView(glview);
    }
    // turn on display FPS
    director->setDisplayStats(false);
    // set FPS. the default value is 1.0/60 if you don't call this
    director->setAnimationInterval((float)(1.0 / 60));
	glview->setDesignResolutionSize(designResolutionSize.width, designResolutionSize.height, ResolutionPolicy::EXACT_FIT);// ResolutionPolicy::NO_BORDER);
    Size frameSize = glview->getFrameSize();
	/*
	if (frameSize.height > mediumResolutionSize.height)     
        director->setContentScaleFactor(MIN(largeResolutionSize.height/designResolutionSize.height, largeResolutionSize.width/designResolutionSize.width));
    else if (frameSize.height > smallResolutionSize.height)
        director->setContentScaleFactor(MIN(mediumResolutionSize.height/designResolutionSize.height, mediumResolutionSize.width/designResolutionSize.width));
    else
		director->setContentScaleFactor(MIN(smallResolutionSize.height/designResolutionSize.height, smallResolutionSize.width/designResolutionSize.width));
    */
	register_all_packages();

    // create a scene. it's an autorelease object
	
	auto scene = CCDxLib::createScene(this);
	director->runWithScene(scene);
	if (SetBackGroundColorR != -72 && SetBackGroundColorG != -72 && SetBackGroundColorB != -72)Get_m_dxlib()->SetBackgroundColor(SetBackGroundColorR, SetBackGroundColorG, SetBackGroundColorB);
	if (BeforeDxInitEmulateTouchByMouseFunctions)
		Get_m_dxlib()->EMULATE_TOUCH_BY_MOUSEFUNCTIONS(BeforeDxInitEmulateTouchByMouseFunctions);
	if (BeforeDxInitEmulateKeyBoardArrowsBySwipe)
		Get_m_dxlib()->EMULATE_KEYBOARD_ARROWS_BY_SWIPE(BeforeDxInitEmulateKeyBoardArrowsBySwipe);
	if (BeforeDxInitEmulateKeyboardBySingleTouchEnabled)
		Get_m_dxlib()->EMULATE_KEYBOARD_BY_SINGLETOUCH(BeforeDxInitEmulateKeyboardBySingleTouchKeyCode,true);
	//if (BeforeDxInitEmulateKeuBoardByAccelerometer)
	//	Get_m_dxlib()->EMULATE_KEYBOARD_ARROWS_BY_ACCELEROMETER(BeforeDxInitEmulateKeuBoardByAccelerometer );


	return true;
}

// This function will be called when the app is inactive. When comes a phone call,it's be invoked too
void AppDelegate::applicationDidEnterBackground() {
    Director::getInstance()->stopAnimation();

    // if you use SimpleAudioEngine, it must be pause
	//isPlayingBGM = SimpleAudioEngine::getInstance()->isBackgroundMusicPlaying();
	//if (isPlayingBGM)SimpleAudioEngine::getInstance()->pauseBackgroundMusic();
}

// this function will be called when the app is active again
void AppDelegate::applicationWillEnterForeground() {
    Director::getInstance()->startAnimation();

    // if you use SimpleAudioEngine, it must resume here
    //if(isPlayingBGM)SimpleAudioEngine::getInstance()->resumeBackgroundMusic();
}



//外部公開用の初期設定のDxlibの関数はここで定義する。

int AppDelegate::SetGraphMode(int SizeX, int SizeY, int Dummy){
	designResolutionSize = cocos2d::Size(SizeX,SizeY);
	return DX_CHANGESCREEN_OK;
}
int AppDelegate::ChangeWindowMode(int Flag){
	isWindowed = Flag == TRUE;
	return DX_CHANGESCREEN_OK;
}
int AppDelegate::SetMainWindowText(char *WindowText){
	if (hasDxInited){}
	ViewName = WindowText;
	return 0;
}
void AppDelegate::EMULATE_TOUCH_BY_MOUSEFUNCTIONS(bool Emulate){
	BeforeDxInitEmulateTouchByMouseFunctions = Emulate;
}
void AppDelegate::EMULATE_KEYBOARD_ARROWS_BY_SWIPE(bool Emulate){
	BeforeDxInitEmulateKeyBoardArrowsBySwipe = Emulate;
}
void AppDelegate::EMULATE_KEYBOARD_BY_SINGLETOUCH(int KeyCode, bool Emulate){
	BeforeDxInitEmulateKeyboardBySingleTouchKeyCode = KeyCode;
	BeforeDxInitEmulateKeyboardBySingleTouchEnabled = Emulate;
}

void AppDelegate::EMULATE_KEYBOARD_ARROWS_BY_ACCELEROMETER(bool Emulate){
	BeforeDxInitEmulateKeuBoardByAccelerometer = Emulate;
}

int AppDelegate::SetBackgroundColor(int Red, int Green, int Blue){
	SetBackGroundColorR = Red;
	SetBackGroundColorG = Green;
	SetBackGroundColorB = Blue;
	return 0;
}

