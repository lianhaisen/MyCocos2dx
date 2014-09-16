#ifndef __UISoldierHeadLayer_H__
#define __UISoldierHeadLayer_H__
/*
	招募士兵—士兵头像
*/
#include "cocos2d.h"
using namespace cocos2d;

#include "UITouchButton.h"
using namespace std;
#include "SoldierConfig.h"

class UISoldierHeadLayer :public Layer
{
public:
	//士兵列表
	typedef struct _Soldier_List
	{
		int nType;				//类型
		int nNum;				//拥有数量
		int nGold;				//所需金币
		int Lv;					//等级
		int nTime;				//创建时间
		Sprite* imgHerd;		//头像节点
		Label* lbNum;			//数量节点
		Label* lbLv;			//等级节点
		Label* lbGold;			//所需金币节点
		UITouchButton* btn;		//可以招募的按钮
		Sprite * imgmiddle;		//不可以招募状态（需设置可见）
		bool bIsRecruit;		//可否招募
		string Path;			//图片地址
	}Soldierinfo;

public:
	UISoldierHeadLayer();
	~UISoldierHeadLayer();
	//void setData(CSoldier* data) {_data = data;}
	CSoldier * getData() {return _data;}

	virtual bool init(CSoldier * data ,int nNum,bool v);
	static UISoldierHeadLayer* create(CSoldier * data ,int nNum,bool v);
	void setRecruitCallBack(const ccMenuCallback& callback);
	void setTipCallBack(const ccMenuCallback& callback);
	//是否可以招募
	void setIsRecruit(bool b);
	Soldierinfo* getSoldierinfo(){return m_Soldier;}
	//设置下方节点
	CC_SYNTHESIZE_RETAIN(Node*,_DownLayer,CurDownLayer);

	//设置详情按钮是否可见
	void setIsTopVisible(bool v);

private:
	void initWithFormat();
	void addRecruitSoldier(Ref* sender);
	void onClickCallBack(Ref* sender);
private:
	Soldierinfo* m_Soldier;
	ccMenuCallback m_RecruitSoldierCallBack;
	ccMenuCallback m_TipCallBack;
	CSoldier *     _data;
	UITouchButton * m_btnTop;
};




#endif //  __UISoldierHeadLayer_H__


