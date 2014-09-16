#include "AppDelegate.h"

#include <vector>
#include <string>
#include "AppMacros.h"
#include "MapHelper.h"
#include "BattleScene.h"
#include "LHSTestScene.h"
#include "BuildingCommon.h"

USING_NS_CC;
using namespace std;

AppDelegate::AppDelegate() {

}

AppDelegate::~AppDelegate() 
{
}

bool AppDelegate::applicationDidFinishLaunching() {
    // initialize director
    auto director = Director::getInstance();
    auto glview = director->getOpenGLView();
    if(!glview) {
        glview = GLView::create("StarCraft");
		glview->setFrameSize(DESIGN_RESOLUTION_SIZE_WIDTH ,DESIGN_RESOLUTION_SIZE_HEIGHT );
		//glview->setFrameZoomFactor(0.5f);
        director->setOpenGLView(glview);
    }

    director->setOpenGLView(glview);


    // Set the design resolution
	glview->setDesignResolutionSize(DESIGN_RESOLUTION_SIZE_WIDTH , DESIGN_RESOLUTION_SIZE_HEIGHT, ResolutionPolicy::NO_BORDER);

	


    
    vector<string> searchPath;
    
	//auto framesize = glview->getFrameSize();
	//if (framesize.height > DESIGN_RESOLUTION_SIZE_HEIGHT)
	//{   
	//    //searchPath.push_back("low");
	//	searchPath.push_back("hight");
	//	director->setContentScaleFactor(MIN(framesize.height/DESIGN_RESOLUTION_SIZE_HEIGHT,
	//		framesize.height/DESIGN_RESOLUTION_SIZE_WIDTH))
	//}
	//else
	//{
	//	searchPath.push_back("low");
	//}

	// set searching path
	FileUtils::getInstance()->setSearchPaths(searchPath);

	
    // turn on display FPS
    director->setDisplayStats(true);

    // set FPS. the default value is 1.0/60 if you don't call this
    director->setAnimationInterval(1.0 / 60);

	//初始化地图坐标点
	MapHelper::initCoord();

	Configuration::getInstance()->loadConfigFile("db/strings.plist");
	BuildingCommon::LoadArmateResource();


    // create a scene. it's an autorelease object
    //auto scene = MainScene::create();
	//auto scene = BattleScene::create();
    auto scene = TestScene::create();

    // run
    director->runWithScene(scene);



    return true;
}

// This function will be called when the app is inactive. When comes a phone call,it's be invoked too
void AppDelegate::applicationDidEnterBackground() {
    Director::getInstance()->stopAnimation();

    // if you use SimpleAudioEngine, it must be pause
    // SimpleAudioEngine::sharedEngine()->pauseBackgroundMusic();
}

// this function will be called when the app is active again
void AppDelegate::applicationWillEnterForeground() {
    Director::getInstance()->startAnimation();

    // if you use SimpleAudioEngine, it must resume here
    // SimpleAudioEngine::sharedEngine()->resumeBackgroundMusic();
}
