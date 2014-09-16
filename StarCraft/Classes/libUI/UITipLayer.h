#ifndef __UITipLayer_H__
#define __UITipLayer_H__
/*
	TipUI
*/
#include "cocos2d.h"
using namespace cocos2d;
#include "UIMsgLayer.h"
#include "UIUserInfoLayer.h"
#include "UITouchButton.h"
#include "BuildingConfig.h"
#include "HeroConfig.h"
#include "BuildingData.h"
#include "SoldierConfig.h"
#include "UISoldierItem.h"
using namespace std;

class HeroTip;
class SoldierTip;


class UITipLayer :public UIMsgLayer
{
public:
	enum class TopBegin
	{
		TopBeginNode	= 1,			//容器1
		TopBeginNode2	= 2,			//容器2
		TopBeginNode3	= 3,			//容器3

	};
public:
	UITipLayer();
	~UITipLayer();
	virtual bool init();
	CREATE_FUNC(UITipLayer);

	void setColseCallBack(const ccMenuCallback& callback);
	//自定义头节点位置 后面的节点尾部添加跟随 
	//@nType 存放的容器类型 @node 节点，@Pos第一个起始位置 其他随意 @str 文字
	void PushTopBeginNode(TopBegin nType,Node* node,Point Pos = Point::ZERO,string str = "",Color3B color = Color3B::WHITE);
	//修改文字或图片 @tag 子节点ID(2~4文字 5~7图片 1已默认文字)
	//修改值@nType 存放的容器类型 @Index m_TopBeginNodeList的下标 @str 文字 
	void setTopBegin1String(TopBegin nType,int Index,const string& str,int tag = 1,Color3B color = Color3B::WHITE);
	void setVisibleBackBtn(bool b);
	void clearVectors();
private:
	void initwith();
	void onClose();
	CC_SYNTHESIZE_RETAIN(Scale9Sprite*,_Bg,CurBgNode);
private:
	ccMenuCallback _closeCallBack;
	vector<Node*> m_TopBeginNodeList;
	vector<Node*> m_TopBeginNodeList2;
	vector<Node*> m_TopBeginNodeList3;
	UITouchButton* _Backbtn;
};

class HeroTip 
{
public:
	HeroTip();
	~HeroTip();
	static HeroTip* showHeroTip(int nHeroId); //创建Tip
	//更新英雄TIP
	void UpDataHeroTip(HeroInfo* info);
	UITipLayer* getTipLayer(){return m_Tip;}
private:
	 UITipLayer* m_Tip;
};

class SoldierTip 
{
public:
	SoldierTip();
	~SoldierTip();
	static SoldierTip* createSoldierTip(CSoldier* data); //创建Tip
	//更新英雄TIP
	void UpDataHeroTip(CSoldier* data);

	UITipLayer* getTipLayer(){return m_Tip;}
private:
	UITipLayer* m_Tip;


};



#endif //  __UITipLayer_H__


