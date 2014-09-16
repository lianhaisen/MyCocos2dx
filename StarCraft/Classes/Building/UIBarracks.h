#ifndef __UIBarracks_H__
#define __UIBarracks_H__
/*
	¾üÓª
*/
#include "cocos2d.h"
#include "UITouchButton.h"
#include "UIMsgLayer.h"
#include "UISoldierHeadLayer.h"
using namespace cocos2d;

class UIBarracks :public UIMsgLayer
{
public:

public:
	UIBarracks();
	~UIBarracks();
    CREATE_FUNC(UIBarracks);
	virtual bool init();
	//static UIBarracks * CreateFromDB(DBuilding * db);
private:
	void initWithForm();

	void onClickCreateTip(Ref *sender);
	void onClickClose();
	void onClicshowConveyor();
private:

};




#endif //  __UIHeroPub_H__


