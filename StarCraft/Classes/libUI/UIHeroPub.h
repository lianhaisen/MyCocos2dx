#ifndef __UIHeroPub_H__
#define __UIHeroPub_H__
/*
	Ӣ�۾Ƶ� 
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
		HERO_RECRUIT = 1,	//��ļӢ��
		HERO_LIST = 2,		//Ӣ���б�������
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
	//���Tip����
	void onClickHeroTip(Ref* sender);
	void createHeroTipLayer(int nHeroId);
	//�ر�TIP
	void onCloseTip();
	//��ļӢ��
	void createRecruitHero();
	//Ӣ���б�
	void createHeroList();
	//��ʱ��
	void updateWorkProcess( float dt );

	//���Ӣ������
	void onClickUpHeroLv(Ref* sender);
	//���Ӣ�ۼ�������
	void onClickUpSkillLv(Ref* sender);
	//��ʼ������
	void initTitle();

	void HeroUpDataMoney(const string str,string str2 = " ",string Path = " ");
	void SkillUpDataMoney(const string str,string str2 = " ",string Path = " ");

	//2�ؼ�����
	void create2NodeMiddle(Node* parent,Node* n1,Node* n2);


	CC_SYNTHESIZE_RETAIN(Scale9Sprite*,_scale9Sprite,CurScale9Sprite);
	CC_SYNTHESIZE_RETAIN(UITouchButton*,m_HeroUpLvBtn,CurHeroUpLvBtn);
	CC_SYNTHESIZE_RETAIN(UITouchButton*,m_SkillUpLvBtn,CurSkillUpLvBtn);
	CC_SYNTHESIZE_RETAIN(Label*,m_HeroUpLvLabel,CurHeroUpLvLabel);
	CC_SYNTHESIZE_RETAIN(Label*,m_SkillUpLvLabel,CurSkillUpLvLabel);

private:
	Vector<UITouchButton*> m_TableBtn;
	vector<string> m_BtnName;
	int m_CurStateIndex;				//��ҳID
	ScrollView* m_RecruitHeroNode;//
	ScrollView* m_HeroListNode;
	DBuilding * _db;
	int _CruClickId;			//��ǰѡ�еİ�ťID ��ֹ�ظ����
	int _CurLvUpHeroId;			//��ǰ����������Ӣ��ID
	int _CruClickTipId;			//��ǰ�����TipID	
	Vector<UIRecruitHeroHerdLayer*> m_HeroHerdLayer;
	HeroTip* m_TipLayer;
};




#endif //  __UIHeroPub_H__


