#ifndef __APPMACROS_H__
#define __APPMACROS_H__
/*
�궨��
*/

#include "cocos2d.h"

#define FACTOR 2

#define DESIGN_RESOLUTION_SIZE_WIDTH  1024	//��Ʒֱ���-��
#define DESIGN_RESOLUTION_SIZE_HEIGHT 768	//��Ʒֱ���-��

#define WINSIZE			CCDirector::getInstance()->getWinSize()			//��Ʒֱ���
#define VISIBLESIZE		CCDirector::getInstance()->getVisibleSize()		//��Ʒֱ��ʿ��������С
#define VISIBLEORIGIN	CCDirector::getInstance()->getVisibleOrigin()	//��Ʒֱ��ʿ����������
#define FRAMESIZE		CCEGLView::sharedOpenGLView()->getFrameSize()	//��Ļ�ֱ���



#define FONT_NAME		"Arial"

#define FONT_SIZE_XL	"24"
#define FONT_SIZE_L		"24"
#define FONT_SIZE_M		"24"
#define FONT_SIZE_S		"24"
//����ͼ���ĸ���
//#define POINT1 ccp(638, 144)		//���£�ԭ��
//#define POINT2 ccp(82, 562)		//����
//#define POINT3 ccp(1194, 562)		//����
//#define POINT4 ccp(638, 980)		//����
#define POINT1  Point(1790,2030)    //�ϣ�ԭ��
#define POINT2  Point(591,1140)	    //��
#define POINT3  Point(1790,250)     //��
#define POINT4  Point(2989,1140)	//��

//#define MAX_GRID_COUNT_UI 40
//#define MAX_GRID_COUNT_BATTLE 80
#define MAX_GRID_COUNT 86

//ÿС��Ŀ���
//#define UNIT_WIDTH_UI 13.9				//(POINT1.x - POINT2.x)/(MAX_GRID_COUNT/2);
//#define UNIT_HEIGHT_UI 10.45			//(POINT1.y - POINT2.y)/(MAX_GRID_COUNT/2);

#define UNIT_WIDTH   ((POINT1.x - POINT2.x)/MAX_GRID_COUNT)		//(POINT1.x - POINT2.x)/MAX_GRID_COUNT;
#define UNIT_HEIGHT  ((POINT1.y - POINT2.y)/MAX_GRID_COUNT)	//(POINT1.y - POINT2.y)/MAX_GRID_COUNT;

#define SCHEDULE_INTERVAL 0.04f //��ʱ�����fps = 25

#endif /* __APPMACROS_H__ */
