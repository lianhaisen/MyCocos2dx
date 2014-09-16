#ifndef __CUSTOMTABLEVIEWCELL_H
#define __CUSTOMTABLEVIEWCELL_H

#include "cocos2d.h"
#include "extensions/cocos-ext.h"
#include "cocostudio/CocoStudio.h"
#include "UIGraySprite.h"
using namespace cocostudio;
using namespace cocos2d::extension;
using namespace cocos2d;

class CustomTableViewCell : public TableViewCell
{
public:
	CustomTableViewCell();
	~CustomTableViewCell();
	virtual bool init();
	CREATE_FUNC(CustomTableViewCell);

	bool onTouchesBegan(Touch * touch, cocos2d::Event  *event);
	void onTouchesMoved(Touch * touch, cocos2d::Event  *event);
	void onTouchesEnded(Touch * touch, cocos2d::Event  *event);
	void onTouchCancelled(Touch * touch, cocos2d::Event  *event);

	void setCilckTarget(const ccMenuCallback& callback) {_clickCallBack = callback;}
	//设置不可用
	//TODO 灰化
	void setDisable(bool isDisable = false,std::string str = "");

	bool getIsDisable(){return _isdisable;}

private:
	CC_SYNTHESIZE_RETAIN(UIGraySprite*,_bg,Bg);
	CC_SYNTHESIZE_RETAIN(UIGraySprite*,_infoPic,InfoPic);
	CC_SYNTHESIZE_RETAIN(Label*,_title,Title);
	CC_SYNTHESIZE_RETAIN(Label*,_des,Des);
	CC_SYNTHESIZE_RETAIN(Label*,_lablecost,LableCost);
	CC_SYNTHESIZE_RETAIN(Label*,_timeLable,TimeLable);
	CC_SYNTHESIZE_RETAIN(Label*,_hasbuildLable,HasBuildLable);
	CC_SYNTHESIZE_RETAIN(Sprite*,_enableBackground,EnableBackground);
	CC_SYNTHESIZE_RETAIN(Label*,_enableTip,EnableTip);

	bool isTouchIn(Touch * touch);
	bool _istouched;
	bool _ismoving;
	void onCloseCallBack();
	ccMenuCallback _clickCallBack;
	bool _isdisable; 


};

#endif



