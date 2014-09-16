#ifndef __UIHeroConfiguration_H__
#define __UIHeroConfiguration_H__
/*
	英雄祭坛配置
*/
#include "cocos2d.h"
using namespace cocos2d;
#include "UIMsgLayer.h"
#include "UITouchButton.h"
#include "UITipLayer.h"
using namespace std;
class UIHeroConfiguration :public UIMsgLayer
{
public:

	UIHeroConfiguration();
	~UIHeroConfiguration();
	CREATE_FUNC(UIHeroConfiguration);

	virtual bool init();
	void tabsFunc(int nHeroId);							//选择界面
	CC_SYNTHESIZE_RETAIN(Node*,_HeroinfoNode,HeroinfoNode);
	static UIHeroConfiguration* CreateFromDB(DBuilding * db);
private:
	void initWithForm();
	void onClickTableBtnCallBack(Ref* sender);
	void createHeroInfoLayer(int nHeroId);//创建

	void onClickShowHeroTip(Ref* sender);//打开英雄信息节点

	void onClickBattleOrOut(Ref* sender);//上阵 或移除
	void UpDataTableBtn();
private:
	Vector<UITouchButton*> m_TableBtn;
	int m_CurStateIndex;
	int _nHeroBattleId;
	DBuilding *_db;
};




#endif //  __UIHeroConfiguration_H__


