#ifndef __UIHeroPub_H__
#define __UIHeroPub_H__
/*
	英雄酒店 
*/
#include "cocos2d.h"
#include "UITouchButton.h"
#include "UIMsgLayer.h"
#include "UIRecruitHeroHerdLayer.h"
#include "BuildingData.h"
#include "UITipLayer.h"

using namespace cocos2d;

class UIHeroPub :public UIMsgLayer
{
public:
	enum class PubStatus
	{
		HERO_RECRUIT = 1,	//招募英雄
		HERO_LIST = 2,		//英雄列表（背包）
	};
public:
	UIHeroPub();
	~UIHeroPub();
    CREATE_FUNC(UIHeroPub);
	virtual bool init();
	void tabsFunc(int index);
	static UIHeroPub * CreateFromDB(DBuilding * db);
private:
	void initWithForm();
	void createTableBtn(int MaxNum);
	void onClickTableBtnCallBack(Ref* sender);

	void onClickRecruit(Ref* sender);
	//点击Tip弹窗
	void onClickHeroTip(Ref* sender);
	void createHeroTipLayer(int nHeroId);
	//关闭TIP
	void onCloseTip();
	//招募英雄
	void createRecruitHero();
	//英雄列表
	void createHeroList();
	//定时器
	void updateWorkProcess( float dt );

	//点击英雄升级
	void onClickUpHeroLv(Ref* sender);
	//点击英雄技能升级
	void onClickUpSkillLv(Ref* sender);
	//初始化标题
	void initTitle();

	void HeroUpDataMoney(const string str,string str2 = " ",string Path = " ");
	void SkillUpDataMoney(const string str,string str2 = " ",string Path = " ");

	//2控件居中
	void create2NodeMiddle(Node* parent,Node* n1,Node* n2);


	CC_SYNTHESIZE_RETAIN(Scale9Sprite*,_scale9Sprite,CurScale9Sprite);
	CC_SYNTHESIZE_RETAIN(UITouchButton*,m_HeroUpLvBtn,CurHeroUpLvBtn);
	CC_SYNTHESIZE_RETAIN(UITouchButton*,m_SkillUpLvBtn,CurSkillUpLvBtn);
	CC_SYNTHESIZE_RETAIN(Label*,m_HeroUpLvLabel,CurHeroUpLvLabel);
	CC_SYNTHESIZE_RETAIN(Label*,m_SkillUpLvLabel,CurSkillUpLvLabel);

private:
	Vector<UITouchButton*> m_TableBtn;
	vector<string> m_BtnName;
	int m_CurStateIndex;				//翻页ID
	ScrollView* m_RecruitHeroNode;//
	ScrollView* m_HeroListNode;
	DBuilding * _db;
	int _CruClickId;			//当前选中的按钮ID 防止重复点击
	int _CurLvUpHeroId;			//当前正在升级的英雄ID
	int _CruClickTipId;			//当前点击的TipID	
	Vector<UIRecruitHeroHerdLayer*> m_HeroHerdLayer;
	HeroTip* m_TipLayer;
};




#endif //  __UIHeroPub_H__


