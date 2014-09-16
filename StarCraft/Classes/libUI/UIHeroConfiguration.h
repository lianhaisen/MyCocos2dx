#ifndef __UIHeroConfiguration_H__
#define __UIHeroConfiguration_H__
/*
	Ӣ�ۼ�̳����
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
	void tabsFunc(int nHeroId);							//ѡ�����
	CC_SYNTHESIZE_RETAIN(Node*,_HeroinfoNode,HeroinfoNode);
	static UIHeroConfiguration* CreateFromDB(DBuilding * db);
private:
	void initWithForm();
	void onClickTableBtnCallBack(Ref* sender);
	void createHeroInfoLayer(int nHeroId);//����

	void onClickShowHeroTip(Ref* sender);//��Ӣ����Ϣ�ڵ�

	void onClickBattleOrOut(Ref* sender);//���� ���Ƴ�
	void UpDataTableBtn();
private:
	Vector<UITouchButton*> m_TableBtn;
	int m_CurStateIndex;
	int _nHeroBattleId;
	DBuilding *_db;
};




#endif //  __UIHeroConfiguration_H__


