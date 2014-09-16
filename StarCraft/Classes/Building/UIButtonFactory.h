#ifndef __UIBUTTONFACTORY_H_
#define __UIBUTTONFACTORY_H_

#include "UITouchButton.h"
class DBuilding;


class UIButtonFactory
{
public:
	enum class UIButtonType
	{   
		//信息
		INFO = 0,
		//升级
		UPGRADE = 1,
		//升级取消
		UPGRADECANCLE = 2,
		//加速升级
		ACCELERATE = 3,
		//收集
		COLLECT = 4,
		//进入
		ENTER = 5,
	    //招募
	    RECRUIT = 6,
		//配置英雄
		HEROCFG = 7,
		//英雄招募
		HERORECRUIT = 8,
        //选中整行
		SELECTROW = 9,
		//旋转行
		ROTATEROW = 10,

	};

public:
	UIButtonFactory(){}
	~UIButtonFactory(){}
	static Vector<UITouchButton*> getButtonsFromDB(DBuilding * db, const ccMenuCallback& callback); 
private:
	static UITouchButton * getButtonFromTypeAndDB(UIButtonType type,DBuilding * db,const ccMenuCallback& callback );
};




#endif



