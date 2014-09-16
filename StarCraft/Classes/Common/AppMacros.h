#ifndef __APPMACROS_H__
#define __APPMACROS_H__
/*
宏定义
*/

#include "cocos2d.h"

#define FACTOR 2

#define DESIGN_RESOLUTION_SIZE_WIDTH  1024	//设计分辨率-宽
#define DESIGN_RESOLUTION_SIZE_HEIGHT 768	//设计分辨率-高

#define WINSIZE			CCDirector::getInstance()->getWinSize()			//设计分辨率
#define VISIBLESIZE		CCDirector::getInstance()->getVisibleSize()		//设计分辨率可视区域大小
#define VISIBLEORIGIN	CCDirector::getInstance()->getVisibleOrigin()	//设计分辨率可视区域起点
#define FRAMESIZE		CCEGLView::sharedOpenGLView()->getFrameSize()	//屏幕分辨率



#define FONT_NAME		"Arial"

#define FONT_SIZE_XL	"24"
#define FONT_SIZE_L		"24"
#define FONT_SIZE_M		"24"
#define FONT_SIZE_S		"24"
//主地图的四个点
//#define POINT1 ccp(638, 144)		//左下，原点
//#define POINT2 ccp(82, 562)		//左上
//#define POINT3 ccp(1194, 562)		//右下
//#define POINT4 ccp(638, 980)		//右上
#define POINT1  Point(1790,2030)    //上，原点
#define POINT2  Point(591,1140)	    //左
#define POINT3  Point(1790,250)     //下
#define POINT4  Point(2989,1140)	//右

//#define MAX_GRID_COUNT_UI 40
//#define MAX_GRID_COUNT_BATTLE 80
#define MAX_GRID_COUNT 86

//每小格的宽，高
//#define UNIT_WIDTH_UI 13.9				//(POINT1.x - POINT2.x)/(MAX_GRID_COUNT/2);
//#define UNIT_HEIGHT_UI 10.45			//(POINT1.y - POINT2.y)/(MAX_GRID_COUNT/2);

#define UNIT_WIDTH   ((POINT1.x - POINT2.x)/MAX_GRID_COUNT)		//(POINT1.x - POINT2.x)/MAX_GRID_COUNT;
#define UNIT_HEIGHT  ((POINT1.y - POINT2.y)/MAX_GRID_COUNT)	//(POINT1.y - POINT2.y)/MAX_GRID_COUNT;

#define SCHEDULE_INTERVAL 0.04f //定时器间隔fps = 25

#endif /* __APPMACROS_H__ */
