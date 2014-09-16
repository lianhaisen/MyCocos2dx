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
		TopBeginNode	= 1,			//����1
		TopBeginNode2	= 2,			//����2
		TopBeginNode3	= 3,			//����3

	};
public:
	UITipLayer();
	~UITipLayer();
	virtual bool init();
	CREATE_FUNC(UITipLayer);

	void setColseCallBack(const ccMenuCallback& callback);
	//�Զ���ͷ�ڵ�λ�� ����Ľڵ�β����Ӹ��� 
	//@nType ��ŵ��������� @node �ڵ㣬@Pos��һ����ʼλ�� �������� @str ����
	void PushTopBeginNode(TopBegin nType,Node* node,Point Pos = Point::ZERO,string str = "",Color3B color = Color3B::WHITE);
	//�޸����ֻ�ͼƬ @tag �ӽڵ�ID(2~4���� 5~7ͼƬ 1��Ĭ������)
	//�޸�ֵ@nType ��ŵ��������� @Index m_TopBeginNodeList���±� @str ���� 
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
	static HeroTip* showHeroTip(int nHeroId); //����Tip
	//����Ӣ��TIP
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
	static SoldierTip* createSoldierTip(CSoldier* data); //����Tip
	//����Ӣ��TIP
	void UpDataHeroTip(CSoldier* data);

	UITipLayer* getTipLayer(){return m_Tip;}
private:
	UITipLayer* m_Tip;


};



#endif //  __UITipLayer_H__


