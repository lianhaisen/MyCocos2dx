#ifndef __UISoldierHeadLayer_H__
#define __UISoldierHeadLayer_H__
/*
	��ļʿ����ʿ��ͷ��
*/
#include "cocos2d.h"
using namespace cocos2d;

#include "UITouchButton.h"
using namespace std;
#include "SoldierConfig.h"

class UISoldierHeadLayer :public Layer
{
public:
	//ʿ���б�
	typedef struct _Soldier_List
	{
		int nType;				//����
		int nNum;				//ӵ������
		int nGold;				//������
		int Lv;					//�ȼ�
		int nTime;				//����ʱ��
		Sprite* imgHerd;		//ͷ��ڵ�
		Label* lbNum;			//�����ڵ�
		Label* lbLv;			//�ȼ��ڵ�
		Label* lbGold;			//�����ҽڵ�
		UITouchButton* btn;		//������ļ�İ�ť
		Sprite * imgmiddle;		//��������ļ״̬�������ÿɼ���
		bool bIsRecruit;		//�ɷ���ļ
		string Path;			//ͼƬ��ַ
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
	//�Ƿ������ļ
	void setIsRecruit(bool b);
	Soldierinfo* getSoldierinfo(){return m_Soldier;}
	//�����·��ڵ�
	CC_SYNTHESIZE_RETAIN(Node*,_DownLayer,CurDownLayer);

	//�������鰴ť�Ƿ�ɼ�
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


