#ifndef _UISOLDIERITEM_H
#define _UISOLDIERITEM_H

#include "cocos2d.h"
using namespace cocos2d;
#include "UIGraySprite.h"
#include "UITouchButton.h"
#include "SoldierConfig.h"


class UISoldierItem : public Layer
{
public:
	enum class ItemStatus
	{   
		//正常
		NOMAL = 0,
		//已满
		FULL  = 1,
		//未能建造
		UNENABLE = 2,
	};
public:
	UISoldierItem();
	~UISoldierItem();
	CREATE_FUNC(UISoldierItem);
	static  UISoldierItem * createFromData(UserSoldierLevel * data);
	void setCallback(const ccMenuCallback& callback);
	void setGray(bool v);
	UserSoldierLevel * getData(){return _soldierlv;}
private:
	CC_SYNTHESIZE_RETAIN(Sprite*,_bg,Bg);
	CC_SYNTHESIZE_RETAIN(UIGraySprite*,_pic,PIC);
    CC_SYNTHESIZE_RETAIN(UITouchButton*,_touchBtn,Button);
	CC_SYNTHESIZE_RETAIN(UIGraySprite*,_btnimg,ButtonImg);
	CC_SYNTHESIZE_RETAIN(Label*,_levellabel,LevelLabel);
	CC_SYNTHESIZE_RETAIN(Label*,_tipLabel,TipLabel);
	CC_SYNTHESIZE_RETAIN(Label*,_costLabel,CostLabel);
private:
	void onBtnClick(Ref * ref);
	void initBackGround();
	ccMenuCallback _callback;
	ItemStatus _status;
	UserSoldierLevel * _soldierlv;
};

class UISoldierUpgradeLayer: public Layer
{
public:
	UISoldierUpgradeLayer();
	~UISoldierUpgradeLayer();
	CREATE_FUNC(UISoldierUpgradeLayer);
	void  setCallback(const ccMenuCallback& callback);
};





#endif



